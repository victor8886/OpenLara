#ifndef H_CONTROLLER
#define H_CONTROLLER

#include "format.h"
#include "frustum.h"
#include "mesh.h"
#include "animation.h"

#define GRAVITY     (6.0f * 30.0f)
#define SPRITE_FPS  10.0f

#define MAX_LAYERS  4
#define MAX_SPHERES 32

#define UNLIMITED_AMMO  10000

struct Controller;

struct ICamera {
    vec4  *reflectPlane;
    vec3  pos;
    float shake;
    bool  firstPerson;

    ICamera() : reflectPlane(NULL), pos(0.0f), shake(0.0f) {}

    virtual void setup(bool calcMatrices) {}
    virtual int  getRoomIndex() const { return TR::NO_ROOM; }
    virtual void doCutscene(const vec3 &pos, float rotation) {}
};

struct IGame {
    virtual ~IGame() {}
    virtual void         loadLevel(TR::LevelID id) {}
    virtual void         loadNextLevel() {}
    virtual void         loadGame(int slot) {}
    virtual void         saveGame(int slot) {}
    virtual void         applySettings(const Core::Settings &settings)  {}

    virtual TR::Level*   getLevel()     { return NULL; }
    virtual MeshBuilder* getMesh()      { return NULL; }
    virtual ICamera*     getCamera()    { return NULL; }
    virtual Controller*  getLara()      { return NULL; }
    virtual bool         isCutscene()   { return false; }
    virtual uint16       getRandomBox(uint16 zone, uint16 *zones) { return 0; }
    virtual uint16       findPath(int ascend, int descend, bool big, int boxStart, int boxEnd, uint16 *zones, uint16 **boxes) { return 0; }
    virtual void setClipParams(float clipSign, float clipHeight) {}
    virtual void setWaterParams(float height) {}
    virtual void waterDrop(const vec3 &pos, float radius, float strength) {}
    virtual void setShader(Core::Pass pass, Shader::Type type, bool underwater = false, bool alphaTest = false) {}
    virtual void setRoomParams(int roomIndex, Shader::Type type, float diffuse, float ambient, float specular, float alpha, bool alphaTest = false) {}
    virtual void setupBinding() {}
    virtual void renderEnvironment(int roomIndex, const vec3 &pos, Texture **targets, int stride = 0, Core::Pass pass = Core::passAmbient) {}
    virtual void renderCompose(int roomIndex) {}
    virtual void renderView(int roomIndex, bool water) {}
    virtual void setEffect(Controller *controller, TR::Effect effect) {}

    virtual void checkTrigger(Controller *controller, bool heavy) {}

    virtual Controller* addEntity(TR::Entity::Type type, int room, const vec3 &pos, float angle = 0.0f) { return NULL; }
    virtual void removeEntity(Controller *controller) {}

    virtual bool invUse(TR::Entity::Type type) { return false; }
    virtual void invAdd(TR::Entity::Type type, int count = 1) {}
    virtual int* invCount(TR::Entity::Type type) { return NULL; }
    virtual bool invChooseKey(TR::Entity::Type hole) { return false; }

    virtual Sound::Sample* playSound(int id, const vec3 &pos = vec3(0.0f), int flags = 0) const { return NULL; }
    virtual void playTrack(uint8 track, bool restart = false) {}
    virtual void stopTrack()                                {}
};

struct Controller {

    static Controller *first;
    Controller  *next;

    IGame       *game;
    TR::Level   *level;
    int         entity;
    
    Animation   animation;
    int         &state;

    vec3    pos;
    vec3    angle;

    int16             roomIndex;
    TR::Entity::Flags flags;

    Basis   *joints;
    int     frameIndex;

    vec3    ambient[6];
    float   specular;
    float   intensity;

    float   timer;

    TR::Room::Light *targetLight;
    vec3 mainLightPos;
    vec4 mainLightColor;

    struct MeshLayer {
        uint32   model;
        uint32   mask;
    } *layers;

    uint32 visibleMask;
    uint32 explodeMask;
    struct ExplodePart {
        Basis basis;
        vec3  velocity;
        int   roomIndex;
    } *explodeParts;

    vec3 lastPos;
    bool invertAim;

    Controller(IGame *game, int entity) : next(NULL), game(game), level(game->getLevel()), entity(entity), animation(level, getModel()), state(animation.state), layers(0), explodeMask(0), explodeParts(0), lastPos(0), invertAim(false) {
        const TR::Entity &e = getEntity();
        pos         = vec3(float(e.x), float(e.y), float(e.z));
        angle       = vec3(0.0f, e.rotation, 0.0f);
        roomIndex   = e.room;
        flags       = e.flags;
        flags.state = TR::Entity::asNone;

        if (level->isCutsceneLevel())
            fixRoomIndex();

        const TR::Model *m = getModel();
        joints     = m ? new Basis[m->mCount] : NULL;
        frameIndex = -1;
        specular   = 0.0f;
        intensity  = e.intensity == -1 ? -1.0f : intensityf(e.intensity);
        timer      = 0.0f;
        ambient[0] = ambient[1] = ambient[2] = ambient[3] = ambient[4] = ambient[5] = vec3(intensityf(getRoom().ambient));
        targetLight = NULL;
        updateLights(false);
        visibleMask = 0xFFFFFFFF;

        if (flags.once) {
            flags.invisible = true;
            flags.once      = false;
        }

        if (flags.active == TR::ACTIVE) {
            flags.active  = 0;
            flags.reverse = true;
            activate();
        }

        if (e.isLara() || e.isActor()) // Lara and cutscene entities is active by default
            activate();
    }

    virtual ~Controller() {
        delete[] joints;
        delete[] layers;
        delete[] explodeParts;
        deactivate(true);
    }

    bool fixRoomIndex() {
        vec3 p = getPos();
        for (int i = 0; i < level->roomsCount; i++)
            if (insideRoom(p, i)) {
                roomIndex = i;
                return true;
            }
        return false;
    }

    void getFloorInfo(int roomIndex, const vec3 &pos, TR::Level::FloorInfo &info) const {
        int x = int(pos.x);
        int y = int(pos.y);
        int z = int(pos.z);

        int dx, dz;
        TR::Room::Sector &s = level->getSector(roomIndex, x, z, dx, dz);

        info.roomFloor    = float(256 * s.floor);
        info.roomCeiling  = float(256 * s.ceiling);
        info.floor        = info.roomFloor;
        info.ceiling      = info.roomCeiling;
        info.slantX       = 0;
        info.slantZ       = 0;
        info.roomNext     = TR::NO_ROOM;
        info.roomBelow    = s.roomBelow;
        info.roomAbove    = s.roomAbove;
        info.floorIndex   = s.floorIndex;
        info.boxIndex     = s.boxIndex;
        info.lava         = false;
        info.trigger      = TR::Level::Trigger::ACTIVATE;
        info.trigCmdCount = 0;

        //if (s.floor == TR::NO_FLOOR) 
        //    return;

        TR::Room::Sector *sBelow = &s;
        while (sBelow->roomBelow != TR::NO_ROOM) sBelow = &level->getSector(sBelow->roomBelow, x, z, dx, dz);
        info.floor = float(256 * sBelow->floor);

        parseFloorData(info, sBelow->floorIndex, dx, dz);

        if (info.roomNext == TR::NO_ROOM) {
            TR::Room::Sector *sAbove = &s;
            while (sAbove->roomAbove != TR::NO_ROOM) sAbove = &level->getSector(sAbove->roomAbove, x, z, dx, dz);
            if (sAbove != sBelow) {
                info.ceiling = float(256 * sAbove->ceiling);
                parseFloorData(info, sAbove->floorIndex, dx, dz);
            }
        } else {
            int tmp = info.roomNext;
            getFloorInfo(tmp, pos, info);
            info.roomNext = tmp;
        }

    // entities collide
        if (info.trigCmdCount) {
            int sx = x / 1024;
            int sz = z / 1024;
            int dx = x % 1024;
            int dz = z % 1024;

            for (int i = 0; i < info.trigCmdCount; i++) {
                TR::FloorData::TriggerCommand cmd = info.trigCmd[i];
                if (cmd.action == TR::Action::CAMERA_SWITCH) {
                    i++;
                    continue;
                }
                if (cmd.action != TR::Action::ACTIVATE) continue;
                    
                TR::Entity &e = level->entities[cmd.args];
                Controller *controller = (Controller*)e.controller;
                if (!controller) continue; // Block UpdateFloor issue while entities initialization
                if (!controller->flags.collision) continue;

                switch (e.type) {
                    case TR::Entity::TRAP_DOOR_1 :
                    case TR::Entity::TRAP_DOOR_2 : {
                        int dirX, dirZ;
                        e.getAxis(dirX, dirZ);

                        int ex = int(controller->pos.x) / 1024;
                        int ey = int(controller->pos.y);
                        int ez = int(controller->pos.z) / 1024;
                        if ((ex == sx && ez == sz) || (ex + dirX == sx && ez + dirZ == sz)) {
                            if (ey >= y - 128 && controller->pos.y < info.floor) {
                                info.floor  = controller->pos.y;
                                info.slantX = info.slantZ = 0;
                                info.lava   = false;
                            }
                            if (ey < y - 128 && controller->pos.y > info.ceiling)
                                info.ceiling = controller->pos.y + 256;
                        }
                        break;
                    }
                    case TR::Entity::TRAP_FLOOR  : {
                        if (sx != int(controller->pos.x) / 1024 || sz != int(controller->pos.z) / 1024) 
                            break;
                        int ey = int(controller->pos.y) - 512;
                        if (ey >= y - 128 && ey < info.floor) {
                            info.floor  = float(ey);
                            info.slantX = info.slantZ = 0;
                            info.lava   = false;
                        }
                        if (ey < y - 128 && ey > info.ceiling)
                            info.ceiling = float(ey);
                        break;
                    }
                    case TR::Entity::DRAWBRIDGE  : {
                        if (controller->flags.active != TR::ACTIVE) continue;
                        int dirX, dirZ;
                        e.getAxis(dirX, dirZ);
                        int ex = int(controller->pos.x) / 1024;
                        int ez = int(controller->pos.z) / 1024;

                        if ((ex - dirX * 1 == sx && ez - dirZ * 1 == sz) ||
                            (ex - dirX * 2 == sx && ez - dirZ * 2 == sz)) {
                            int ey = int(controller->pos.y);
                            if (ey >= y - 128 && controller->pos.y < info.floor) {
                                info.floor  = controller->pos.y;
                                info.slantX = info.slantZ = 0;
                                info.lava   = false;
                            }
                            if (ey < y - 128 && controller->pos.y > info.ceiling)
                                info.ceiling = controller->pos.y + 256;
                        }
                        break;
                    }
                    case TR::Entity::HAMMER_HANDLE : {
                        int dirX, dirZ;
                        e.getAxis(dirX, dirZ);
                        if (abs(int(controller->pos.x) + dirX * 1024 * 3 - x) < 512 && abs(int(controller->pos.z) + dirZ * 1024 * 3 - z) < 512)
                            info.floor -= 1024 * 3;
                        break;
                    }
                    case TR::Entity::BRIDGE_1    : 
                    case TR::Entity::BRIDGE_2    : 
                    case TR::Entity::BRIDGE_3    : {
                        if (sx != int(controller->pos.x) / 1024 || sz != int(controller->pos.z) / 1024) 
                            break;

                        int s = (e.type == TR::Entity::BRIDGE_2) ? 1 :
                                (e.type == TR::Entity::BRIDGE_3) ? 2 : 0;

                        int ey = int(controller->pos.y), sx = 0, sz = 0; 

                        if (s > 0) {
                            switch (e.rotation.value / 0x4000) { // get slantXZ by direction
                                case 0 : sx =  s; break;
                                case 1 : sz = -s; break;
                                case 2 : sx = -s; break;
                                case 3 : sz =  s; break;
                            }

                            ey -= sx * (sx > 0 ? (dx - 1024) : dx) >> 2;
                            ey -= sz * (sz > 0 ? (dz - 1024) : dz) >> 2;
                        }

                        if (y - 128 <= ey) {
                            info.floor  = float(ey);
                            info.slantX = sx;
                            info.slantZ = sz;
                            info.lava   = false;
                        }
                        if (ey  < y - 128)
                            info.ceiling = float(ey + 64);
                        break;
                    }

                    default : ;
                }
            }
        }
    }

    void parseFloorData(TR::Level::FloorInfo &info, int floorIndex, int dx, int dz) const {
        if (!floorIndex) return;

        TR::FloorData *fd = &level->floors[floorIndex];
        TR::FloorData::Command cmd;

        do {
            cmd = (*fd++).cmd;
                
            switch (cmd.func) {

                case TR::FloorData::PORTAL  :
                    info.roomNext = (*fd++).data;
                    break;

                case TR::FloorData::FLOOR   : // floor & ceiling
                case TR::FloorData::CEILING : { 
                    TR::FloorData::Slant slant = (*fd++).slant;
                    int sx = (int)slant.x;
                    int sz = (int)slant.z;
                    if (cmd.func == TR::FloorData::FLOOR) {
                        info.slantX = sx;
                        info.slantZ = sz;
                        info.floor -= sx * (sx > 0 ? (dx - 1024) : dx) >> 2;
                        info.floor -= sz * (sz > 0 ? (dz - 1024) : dz) >> 2;
                    } else {
                        info.ceiling -= sx * (sx < 0 ? (dx - 1024) : dx) >> 2; 
                        info.ceiling += sz * (sz > 0 ? (dz - 1024) : dz) >> 2; 
                    }
                    break;
                }

                case TR::FloorData::TRIGGER :  {
                    info.trigger        = (TR::Level::Trigger)cmd.sub;
                    info.trigCmdCount   = 0;
                    info.trigInfo       = (*fd++).triggerInfo;
                    TR::FloorData::TriggerCommand trigCmd;
                    do {
                        ASSERT(info.trigCmdCount < MAX_TRIGGER_COMMANDS);
                        trigCmd = (*fd++).triggerCmd; // trigger action
                        info.trigCmd[info.trigCmdCount++] = trigCmd;
                    } while (!trigCmd.end);
                    break;
                }

                case TR::FloorData::LAVA :
                    info.lava = true;
                    break;

                case TR::FloorData::CLIMB :
                    info.climb = cmd.sub; // climb mask
                    break;

                case 0x07 :
                case 0x08 :
                case 0x09 :
                case 0x0A :
                case 0x0B :
                case 0x0C :
                case 0x0D :
                case 0x0E :
                case 0x0F :
                case 0x10 :
                case 0x11 :
                case 0x12 : fd++; break; // TODO TR3 triangulation

                case 0x13 : break; // TODO TR3 monkeyswing

                case 0x14 : 
                case 0x15 : break; // TODO TR3 minecart

                default : LOG("unknown func: %d\n", cmd.func);
            }

        } while (!cmd.end);
    }

    virtual bool getSaveData(TR::SaveGame::Entity &data) {
        const TR::Entity &e = getEntity();
        const TR::Model  *m = getModel();
        if (entity < level->entitiesBaseCount) {
            data.x        = e.x ^ int32(pos.x);
            data.y        = e.y ^ int32(pos.y);
            data.z        = e.z ^ int32(pos.z);
            data.rotation = e.rotation.value ^ TR::angle(normalizeAngle(angle.y)).value;
            data.type     = 0;
            data.room     = e.room ^ roomIndex;
        } else {
            data.x        = int32(pos.x);
            data.y        = int32(pos.y);
            data.z        = int32(pos.z);
            data.rotation = TR::angle(normalizeAngle(angle.y)).value;
            data.type     = int16(e.type);
            data.room     = uint8(roomIndex);
        }

        data.flags      = e.flags.value ^ flags.value;
        data.timer      = timer == 0.0f ? 0 : (timer < 0.0f ? -1 : int16(timer * 30.0f));
    // animation
        data.animIndex  = m ? animation.index : 0;
        data.animFrame  = m ? animation.frameIndex : 0;

        data.extraSize  = 0;

        return true;
    }

    virtual void setSaveData(const TR::SaveGame::Entity &data) {
        const TR::Entity &e = getEntity();
        const TR::Model  *m = getModel();
        if (entity < level->entitiesBaseCount) {
            pos.x       = float(e.x ^ data.x);
            pos.y       = float(e.y ^ data.y);
            pos.z       = float(e.z ^ data.z);
            angle.y     = TR::angle(uint16(e.rotation.value ^ data.rotation));
            roomIndex   = e.room ^ data.room;
        } else {
            pos.x       = float(data.x);
            pos.y       = float(data.y);
            pos.z       = float(data.z);
            angle.y     = TR::angle(uint16(data.rotation));
            flags.value = data.flags;
            roomIndex   = data.room;
        }
        flags.value = e.flags.value ^ data.flags;
        timer       = data.timer == -1 ? -1.0f : (timer / 30.0f);
    // animation
        if (m) animation.setAnim(data.animIndex, -data.animFrame);
    }

    bool isActive() {
        if (flags.active != TR::ACTIVE)
            return flags.reverse;

        if (timer == 0.0f)
            return !flags.reverse;

        if (timer == -1.0f)
            return flags.reverse;

        timer = max(0.0f, timer - Core::deltaTime);

        if (timer == 0.0f)
            timer = -1.0f;

        return !flags.reverse;
    }

    virtual bool activate() {
        if (flags.state != TR::Entity::asNone)
            return false;
        flags.invisible = false;
        flags.state = TR::Entity::asActive;
        next = first;
        first = this;
        return true;
    }

    virtual void deactivate(bool removeFromList = false) {
        flags.state = TR::Entity::asInactive;
        if (removeFromList) {
            flags.state = TR::Entity::asNone;
            Controller *prev = NULL;
            Controller *c = first;
            while (c) {
                if (c == this) {
                    if (prev)
                        prev->next = c->next;
                    else
                        first = c->next;
                    break;
                } else
                    prev = c;
                c = c->next;
            }
        }
    }

    static void clearInactive() {
        Controller *prev = NULL;
        Controller *c = first;
        while (c) {
            if (c->flags.state == TR::Entity::asInactive) {
                if (prev)
                    prev->next = c->next;
                else
                    first = c->next;
                c->flags.state = TR::Entity::asNone;
            } else
                prev = c;
            c = c->next;
        }
    }

    void initMeshOverrides() {
        if (layers) return;
        layers = new MeshLayer[MAX_LAYERS];
        memset(layers, 0, sizeof(MeshLayer) * MAX_LAYERS);
        layers[0].model = getEntity().modelIndex - 1;
        layers[0].mask  = 0xFFFFFFFF;
    }

    void meshSwap(int layer, int16 model, uint32 mask = 0xFFFFFFFF) {
        if (model < 0) return;

        if (!layers) initMeshOverrides();

        TR::Model &m = level->models[model];
        for (int i = 0; i < m.mCount; i++) {
            if (((1 << i) & mask) && !level->meshOffsets[m.mStart + i] && m.mStart + i > 0)
                mask &= ~(1 << i);
        }

        layers[layer].model = model;
        layers[layer].mask  = mask;
    }

    bool aim(Controller *target, int joint, const vec4 &angleRange, quat &rot, quat *rotAbs = NULL) {
        if (target) {
            Box box = target->getBoundingBox();
            vec3 t = (box.min + box.max) * 0.5f;

            Basis b = animation.getJoints(Basis(getMatrix()), joint);
            vec3 delta = (b.inverse() * t).normal();
            if (invertAim)
                delta = -delta;

            float angleY = clampAngle(atan2f(delta.x, delta.z));
            float angleX = clampAngle(asinf(delta.y));

            if (angleX > angleRange.x && angleX <= angleRange.y &&
                angleY > angleRange.z && angleY <= angleRange.w) {

                quat ax(vec3(1, 0, 0), -angleX);
                quat ay(vec3(0, 1, 0), angleY);

                rot = ay * ax;
                if (rotAbs)
                    *rotAbs = b.rot * rot;
                return true;
            }
        }

        if (rotAbs)
            *rotAbs = rotYXZ(angle);
        return false;
    }

    bool insideRoom(const vec3 &pos, int roomIndex) const {
        TR::Room &r = level->rooms[roomIndex];
        vec3 min = vec3((float)r.info.x + 1024, (float)r.info.yTop, (float)r.info.z + 1024);
        vec3 max = min + vec3(float((r.xSectors - 2) * 1024), float(r.info.yBottom - r.info.yTop), float((r.zSectors - 2) * 1024));

        return  pos.x >= min.x && pos.x <= max.x &&
                pos.y >= min.y && pos.y <= max.y &&
                pos.z >= min.z && pos.z <= max.z;
    }

    const TR::Model* getModel() const {
        int index = getEntity().modelIndex;
        return index > 0 ? &level->models[index - 1] : NULL;
    }

    const TR::Entity& getEntity() const {
        return level->entities[entity];
    }

    TR::Room& getRoom() {
        int index = getRoomIndex();
        ASSERT(index >= 0 && index < level->roomsCount);
        return level->rooms[index];
    }

    virtual int getRoomIndex() const {
        int index = roomIndex;
        if (level->state.flags.flipped && level->rooms[index].alternateRoom > -1)
            index = level->rooms[index].alternateRoom;
        return index;
    }

    virtual vec3 getPos() {
        return getEntity().isActor() ? animation.getJoints(getMatrix(), 0).pos : pos;
    }

    vec3 getDir() const {
        return vec3(angle.x, angle.y);
    }

    static inline void applyGravity(float &speed) {
        speed += (speed < 128.0f ? GRAVITY : 30.0f) * Core::deltaTime;
    }

    bool alignToWall(float offset = 0.0f, int quadrant = -1, int maxDist = 0, int maxWidth = 0) {
        int q  = angleQuadrant(angle.y);
        int ix = int(pos.x);
        int iz = int(pos.z);
        int x  = ix & ~1023;
        int z  = iz & ~1023;

        if (quadrant > -1 && quadrant != q)
            return false;

        if (maxDist) { // check dist
            int dist = 0;
            switch (q) {
                case 0 : dist = z + 1024 - iz; break;
                case 1 : dist = x + 1024 - ix; break;
                case 2 : dist = iz - z;        break;
                case 3 : dist = ix - x;        break;
            }
            if (dist > maxDist)
                return false;
        }

        if (maxWidth) {
            int width = abs( ((q % 2) ? (iz - z) : (ix - x)) - 512);
            if (width > maxWidth)
                return false;
        }

        switch (q) {
            case 0 : pos.z = z + 1024 + offset; break;
            case 1 : pos.x = x + 1024 + offset; break;
            case 2 : pos.z = z - offset;        break;
            case 3 : pos.x = x - offset;        break;
        }

        angle.y = q * (PI * 0.5f);
        return true;
    }

    Box getBoundingBox() {
        return getBoundingBoxLocal() * getMatrix();
    }

    Box getBoundingBoxLocal(bool oriented = false) {
        return animation.getBoundingBox(vec3(0, 0, 0), oriented ? getEntity().rotation.value / 0x4000 : 0);
    }

    void getSpheres(Sphere *spheres, int &count) {
        const TR::Model *m = getModel();
        ASSERT(m->mCount <= MAX_SPHERES);

        Basis basis(getMatrix());
    // TODO: optimize (check frame index for animation updates, use joints array)
        count = 0;
        for (int i = 0; i < m->mCount; i++) {
            TR::Mesh &aMesh = level->meshes[level->meshOffsets[m->mStart + i]];
            if (aMesh.radius <= 0) continue;
            vec3 center = animation.getJoints(basis, i, true) * aMesh.center;
            spheres[count++] = Sphere(center, aMesh.radius);
        }
    }

    int collide(Controller *controller, bool checkBoxes = true) {
        const TR::Model *a = getModel();
        const TR::Model *b = controller->getModel();
        if (!a || !b) 
            return 0;

        if (checkBoxes && !getBoundingBox().intersect(controller->getBoundingBox()))
            return 0;

        ASSERT(a->mCount <= 34);
        ASSERT(b->mCount <= 34);

        Sphere aSpheres[MAX_SPHERES];
        Sphere bSpheres[MAX_SPHERES];
        int aCount, bCount;

        getSpheres(aSpheres, aCount);
        controller->getSpheres(bSpheres, bCount);

        int mask = 0;
        for (int i = 0; i < aCount; i++) 
            for (int j = 0; j < bCount; j++)
                if (bSpheres[j].intersect(aSpheres[i])) {
                    mask |= (1 << i);
                    break;
                }
        return mask;
    }

    bool collide(const Sphere &sphere) {
        return getBoundingBoxLocal().intersect(Sphere(getMatrix().inverse() * sphere.center, sphere.radius));
    }

    vec3 trace(int fromRoom, const vec3 &from, const vec3 &to, int &room, bool isCamera) { // TODO: use Bresenham
        room = fromRoom;

        vec3 pos = from, dir = to - from;
        int px = (int)pos.x, py = (int)pos.y, pz = (int)pos.z;

        float dist = dir.length();
        dir = dir * (1.0f / dist);

        int lr = -1, lx = -1, lz = -1;
        TR::Level::FloorInfo info;
        while (dist > 1.0f) {
            int sx = px / 1024 * 1024 + 512,
                sz = pz / 1024 * 1024 + 512;

            if (lr != room || lx != sx || lz != sz) {
                getFloorInfo(room, vec3(float(sx), float(py), float(sz)), info);
                if (info.roomNext != TR::NO_ROOM) {
                    room = info.roomNext;
                    getFloorInfo(room, vec3(float(sx), float(py), float(sz)), info);
                }
                lr = room;
                lx = sx;
                lz = sz;
            }

            if (isCamera) {
                if (py > info.floor && info.roomBelow != TR::NO_ROOM)
                    room = info.roomBelow;
                else if (py < info.ceiling && info.roomAbove != TR::NO_ROOM)
                    room = info.roomAbove;
                else if (py > info.floor || py < info.ceiling) {
                    int minX = px / 1024 * 1024;
                    int minZ = pz / 1024 * 1024;
                    int maxX = minX + 1024;
                    int maxZ = minZ + 1024;

                    pos = vec3(float(clamp(px, minX, maxX)), pos.y, float(clamp(pz, minZ, maxZ))) + boxNormal(px, pz) * 256.0f;
                    dir = (pos - from).normal();
                }
            } else {
                if (py > info.floor) {
                    if (info.roomBelow != TR::NO_ROOM) 
                        room = info.roomBelow;
                    else
                        break;
                }

                if (py < info.ceiling) {
                    if (info.roomAbove != TR::NO_ROOM)
                        room = info.roomAbove;
                    else
                        break;
                }
            }

            float d = min(dist, 32.0f);    // STEP = 32
            dist -= d;
            pos = pos + dir * d;

            px = (int)pos.x, py = (int)pos.y, pz = (int)pos.z;
        }

        return pos;
    }

    bool checkRange(Controller *target, float range) {
        vec3 d = target->pos - pos;
        return fabsf(d.x) < range && fabsf(d.z) < range && fabsf(d.y) < range;
    }

    virtual void hit(float damage, Controller *enemy = NULL, TR::HitType hitType = TR::HIT_DEFAULT) {}

    virtual void  doCustomCommand               (int curFrame, int prevFrame) {}
    virtual void  checkRoom()                   {}
    virtual void  applyFlow(TR::Camera &sink)   {}

    virtual void  cmdOffset(const vec3 &offset) {
        pos = pos + offset.rotateY(-angle.y);
        checkRoom();
    }

    virtual void  cmdJump(const vec3 &vel)      {}
    virtual void  cmdEmpty()                    {}

    virtual void  cmdEffect(int fx) { 
        switch (fx) {
            case TR::Effect::MESH_SWAP_1    : 
            case TR::Effect::MESH_SWAP_2    : 
            case TR::Effect::MESH_SWAP_3    : {
                if (!layers) initMeshOverrides();
                uint32 mask = (layers[1].mask == 0xFFFFFFFF) ? 0 : 0xFFFFFFFF;
                meshSwap(1, level->extra.meshSwap[fx - TR::Effect::MESH_SWAP_1], mask);
                break;
            }
            case TR::Effect::INV_ON         : flags.invisible = true;  break;
            case TR::Effect::INV_OFF        : flags.invisible = false; break;
            case TR::Effect::DYN_ON         : break; // TODO TR2
            case TR::Effect::DYN_OFF        : break; // TODO TR2
            case TR::Effect::FOOTPRINT      : break; // TODO TR3
            default : ASSERT(false);
        }
    }

    virtual void updateAnimation(bool commands) {
        animation.update();

        if (level->isCutsceneLevel() && getEntity().isActor()) {
            vec3 p = getPos();
            if ((p - lastPos).length2() > 256 * 256) {
                game->waterDrop(p, 96.0, 0.1f);
                lastPos = p;
            }
        }

        TR::Animation *anim = animation;

    // apply animation commands
        if (commands) {
            int16 *ptr = &level->commands[anim->animCommand];

            for (int i = 0; i < anim->acCount; i++) {
                int cmd = *ptr++; 
                switch (cmd) {
                    case TR::ANIM_CMD_OFFSET : ptr += 3;   break;
                    case TR::ANIM_CMD_JUMP   : ptr += 2;   break;      
                    case TR::ANIM_CMD_EMPTY  : cmdEmpty(); break;
                    case TR::ANIM_CMD_KILL   : 
                        if (animation.isEnded)
                            deactivate();
                        break;
                    case TR::ANIM_CMD_SOUND  :
                    case TR::ANIM_CMD_EFFECT : {
                        int frame = (*ptr++) - anim->frameStart;
                        int16 sfx = (*ptr++);
                        int fx    = sfx & 0x3FFF;
                        if (animation.isFrameActive(frame)) {
                            if (cmd == TR::ANIM_CMD_EFFECT) {
                                switch (fx) {
                                    case TR::Effect::ROTATE_180   : angle.y = angle.y + PI; break;
                                    case TR::Effect::FLOOR_SHAKE  : game->setEffect(this, TR::Effect(fx)); break;
                                    case TR::Effect::FINISH_LEVEL : game->loadNextLevel(); break;
                                    case TR::Effect::FLIP_MAP     : level->state.flags.flipped = !level->state.flags.flipped; break;
                                    default                       : cmdEffect(fx); break;
                                }
                            } else {
                                if (!(sfx & 0x8000)) { // TODO 0x4000 / 0x8000 for ground / water foot steps
                                    game->playSound(fx, pos, Sound::Flags::PAN);
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }

        if (animation.frameIndex != animation.framePrev)
            doCustomCommand(animation.frameIndex, animation.framePrev);

        if (animation.isEnded) { // if animation is end - switch to next
            if (animation.offset != 0.0f) cmdOffset(animation.offset);
            if (animation.jump   != 0.0f) cmdJump(animation.jump);
            animation.playNext();
        } else
            animation.framePrev = animation.frameIndex;
    }
    
    void updateExplosion() {
        if (!explodeMask) return;
        const TR::Model *model = getModel();
        for (int i = 0; i < model->mCount; i++)
            if (explodeMask & (1 << i)) {
                ExplodePart &part = explodeParts[i];
                applyGravity(part.velocity.y);

                quat q = quat(vec3(1, 0, 0), PI * Core::deltaTime) * quat(vec3(0, 0, 1), PI * 2.0f * Core::deltaTime);
                part.basis = Basis(part.basis.rot * q, part.basis.pos + part.velocity * (30.0f * Core::deltaTime));

                vec3 p = part.basis.pos;
                //TR::Room::Sector *s = level->getSector(part.roomIndex, int(p.x), int(p.y), int(p.z));
                TR::Level::FloorInfo info;
                getFloorInfo(part.roomIndex, p, info);

                if (info.roomNext != TR::NO_ROOM)
                    part.roomIndex = info.roomNext;

                bool explode = false;

                if (p.y < info.roomCeiling) {
                    if (info.roomAbove != TR::NO_ROOM)
                        part.roomIndex = info.roomAbove;
                    else {
                        if (info.roomCeiling != 0xffff8100)
                            p.y = (float)info.roomCeiling;
                        explode = true;
                    }
                }

                if (p.y > info.roomFloor) {
                    if (info.roomBelow != TR::NO_ROOM)
                        part.roomIndex = info.roomBelow;
                    else {
                        if (info.roomFloor != 0xffff8100)
                            p.y = (float)info.roomFloor;
                        explode = true;
                    }
                }

                if (explode) {
                    explodeMask &= ~(1 << i);
                    game->addEntity(TR::Entity::EXPLOSION, part.roomIndex, p);
                }
            }

        if (!explodeMask) {
            delete[] explodeParts;
            explodeParts = NULL;
        }
    }

    virtual void update() {
        updateAnimation(true);
        updateExplosion();
        updateLights(true);
    }

    void updateLights(bool lerp = true) {
        TR::Room::Light sunLight;

        if (getModel()) {
            const TR::Room &room = getRoom();

            vec3 center = getBoundingBox().center();
            float maxAtt = 0.0f;
            /*
            if (room.flags.sky) { // TODO trace rooms up for sun light, add direct light projection
                sunLight.x      = int32(center.x);
                sunLight.y      = int32(center.y) - 8192;
                sunLight.z      = int32(center.z);
                sunLight.color  = TR::Color32(255, 255, 255, 255);
                sunLight.radius = 1000 * 1024;
                targetLight     = &sunLight;
            } else {
            */
            {
                for (int i = 0; i < room.lightsCount; i++) {
                    TR::Room::Light &light = room.lights[i];
                    if ((light.color.r | light.color.g | light.color.b) == 0) continue;

                    vec3 dir = vec3(float(light.x), float(light.y), float(light.z)) - center;
                    float att = max(0.0f, 1.0f - dir.length2() / float(light.radius) / float(light.radius)) * ((light.color.r + light.color.g + light.color.b) / (3.0f * 255.0f));

                    if (att > maxAtt) {
                        maxAtt = att;
                        targetLight = &light;
                    }
                }
            }
        } else 
            targetLight = NULL;

        if (targetLight == NULL) {
            mainLightPos   = vec3(0);
            mainLightColor = vec4(0, 0, 0, 1);
            return;
        }

        vec3 tpos   = vec3(float(targetLight->x), float(targetLight->y), float(targetLight->z));
        vec4 tcolor = vec4(vec3(targetLight->color.r, targetLight->color.g, targetLight->color.b) * (1.0f / 255.0f), float(targetLight->radius));

        if (lerp) {
            float t = Core::deltaTime * 2.0f;
            mainLightPos   = mainLightPos.lerp(tpos, t);
            mainLightColor = mainLightColor.lerp(tcolor, t);
        } else {
            mainLightPos   = tpos;
            mainLightColor = tcolor;
        }
    }

    mat4 getMatrix() {
        if (level->isCutsceneLevel() && (getEntity().isActor() || getEntity().isLara())) 
            return level->cutMatrix;

        mat4 matrix;
        matrix.identity();
        matrix.translate(pos);
        if (angle.y != 0.0f) matrix.rotateY(angle.y - (animation.flip ? PI * animation.delta : 0.0f));
        if (angle.x != 0.0f) matrix.rotateX(angle.x);
        if (angle.z != 0.0f) matrix.rotateZ(angle.z);
        return matrix;
    }

    void explode(int32 mask) {
        const TR::Model *model = getModel();

        if (!layers) initMeshOverrides();

        mask &= layers[0].mask;
        layers[0].mask &= ~mask;
        
        explodeParts = new ExplodePart[model->mCount];
        explodeMask  = 0;
       
        animation.getJoints(getMatrix(), -1, true, joints);
        int roomIndex = getRoomIndex();
        for (int i = 0; i < model->mCount; i++) {
            if (!(mask & (1 << i)))
                continue;
            explodeMask |= (1 << i);
            float angle = randf() * PI * 2.0f;
            float speed = randf() * 256.0f;

            ExplodePart &part = explodeParts[i];
            part.basis     = joints[i];
            part.basis.w   = 1.0f;
            part.velocity  = vec3(cosf(angle), (randf() - 0.5f) * 0.25f, sinf(angle)) * speed;
            part.roomIndex = roomIndex;
        }
    }

    void renderShadow(MeshBuilder *mesh) {
        if (Core::pass != Core::passCompose || level->isCutsceneLevel())
            return;

        Box boxL = getBoundingBoxLocal();
        Box boxA = boxL * getMatrix();

        vec3 center = boxA.center();

        TR::Level::FloorInfo info;
        getFloorInfo(getRoomIndex(), center, info);

        const vec3 size = boxL.size() * (1.0f / 1024.0f);

        vec3 dir   = getDir();
        vec3 up    = info.getNormal();
        vec3 right = dir.cross(up).normal();
        dir = up.cross(right).normal();

        mat4 m;
        m.identity();
        m.dir    = vec4(dir * size.z, 0.0f);
        m.up     = vec4(up, 0.0f);
        m.right  = vec4(right * size.x, 0.0f);
        m.offset = vec4(center.x, info.floor - 8.0f, center.z, 1.0f);

        Basis b;
        b.identity();

        game->setShader(Core::pass, Shader::FLASH, false, false);
        Core::active.shader->setParam(uViewProj, Core::mViewProj * m);
        Core::active.shader->setParam(uBasis, b);
        float alpha = lerp(0.7f, 0.90f, clamp((info.floor - boxA.max.y) / 1024.0f, 0.0f, 1.0f) );
        Core::active.shader->setParam(uMaterial, vec4(vec3(0.5f * (1.0f - alpha)), alpha));
        Core::active.shader->setParam(uAmbient, vec3(0.0f));

        Core::setDepthWrite(false);
        mesh->renderShadowBlob();
        Core::setDepthWrite(true);
    }

    virtual void render(Frustum *frustum, MeshBuilder *mesh, Shader::Type type, bool caustics) { // TODO: animation.calcJoints
        mat4 matrix = getMatrix();

        Box box = animation.getBoundingBox(vec3(0, 0, 0), 0);
        if (!explodeMask && frustum && !frustum->isVisible(matrix, box.min, box.max))
            return;

        const TR::Model *model = getModel();
        ASSERT(model);

        flags.rendered = true;

        if (Core::stats.frame != frameIndex)
            animation.getJoints(matrix, -1, true, joints);

        if (layers) {
            uint32 mask = 0;

            for (int i = MAX_LAYERS - 1; i >= 0; i--) {
                uint32 vmask = (layers[i].mask & ~mask) | (!i ? explodeMask : 0);
                vmask &= visibleMask;
                if (!vmask) continue;
                mask |= layers[i].mask;
            // set meshes visibility
                for (int j = 0; j < model->mCount; j++)
                    joints[j].w = (vmask & (1 << j)) ? 1.0f : -1.0f; // AHAHA

                if (explodeMask) {
                    ASSERT(explodeParts);
                    const TR::Model *model = getModel();
                    for (int i = 0; i < model->mCount; i++)
                        if (explodeMask & (1 << i))
                            joints[i] = explodeParts[i].basis;
                }

            //    if (entity.type == TR::Entity::LARA && Core::eye != 0)
            //        joints[14].w = -1.0f;
            // render
                Core::active.shader->setParam(uBasis, joints[0], model->mCount);
                mesh->renderModel(layers[i].model);
            }
        } else {
            Core::active.shader->setParam(uBasis, joints[0], model->mCount);
            mesh->renderModel(getEntity().modelIndex - 1);
        }

        frameIndex = Core::stats.frame;
    }
};

Controller *Controller::first = NULL;

#endif