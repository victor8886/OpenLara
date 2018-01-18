#ifndef H_LARA
#define H_LARA
/*****************************************/
/*     Desine sperare qui hic intras     */
/*****************************************/
#include "character.h"
#include "trigger.h"
#include "sprite.h"
#include "enemy.h"

#define TURN_FAST           PI
#define TURN_FAST_BACK      PI * 3.0f / 4.0f
#define TURN_NORMAL         PI / 2.0f
#define TURN_SLOW           PI / 3.0f
#define TURN_WATER_FAST     (DEG2RAD * 150.0f)
#define TURN_WATER_SLOW     (DEG2RAD * 60.0f)
#define TURN_WALL_Y         (DEG2RAD * 150.0f)
#define TURN_WALL_X         (DEG2RAD * 60.0f)
#define TURN_WALL_X_CLAMP   (DEG2RAD * 35.0f)

#define LARA_TILT_SPEED     (DEG2RAD * 37.5f)
#define LARA_TILT_MAX       (DEG2RAD * 10.0f)

#define LARA_MAX_HEALTH     1000.0f
#define LARA_MAX_OXYGEN     60.0f

#define LARA_HANG_OFFSET    724
#define LARA_HEIGHT         762
#define LARA_HEIGHT_WATER   400
#define LARA_RADIUS         100.0f
#define LARA_RADIUS_WATER   300.0f

#define LARA_WATER_ACCEL    2.0f
#define LARA_SURF_SPEED     15.0f
#define LARA_SWIM_SPEED     50.0f
#define LARA_SWIM_FRICTION  1.0f

#define LARA_WET_SPECULAR   0.5f
#define LARA_WET_TIMER      (LARA_WET_SPECULAR / 16.0f)   // 4 sec

#define LARA_DAMAGE_TIME    (40.0f / 30.0f)

#define PICKUP_FRAME_GROUND     40
#define PICKUP_FRAME_UNDERWATER 18
#define PUZZLE_FRAME            80
#define KEY_FRAME               110

#define MAX_TRIGGER_ACTIONS 64

#define DESCENT_SPEED       2048.0f
#define MUZZLE_FLASH_TIME   0.1f
#define FLASH_LIGHT_COLOR   vec4(0.6f, 0.5f, 0.1f, 1.0f / 3072.0f)
#define TARGET_MAX_DIST     (8.0f * 1024.0f)

struct Lara : Character {

    // http://www.tombraiderforums.com/showthread.php?t=148859
    enum {
        ANIM_STAND_LEFT         = 2,
        ANIM_STAND_RIGHT        = 3,

        ANIM_RUN_START          = 6,

        ANIM_STAND              = 11,

        ANIM_CLIMB_JUMP         = 26,

        ANIM_FALL_HANG          = 28,

        ANIM_SMASH_JUMP         = 32,

        ANIM_FALL_FORTH         = 34,

        ANIM_CLIMB_3            = 42,

        ANIM_CLIMB_2            = 50,

        ANIM_SMASH_RUN_LEFT     = 53,
        ANIM_SMASH_RUN_RIGHT    = 54,
        ANIM_RUN_ASCEND_LEFT    = 55,
        ANIM_RUN_ASCEND_RIGHT   = 56,
        ANIM_WALK_ASCEND_LEFT   = 57,
        ANIM_WALK_ASCEND_RIGHT  = 58,
        ANIM_WALK_DESCEND_RIGHT = 59,
        ANIM_WALK_DESCEND_LEFT  = 60,
        ANIM_BACK_DESCEND_LEFT  = 61,
        ANIM_BACK_DESCEND_RIGHT = 62,

        ANIM_SLIDE_FORTH        = 70,

        ANIM_FALL_BACK          = 93,

        ANIM_HANG               = 96,

        ANIM_STAND_NORMAL       = 103,

        ANIM_SLIDE_BACK         = 105,

        ANIM_UNDERWATER         = 108,

        ANIM_WATER_FALL         = 112,
        ANIM_TO_ONWATER         = 114,
        ANIM_TO_UNDERWATER      = 119,
        ANIM_HIT_FRONT          = 125,
        ANIM_HIT_BACK           = 126,
        ANIM_HIT_LEFT           = 127,
        ANIM_HIT_RIGHT          = 128,

        ANIM_DEATH_BOULDER      = 139,

        ANIM_STAND_ROLL_BEGIN   = 146,
        ANIM_STAND_ROLL_END     = 147,

        ANIM_DEATH_SPIKES       = 149,
        ANIM_HANG_SWING         = 150,

        ANIM_SWITCH_BIG_DOWN    = 195,
        ANIM_SWITCH_BIG_UP      = 196,
        ANIM_PUSH_BUTTON        = 197,
    };

    // http://www.tombraiderforums.com/showthread.php?t=211681
    enum {
        STATE_WALK,
        STATE_RUN,
        STATE_STOP,
        STATE_FORWARD_JUMP,
        STATE_POSE,
        STATE_FAST_BACK,
        STATE_TURN_RIGHT,
        STATE_TURN_LEFT,
        STATE_DEATH,
        STATE_FALL,
        STATE_HANG,
        STATE_REACH,
        STATE_SPLAT,
        STATE_TREAD,
        STATE_FAST_TURN_14,
        STATE_COMPRESS,
        STATE_BACK,
        STATE_SWIM,
        STATE_GLIDE,
        STATE_HANG_UP,
        STATE_FAST_TURN,
        STATE_STEP_RIGHT,
        STATE_STEP_LEFT,
        STATE_ROLL_1,
        STATE_SLIDE,
        STATE_BACK_JUMP,
        STATE_RIGHT_JUMP,
        STATE_LEFT_JUMP,
        STATE_UP_JUMP,
        STATE_FALL_BACK,
        STATE_HANG_LEFT,
        STATE_HANG_RIGHT,
        STATE_SLIDE_BACK,
        STATE_SURF_TREAD,
        STATE_SURF_SWIM,
        STATE_DIVE,
        STATE_PUSH_BLOCK,
        STATE_PULL_BLOCK,
        STATE_PUSH_PULL_READY,
        STATE_PICK_UP,
        STATE_SWITCH_DOWN,
        STATE_SWITCH_UP,
        STATE_USE_KEY,
        STATE_USE_PUZZLE,
        STATE_UNDERWATER_DEATH,
        STATE_ROLL_2,
        STATE_SPECIAL,
        STATE_SURF_BACK,
        STATE_SURF_LEFT,
        STATE_SURF_RIGHT,
        STATE_MIDAS_USE,
        STATE_MIDAS_DEATH,
        STATE_SWAN_DIVE,
        STATE_FAST_DIVE,
        STATE_HANDSTAND,
        STATE_WATER_OUT,
        STATE_MAX };

    enum : int {
        BODY_HIP        = 0x0001,
        BODY_LEG_L1     = 0x0002,
        BODY_LEG_L2     = 0x0004,
        BODY_LEG_L3     = 0x0008,
        BODY_LEG_R1     = 0x0010,
        BODY_LEG_R2     = 0x0020,
        BODY_LEG_R3     = 0x0040,
        BODY_CHEST      = 0x0080,
        BODY_ARM_R1     = 0x0100,
        BODY_ARM_R2     = 0x0200,
        BODY_ARM_R3     = 0x0400,
        BODY_ARM_L1     = 0x0800,
        BODY_ARM_L2     = 0x1000,
        BODY_ARM_L3     = 0x2000,
        BODY_HEAD       = 0x4000,
        BODY_ARM_L      = BODY_ARM_L1 | BODY_ARM_L2 | BODY_ARM_L3,
        BODY_ARM_R      = BODY_ARM_R1 | BODY_ARM_R2 | BODY_ARM_R3,
        BODY_LEG_L      = BODY_LEG_L1 | BODY_LEG_L2 | BODY_LEG_L3,
        BODY_LEG_R      = BODY_LEG_R1 | BODY_LEG_R2 | BODY_LEG_R3,
        BODY_UPPER      = BODY_CHEST  | BODY_ARM_L  | BODY_ARM_R,       // without head
        BODY_LOWER      = BODY_HIP    | BODY_LEG_L  | BODY_LEG_R,
        BODY_BRAID_MASK = BODY_HEAD   | BODY_CHEST  | BODY_ARM_L1 | BODY_ARM_L2 | BODY_ARM_R1 | BODY_ARM_R2,
    };

    bool dozy;

    struct Weapon {
        enum Type  { EMPTY = -1, PISTOLS, SHOTGUN, MAGNUMS, UZIS, MAX };
        enum State { IS_HIDDEN, IS_ARMED, IS_FIRING };
        enum Anim  { NONE, PREPARE, UNHOLSTER, HOLSTER, HOLD, AIM, FIRE };
    };

    Weapon::Type    wpnCurrent;
    Weapon::Type    wpnNext;
    Weapon::State   wpnState;
    int             *wpnAmmo;
    vec3            chestOffset;

    struct Arm {
        Controller      *tracking;       // tracking target (main target)
        Controller      *target;         // target for shooting
        float           shotTimer;
        quat            rot, rotAbs;
        Weapon::Anim    anim;
        Animation       animation;

        Arm() : tracking(NULL), target(NULL) {}
    } arms[2];

    TR::Entity::Type  usedKey;
    int               pickupListCount;
    Controller        *pickupList[32];
    KeyHole           *keyHole;
    Lightning         *lightning;
    Texture           *environment;
    vec2              rotFactor;

    float       oxygen;
    float       damageTime;
    float       hitTime;
    int         hitDir;
    vec3        collisionOffset;
    vec3        flowVelocity;
#ifdef _DEBUG
    //uint16      *dbgBoxes;
    //int         dbgBoxesCount;
#endif
    struct Braid {
        Lara *lara;
        vec3 offset;

        Basis *basis;
        struct Joint {
            vec3 posPrev, pos;
            float length;
        } *joints;
        int jointsCount;
        float time;

        Braid(Lara *lara, const vec3 &offset) : lara(lara), offset(offset), time(0.0f) {
            TR::Level *level = lara->level;
            TR::Model *model = getModel();
            jointsCount = model->mCount + 1;
            joints      = new Joint[jointsCount];
            basis       = new Basis[jointsCount - 1];

            Basis basis = getBasis();
            basis.translate(offset);

            TR::Node *node = (int)model->node < level->nodesDataSize ? (TR::Node*)&level->nodesData[model->node] : NULL;
            for (int i = 0; i < jointsCount - 1; i++) {
                TR::Node &t = node[min(i, model->mCount - 2)];
                joints[i].posPrev = joints[i].pos = basis.pos;
                joints[i].length  = float(t.z);
                basis.translate(vec3(0.0f, 0.0f, -joints[i].length));
            }
            joints[jointsCount - 1].posPrev = joints[jointsCount - 1].pos = basis.pos;
            joints[jointsCount - 1].length  = 1.0f;
        }

        ~Braid() {
            delete[] joints;
            delete[] basis;
        }

        TR::Model* getModel() const {
            return &lara->level->models[lara->level->extra.braid];
        }

        Basis getBasis() {
            return lara->animation.getJoints(lara->getMatrix(), 14, true);
        }

        vec3 getPos() {
            return getBasis() * offset;
        }

        void integrate() {
            float TIMESTEP = Core::deltaTime;
            float ACCEL    = 16.0f * GRAVITY * TIMESTEP * TIMESTEP;
            float DAMPING  = 1.5f;

            if (lara->getRoom().flags.water) {
                ACCEL *= -0.5f;
                DAMPING = 4.0f;
            }

            DAMPING = 1.0f / (1.0f + DAMPING * TIMESTEP); // Pade approximation

            for (int i = 1; i < jointsCount; i++) {
                Joint &j = joints[i];
                vec3 delta = j.pos - j.posPrev;
                delta = delta.normal() * (min(delta.length(), 2048.0f * Core::deltaTime) * DAMPING); // speed limit
                j.posPrev  = j.pos;
                j.pos     += delta;
                if (lara->stand == STAND_ONWATER) {
                    if (j.pos.y > lara->pos.y)
                        j.pos.y += ACCEL;
                    else
                        j.pos.y -= ACCEL;
                } else
                    j.pos.y += ACCEL;
            }
        }

        void collide() {
            TR::Level *level = lara->level;
            const TR::Model *model = lara->getModel();

            TR::Level::FloorInfo info;
            lara->getFloorInfo(lara->getRoomIndex(), lara->getViewPoint(), info);

            for (int j = 1; j < jointsCount; j++)
                if (joints[j].pos.y > info.floor)
                    joints[j].pos.y = info.floor;

            #define BRAID_RADIUS 16.0f

            for (int i = 0; i < model->mCount; i++) {
                if (!(BODY_BRAID_MASK & (1 << i))) continue;

                int offset = level->meshOffsets[model->mStart + i];
                TR::Mesh *mesh = (TR::Mesh*)&level->meshes[offset];

                vec3 center    = lara->animation.getJoints(lara->getMatrix(), i, true) * mesh->center;
                float radiusSq = mesh->radius + BRAID_RADIUS;
                radiusSq *= radiusSq;

                for (int j = 1; j < jointsCount; j++) {
                    vec3 dir = joints[j].pos - center;
                    float len = dir.length2() + EPS;
                    if (len < radiusSq) {
                        len = sqrtf(len);
                        dir *= (mesh->radius + BRAID_RADIUS- len) / len;
                        joints[j].pos += dir * 0.9f;
                    }
                }
            }

            #undef BRAID_RADIUS
        }

        void solve() {
            for (int i = 0; i < jointsCount - 1; i++) {
                Joint &a = joints[i];
                Joint &b = joints[i + 1];

                vec3 dir = b.pos - a.pos;
                float len = dir.length() + EPS;
                dir *= 1.0f / len;

                float d = a.length - len;

                if (i > 0) {
                    dir *= d * (0.5f * 1.0f);
                    a.pos -= dir;
                    b.pos += dir;
                } else
                    b.pos += dir * (d * 1.0f);
            }
        }

        void update() {
            joints[0].pos = getPos();
            integrate(); // Verlet integration step
            collide();   // check collision with Lara's mesh
            for (int i = 0; i < jointsCount; i++) // solve connections (springs)
                solve();

            vec3 headDir = getBasis().rot * vec3(0.0f, 0.0f, -1.0f);

            for (int i = 0; i < jointsCount - 1; i++) {
                vec3 d = (joints[i + 1].pos - joints[i].pos).normal();
                vec3 r = d.cross(headDir).normal();
                vec3 u = d.cross(r).normal();

                mat4 m;
                m.up     = vec4(u, 0.0f);
                m.dir    = vec4(d, 0.0f);
                m.right  = vec4(r, 0.0f);
                m.offset = vec4(0.0f, 0.0f, 0.0f, 1.0f);

                basis[i].identity();
                basis[i].translate(joints[i].pos);
                basis[i].rotate(m.getRot());
            }
        }
        
        void render(MeshBuilder *mesh) {
            Core::active.shader->setParam(uBasis, basis[0], jointsCount - 1);
            mesh->renderModel(lara->level->extra.braid);
        }

    } *braid;

    Lara(IGame *game, int entity) : Character(game, entity, LARA_MAX_HEALTH), dozy(false), wpnCurrent(Weapon::EMPTY), wpnNext(Weapon::EMPTY), chestOffset(pos), braid(NULL) {
        if (level->extra.laraSkin > -1)
            level->entities[entity].modelIndex = level->extra.laraSkin + 1;

        jointChest = 7;
        jointHead  = 14;
        rangeChest = vec4(-0.40f, 0.40f, -0.90f, 0.90f) * PI;
        rangeHead  = vec4(-0.25f, 0.25f, -0.50f, 0.50f) * PI;

        oxygen     = LARA_MAX_OXYGEN;
        hitDir     = -1;
        damageTime = LARA_DAMAGE_TIME;
        hitTime    = 0.0f;

        keyHole     = NULL;
        lightning   = NULL;
        environment = NULL;

        flags.active = 1;
        initMeshOverrides();

        if (level->isHome()) {
            if (level->version & TR::VER_TR1)
                meshSwap(1, TR::MODEL_LARA_SPEC, BODY_UPPER | BODY_LOWER);
        } else {
            if (level->id == TR::LVL_TR2_HOUSE)
                wpnSet(Weapon::SHOTGUN);
            else
                wpnSet(Weapon::PISTOLS);
        }

        for (int i = 0; i < 2; i++) {
            arms[i].shotTimer = MUZZLE_FLASH_TIME + 1.0f;
            arms[i].rot       = quat(0, 0, 0, 1);
            arms[i].rotAbs    = quat(0, 0, 0, 1);
        }

        if (level->extra.braid > -1)
            braid = new Braid(this, (level->version & (TR::VER_TR2 | TR::VER_TR3)) ? vec3(-2.0f, -16.0f, -48.0f) : vec3(-4.0f, 24.0f, -48.0f));
    #ifdef _DEBUG
        //reset(14, vec3(40448, 3584, 60928), PI * 0.5f, STAND_ONWATER);  // gym (pool)
        //reset(0, vec3(74858, 3072, 20795), 0);           // level 1 (dart)
        //reset(14, vec3(20215, 6656, 52942), PI);         // level 1 (bridge)
        //reset(20, vec3(8952, 3840, 68071), PI);          // level 1 (crystal)
        //reset(26, vec3(24475, 6912, 83505), 90 * DEG2RAD);     // level 1 (switch timer)
        //reset(33, vec3(48229, 4608, 78420), 270 * DEG2RAD);     // level 1 (end)
        //reset(9, vec3(63008, 0, 37787), 0);              // level 2 (switch)
        //reset(15, vec3(70067, -256, 29104), -0.68f);     // level 2 (pool)
        //reset(26, vec3(71980, 1546, 19000), 270 * DEG2RAD);     // level 2 (underwater switch)
        //reset(61, vec3(27221, -1024, 29205), -PI * 0.5f); // level 2 (blade)
        //reset(43, vec3(31400, -2560, 25200), PI);        // level 2 (reach)
        //reset(16, vec3(60907, 0, 39642), PI * 3 / 2);    // level 2 (hang & climb)
        //reset(19, vec3(60843, 1024, 30557), PI);         // level 2 (block)
        //reset(1,  vec3(62630, -1280, 19633), 0);         // level 2 (dark medikit)
        //reset(7,  vec3(64108, -512, 16514), -PI * 0.5f); // level 2 (bat trigger)
        //reset(15, vec3(70082, -512, 26935), PI * 0.5f);  // level 2 (bear)
        //reset(63, vec3(31390, -2048, 33472), 0.0f);      // level 2 (trap floor)
        //reset(61, vec3(21987, -1024, 29144), PI * 3.0f * 0.5f); // level 2 (trap door)
        //reset(51, vec3(41015, 3584, 34494), -PI);        // level 3a (t-rex)
        //reset(5,  vec3(38643, -3072, 92370), PI * 0.5f); // level 3a (gears)
        //reset(43, vec3(64037, 6656, 48229), PI);         // level 3b (movingblock)
        //reset(27, vec3(72372, 8704, 46547), PI * 0.5f);  // level 3b (spikes)
        //reset(5, vec3(73394, 3840, 60758), 0);           // level 3b (scion)
        //reset(20, vec3(57724, 6656, 61941), 90 * DEG2RAD); // level 3b (boulder)
        //reset(18, vec3(34914, 11008, 41315), 90 * DEG2RAD); // level 4 (main hall)
        //reset(19, vec3(33368, 19968, 45643), 270 * DEG2RAD); // level 4 (damocles)
        //reset(24, vec3(45609, 18176, 41500), 90 * DEG2RAD); // level 4 (thor)
        //reset(19, vec3(41418, -3707, 58863), 270 * DEG2RAD);  // level 5 (triangle)
        //reset(21, vec3(24106, -4352, 52089), 0);              // level 6 (flame traps)
        //reset(73, vec3(73372, 122, 51687), PI * 0.5f);       // level 6 (midas hand)
        //reset(64, vec3(36839, -2560, 48769), 270 * DEG2RAD);  // level 6 (flipmap effect)
        //reset(99,  vec3(45562, -3328, 63366), 225 * DEG2RAD); // level 7a (flipmap)
        //reset(77,  vec3(36943, -4096, 62821), 270 * DEG2RAD); // level 7b (heavy trigger)
        //reset(90,  vec3(19438, 3840, 78341), 90 * DEG2RAD); // level 7b (statues)
        //reset(90,  vec3(29000, 3840 - 512, 78341), 90 * DEG2RAD); // level 7b (statues)
        //reset(57,  vec3(54844, -3328, 53145), 0);        // level 8b (bridge switch)
        //reset(12,  vec3(34236, -2415, 14974), 0);        // level 8b (sphinx)
        //reset(0,  vec3(40913, -1012, 42252), PI);        // level 8c
        //reset(56,  vec3(18541, 512, 52869), PI * 0.5f);        // level 8c
        //reset(30, vec3(69689, -8448, 34922), 330 * DEG2RAD);      // Level 10a (cabin)
        //reset(27, vec3(52631, -4352, 57893), 270 * DEG2RAD);      // Level 10a (TNT / Cowboy)
        //reset(68, vec3(52458, -9984, 93724), 270 * DEG2RAD);      // Level 10a (MrT)
        //reset(44, vec3(75803, -11008, 21097), 90 * DEG2RAD);      // Level 10a (boat)
        //reset(47, vec3(50546, -13056, 53783), 270 * DEG2RAD);      // Level 10b (trap door slope)
        //reset(59, vec3(42907, -13056, 63012), 270 * DEG2RAD);      // Level 10b (doppelganger)
        //reset(50, vec3(52122, -18688, 47313), 150 * DEG2RAD);      // Level 10b (scion holder pickup)
        //reset(50, vec3(53703, -18688, 13769), PI);                // Level 10c (scion holder)
        //reset(19, vec3(35364, -512, 40199), PI * 0.5f);           // Level 10c (lava flow)
        //reset(9, vec3(69074, -14592, 25192), 0);                  // Level 10c (trap slam)
        //reset(21, vec3(47668, -10752, 32163), 0);                 // Level 10c (lava emitter)
        //reset(10, vec3(90443, 11264 - 256, 114614), PI, STAND_ONWATER);   // villa mortal 2
        //dbgBoxes = NULL;
    #endif

        if (!level->isCutsceneLevel()) {
            if (getRoom().flags.water) {
                stand = STAND_UNDERWATER;
                animation.setAnim(ANIM_UNDERWATER);
            } else
                animation.setAnim(ANIM_STAND);
        }

        chestOffset = animation.getJoints(getMatrix(), jointChest).pos;
    }

    virtual ~Lara() {
        delete braid;
        delete environment;
    }

    bool canSaveGame() {
        return health > 0.0f && (state == STATE_STOP || state == STATE_TREAD || state == STATE_SURF_TREAD);
    }

    virtual bool getSaveData(TR::SaveGame::Entity &data) {
        Character::getSaveData(data);
        data.extraSize = sizeof(data.extra.lara);
        data.extra.lara.velX        = velocity.x;
        data.extra.lara.velY        = velocity.y;
        data.extra.lara.velZ        = velocity.z;
        data.extra.lara.angleX      = angle.x;
        data.extra.lara.health      = health;
        data.extra.lara.oxygen      = oxygen;
        data.extra.lara.stamina     = 0.0f;
        data.extra.lara.poison      = 0.0f;
        data.extra.lara.freeze      = 0.0f;
        data.extra.lara.itemHands   = TR::Entity::LARA;
        data.extra.lara.itemBack    = TR::Entity::SHOTGUN;
        data.extra.lara.itemHolster = TR::Entity::PISTOLS;
        data.extra.lara.flags.value = 0;
        data.extra.lara.flags.burn  = 0; // TODO
        data.extra.lara.flags.wet   = 0; // TODO
        return true;
    }

    virtual void setSaveData(const TR::SaveGame::Entity &data) {
        Character::setSaveData(data);
        velocity = vec3(data.extra.lara.velX, data.extra.lara.velY, data.extra.lara.velZ);
        angle.x  = TR::angle(data.extra.lara.angleX);
        health   = data.extra.lara.health;
        oxygen   = data.extra.lara.oxygen;

        layers[1].mask = layers[2].mask = layers[3].mask = 0;
        wpnState   = Weapon::IS_HIDDEN;
        wpnCurrent = Weapon::EMPTY;
/*
        wpnSet(Weapon::Type(data.extra.lara.curWeapon));
        if (!data.extra.lara.emptyHands)
            wpnDraw(true);
*/
    }

    int getRoomByPos(const vec3 &pos) {
        int x = int(pos.x),
            y = int(pos.y),
            z = int(pos.z);

        for (int i = 0; i < level->roomsCount; i++) {
            TR::Room &r = level->rooms[i];
            int mx = r.info.x + r.xSectors * 1024;
            int mz = r.info.z + r.zSectors * 1024;
            if (x >= r.info.x && x < mx && z >= r.info.z && z < mz && y >= r.info.yTop && y < r.info.yBottom)
                return i;
        }
        return TR::NO_ROOM;
    }

    void reset(int room, const vec3 &pos, float angle, Stand forceStand = STAND_GROUND) {
        visibleMask = 0xFFFFFFFF;

        if (room == TR::NO_ROOM) {
            stand = STAND_AIR;
            room  = getRoomByPos(pos);
        }

        if (room == TR::NO_ROOM)
            return;

        if (level->rooms[room].flags.water) {
            stand = STAND_UNDERWATER;
            animation.setAnim(ANIM_UNDERWATER);
        } else {
            stand = STAND_GROUND;
            animation.setAnim(ANIM_STAND);
        }

        velocity = vec3(0.0f);

        roomIndex   = room;
        this->pos   = pos;
        this->angle = vec3(0.0f, angle, 0.0f);

        if (forceStand != STAND_GROUND) {
            stand = forceStand;
            switch (stand) {
                case STAND_ONWATER    : animation.setAnim(ANIM_TO_ONWATER); break;
                case STAND_UNDERWATER : animation.setAnim(ANIM_UNDERWATER); break;
                default               : ;
            }
        }

        updateLights(false);
    }

    TR::Entity::Type getCurrentWeaponInv() {
        switch (wpnCurrent) {
            case Weapon::Type::PISTOLS : return TR::Entity::PISTOLS;
            case Weapon::Type::SHOTGUN : return TR::Entity::SHOTGUN;
            case Weapon::Type::MAGNUMS : return TR::Entity::MAGNUMS;
            case Weapon::Type::UZIS    : return TR::Entity::UZIS;
            default                    : return TR::Entity::LARA;
        }
    }

    void wpnSet(Weapon::Type wType) {
        wpnCurrent = wType;
        wpnState   = Weapon::IS_FIRING;

        TR::Entity::Type invType = getCurrentWeaponInv();

        wpnAmmo = game->invCount(invType);

        arms[0].animation = arms[1].animation = Animation(level, &level->models[wType == Weapon::SHOTGUN ? TR::MODEL_SHOTGUN : TR::MODEL_PISTOLS]);

        wpnSetAnim(arms[0], Weapon::IS_HIDDEN, Weapon::Anim::NONE, 0.0f, 0.0f);
        wpnSetAnim(arms[1], Weapon::IS_HIDDEN, Weapon::Anim::NONE, 0.0f, 0.0f);
    }

    void wpnSetAnim(Arm &arm, Weapon::State wState, Weapon::Anim wAnim, float wAnimTime, float wAnimDir, bool playing = true) {
        arm.animation.setAnim(wpnGetAnimIndex(wAnim), 0, wAnim == Weapon::Anim::FIRE);
        arm.animation.dir = playing ? wAnimDir : 0.0f;

        if (arm.anim != wAnim)
            arm.animation.frameIndex = 0xFFFF;

        arm.anim = wAnim;

        if (wAnimDir > 0.0f)
            arm.animation.time = wAnimTime;
        else
            if (wAnimDir < 0.0f)
                arm.animation.time = arm.animation.timeMax + wAnimTime;
        arm.animation.updateInfo();

        wpnSetState(wState);
    }

    float wpnGetDamage() {
        switch (wpnCurrent) {
            case Weapon::PISTOLS : return 1;
            case Weapon::SHOTGUN : return 1;
            case Weapon::MAGNUMS : return 2;
            case Weapon::UZIS    : return 1;
            default : ;
        }
        return 0;
    }

    void wpnSetState(Weapon::State wState) {
        if (wpnState == wState || !layers) return;

        int mask = 0;
        switch (wpnCurrent) {
            case Weapon::EMPTY   : break;
            case Weapon::PISTOLS :
            case Weapon::MAGNUMS :
            case Weapon::UZIS    :
                switch (wState) {
                    case Weapon::IS_HIDDEN : mask = BODY_LEG_L1 | BODY_LEG_R1;              break;
                    case Weapon::IS_ARMED  : mask = BODY_ARM_L3 | BODY_ARM_R3;              break;
                    case Weapon::IS_FIRING : mask = BODY_ARM_L3 | BODY_ARM_R3 | BODY_HEAD;  break;
                }
                break;
            case Weapon::SHOTGUN :
                switch (wState) {
                    case Weapon::IS_HIDDEN : mask = BODY_CHEST;                             break;
                    case Weapon::IS_ARMED  : mask = BODY_ARM_L3 | BODY_ARM_R3;              break;
                    case Weapon::IS_FIRING : mask = BODY_ARM_L3 | BODY_ARM_R3 | BODY_HEAD;  break;
                }
                break;
            default : ;
        }

        if (wpnState == Weapon::IS_HIDDEN && wState == Weapon::IS_ARMED)  game->playSound(TR::SND_UNHOLSTER, pos, Sound::Flags::PAN);
        if (wpnState == Weapon::IS_ARMED  && wState == Weapon::IS_HIDDEN) game->playSound(TR::SND_HOLSTER,   pos, Sound::Flags::PAN);

    // swap layers
    // 0 - body (full)
    // 1 - legs (hands, legs)
    // 2 - shotgun (hands, chest)
    // 3 - angry (head)

        // swap weapon parts
        if (wpnCurrent != Weapon::SHOTGUN) {
            meshSwap(1, level->extra.weapons[wpnCurrent], mask);
            // have a shotgun in inventory place it on the back if another weapon is in use
            meshSwap(2, level->extra.weapons[Weapon::SHOTGUN], game->invCount(TR::Entity::INV_SHOTGUN) ? BODY_CHEST : 0);
        } else {
            meshSwap(2, level->extra.weapons[wpnCurrent], mask);
        }

        // mesh swap to angry Lara's head while firing (from uzis model)
        meshSwap(3, level->extra.weapons[Weapon::UZIS], (wState == Weapon::IS_FIRING) ? BODY_HEAD : 0);

        wpnState = wState;
    }

    bool emptyHands() {
        return wpnCurrent == Weapon::EMPTY || arms[0].anim == Weapon::Anim::NONE;
    }

    bool canLookAt() {
        return (stand == STAND_GROUND || stand == STAND_SLIDE)
               && state != STATE_REACH
               && state != STATE_PUSH_BLOCK
               && state != STATE_PULL_BLOCK
               && state != STATE_PUSH_PULL_READY
               && state != STATE_PICK_UP;
    }

    bool canDrawWeapon() {
        if (dozy) return true;

        return wpnCurrent != Weapon::EMPTY
               && emptyHands()
               && animation.index != ANIM_CLIMB_3
               && animation.index != ANIM_CLIMB_2
               && state != STATE_DEATH
               && state != STATE_HANG
               && state != STATE_REACH
               && state != STATE_TREAD
               && state != STATE_SWIM
               && state != STATE_GLIDE
               && state != STATE_HANG_UP
               && state != STATE_FALL_BACK
               && state != STATE_HANG_LEFT
               && state != STATE_HANG_RIGHT
               && state != STATE_SURF_TREAD
               && state != STATE_SURF_SWIM
               && state != STATE_DIVE
               && state != STATE_PUSH_BLOCK
               && state != STATE_PULL_BLOCK
               && state != STATE_PUSH_PULL_READY
               && state != STATE_PICK_UP
               && state != STATE_SWITCH_DOWN
               && state != STATE_SWITCH_UP
               && state != STATE_USE_KEY
               && state != STATE_USE_PUZZLE
               && state != STATE_UNDERWATER_DEATH
               && state != STATE_SPECIAL
               && state != STATE_SURF_BACK
               && state != STATE_SURF_LEFT
               && state != STATE_SURF_RIGHT
               && state != STATE_SWAN_DIVE
               && state != STATE_FAST_DIVE
               && state != STATE_HANDSTAND
               && state != STATE_WATER_OUT;
    }

    bool canHitAnim() {
        return    state == STATE_WALK
               || state == STATE_RUN
               || state == STATE_STOP
               || state == STATE_FAST_BACK
               || state == STATE_TURN_RIGHT
               || state == STATE_TURN_LEFT
               || state == STATE_BACK
               || state == STATE_FAST_TURN
               || state == STATE_STEP_RIGHT
               || state == STATE_STEP_LEFT;
    }

    bool wpnReady() {
        return arms[0].anim != Weapon::Anim::PREPARE && arms[0].anim != Weapon::Anim::UNHOLSTER && arms[0].anim != Weapon::Anim::HOLSTER;
    }

    void wpnDraw(bool instant = false) {
        if (!canDrawWeapon()) return;

        if (wpnReady() && emptyHands()) {
            if (wpnCurrent != Weapon::SHOTGUN) {
                wpnSetAnim(arms[0], wpnState, instant ? Weapon::Anim::AIM : Weapon::Anim::PREPARE, 0.0f, 1.0f);
                wpnSetAnim(arms[1], wpnState, instant ? Weapon::Anim::AIM : Weapon::Anim::PREPARE, 0.0f, 1.0f);
            } else
                wpnSetAnim(arms[0], wpnState, instant ? Weapon::Anim::AIM : Weapon::Anim::UNHOLSTER, 0.0f, 1.0f);
        }
    }

    void wpnHide() {
        if (wpnReady() && !emptyHands()) {
            if (wpnCurrent != Weapon::SHOTGUN) {
                wpnSetAnim(arms[0], wpnState, Weapon::Anim::UNHOLSTER, 0.0f, -1.0f);
                wpnSetAnim(arms[1], wpnState, Weapon::Anim::UNHOLSTER, 0.0f, -1.0f);
            } else
                wpnSetAnim(arms[0], wpnState, Weapon::Anim::HOLSTER, 0.0f, 1.0f);
        }
    }

    void wpnChange(Weapon::Type wType) {
        if (wpnCurrent == wType || level->isHome()) {
            if (emptyHands())
                wpnDraw();
            return;
        }
        wpnNext = wType;
        wpnHide();
    }

    int wpnGetAnimIndex(Weapon::Anim wAnim) {
        if (wpnCurrent == Weapon::SHOTGUN) {
            switch (wAnim) {
                case Weapon::Anim::PREPARE   : ASSERT(false); break;    // rifle has no prepare animation
                case Weapon::Anim::UNHOLSTER : return 1;
                case Weapon::Anim::HOLSTER   : return 3;
                case Weapon::Anim::HOLD      :
                case Weapon::Anim::AIM       : return 0;
                case Weapon::Anim::FIRE      : return 2;
                default : ;
            }
        } else
            switch (wAnim) {
                case Weapon::Anim::PREPARE   : return 1;
                case Weapon::Anim::UNHOLSTER : return 2;
                case Weapon::Anim::HOLSTER   : ASSERT(false); break;    // pistols has no holster animation (it's reversed unholster)
                case Weapon::Anim::HOLD      :
                case Weapon::Anim::AIM       : return 0;
                case Weapon::Anim::FIRE      : return 3;
                default : ;
            }
        return 0;
    }

    int wpnGetSound() {
        switch (wpnCurrent) {
            case Weapon::PISTOLS : return TR::SND_PISTOLS_SHOT;
            case Weapon::SHOTGUN : return TR::SND_SHOTGUN_SHOT;
            case Weapon::MAGNUMS : return TR::SND_MAGNUMS_SHOT;
            case Weapon::UZIS    : return TR::SND_UZIS_SHOT;
            default              : return TR::SND_NO;
        }
    }

    void wpnFire() {
        bool armShot[2] = { false, false };
        for (int i = 0; i < 2; i++) {
            Arm &arm = arms[i];
            if (arm.anim == Weapon::Anim::FIRE) {
                Animation &anim = arm.animation;
                //int realFrameIndex = int(arms[i].animation.time * 30.0f / anim->frameRate) % ((anim->frameEnd - anim->frameStart) / anim->frameRate + 1);
                if (anim.frameIndex != anim.framePrev) {
                    if (anim.frameIndex == 0) { //realFrameIndex < arms[i].animation.framePrev) {
                        if ((input & ACTION) && (!arm.tracking || arm.target))
                            armShot[i] = true;
                        else
                            wpnSetAnim(arm, Weapon::IS_ARMED, Weapon::Anim::AIM, 0.0f, -1.0f, arm.target == NULL);
                    }
                // shotgun reload sound
                    if (wpnCurrent == Weapon::SHOTGUN) {
                        if (anim.frameIndex == 10)
                            game->playSound(TR::SND_SHOTGUN_RELOAD, pos, Sound::Flags::PAN);
                    }
                }
            }
            arm.animation.framePrev = arm.animation.frameIndex;

            if (wpnCurrent == Weapon::SHOTGUN) break;
        }

        if (armShot[0] || armShot[1])
            doShot(armShot[0], armShot[1]);
    }

    void doShot(bool rightHand, bool leftHand) {
        int count = wpnCurrent == Weapon::SHOTGUN ? 6 : 2;
        float nearDist = 32.0f * 1024.0f;
        vec3  nearPos;
        int   shots = 0;

        for (int i = 0; i < count; i++) {
            int armIndex;
            if (wpnCurrent == Weapon::SHOTGUN) {
                if (!rightHand) continue;
                armIndex = 0;
            } else {
                if (!(i ? leftHand : rightHand)) continue;
                armIndex = i;
            }
            Arm *arm = &arms[armIndex];

            if (wpnAmmo && *wpnAmmo != UNLIMITED_AMMO) {
                if (*wpnAmmo <= 0)
                    continue;
                if (wpnCurrent != Weapon::SHOTGUN)
                    *wpnAmmo -= 1;
            }

            arm->shotTimer = 0.0f;
            shots++;

            int joint = wpnCurrent == Weapon::SHOTGUN ? 8 : (i ? 11 : 8);

            vec3 p = animation.getJoints(getMatrix(), joint, false).pos;
            vec3 d = arm->rotAbs * vec3(0, 0, 1);
            vec3 t = p + d * (24.0f * 1024.0f) + ((vec3(randf(), randf(), randf()) * 2.0f) - vec3(1.0f)) * 1024.0f;

            int room;
            vec3 hit = trace(getRoomIndex(), p, t, room, false);
            if (arm->target && checkHit(arm->target, p, hit, hit)) {
                TR::Entity::Type type = arm->target->getEntity().type;
                ((Character*)arm->target)->hit(wpnGetDamage(), this);
                hit -= d * 64.0f;
                if (type != TR::Entity::SCION_TARGET)
                    game->addEntity(TR::Entity::BLOOD, room, hit);
            } else {
                hit -= d * 64.0f;
                game->addEntity(TR::Entity::RICOCHET, room, hit);

                float dist = (hit - p).length();
                if (dist < nearDist) {
                    nearPos  = hit;
                    nearDist = dist;
                }
            }

            Core::lightPos[1 + armIndex]   = animation.getJoints(getMatrix(), armIndex == 0 ? 10 : 13, false).pos;
            Core::lightColor[1 + armIndex] = FLASH_LIGHT_COLOR;
        }

        if (shots) {
            game->playSound(wpnGetSound(), pos, Sound::Flags::PAN);
            game->playSound(TR::SND_RICOCHET, nearPos, Sound::Flags::PAN);

             if (wpnAmmo && *wpnAmmo != UNLIMITED_AMMO && wpnCurrent == Weapon::SHOTGUN)
                *wpnAmmo -= 1;
        }

        if (wpnAmmo && *wpnAmmo != UNLIMITED_AMMO && *wpnAmmo <= 0) {
            wpnChange(Weapon::PISTOLS);
        }
    }

    void updateWeapon() {
        if (level->isCutsceneLevel()) return;

        if (wpnNext != Weapon::EMPTY && emptyHands()) {
            wpnSet(wpnNext);
            wpnDraw();
            wpnNext = Weapon::EMPTY;
        }

    // apply weapon state changes
        if (input & WEAPON) {
            if (emptyHands())
                wpnDraw();
            else
                wpnHide();
        }

        if (!emptyHands()) {
            bool isRifle = wpnCurrent == Weapon::SHOTGUN;

            for (int i = 0; i < 2; i++) {
                Arm &arm = arms[i];

                if (arm.target || ((input & ACTION) && !arm.tracking)) {
                    if (arm.anim == Weapon::Anim::HOLD)
                        wpnSetAnim(arm, wpnState, Weapon::Anim::AIM, 0.0f, 1.0f);
                } else
                    if (arm.anim == Weapon::Anim::AIM)
                        arm.animation.dir = -1.0f;

                if (isRifle) break;
            }

            for (int i = 0; i < 2; i++)
                arms[i].animation.update();

            if (isRifle)
                animateShotgun();
            else
                animatePistols();

            wpnFire(); // make a shot
        }
    }

    void animatePistols() {
        for (int i = 0; i < 2; i++) {
            Arm &arm = arms[i];

            if (!arm.animation.isEnded) continue;

            if (arm.animation.dir >= 0.0f)
                switch (arm.anim) {
                    case Weapon::Anim::PREPARE   : wpnSetAnim(arm, Weapon::IS_ARMED, Weapon::Anim::UNHOLSTER, arm.animation.time - arm.animation.timeMax, 1.0f); break;
                    case Weapon::Anim::UNHOLSTER : wpnSetAnim(arm, Weapon::IS_ARMED, Weapon::Anim::HOLD, 0.0f, 1.0f, false); break;
                    case Weapon::Anim::AIM       :
                    case Weapon::Anim::FIRE      :
                        if (input & ACTION)
                            wpnSetAnim(arm, Weapon::IS_FIRING, Weapon::Anim::FIRE, arm.animation.time - arm.animation.timeMax, wpnCurrent == Weapon::UZIS ? 2.0f : 1.0f);
                        else
                            wpnSetAnim(arm, Weapon::IS_ARMED, Weapon::Anim::AIM, 0.0f, -1.0f, false);
                        break;
                    default : ;
                };

            if (arm.animation.dir < 0.0f)
                switch (arm.anim) {
                    case Weapon::Anim::PREPARE   : wpnSetAnim(arm, Weapon::IS_HIDDEN, Weapon::Anim::NONE,    0.0f, 1.0f, false);    break;
                    case Weapon::Anim::UNHOLSTER : wpnSetAnim(arm, Weapon::IS_HIDDEN, Weapon::Anim::PREPARE, arm.animation.time, -1.0f);  break;
                    case Weapon::Anim::AIM       : wpnSetAnim(arm, Weapon::IS_ARMED,  Weapon::Anim::HOLD,    0.0f, 1.0f, false);    break;
                    default : ;
                };
        }
    }

    void animateShotgun() {
        Arm &arm = arms[0];
        if (arm.animation.dir >= 0.0f) {
            if (arm.animation.isEnded) {
                switch (arm.anim) {
                    case Weapon::Anim::UNHOLSTER : wpnSetAnim(arm, Weapon::IS_ARMED,  Weapon::Anim::HOLD, 0.0f, 1.0f, false); break;
                    case Weapon::Anim::HOLSTER   : wpnSetAnim(arm, Weapon::IS_HIDDEN, Weapon::Anim::NONE, 0.0f, 1.0f, false); break;
                    case Weapon::Anim::AIM       :
                    case Weapon::Anim::FIRE      :
                        if (input & ACTION)
                            wpnSetAnim(arm, Weapon::IS_FIRING, Weapon::Anim::FIRE, arm.animation.time - arm.animation.timeMax, 1.0f);
                        else
                            wpnSetAnim(arm, Weapon::IS_ARMED,  Weapon::Anim::AIM, 0.0f, -1.0f, false);
                        break;
                    default : ;
                }
            } else
                if (arm.animation.frameIndex != arm.animation.framePrev) {
                    float delta = arm.animation.time / arm.animation.timeMax;
                    switch (arm.anim) {
                        case Weapon::Anim::UNHOLSTER : if (delta >= 0.3f) wpnSetAnim(arm, Weapon::IS_ARMED,  arm.anim, arm.animation.time, 1.0f); break;
                        case Weapon::Anim::HOLSTER   : if (delta >= 0.7f) wpnSetAnim(arm, Weapon::IS_HIDDEN, arm.anim, arm.animation.time, 1.0f); break;
                        default : ;
                    }
                }
        } else
            if (arm.animation.isEnded && arm.anim == Weapon::Anim::AIM)
                wpnSetAnim(arm, Weapon::IS_ARMED, Weapon::Anim::HOLD, 0.0f, 1.0f, false);
    }

    void updateOverrides() {
        int overrideMask = 0;
        // head & chest
        overrideMask |= BODY_CHEST | BODY_HEAD;

        animation.overrides[jointChest] = animation.getJointRot(jointChest);
        animation.overrides[jointHead]  = animation.getJointRot(jointHead);

    /* TODO: shotgun full body animation
        if (wpnCurrent == Weapon::SHOTGUN) {
            animation.frameA = arms[0].animation.frameA;
            animation.frameB = arms[0].animation.frameB;
            animation.delta  = arms[0].animation.delta;
        }
    */

    // arms
        if (!emptyHands()) {
            // right arm
            Arm *arm = &arms[0];
            animation.overrides[ 8] = arm->animation.getJointRot( 8);
            animation.overrides[ 9] = arm->animation.getJointRot( 9);
            animation.overrides[10] = arm->animation.getJointRot(10);
            // left arm
            if (wpnCurrent != Weapon::SHOTGUN) arm = &arms[1];
            animation.overrides[11] = arm->animation.getJointRot(11);
            animation.overrides[12] = arm->animation.getJointRot(12);
            animation.overrides[13] = arm->animation.getJointRot(13);

            overrideMask |=  (BODY_ARM_R | BODY_ARM_L);
        } else
            overrideMask &= ~(BODY_ARM_R | BODY_ARM_L);

    // update hit anim
        if (hitDir >= 0) {
            Animation hitAnim = Animation(level, getModel());
            switch (hitDir) {
                case 0 : hitAnim.setAnim(ANIM_HIT_FRONT, 0, false); break;
                case 1 : hitAnim.setAnim(ANIM_HIT_LEFT,  0, false); break;
                case 2 : hitAnim.setAnim(ANIM_HIT_BACK , 0, false); break;
                case 3 : hitAnim.setAnim(ANIM_HIT_RIGHT, 0, false); break;
            }
            hitTime = min(hitTime, hitAnim.timeMax - EPS);
            hitAnim.time = hitTime;
            hitAnim.updateInfo();
            
            overrideMask &= ~(BODY_CHEST | BODY_HEAD);
            int hitMask = (BODY_UPPER | BODY_LOWER | BODY_HEAD) & ~overrideMask;
            int index    = 0;
            while (hitMask) {
                if (hitMask & 1)
                    animation.overrides[index] = hitAnim.getJointRot(index);
                index++;
                hitMask >>= 1;
            }

            hitTime += Core::deltaTime;
            overrideMask = BODY_UPPER | BODY_LOWER | BODY_HEAD;
        }

        animation.overrideMask = overrideMask;
    }

    virtual void lookAt(Controller *target) {
        if (health <= 0.0f)
            return;

        updateOverrides();

        Character::lookAt(canLookAt() ? target : NULL);

        if (!emptyHands()) {
            updateTargets();

            if (wpnCurrent == Weapon::SHOTGUN)
                aimShotgun();
            else
                aimPistols();
        }
    }

    void aimShotgun() {
        quat rot;

        Arm &arm = arms[0];
        arm.target = aim(arm.target, 14, vec4(-PI * 0.4f, PI * 0.4f, -PI * 0.25f, PI * 0.25f), rot, &arm.rotAbs) ? arm.target : NULL;
    }

    void aimPistols() {
        float speed = 8.0f * Core::deltaTime;

        int joints[2] = { 8, 11 };

        vec4 ranges[2] = {
            vec4(-PI * 0.4f, PI * 0.4f, -PI * 0.2f, PI * 0.5f),
            vec4(-PI * 0.4f, PI * 0.4f, -PI * 0.5f, PI * 0.2f),
        };
        
        for (int i = 0; i < 2; i++) {
            quat rot;
            Arm &arm = arms[i];
            int j = joints[i];

            if (!aim(arm.target, j, ranges[i], rot, &arm.rotAbs)) {                
                arm.target = arms[i^1].target;
                if (!aim(arm.target, j, ranges[i], rot, &arm.rotAbs)) {
                    rot = quat(0, 0, 0, 1);
                    arm.target = NULL;
                }
            }

            float t;
            if (arm.anim == Weapon::Anim::FIRE)
                t = 1.0f;
            else if (arm.anim == Weapon::Anim::AIM)
                t = arm.animation.time / arm.animation.timeMax;
            else
                t = 0.0f;

            arm.rot = arm.rot.slerp(rot, speed);
            animation.overrides[j] = animation.overrides[j].slerp(arm.rot * animation.overrides[j], t);
        }
    }

    void updateTargets() {
        arms[0].target = arms[1].target = NULL;
        viewTarget = NULL;

        if (emptyHands() || !wpnReady()) {
            arms[0].tracking = arms[1].tracking = NULL;
            return;
        }

        // auto retarget 
        bool retarget = false;
        if (Core::settings.controls.retarget) {
            for (int i = 0; i < 2; i++)
                if (!arms[i].tracking || ((Character*)arms[i].tracking)->health <= 0.0f) {
                    retarget = true;
                    break;
                }
        }

        int count = wpnCurrent != Weapon::SHOTGUN ? 2 : 1;
        if (!(input & ACTION) || retarget) {
            getTargets(arms[0].tracking, arms[1].tracking);
            if (count == 1)
                arms[1].tracking = NULL;
            else if (!arms[0].tracking && arms[1].tracking)
                arms[0].tracking = arms[1].tracking;
            else if (!arms[1].tracking && arms[0].tracking)
                arms[1].tracking = arms[0].tracking;
            arms[0].target = arms[0].tracking;
            arms[1].target = arms[1].tracking;
        } else {
            if (!arms[0].tracking && !arms[1].tracking)
                return;

        // flip left and right by relative target direction
            if (count > 1) {
                int side[2] = { 0, 0 };
                vec3 dir = getDir();
                dir.y = 0.0f;

                for (int i = 0; i < count; i++)
                    if (arms[i].tracking) {
                        vec3 v = arms[i].tracking->pos - pos;
                        v.y = 0;
                        side[i] = sign(v.cross(dir).y);
                    }

                if (side[0] > 0 && side[1] < 0)
                    swap(arms[0].tracking, arms[1].tracking);
            }

        // check occlusion for tracking targets
            for (int i = 0; i < count; i++)
                if (arms[i].tracking) {
                    Controller *enemy = (Controller*)arms[i].tracking;

                    Box box = enemy->getBoundingBox();
                    vec3 to = box.center();
                    to.y = box.min.y + (box.max.y - box.min.y) / 3.0f;

                    vec3 from = pos - vec3(0, 650, 0);
                    arms[i].target = checkOcclusion(from, to, (to - from).length()) ? arms[i].tracking : NULL;
                }

            if (count == 1)
                arms[1].target = NULL;
            else if (!arms[0].target && arms[1].target)
                arms[0].target = arms[1].target;
            else if (!arms[1].target && arms[0].target)
                arms[1].target = arms[0].target;
        }

        if (arms[0].target && arms[1].target && arms[0].target != arms[1].target) {
            viewTarget = NULL; //arms[0].target;
        } else if (arms[0].target)
            viewTarget = arms[0].target;
        else if (arms[1].target)
            viewTarget = arms[1].target;
        else if (arms[0].tracking)
            viewTarget = arms[0].tracking;
        else if (arms[1].tracking)
            viewTarget = arms[1].tracking;
    }

    void getTargets(Controller *&target1, Controller *&target2) {
        vec3 dir = getDir().normal();
        float dist[2]  = { TARGET_MAX_DIST, TARGET_MAX_DIST };

        target1 = target2 = NULL;

        vec3 from = pos - vec3(0, 650, 0);

        Controller *c = Controller::first;
        do {
            if (!c->getEntity().isEnemy())
                continue;

            Character *enemy = (Character*)c;
            if (enemy->health <= 0)
                continue;

            Box box = enemy->getBoundingBox();
            vec3 p = box.center();
            p.y = box.min.y + (box.max.y - box.min.y) / 3.0f;
            
            vec3 v = p - pos;
            if (dir.dot(v.normal()) <= 0.5f)
                continue; // target is out of view range -60..+60 degrees

            float d = v.length();

            if ((d > dist[0] && d > dist[1]) || !checkOcclusion(from, p, d)) 
                continue;

            if (d < dist[0]) {
                target2 = target1;
                dist[1] = dist[0];
                target1 = enemy;
                dist[0] = d;
            } else if (d < dist[1]) {
                target2 = enemy;
                dist[1] = d;
            }
        } while ((c = c->next));

        if (!target2 || dist[1] > dist[0] * 4)
            target2 = target1;
    }

    bool checkOcclusion(const vec3 &from, const vec3 &to, float dist) {
        int room;
        vec3 d = trace(getRoomIndex(), from, to, room, false); // check occlusion
        return ((d - from).length() > (dist - 512.0f));
    }

    bool checkHit(Controller *target, const vec3 &from, const vec3 &to, vec3 &point) {
        Box box = target->getBoundingBoxLocal();
        mat4 m  = target->getMatrix();

        float t;
        vec3 v = to - from;
        
        if (box.intersect(m, from, v, t)) {
            t *= v.length();
            v = v.normal();
            Sphere spheres[MAX_SPHERES];
            int count;
            target->getSpheres(spheres, count);
            for (int i = 0; i < count; i++) {
                float st;
                if (spheres[i].intersect(from, v, st)) {
                    point = from + v * max(t, st);
                    return true;
                }
            }
        }
        return false;
    }

    virtual void cmdEmpty() {
        wpnHide();
    }

    virtual void cmdOffset(const vec3 &offset) {
        Character::cmdOffset(offset);
        move();
    }

    virtual void cmdJump(const vec3 &vel) {
        vec3 v = vel;
        if (state == STATE_HANG_UP)
            v.y = (3.0f - sqrtf(-2.0f * GRAVITY / 30.0f * (collision.info[Collision::FRONT].floor - pos.y + 800.0f - 128.0f)));
        Character::cmdJump(v);
    }

    void drawGun(int right) {
        int mask = (right ? BODY_ARM_R3 : BODY_ARM_L3); // unholster
        if (layers[1].mask & mask)
            mask = (layers[1].mask & ~mask) | (right ? BODY_LEG_R1 : BODY_LEG_L1); // holster
        else
            mask |= layers[1].mask;
        meshSwap(1, level->extra.weapons[wpnCurrent], mask);
    }

    void doBubbles() {
        int count = rand() % 3;
        if (!count) return;
        game->playSound(TR::SND_BUBBLE, pos, Sound::Flags::PAN);
        vec3 head = animation.getJoints(getMatrix(), 14, true) * vec3(0.0f, 0.0f, 50.0f);
        for (int i = 0; i < count; i++)
            game->addEntity(TR::Entity::BUBBLE, getRoomIndex(), head, 0);
    }

    virtual void cmdEffect(int fx) {
        switch (fx) {
            case TR::Effect::LARA_NORMAL    : animation.setAnim(ANIM_STAND); break;
            case TR::Effect::LARA_BUBBLES   : doBubbles(); break;
            case TR::Effect::LARA_HANDSFREE : break;//meshSwap(1, level->extra.weapons[wpnCurrent], BODY_LEG_L1 | BODY_LEG_R1); break;
            case TR::Effect::DRAW_RIGHTGUN  : drawGun(true); break;
            case TR::Effect::DRAW_LEFTGUN   : drawGun(false); break;
            case TR::Effect::SHOT_RIGHTGUN  : arms[0].shotTimer = 0; break;
            case TR::Effect::SHOT_LEFTGUN   : arms[1].shotTimer = 0; break;
            case TR::Effect::MESH_SWAP_1    : 
            case TR::Effect::MESH_SWAP_2    : 
            case TR::Effect::MESH_SWAP_3    : Character::cmdEffect(fx);
            case 26 : break; // TODO TR2 reset_hair
            case 32 : break; // TODO TR3 footprint
            default : LOG("unknown effect command %d (anim %d)\n", fx, animation.index); ASSERT(false);
        }
    }

    void addSparks(uint32 mask) {
        Sphere spheres[MAX_SPHERES];
        int count;
        getSpheres(spheres, count);
        for (int i = 0; i < count; i++)
            if (mask & (1 << i)) {
                vec3 sprPos = spheres[i].center + (vec3(randf(), randf(), randf()) * 2.0f - 1.0f) * spheres[i].radius;
                game->addEntity(TR::Entity::SPARKLES, getRoomIndex(), sprPos);
            }
    }

    void addBlood(const vec3 &sprPos, const vec3 &sprVel) {
        Sprite *sprite = (Sprite*)game->addEntity(TR::Entity::BLOOD, getRoomIndex(), sprPos, 0);
        if (sprite)
            sprite->velocity = sprVel;
    }

    void addBlood(float radius, float height, const vec3 &sprVel) {
        vec3 p = pos + vec3((randf() * 2.0f - 1.0f) * radius, -randf() * height, (randf() * 2.0f - 1.0f) * radius);
        addBlood(p, sprVel);
    }

    void addBloodSpikes() {
        float ang = randf() * PI * 2.0f;
        addBlood(64.0f,  512.0f, vec3(sinf(ang), 0.0f, cosf(ang)) * 20.0f);
    }

    void addBloodBlade() {
        float ang = angle.y + (randf() - 0.5f) * 30.0f * DEG2RAD;
        addBlood(64.0f, 744.0f, vec3(sinf(ang), 0.0f, cosf(ang)) * speed);
    }

    void addBloodSlam(Controller *trapSlam) {
        for (int i = 0; i < 6; i++)
            addBloodSpikes();
    }

    void bakeEnvironment() {
        flags.invisible = true;
        if (!environment)
            environment = new Texture(256, 256, Texture::RGBA, true, NULL, true, true);
        Core::beginFrame();
        game->renderEnvironment(getRoomIndex(), pos - vec3(0.0f, 384.0f, 0.0f), &environment, 0, Core::passCompose);
        environment->generateMipMap();
        Core::endFrame();
        flags.invisible = false;
    }

    virtual void hit(float damage, Controller *enemy = NULL, TR::HitType hitType = TR::HIT_DEFAULT) {
        if (dozy || level->isCutsceneLevel()) return;

        if (health <= 0.0f) return;

        damageTime = LARA_DAMAGE_TIME;

        Character::hit(damage, enemy, hitType);

        switch (hitType) {
            case TR::HIT_DART      : addBlood(enemy->pos, vec3(0));
            case TR::HIT_BLADE     : addBloodBlade(); break;
            case TR::HIT_SPIKES    : addBloodSpikes(); break;
            case TR::HIT_SWORD     : addBloodBlade(); break;
            case TR::HIT_SLAM      : addBloodSlam(enemy); break;
            case TR::HIT_LIGHTNING : lightning = (Lightning*)enemy; break;
            default                : ;
        }

        if (health > 0.0f)
            return;

        game->stopTrack();

        Core::lightColor[1 + 0] = Core::lightColor[1 + 1] = vec4(0, 0, 0, 1);
        arms[0].shotTimer = arms[1].shotTimer = MUZZLE_FLASH_TIME + 1.0f;
        arms[0].tracking  = arms[1].tracking  = NULL;
        arms[0].target    = arms[1].target    = NULL;
        viewTarget        = NULL;
        animation.overrideMask = 0;

        switch (hitType) {
            case TR::HIT_FALL : {
                animation.setState(STATE_DEATH);
                break;
            }
            case TR::HIT_BOULDER : {
                animation.setAnim(ANIM_DEATH_BOULDER);

                vec3 v(0.0f);
                if (enemy && enemy->getEntity().type == TR::Entity::TRAP_BOULDER) {
                    angle = enemy->angle;
                    TR::Level::FloorInfo info;
                    getFloorInfo(getRoomIndex(), pos, info);
                    vec3 d = getDir();
                    v = info.getSlant(d);
                    float dp = d.dot(v);
                    if (fabsf(dp) < 0.999)
                        angle.x = -acosf(dp);
                    v = ((TrapBoulder*)enemy)->velocity * 2.0f;
                }

                for (int i = 0; i < 15; i++)
                    addBlood(256.0f, 512.0f, v);
                break;
            }
            case TR::HIT_SPIKES : {
                pos.y = enemy->pos.y;
                animation.setAnim(ANIM_DEATH_SPIKES);
                for (int i = 0; i < 19; i++)
                    addBloodSpikes();
                break;
            }
            case TR::HIT_REX : {
                pos   = enemy->pos;
                angle = enemy->angle;

                meshSwap(1, TR::MODEL_LARA_SPEC, BODY_UPPER | BODY_LOWER);
                meshSwap(2, level->extra.weapons[Weapon::SHOTGUN], 0);
                meshSwap(3, level->extra.weapons[Weapon::UZIS],    0);

                animation.setAnim(level->models[TR::MODEL_LARA_SPEC].animation + 1);
                break;
            }
            case TR::HIT_MIDAS : {
            // generate environment map for reflections
                bakeEnvironment();
            // set death animation
                animation.setAnim(level->models[TR::MODEL_LARA_SPEC].animation + 1);
                game->getCamera()->doCutscene(pos, angle.y);
                break;
            }
            default : ;
        }

        if (hitType != TR::HIT_LAVA) {
            TR::Level::FloorInfo info;
            getFloorInfo(getRoomIndex(), pos, info);

            if (info.lava && info.floor == pos.y)
                hitType = TR::HIT_LAVA;
        }

        if (hitType == TR::HIT_LAVA) {
            for (int i = 0; i < 10; i++)
                Flame::add(game, this, int(randf() * 24.0f));
        }
    };

    bool useItem(TR::Entity::Type item) {
        if (game->isCutscene()) return false;

        switch (item) {
            case TR::Entity::INV_PISTOLS       : wpnChange(Lara::Weapon::PISTOLS); break;
            case TR::Entity::INV_SHOTGUN       : wpnChange(Lara::Weapon::SHOTGUN); break;
            case TR::Entity::INV_MAGNUMS       : wpnChange(Lara::Weapon::MAGNUMS); break;
            case TR::Entity::INV_UZIS          : wpnChange(Lara::Weapon::UZIS);    break;
            case TR::Entity::INV_MEDIKIT_SMALL :
            case TR::Entity::INV_MEDIKIT_BIG   :
                damageTime = LARA_DAMAGE_TIME;
                health = min(LARA_MAX_HEALTH, health + (item == TR::Entity::INV_MEDIKIT_SMALL ? LARA_MAX_HEALTH / 2 : LARA_MAX_HEALTH));
                game->playSound(TR::SND_HEALTH, pos, Sound::PAN);
                //TODO: remove medikit item
                break;
            case TR::Entity::INV_PUZZLE_1 :
            case TR::Entity::INV_PUZZLE_2 :
            case TR::Entity::INV_PUZZLE_3 :
            case TR::Entity::INV_PUZZLE_4 :
            case TR::Entity::INV_KEY_1    :
            case TR::Entity::INV_KEY_2    :
            case TR::Entity::INV_KEY_3    :
            case TR::Entity::INV_KEY_4    :
                if (usedKey == item)
                    return false;
                usedKey = item;
                break;
            case TR::Entity::INV_LEADBAR  :
                for (int i = 0; i < level->entitiesCount; i++) {
                    const TR::Entity &e = level->entities[i];
                    if (e.controller && e.type == TR::Entity::MIDAS_HAND) {
                        MidasHand *controller = (MidasHand*)e.controller;
                        if (controller->interaction) {
                            controller->invItem = item;
                            return false; // remove item from inventory
                        }
                        return true;
                    }
                }
                return false;
            default : return false;
        }
        return true;
    }

    bool waterOut() {
        // TODO: playSound 36
        if (collision.side != Collision::FRONT || pos.y - collision.info[Collision::FRONT].floor > 256 + 128)
            return false;

        vec3 dst = pos + getDir() * (LARA_RADIUS + 32.0f);

        TR::Level::FloorInfo info;
        getFloorInfo(getRoomIndex(), pos, info);
        int roomAbove = info.roomAbove;
        if (roomAbove == TR::NO_ROOM)
            return false;

        getFloorInfo(roomAbove, dst, info);

        int h = int(pos.y - info.floor);

        if (h >= 0 && h <= (256 + 128) && (state == STATE_SURF_TREAD || animation.setState(STATE_SURF_TREAD)) && animation.setState(STATE_STOP)) {
            alignToWall(LARA_RADIUS);
            roomIndex = roomAbove;
            pos.y    = info.floor;
            specular = LARA_WET_SPECULAR;
            move();
            return true;
        }

        return false;
    }

    int goUnderwater() {
        angle.x = -PI * 0.25f;
        game->waterDrop(pos, 256.0f, 0.2f);
        stand = STAND_UNDERWATER;
        return animation.setAnim(ANIM_TO_UNDERWATER);
    }

    bool doPickUp() {
        if (!animation.canSetState(STATE_PICK_UP))
            return false;

        int room = getRoomIndex();

        pickupListCount = 0;

        for (int i = 0; i < level->entitiesCount; i++) {
            TR::Entity &entity = level->entities[i];
            if (!entity.controller || !entity.isPickup())
                continue;

            Controller *controller = (Controller*)entity.controller;

            if (controller->getRoomIndex() != room || controller->flags.invisible || !canPickup(controller))
                continue;

            ASSERT(pickupListCount < COUNT(pickupList));
            pickupList[pickupListCount++] = controller;
        }

        if (pickupListCount > 0) {
            state = STATE_PICK_UP;
            return true;
        }

        return false;
    }

    bool canPickup(Controller *controller) {
        TR::Entity::Type type = controller->getEntity().type;

        // get limits
        TR::Limits::Limit *limit;
        switch (type) {
            case TR::Entity::SCION_PICKUP_QUALOPEC : limit = &TR::Limits::SCION; break;
            case TR::Entity::SCION_PICKUP_HOLDER   : limit = &TR::Limits::SCION_HOLDER; break;
            default : limit = level->rooms[getRoomIndex()].flags.water ? &TR::Limits::PICKUP_UNDERWATER : &TR::Limits::PICKUP;
        }

        if (!checkInteraction(controller, limit, true))
            return false;

        if (stand == Character::STAND_UNDERWATER)
            angle.x = -25 * DEG2RAD;

        // set new state
        switch (type) {
            case TR::Entity::SCION_PICKUP_QUALOPEC :
                animation.setAnim(level->models[TR::MODEL_LARA_SPEC].animation);
                game->getCamera()->doCutscene(pos, angle.y);
                break;
            case TR::Entity::SCION_PICKUP_HOLDER   :
                animation.setAnim(level->models[TR::MODEL_LARA_SPEC].animation);
                
                angle = controller->angle;
                pos   = controller->pos - vec3(0, -280, LARA_RADIUS + 512).rotateY(angle.y);

                game->getCamera()->doCutscene(pos, angle.y - PI * 0.5f);
                break;
            default : ; 
        }

        return true;
    }

    int doTutorial(int track) {
        if (level->version == TR::VER_TR1_PC || level->version == TR::VER_TR1_PSX)
            switch (track) { // GYM tutorial routine
                case 28 : if (level->state.tracks[track].once && state == STATE_UP_JUMP) track = 29; break;
                case 37 : 
                case 41 : if (state != STATE_HANG) return 0; break;
                case 42 : if (level->state.tracks[track].once && state == STATE_HANG) track = 43; break;
                case 49 : if (state != STATE_SURF_TREAD) return 0; break;
                case 50 : // end of GYM
                    if (level->state.tracks[track].once) {
                        timer += Core::deltaTime;
                        if (timer > 3.0f)
                            game->loadNextLevel();
                    } else {
                        if (state != STATE_WATER_OUT)
                            return 0;
                        timer = 0.0f;
                    }
                    break;
            }
        return track;
    }


    bool checkInteraction(Controller *controller, const TR::Limits::Limit *limit, bool action) {
        if ((state != STATE_STOP && state != STATE_TREAD && state != STATE_PUSH_PULL_READY) || !action || !emptyHands())
            return false;

        vec3 tmpAngle = controller->angle;
        vec3 ctrlAngle = controller->angle;
        if (stand == STAND_UNDERWATER)
            ctrlAngle.x = -25 * DEG2RAD;
        if (!limit->alignAngle)
            ctrlAngle.y = angle.y;
        controller->angle = ctrlAngle;
        mat4 m = controller->getMatrix();
        controller->angle = tmpAngle;

        float fx = 0.0f;
        if (!limit->alignHoriz)
            fx = (m.transpose() * vec4(pos - controller->pos, 0.0f)).x;

        vec3 targetPos = controller->pos + (m * vec4(fx, limit->dy, limit->dz, 0.0f)).xyz;

        vec3 deltaAbs = pos - targetPos;

        vec3 deltaRel = (m.transpose() * vec4(pos - controller->pos, 0.0f)).xyz; // inverse transform
        
        // set item orientation to hack limits check
        if (limit->box.contains(deltaRel)) {
            float deltaAngY = shortAngle(angle.y, ctrlAngle.y);

            if (stand == STAND_UNDERWATER) {
                float deltaAngX = shortAngle(angle.x, ctrlAngle.x);

                if (deltaAbs.length() > 64.0f || max(fabs(deltaAngX), fabs(deltaAngY)) > (10.0f * DEG2RAD)) {
                    pos     -= deltaAbs.normal() * min(deltaAbs.length(), Core::deltaTime * 512.0f);
                    angle.x += sign(deltaAngX)   * min(fabsf(deltaAngX), Core::deltaTime * (90.0f * DEG2RAD));
                    angle.y += sign(deltaAngY)   * min(fabsf(deltaAngY), Core::deltaTime * (90.0f * DEG2RAD));
                    return false;
                }
            }

            if (fabsf(deltaAngY) <= limit->ay * DEG2RAD) {
            // align
                if (limit->alignAngle)
                    angle = controller->angle;
                else
                    angle.x = angle.z = 0.0f;

                pos      = targetPos;
                velocity = vec3(0.0f);
                speed    = 0.0f;

                return true;
            }
        }

        return false;
    }

    void checkTrigger(Controller *controller, bool heavy) {
        TR::Level::FloorInfo info;
        getFloorInfo(controller->getRoomIndex(), controller->pos, info);

        if (getEntity().isLara() && info.lava && info.floor == pos.y) {
            hit(LARA_MAX_HEALTH + 1, NULL, TR::HIT_LAVA);
            return;
        }

        if (!info.trigCmdCount) return; // has no trigger

        TR::Limits::Limit *limit = NULL;
        bool switchIsDown = false;
        float timer = info.trigInfo.timer == 1 ? EPS : float(info.trigInfo.timer);
        int cmdIndex = 0;
        int actionState = state;

        switch (info.trigger) {
            case TR::Level::Trigger::ACTIVATE : break;

            case TR::Level::Trigger::SWITCH : {
                Switch *controller = (Switch*)level->entities[info.trigCmd[cmdIndex++].args].controller;

                if (controller->flags.state == TR::Entity::asNone) {
                    limit = state == STATE_STOP ? &TR::Limits::SWITCH : &TR::Limits::SWITCH_UNDERWATER;
                    if (checkInteraction(controller, limit, Input::state[cAction])) {
                        actionState = (controller->state == Switch::STATE_DOWN && stand == STAND_GROUND) ? STATE_SWITCH_UP : STATE_SWITCH_DOWN;

                        int animIndex;
                        switch (controller->getEntity().type) {
                            case TR::Entity::SWITCH_BUTTON : animIndex = ANIM_PUSH_BUTTON; break;
                            case TR::Entity::SWITCH_BIG    : animIndex = controller->state == Switch::STATE_DOWN ? ANIM_SWITCH_BIG_UP : ANIM_SWITCH_BIG_DOWN; break;
                            default : animIndex = -1;
                        }

                        if (animation.setState(actionState, animIndex)) 
                            controller->activate();
                    }
                }

                if (!controller->setTimer(timer))
                    return;

                switchIsDown = controller->state == Switch::STATE_DOWN;
                break;
            }

            case TR::Level::Trigger::KEY : {
                TR::Entity &entity = level->entities[info.trigCmd[cmdIndex++].args];
                KeyHole *controller = (KeyHole*)entity.controller;

                if (controller->flags.state == TR::Entity::asNone) {
                    if (controller->flags.active == TR::ACTIVE || state != STATE_STOP)
                        return;

                    actionState = entity.isPuzzleHole() ? STATE_USE_PUZZLE : STATE_USE_KEY;
                    if (!animation.canSetState(actionState))
                        return;

                    limit = actionState == STATE_USE_PUZZLE ? &TR::Limits::PUZZLE_HOLE : &TR::Limits::KEY_HOLE;
                    if (!checkInteraction(controller, limit, isPressed(ACTION) || usedKey != TR::Entity::LARA))
                        return;

                    if (usedKey == TR::Entity::LARA) {
                        if (isPressed(ACTION) && !game->invChooseKey(entity.type))
                            game->playSound(TR::SND_NO, pos, Sound::PAN); // no compatible items in inventory
                        return;
                    }

                    if (TR::Entity::convToInv(TR::Entity::getItemForHole(entity.type)) != usedKey) { // check compatibility if user select other
                        game->playSound(TR::SND_NO, pos, Sound::PAN); // uncompatible item
                        return;
                    }

                    keyHole = controller;
                    game->invUse(usedKey);

                    animation.setState(actionState);
                }

                if (controller->flags.state != TR::Entity::asInactive)
                    return;

                break;
            }

            case TR::Level::Trigger::PICKUP : {
                Controller *controller = (Controller*)level->entities[info.trigCmd[cmdIndex++].args].controller;
                if (!controller->flags.invisible)
                    return;
                break;
            }

            case TR::Level::Trigger::COMBAT :
                if (emptyHands())
                    return;
                break;

            case TR::Level::Trigger::PAD :
            case TR::Level::Trigger::ANTIPAD :
                if (pos.y != info.floor) return;
                break;

            case TR::Level::Trigger::HEAVY :
                if (!heavy) return;
                break;
            case TR::Level::Trigger::DUMMY :
                return;
        }

        bool needFlip = false;
        TR::Effect effect = TR::Effect::NONE;

        int         cameraIndex  = -1;
        Controller *cameraTarget = NULL;
        Camera *camera = (Camera*)level->cameraController;

        while (cmdIndex < info.trigCmdCount) {
            TR::FloorData::TriggerCommand &cmd = info.trigCmd[cmdIndex++];

            switch (cmd.action) {
                case TR::Action::ACTIVATE : {
                    TR::Entity &e = level->entities[cmd.args];
                    Controller *controller = (Controller*)e.controller;
                    ASSERT(controller);
                    TR::Entity::Flags &flags = controller->flags;

                    if (flags.once)
                        break;
                    controller->timer = timer;

                    if (info.trigger == TR::Level::Trigger::SWITCH)
                        flags.active ^= info.trigInfo.mask;
                    else if (info.trigger == TR::Level::Trigger::ANTIPAD)
                        flags.active &= ~info.trigInfo.mask;
                    else
                        flags.active |= info.trigInfo.mask;

                    if (flags.active != TR::ACTIVE)
                        break;

                    flags.once |= info.trigInfo.once;
                    
                    controller->activate();
                    break;
                }
                case TR::Action::CAMERA_SWITCH : {
                    TR::FloorData::TriggerCommand &cam = info.trigCmd[cmdIndex++];
                    if (level->cameras[cmd.args].flags.once)
                        break;

                    if (info.trigger == TR::Level::Trigger::COMBAT)
                        break;
                    if (info.trigger == TR::Level::Trigger::SWITCH && info.trigInfo.timer && switchIsDown)
                        break;
 
                    if (info.trigger == TR::Level::Trigger::SWITCH || cmd.args != camera->viewIndexLast) {
                        level->cameras[cmd.args].flags.once |= cam.once;
                        camera->setView(cmd.args, cam.timer == 1 ? EPS : float(cam.timer), cam.speed * 8.0f);
                    }

                    if (cmd.args == camera->viewIndexLast)
                        cameraIndex = cmd.args;

                    break;
                }
                case TR::Action::FLOW :
                    applyFlow(level->cameras[cmd.args]);
                    break;
                case TR::Action::FLIP : {
                    TR::ByteFlags &flip = level->state.flipmaps[cmd.args];

                    if (flip.once)
                        break;

                    if (info.trigger == TR::Level::Trigger::SWITCH)
                        flip.active ^= info.trigInfo.mask;
                    else
                        flip.active |= info.trigInfo.mask;

                    if (flip.active == TR::ACTIVE)
                        flip.once |= info.trigInfo.once;

                    if ((flip.active == TR::ACTIVE) ^ level->state.flags.flipped)
                         needFlip = true;

                    break;
                }
                case TR::Action::FLIP_ON :
                    if (level->state.flipmaps[cmd.args].active == TR::ACTIVE && !level->state.flags.flipped)
                        needFlip = true;
                    break;
                case TR::Action::FLIP_OFF :
                    if (level->state.flipmaps[cmd.args].active == TR::ACTIVE && level->state.flags.flipped)
                        needFlip = true;
                    break;
                case TR::Action::CAMERA_TARGET :
                    cameraTarget = (Controller*)level->entities[cmd.args].controller;
                    break;
                case TR::Action::END :
                    game->loadNextLevel();
                    break;
                case TR::Action::SOUNDTRACK : {
                    int track = doTutorial(cmd.args);

                    if (track == 0) break;

                // check trigger
                    TR::ByteFlags &flags = level->state.tracks[track];

                    if (flags.once)
                        break;

                    if (info.trigger == TR::Level::Trigger::SWITCH)
                        flags.active ^= info.trigInfo.mask;
                    else if (info.trigger == TR::Level::Trigger::ANTIPAD)
                        flags.active &= ~info.trigInfo.mask;
                    else
                        flags.active |= info.trigInfo.mask;

                    if ( (flags.active == TR::ACTIVE) || (((level->version & (TR::VER_TR2 | TR::VER_TR3))) && flags.active) ) {
                        flags.once |= info.trigInfo.once;
                        game->playTrack(track);
                    } else
                        game->stopTrack();

                    break;
                }
                case TR::Action::EFFECT :
                    effect = TR::Effect(cmd.args);
                    break;
                case TR::Action::SECRET :
                    if (!(level->state.progress.secrets & (1 << cmd.args))) {
                        level->state.progress.secrets |= 1 << cmd.args;
                        if (!game->playSound(TR::SND_SECRET, pos))
                            game->playTrack(TR::TRACK_TR1_SECRET);
                    }
                    break;
            }
        }

        if (cameraTarget && (camera->state == Camera::STATE_STATIC || cameraIndex == -1))
           camera->viewTarget = cameraTarget;

        if (!cameraTarget && cameraIndex > -1)
            camera->viewIndex = cameraIndex;

        if (needFlip) {
            level->state.flags.flipped = !level->state.flags.flipped;
            game->setEffect(this, effect);
        }
    }

    virtual Stand getStand() {
        if (dozy) return STAND_UNDERWATER;

        if (state == STATE_HANG || state == STATE_HANG_LEFT || state == STATE_HANG_RIGHT) {
            if (input & ACTION)
                return STAND_HANG;
            animation.setAnim(ANIM_FALL_HANG);
            velocity = vec3(0.0f);
            pos.y += 128.0f;
            return STAND_AIR;
        }

        if (state == STATE_HANDSTAND || (state == STATE_HANG_UP && animation.index != ANIM_CLIMB_JUMP))
            return STAND_HANG;

        if (stand == STAND_ONWATER && state != STATE_STOP) {
            if (!getRoom().flags.water && state != STATE_WATER_OUT)
                return STAND_AIR;
            if (state != STATE_DIVE)
                return stand;
        }

        TR::Level::FloorInfo info;
        getFloorInfo(getRoomIndex(), pos, info);

        if (getRoom().flags.water) {
            if (stand == STAND_UNDERWATER || stand == STAND_ONWATER)
                return stand;
            wpnHide();
            if (stand == STAND_AIR) {
                //if (stand != STAND_UNDERWATER && stand != STAND_ONWATER && (state != STATE_FALL && state != STATE_REACH && state != STATE_SWAN_DIVE && state != STATE_FAST_DIVE))
                //    animation.setAnim(ANIM_FALL_FORTH);
                stopScreaming();
                return STAND_UNDERWATER;
            } else {
                pos.y = info.roomCeiling;
                return STAND_ONWATER;
            }
        }

        if ((stand == STAND_SLIDE || stand == STAND_GROUND) && (state != STATE_FORWARD_JUMP && state != STATE_BACK_JUMP)) {
            if (pos.y + 8 >= info.floor && (abs(info.slantX) > 2 || abs(info.slantZ) > 2)) {
                pos.y = info.floor;

                if (stand == STAND_GROUND)
                    slideStart();

                return STAND_SLIDE;
            }
        }

        int extra = (stand != STAND_AIR && stand != STAND_SLIDE) ? 256 : 0;

        if (pos.y + extra >= info.floor && !(stand == STAND_AIR && velocity.y < 0)) {
            if (stand != STAND_GROUND) {
                pos.y = info.floor;
            // get damage from falling
                if (velocity.y > 0.0f) {
                    stopScreaming();
                    if (state == STATE_FAST_DIVE && velocity.y > 133.0f) {
                        hit(health + 1.0f, NULL, TR::HIT_FALL);
                    } else {
                        float v = velocity.y - 140.0f;
                        if (v > 14.0f)
                            hit(health + 1.0f, NULL, TR::HIT_FALL);
                        else
                            if (v > 0.0f)
                                hit(v * v * LARA_MAX_HEALTH / 196.0f, NULL, TR::HIT_FALL);
                    }
                }
            }
            if (stand == STAND_UNDERWATER || stand == STAND_ONWATER)
                animation.setAnim(ANIM_STAND);
            return STAND_GROUND;
        }

        return STAND_AIR;
    }

    void stopScreaming() {
        if (velocity.y >= 154.0f)
            Sound::stop(TR::SND_SCREAM);
    }

    virtual int getHeight() {
        if (stand == STAND_GROUND || stand == STAND_AIR)
            return 768;
        return 0;
    }

    virtual int getStateAir() {
        angle.x = 0.0f;

        if (velocity.y > 131.0f && state != STATE_SWAN_DIVE && state != STATE_FAST_DIVE)
            return STATE_FALL;

        if (state == STATE_REACH && getDir().dot(vec3(velocity.x, 0.0f, velocity.z)) < 0)
            velocity.x = velocity.z = 0.0f;

        if ((state == STATE_REACH || state == STATE_UP_JUMP) && (input & ACTION) && emptyHands()) {
            if (state == STATE_REACH && velocity.y < 0.0f)
                return state;

            Box bounds = animation.getBoundingBox(pos, 0);

            vec3 p = vec3(pos.x, bounds.min.y, pos.z);

            Collision c = Collision(this, getRoomIndex(), p, getDir() * 128.0f, vec3(0.0f), LARA_RADIUS, angleExt, 0, 0, 0, 0);

            if (c.side != Collision::FRONT)
                return state;

            float floor   = c.info[Collision::FRONT].floor;
            float ceiling = c.info[Collision::FRONT].ceiling;
            float hands   = bounds.min.y;

            if (fabsf(floor - hands) < 64 && floor != ceiling) {
                alignToWall(-LARA_RADIUS);
                pos.y = float(floor + LARA_HANG_OFFSET);
                stand = STAND_HANG;

                if (state == STATE_REACH) {
                    velocity = vec3(0.0f);
                    TR::Level::FloorInfo info;
                    getFloorInfo(getRoomIndex(), pos + getDir() * 256.0f, info);
                    int h = int(info.ceiling - floor);
                    return animation.setAnim((h > 0 && h < 400) ? ANIM_HANG_SWING : ANIM_HANG);
                } else
                    return animation.setAnim(ANIM_HANG, -15);
            }
        }

        if (state == STATE_FORWARD_JUMP || state == STATE_FALL_BACK) {
            if (emptyHands()) {
                if (input & ACTION) return STATE_REACH;
                if ((input & (JUMP | FORTH | WALK)) == (JUMP | FORTH | WALK)) return STATE_SWAN_DIVE;
            }
        } else
            if (state != STATE_FALL && state != STATE_FALL_BACK && state != STATE_SWAN_DIVE && state != STATE_FAST_DIVE && state != STATE_REACH && state != STATE_UP_JUMP && state != STATE_BACK_JUMP && state != STATE_LEFT_JUMP && state != STATE_RIGHT_JUMP)
                return animation.setAnim(ANIM_FALL_FORTH);// (state == STATE_FAST_BACK || state == STATE_SLIDE_BACK || state == STATE_ROLL_2) ? ANIM_FALL_BACK :  ANIM_FALL_FORTH);

        if (state == STATE_SWAN_DIVE)
            return STATE_FAST_DIVE;

        return state;
    }

    int entityQuadrant(const TR::Entity &entity) {
        int ix = int(pos.x) / 1024;
        int iz = int(pos.z) / 1024;

        int bx = entity.x / 1024;
        int bz = entity.z / 1024;

        int q = -1;
        if (abs(bx - ix) ^ abs(bz - iz)) {
            if (bx > ix) q = 1;
            if (bx < ix) q = 3;
            if (bz > iz) q = 0;
            if (bz < iz) q = 2;
        }

        return q;
    }

    Block* getBlock() {
        for (int i = 0; i < level->entitiesCount; i++) {
            TR::Entity &e = level->entities[i];
            if (!e.controller || !e.isBlock())
                continue;

            Block *block = (Block*)e.controller;
            float oldAngle = block->angle.y;
            block->angle.y = angleQuadrant(angle.y) * (PI * 0.5f);

            if (!checkInteraction(block, &TR::Limits::BLOCK, (input & ACTION) != 0)) {
                block->angle.y = oldAngle;
                continue;
            }

            return block;
        }
        return NULL;
    }

    virtual int getStateGround() {
        int res = STATE_STOP;
        angle.x = 0.0f;

        if ((input == ACTION) && (state == STATE_STOP) && emptyHands() && doPickUp())
            return state;

        if ((input & (FORTH | ACTION)) == (FORTH | ACTION) && (animation.index == ANIM_STAND || animation.index == ANIM_STAND_NORMAL) && emptyHands() && collision.side == Collision::FRONT) { // TODO: get rid of animation.index
            float floor   = collision.info[Collision::FRONT].floor;
            float ceiling = collision.info[Collision::FRONT].ceiling; 

            float h = pos.y - floor;

            int aIndex = animation.index;
            if (floor == ceiling || h < 256)
                ;// do nothing
            else if (h <= 2 * 256 + 128) {
                aIndex = ANIM_CLIMB_2;
                pos.y  = floor + 512.0f;
            } else if (h <= 3 * 256 + 128) {
                aIndex = ANIM_CLIMB_3;
                pos.y  = floor + 768.0f;
            } else if (h <= 7 * 256 + 128)
                aIndex = ANIM_CLIMB_JUMP;

            if (aIndex != animation.index) {
                alignToWall(-LARA_RADIUS);
                return animation.setAnim(aIndex);
            }
        }

        if ( (input & (FORTH | BACK)) == (FORTH | BACK) && (animation.index == ANIM_STAND_NORMAL || state == STATE_RUN) )
            return animation.setAnim(ANIM_STAND_ROLL_BEGIN);

        // ready to jump
        if (state == STATE_COMPRESS) {
            float   ext = angle.y;
            switch (input & (RIGHT | LEFT | FORTH | BACK)) {
                case RIGHT         : res = STATE_RIGHT_JUMP;    ext +=  PI * 0.5f; break;
                case LEFT          : res = STATE_LEFT_JUMP;     ext -=  PI * 0.5f; break;
                case FORTH | LEFT  :
                case FORTH | RIGHT :
                case FORTH         : res = STATE_FORWARD_JUMP;  break;
                case BACK          : res = STATE_BACK_JUMP;     ext +=  PI;        break;
                default            : res = STATE_UP_JUMP;       break;
            }

            if (res != STATE_UP_JUMP) {
                vec3 p = pos;
                collision  = Collision(this, getRoomIndex(), p, vec3(0.0f), vec3(0.0f), LARA_RADIUS * 2.5f, ext, 0, LARA_HEIGHT, 256 + 128, 0xFFFFFF);
                if (collision.side == Collision::FRONT)
                    res = STATE_UP_JUMP;
            }

            return res;
        }

        // jump button is pressed
        if (input & JUMP) {
            if ((input & FORTH) && state == STATE_FORWARD_JUMP)
                return STATE_RUN;
            if (state == STATE_RUN)
                return STATE_FORWARD_JUMP;
            if (animation.index == ANIM_SLIDE_BACK) // TODO: animation index? %)
                return STATE_SLIDE_BACK;
            return STATE_COMPRESS;
        }

        // walk button is pressed
        if ((input & WALK) && animation.index != ANIM_RUN_START) {
            float ext = angle.y;

            if (input & FORTH) { 
                if (state == STATE_BACK)
                    res = STATE_STOP;
                else
                    res = STATE_WALK;
            } else if (input & BACK) {
                res = STATE_BACK;
                ext += PI;
            } else if (input & LEFT) {
                res = STATE_STEP_LEFT;
                ext -= PI * 0.5f;
            } else if (input & RIGHT) {
                res = STATE_STEP_RIGHT;
                ext += PI * 0.5f;
            }

            int maxAscent  = 256 + 128;
            int maxDescent = maxAscent;

            if (state == STATE_STEP_LEFT || state == STATE_STEP_RIGHT)
                maxAscent = maxDescent = 64;

            if (state == STATE_STOP && res != STATE_STOP) {
                vec3 p = pos;
                collision  = Collision(this, getRoomIndex(), p, vec3(0.0f), vec3(0.0f), LARA_RADIUS * 1.1f, ext, 0, LARA_HEIGHT, maxAscent, maxDescent);
                if (collision.side == Collision::FRONT)
                    res = STATE_STOP;
            }

            return res;
        }

        if ((input & ACTION) && emptyHands()) {
            if (state == STATE_PUSH_PULL_READY && (input & (FORTH | BACK))) {
                int pushState = (input & FORTH) ? STATE_PUSH_BLOCK : STATE_PULL_BLOCK;
                Block *block = getBlock();
                if (block && animation.canSetState(pushState) && block->doMove((input & FORTH) != 0))
                    return pushState;
            }

            if (state == STATE_PUSH_PULL_READY || getBlock())
                return STATE_PUSH_PULL_READY;
        }

        // only dpad buttons pressed
        if (input & FORTH)
            res = STATE_RUN;
        else if (input & BACK)
            res = STATE_FAST_BACK;
        else if (input & (LEFT | RIGHT)) {
            if (state == STATE_FAST_TURN)
                return state;

            if (input & LEFT)  return (state == STATE_TURN_LEFT  && animation.prev == animation.index) ? STATE_FAST_TURN : STATE_TURN_LEFT;
            if (input & RIGHT) return (state == STATE_TURN_RIGHT && animation.prev == animation.index) ? STATE_FAST_TURN : STATE_TURN_RIGHT;
        }

        if (state == STATE_STOP && res != STATE_STOP) {
            float ext = angle.y + (res == STATE_RUN ? 0.0f : PI);
            vec3 p = pos;
            collision  = Collision(this, getRoomIndex(), p, vec3(0.0f), vec3(0.0f), LARA_RADIUS * 1.1f, ext, 0, LARA_HEIGHT, 256 + 128, 0xFFFFFF);
            if (collision.side == Collision::FRONT)
                res = STATE_STOP;
        }

        return res;
    }

    void slideStart() {
        if (state != STATE_SLIDE && state != STATE_SLIDE_BACK) {
            TR::Level::FloorInfo info;
            getFloorInfo(getRoomIndex(), pos, info);

            int sx = abs(info.slantX), sz = abs(info.slantZ);
            // get direction
            float dir;
            if (sx > sz)
                dir = info.slantX > 0 ? 3.0f : 1.0f;
            else
                dir = info.slantZ > 0 ? 2.0f : 0.0f;
            dir *= PI * 0.5f;

            int aIndex = ANIM_SLIDE_FORTH;
            if (fabsf(shortAngle(dir, angle.y)) > PI * 0.5f) {
                aIndex = ANIM_SLIDE_BACK;
                dir += PI;
            }

            angle.y = dir;
            animation.setAnim(aIndex);
        }
    }

    virtual int getStateSlide() {
        if (input & JUMP)
            return state == STATE_SLIDE ? STATE_FORWARD_JUMP : STATE_BACK_JUMP;
        return state;
    }

    virtual int getStateHang() {
        if (input & LEFT)  return STATE_HANG_LEFT;
        if (input & RIGHT) return STATE_HANG_RIGHT;
        if (input & FORTH) {
            // possibility check
            TR::Level::FloorInfo info;
            getFloorInfo(getRoomIndex(), pos + getDir() * (LARA_RADIUS + 2.0f), info);
            if (info.floor - info.ceiling >= LARA_HEIGHT)
                return (input & WALK) ? STATE_HANDSTAND : STATE_HANG_UP;            
        }
        return STATE_HANG;
    }

    virtual int getStateUnderwater() {
        if ((input == ACTION) && (state == STATE_TREAD) && emptyHands() && doPickUp())
            return state;

        if (state == STATE_FORWARD_JUMP || state == STATE_UP_JUMP || state == STATE_BACK_JUMP || state == STATE_LEFT_JUMP || state == STATE_RIGHT_JUMP || state == STATE_FALL || state == STATE_REACH || state == STATE_SLIDE || state == STATE_SLIDE_BACK) {
            game->waterDrop(pos, 256.0f, 0.2f);
            if (level->extra.waterSplash > -1)
                game->addEntity(TR::Entity::WATER_SPLASH, getRoomIndex(), pos);
            pos.y += 100.0f;
            angle.x = -45.0f * DEG2RAD;
            return animation.setAnim(ANIM_WATER_FALL); // TODO: wronng animation
        }

        if (state == STATE_SWAN_DIVE || state == STATE_FAST_DIVE) {
            angle.x = -PI * 0.5f;
            game->waterDrop(pos, 128.0f, 0.2f);
            if (level->extra.waterSplash > -1)
                game->addEntity(TR::Entity::WATER_SPLASH, getRoomIndex(), pos);
            return STATE_DIVE;
        }

        if (input & JUMP) return STATE_SWIM;

        if (state == STATE_GLIDE && speed < LARA_SWIM_SPEED * 2.0f / 3.0f)
            return STATE_TREAD;

        return STATE_GLIDE;
    }

    virtual int getStateOnwater() {
        angle.x = 0.0f;

        if (state == STATE_WATER_OUT) return state;

        if (state != STATE_SURF_TREAD && state != STATE_SURF_LEFT && state != STATE_SURF_RIGHT && state != STATE_SURF_SWIM && state != STATE_SURF_BACK && state != STATE_STOP) {
            game->waterDrop(pos, 128.0f, 0.2f);
            specular = LARA_WET_SPECULAR;
            return animation.setAnim(ANIM_TO_ONWATER);
        }

        if (state == STATE_SURF_TREAD) {
            if (animation.isFrameActive(0))
                game->waterDrop(animation.getJoints(getMatrix(), jointHead).pos, 96.0f, 0.03f);
        } else {
            if (animation.frameIndex % 4 == 0)
                game->waterDrop(animation.getJoints(getMatrix(), jointHead).pos, 96.0f, 0.02f);
        }

        if (input & FORTH) {
            if (input & JUMP) 
                return goUnderwater();

            if ((input & ACTION) && waterOut()) {
                game->waterDrop(pos, 128.0f, 0.2f);
                return state;
            }

            return STATE_SURF_SWIM;
        }

        if (input & BACK)  return STATE_SURF_BACK;
        if (input & WALK) {
            if (input & LEFT)  return STATE_SURF_LEFT;
            if (input & RIGHT) return STATE_SURF_RIGHT;
        }
        return STATE_SURF_TREAD;
    }

    virtual int getStateDeath() {
        velocity = vec3(0.0f);
        return (stand == STAND_UNDERWATER || stand == STAND_ONWATER) ? STATE_UNDERWATER_DEATH : (state == STATE_MIDAS_DEATH ? STATE_MIDAS_DEATH : STATE_DEATH);
    }

    virtual int getStateDefault() {
        if (state == STATE_DIVE || (state == STATE_RUN && (input & JUMP)) ) return state;
        switch (stand) {
            case STAND_GROUND     : return STATE_STOP;
            case STAND_HANG       : return STATE_HANG;
            case STAND_ONWATER    : return STATE_SURF_TREAD;
            case STAND_UNDERWATER : return STATE_TREAD;
            default : ;
        }
        return STATE_FALL;
    }

    virtual int getInput() { // TODO: updateInput
        if (level->isCutsceneLevel()) return 0;
        input = 0;

        if (!dozy && ((Input::state[cAction] && Input::state[cJump] && Input::state[cLook] && Input::state[cStepRight]) || Input::down[ikO])) {
            dozy = true;
            health = LARA_MAX_HEALTH;
            oxygen = LARA_MAX_OXYGEN;
            reset(getRoomIndex(), pos - vec3(0, 512, 0), angle.y, STAND_UNDERWATER);
            return input;
        }

        if (dozy && Input::state[cWalk]) {
            dozy = false;
            return input;
        }

        input = Character::getInput();
        if (input & DEATH) return input;

        if (Input::state[cUp])        input |= FORTH;
        if (Input::state[cRight])     input |= RIGHT;
        if (Input::state[cDown])      input |= BACK;
        if (Input::state[cLeft])      input |= LEFT;
        if (Input::state[cRoll])      input  = FORTH | BACK;
        if (Input::state[cStepRight]) input  = WALK  | RIGHT;
        if (Input::state[cStepLeft])  input  = WALK  | LEFT;
        if (Input::state[cJump])      input |= JUMP;
        if (Input::state[cWalk])      input |= WALK;
        if (Input::state[cAction])    input |= ACTION;
        if (Input::state[cWeapon])    input |= WEAPON;

    // scion debug (TODO: remove)
        if (Input::down[ikP]) {
            switch (level->id) {
                case TR::LVL_TR1_3A :
                    reset(51, vec3(41015, 3584, 34494), -PI);        // level 3a (t-rex)
                    break;
                case TR::LVL_TR1_3B :
                    reset(5, vec3(73394, 3840, 60758), 0); // level 3b (scion)
                    break;
                case TR::LVL_TR1_4 :
                    reset(18, vec3(34914, 11008, 41315), 90 * DEG2RAD); // main hall
                    break;
                case TR::LVL_TR1_6 :
                    reset(73, vec3(73372, 122, 51687), PI * 0.5f);       // level 6 (midas hand)
                    break;
                case TR::LVL_TR1_7B :
                    reset(77, vec3(36943, -4096, 62821), 270 * DEG2RAD); // level 7b (heavy trigger)
                    break;
                case TR::LVL_TR2_WALL :
                    //reset(44, vec3(62976, 1536, 23040), 0);
                    reset(44, vec3(62976, 1536, 23040), 0);
                    break;
                case TR::LVL_TR3_TEMPLE :
                    reset(204, vec3(40562, 3584, 58694), 0);
                    break;
                default : game->playSound(TR::SND_NO, pos, Sound::PAN);
            }
        }

    // analog control
        rotFactor = vec2(1.0f);

        if ((state == STATE_STOP || state == STATE_SURF_TREAD || state == STATE_HANG) && fabsf(Input::joy.L.x) < 0.5f && fabsf(Input::joy.L.y) < 0.5f)
            return input;

        bool moving = state == STATE_RUN || state == STATE_WALK || state == STATE_BACK || state == STATE_FAST_BACK || state == STATE_SURF_SWIM || state == STATE_SURF_BACK || state == STATE_FORWARD_JUMP;

        if (!moving) {
            if (fabsf(Input::joy.L.x) < fabsf(Input::joy.L.y))
                Input::joy.L.x = 0.0f;
            else
                Input::joy.L.y = 0.0f;
        }

        if (Input::joy.L.x != 0.0f) {
            input |= (Input::joy.L.x < 0.0f) ? LEFT : RIGHT;
            if (moving || stand == STAND_UNDERWATER || stand == STAND_ONWATER)
                rotFactor.y = min(fabsf(Input::joy.L.x) / 0.9f, 1.0f);
        }

        if (Input::joy.L.y != 0.0f) {
            input |= (Input::joy.L.y < 0.0f) ? FORTH : BACK;
            if (stand == STAND_UNDERWATER)
                rotFactor.x = min(fabsf(Input::joy.L.y) / 0.9f, 1.0f);
        }

        return input;
    }

    virtual void doCustomCommand(int curFrame, int prevFrame) {
        switch (state) {
            case STATE_PICK_UP : {
                int pickupFrame = stand == STAND_GROUND ? PICKUP_FRAME_GROUND : PICKUP_FRAME_UNDERWATER;
                if (animation.isFrameActive(pickupFrame)) {
                    for (int i = 0; i < pickupListCount; i++) {
                        if (pickupList[i]->getEntity().type == TR::Entity::SCION_PICKUP_HOLDER)
                            continue;
                        pickupList[i]->deactivate();
                        pickupList[i]->flags.invisible = true;
                        game->invAdd(pickupList[i]->getEntity().type, 1);
                    }
                    pickupListCount = 0;
                }
                break;
            }
            case STATE_USE_KEY    :                                  
            case STATE_USE_PUZZLE : {
                if (keyHole && animation.isFrameActive(state == STATE_USE_PUZZLE ? PUZZLE_FRAME : KEY_FRAME)) {
                    keyHole->activate();
                    keyHole = NULL;
                }
                break;
            }
        }
    }

    virtual void update() {
        if (level->isCutsceneLevel()) {
            updateAnimation(true);

            updateLights();

            if (fixRoomIndex() && braid)
                braid->update();
        } else {
            Character::update();
            if (braid)
                braid->update();
        }
        
        if (level->isCutsceneLevel())
            return;

        if (damageTime > 0.0f)
            damageTime = max(0.0f, damageTime - Core::deltaTime);

        if (stand == STAND_UNDERWATER && !dozy) {
            if (oxygen > 0.0f)
                oxygen -= Core::deltaTime;
            else
                hit(Core::deltaTime * 150.0f);
        } else
            if (oxygen < LARA_MAX_OXYGEN)
                oxygen = min(LARA_MAX_OXYGEN, oxygen += Core::deltaTime * 10.0f);

        usedKey = TR::Entity::LARA;
    }

    void updateFlash() {
        for (int i = 0; i < 2; i++) {
            if (arms[i].shotTimer < MUZZLE_FLASH_TIME) {
                arms[i].shotTimer += Core::deltaTime;
                float intensity = clamp((0.1f - arms[i].shotTimer) * 20.0f, EPS, 1.0f);
                Core::lightColor[1 + i] = FLASH_LIGHT_COLOR * vec4(intensity, intensity, intensity, 1.0f / sqrtf(intensity));
                Core::lightPos[1 + i]   = animation.getJoints(getMatrix(), i == 0 ? 10 : 13, false).pos;
            } else {
                Core::lightColor[1 + i] = vec4(0, 0, 0, 1);
            }
        }
    }

    virtual void updateAnimation(bool commands) {
        Controller::updateAnimation(commands);
        updateWeapon();
        updateFlash();
        if (stand == STAND_UNDERWATER)
            specular = 0.0f;
        else
            if (specular > 0.0f)
                specular = max(0.0f, specular - LARA_WET_TIMER * Core::deltaTime);

        if (state == STATE_MIDAS_DEATH || state == STATE_MIDAS_USE) {
            uint32 sparklesMask = getMidasMask();

            if (state == STATE_MIDAS_DEATH)
                visibleMask = sparklesMask ^ 0xFFFFFFFF;

            timer += Core::deltaTime;
            if (timer >= 1.0f / 30.0f) {
                timer -= 1.0f / 30.0f;
                addSparks(sparklesMask);
            }
        }
    }

    virtual void updateVelocity() {
        flowVelocity = vec3(0);

        if (!(input & DEATH) && !level->isCutsceneLevel())
            checkTrigger(this, false);

    // get turning angle
        float w = (input & LEFT) ? -1.0f : ((input & RIGHT) ? 1.0f : 0.0f);

        if (state == STATE_SWIM || state == STATE_GLIDE)
            w *= TURN_WATER_FAST;
        else if (state == STATE_TREAD || state == STATE_SURF_TREAD || state == STATE_SURF_SWIM || state == STATE_SURF_BACK)
            w *= TURN_WATER_FAST;
        else if (state == STATE_RUN)
            w  *= sign(w) != sign(tilt) ? 0.0f : w * TURN_FAST * tilt / LARA_TILT_MAX;
        else if (state == STATE_FAST_TURN)
            w *= TURN_FAST;
        else if (state == STATE_FAST_BACK)
            w *= TURN_FAST_BACK;
        else if (state == STATE_TURN_LEFT || state == STATE_TURN_RIGHT || state == STATE_WALK)
            w *= TURN_NORMAL;
        else if (state == STATE_FORWARD_JUMP || state == STATE_BACK)
            w *= TURN_SLOW;
        else
            w = 0.0f;

        if (w != 0.0f)
            rotateY(w * rotFactor.y * Core::deltaTime);
    // pitch (underwater only)
        if (stand == STAND_UNDERWATER && (input & (FORTH | BACK)))
            rotateX(((input & FORTH) ? -TURN_WATER_SLOW : TURN_WATER_SLOW) * rotFactor.x * Core::deltaTime);

    // get animation direction
        angleExt = angle.y;
        switch (state) {
            case STATE_BACK :
            case STATE_SURF_BACK  :
            case STATE_BACK_JUMP  :
            case STATE_FAST_BACK  :
            case STATE_SLIDE_BACK :
            case STATE_ROLL_1     :
                angleExt += PI;
                break;
            case STATE_LEFT_JUMP  :
            case STATE_STEP_LEFT  :
            case STATE_SURF_LEFT  :
            case STATE_HANG_LEFT  :
                angleExt -= PI * 0.5f;
                break;
            case STATE_RIGHT_JUMP :
            case STATE_STEP_RIGHT :
            case STATE_SURF_RIGHT :
            case STATE_HANG_RIGHT :
                angleExt +=  PI * 0.5f;
                break;
        }

        switch (stand) {
            case STAND_AIR :
                applyGravity(velocity.y);
                if (velocity.y >= 154.0f && state == STATE_FALL)
                    game->playSound(TR::SND_SCREAM, pos, Sound::PAN);
                /*
                if (state == STATE_FALL || state == STATE_FAST_DIVE) {
                    velocity.x *= 0.95 * Core::deltaTime;
                    velocity.z *= 0.95 * Core::deltaTime;
                }
                */
                break;
            case STAND_GROUND  :
            case STAND_SLIDE   :
            case STAND_HANG    :
            case STAND_ONWATER : {

                switch (state) {
                    case STATE_SURF_SWIM  :
                    case STATE_SURF_BACK  :
                    case STATE_SURF_LEFT  :
                    case STATE_SURF_RIGHT :
                        speed = min(speed + 30.0f * LARA_WATER_ACCEL * Core::deltaTime, LARA_SURF_SPEED);
                        break;
                    default :
                        speed = animation.getSpeed();
                }

                if (stand == STAND_ONWATER) {
                    velocity.x = sinf(angleExt) * speed;
                    velocity.z = cosf(angleExt) * speed;
                    velocity.y = 0.0f;
                } else {
                    TR::Level::FloorInfo info;
                    if (stand == STAND_HANG) {
                        vec3 p = pos + getDir() * (LARA_RADIUS + 2.0f);
                        getFloorInfo(getRoomIndex(), p, info);
                        if (info.roomAbove != TR::NO_ROOM && info.floor >= pos.y - LARA_HANG_OFFSET)
                            getFloorInfo(info.roomAbove, p, info);
                    } else
                        getFloorInfo(getRoomIndex(), pos, info);

                    vec3 v(sinf(angleExt), 0.0f, cosf(angleExt));
                    velocity = info.getSlant(v) * speed;
                }
                break;
            }
            case STAND_UNDERWATER : {
                if (animation.index == ANIM_TO_UNDERWATER)
                    speed = 15.0f;
                if (state == STATE_SWIM)
                    speed = min(speed + 30.0f * LARA_WATER_ACCEL * Core::deltaTime, LARA_SWIM_SPEED);
                if (state == STATE_TREAD || state == STATE_GLIDE)
                    speed = max(speed - 30.0f * LARA_SWIM_FRICTION * Core::deltaTime, 0.0f);
                velocity = vec3(angle.x, angle.y) * speed;
                // TODO: apply flow velocity
                break;
            }
        }

        if (state == STATE_DEATH || state == STATE_UNDERWATER_DEATH)
            velocity.x = velocity.z = 0.0f;
    }

    virtual void updatePosition() { // TODO: sphere / bbox collision
        if (level->isCutsceneLevel())
            return;

        // tilt control
        vec2 vTilt(LARA_TILT_SPEED * Core::deltaTime, LARA_TILT_MAX);
        if (stand == STAND_UNDERWATER)
            vTilt *= 2.0f;
        vTilt *= rotFactor.y;
        updateTilt(state == STATE_RUN || stand == STAND_UNDERWATER, vTilt.x, vTilt.y);

        collisionOffset = vec3(0.0f);

        if (checkCollisions() || (velocity + flowVelocity + collisionOffset).length2() >= 1.0f) // TODO: stop & smash anim
            move();
    }

    virtual vec3 getPos() {
        return level->isCutsceneLevel() ? chestOffset : pos;
    }

    bool checkCollisions() {
    // check static objects (TODO: check linked rooms?)
        const TR::Room &room = getRoom();
        Box box(pos - vec3(LARA_RADIUS, LARA_HEIGHT, LARA_RADIUS), pos + vec3(LARA_RADIUS, 0.0f, LARA_RADIUS));

        for (int i = 0; i < room.meshesCount; i++) {
            TR::Room::Mesh &m  = room.meshes[i];
            TR::StaticMesh &sm = level->staticMeshes[m.meshIndex];
            if (sm.flags != 2) continue;
            Box meshBox;
            sm.getBox(true, m.rotation, meshBox);
            meshBox.translate(vec3(float(m.x), float(m.y), float(m.z)));
            if (!box.intersect(meshBox)) continue;

            collisionOffset += meshBox.pushOut2D(box);
        }

    // check enemies & doors
        for (int i = 0; i < level->entitiesCount; i++) {
            const TR::Entity &e = level->entities[i];

            if (!e.controller || !e.isCollider()) continue;

            Controller *controller = (Controller*)e.controller;

            if (e.isEnemy()) {
                if (e.type != TR::Entity::ENEMY_REX && (controller->flags.active != TR::ACTIVE || ((Character*)controller)->health <= 0)) continue;
            } else {
            // fast distance check for object
                if (e.type != TR::Entity::HAMMER_HANDLE && e.type != TR::Entity::HAMMER_BLOCK && e.type != TR::Entity::SCION_HOLDER)
                    if (fabsf(pos.x - controller->pos.x) > 1024 || fabsf(pos.z - controller->pos.z) > 1024 || fabsf(pos.y - controller->pos.y) > 2048) continue;
            }

            vec3 dir = pos - vec3(0.0f, 128.0f, 0.0f) - controller->pos;
            vec3 p   = dir.rotateY(controller->angle.y);

            Box box = controller->getBoundingBoxLocal();
            box.expand(vec3(LARA_RADIUS + 50.0f, 0.0f, LARA_RADIUS + 50.0f));
            box.max.y += 768;

            if (!box.contains(p)) // TODO: Box vs Box or check Lara's head point? (check thor hammer handle)
                continue;

            if (e.isEnemy()) { // enemy collision
                if (!collide(controller, false))
                    continue;
            //    velocity.x = velocity.y = 0.0f;
            } else { // door collision
                p += box.pushOut2D(p);
                p = (p.rotateY(-controller->angle.y) + controller->pos) - pos;
                collisionOffset += vec3(p.x, 0.0f, p.z);
            }

            if (e.type == TR::Entity::ENEMY_REX && ((Character*)controller)->health <= 0)
                return true;
            if (!e.isEnemy() || e.type == TR::Entity::ENEMY_BAT)
                return true;

            if (canHitAnim()) { // TODO: check enemy type and health here
            // get hit dir
                if (hitDir == -1) {
                    if (health > 0)
                        game->playSound(TR::SND_HIT, pos, Sound::PAN);
                    hitTime = 0.0f;
                }

                hitDir = angleQuadrant(dir.rotateY(angle.y + PI * 0.5f).angleY());
                return true;
            }
        };

        if (lightning && lightning->flash && !lightning->armed) {
            if (hitDir == -1)
                hitTime = 0.0f;
            hitDir = int(randf() * 4);
        } else {
            hitDir = -1;
            lightning = NULL;
        }
        return false;
    }

    void move() {
        vec3 vel = (velocity + flowVelocity) * Core::deltaTime * 30.0f + collisionOffset;
        vec3 opos(pos), offset(0.0f);

        float radius   = stand == STAND_UNDERWATER ? LARA_RADIUS_WATER : LARA_RADIUS;
        int maxHeight  = stand == STAND_UNDERWATER ? LARA_HEIGHT_WATER : LARA_HEIGHT;
        int minHeight  = 0;
        int maxAscent  = 256 + 128;
        int maxDescent = 0xFFFFFF;

        int room = getRoomIndex();

        if (state == STATE_WALK || state == STATE_BACK)
            maxDescent = maxAscent;
        if (state == STATE_STEP_LEFT || state == STATE_STEP_RIGHT)
            maxAscent = maxDescent = 64;
        if (stand == STAND_ONWATER) {
            maxAscent = -1;
            maxHeight =  0;
            offset.y  = -1;
        }

        bool standHang = stand == STAND_HANG && state != STATE_HANG_UP && state != STATE_HANDSTAND;

        if (standHang) {
            maxHeight = 0;
            maxAscent = maxDescent = 64;
            offset    = getDir() * (LARA_RADIUS + 32.0f);
            offset.y  -= LARA_HANG_OFFSET + 32;
        }
        if (stand == STAND_UNDERWATER) {
            offset.y += LARA_HEIGHT_WATER * 0.5f;
        }

        collision = Collision(this, room, pos, offset, vel, radius, angleExt, minHeight, maxHeight, maxAscent, maxDescent);

        if (!standHang && (collision.side == Collision::LEFT || collision.side == Collision::RIGHT)) {
            float rot = TURN_WALL_Y * Core::deltaTime;
            rotateY((collision.side == Collision::LEFT) ? rot : -rot);
        }

        if (standHang && collision.side != Collision::FRONT) {
            offset.x = offset.z = 0.0f;
            minHeight  = LARA_HANG_OFFSET;
            maxDescent = 0xFFFFFF;
            maxAscent  = -LARA_HANG_OFFSET;
            vec3 p = pos;
            collision  = Collision(this, room, p, offset, vec3(0.0f), radius, angleExt, minHeight, maxHeight, maxAscent, maxDescent);
            if (collision.side == Collision::FRONT)
                pos = opos;
        }

    // get current leading foot in animation
        int rightStart = 0;
        if (state == STATE_RUN)  rightStart = 6;
        if (state == STATE_WALK) rightStart = 13;
        if (state == STATE_BACK) rightStart = 28;
        bool isLeftFoot = animation.frameIndex < rightStart || animation.frameIndex > (rightStart + animation.framesCount / 2);


        if (stand == STAND_UNDERWATER) {
            if (collision.side == Collision::TOP)
                rotateX(-TURN_WALL_X * Core::deltaTime);
            if (collision.side == Collision::BOTTOM)
                rotateX( TURN_WALL_X * Core::deltaTime);
        }

        if (stand == STAND_AIR && collision.side == Collision::TOP && velocity.y < 0.0f)
            velocity.y = 30.0f;

        if (collision.side == Collision::FRONT) {
            float floor = collision.info[Collision::FRONT].floor;
/*
            switch (angleQuadrant(angleExt - angle.y)) {
                case 0 : collision.side = Collision::FRONT; LOG("FRONT\n"); break;
                case 1 : collision.side = Collision::RIGHT; LOG("RIGHT\n"); break;
                case 2 : collision.side = Collision::BACK;  LOG("BACK\n");  break;
                case 3 : collision.side = Collision::LEFT;  LOG("LEFT\n");  break;
            }
*/
            if (velocity.dot(getDir()) <= EPS) 
                collision.side = Collision::NONE;

        // hit the wall
            switch (stand) {
                case STAND_AIR :
                    if (state == STATE_UP_JUMP || state == STATE_REACH || state == STATE_FALL_BACK)
                        velocity.x = velocity.z = 0.0f;

                    if (velocity.x != 0.0f || velocity.z != 0.0f) {
                        animation.setAnim(ANIM_SMASH_JUMP);
                        velocity.x = -velocity.x * 0.25f;
                        velocity.z = -velocity.z * 0.25f;
                        if (velocity.y < 0.0f)
                            velocity.y = 30.0f;
                    }
                    break;
                case STAND_GROUND :
                case STAND_HANG   :
                    if (opos.y - floor > (256 * 3 - 128) && state == STATE_RUN)
                        animation.setAnim(isLeftFoot ? ANIM_SMASH_RUN_LEFT : ANIM_SMASH_RUN_RIGHT);
                    else if (stand == STAND_HANG)
                        animation.setAnim(ANIM_HANG, -21);
                    else if (state != STATE_ROLL_1 && state != STATE_ROLL_2)
                        animation.setAnim((state == STATE_RUN || state == STATE_WALK) ? (isLeftFoot ? ANIM_STAND_LEFT : ANIM_STAND_RIGHT) : ANIM_STAND);
                    velocity.x = velocity.z = 0.0f;
                    break;
                case STAND_UNDERWATER :
                    if (fabsf(angle.x) > TURN_WALL_X_CLAMP)
                        rotateX(TURN_WALL_X * Core::deltaTime * sign(angle.x));
                    else
                        pos.y = opos.y;
                    break;
                default : ;// no smash animation
            }
        } else {
            if (stand == STAND_GROUND) {
                float floor = collision.info[Collision::NONE].floor;
                float h = floor - opos.y;

                if (h >= 128 && (state == STATE_WALK || state == STATE_BACK)) { // descend
                    if (state == STATE_WALK) animation.setAnim(isLeftFoot ? ANIM_WALK_DESCEND_LEFT : ANIM_WALK_DESCEND_RIGHT);
                    if (state == STATE_BACK) animation.setAnim(isLeftFoot ? ANIM_BACK_DESCEND_LEFT : ANIM_BACK_DESCEND_RIGHT);
                    pos.y = float(floor);
                } else if (h > -1.0f) {
                    pos.y = min(float(floor), pos.y += DESCENT_SPEED * Core::deltaTime);
                } else if (h > -128) {
                    pos.y = float(floor);
                } else if (h >= -(256 + 128) && (state == STATE_RUN || state == STATE_WALK)) { // ascend
                    if (state == STATE_RUN)  animation.setAnim(isLeftFoot ? ANIM_RUN_ASCEND_LEFT  : ANIM_RUN_ASCEND_RIGHT);
                    if (state == STATE_WALK) animation.setAnim(isLeftFoot ? ANIM_WALK_ASCEND_LEFT : ANIM_WALK_ASCEND_RIGHT);
                    pos.y = float(floor);
                } else
                    pos.y = float(floor);
            }
            collision.side = Collision::NONE;
        }

        if (dozy) stand = STAND_GROUND;
        checkRoom();
        if (dozy) stand = STAND_UNDERWATER;
    }

    virtual void applyFlow(TR::Camera &sink) {
        if (stand != STAND_UNDERWATER && stand != STAND_ONWATER) return;

        vec3 target = vec3(float(sink.x), float(sink.y), float(sink.z));

    #ifdef _DEBUG
        //delete[] dbgBoxes;
        //dbgBoxes = NULL;
    #endif

        if (box != sink.flags.boxIndex) {
            uint16 *boxes;
            uint16 count = game->findPath(0xFFFFFF, -0xFFFFFF, false, box, sink.flags.boxIndex, getZones(), &boxes);
            if (count > 1) {
            #ifdef _DEBUG
                //dbgBoxesCount = count;
                //dbgBoxes = new uint16[dbgBoxesCount];
                //memcpy(dbgBoxes, boxes, sizeof(uint16) * dbgBoxesCount);
            #endif
                TR::Box &b = level->boxes[boxes[1]];
                target.x = (b.minX + b.maxX) * 0.5f;
                if (target.y > b.floor)
                    target.y = float(b.floor);
                target.z = (b.minZ + b.maxZ) * 0.5f;
            }
        }

        flowVelocity = vec3(0);
        if (!dozy) {
            float speed = sink.speed * 6.0f;
            flowVelocity.x = clamp(target.x - pos.x, -speed, +speed);
            flowVelocity.y = clamp(target.y - pos.y, -speed, +speed);
            flowVelocity.z = clamp(target.z - pos.z, -speed, +speed);

            if (stand == STAND_ONWATER)
                goUnderwater();
        }
    }

    uint32 getMidasMask() {
        if (state == STATE_MIDAS_USE)
            return BODY_ARM_L3 | BODY_ARM_R3;

        uint32 mask = 0;
        int frame = animation.frameIndex;
        if (frame > 4  ) mask |= BODY_LEG_L3 | BODY_LEG_R3;
        if (frame > 69 ) mask |= BODY_LEG_L2;
        if (frame > 79 ) mask |= BODY_LEG_L1;
        if (frame > 99 ) mask |= BODY_LEG_R2;
        if (frame > 119) mask |= BODY_LEG_R1 | BODY_HIP;
        if (frame > 134) mask |= BODY_CHEST;
        if (frame > 149) mask |= BODY_ARM_L1;
        if (frame > 162) mask |= BODY_ARM_L2;
        if (frame > 173) mask |= BODY_ARM_L3;
        if (frame > 185) mask |= BODY_ARM_R1;
        if (frame > 194) mask |= BODY_ARM_R2;
        if (frame > 217) mask |= BODY_ARM_R3;
        if (frame > 224) mask |= BODY_HEAD;
        return mask;
    }

    void renderMuzzleFlash(MeshBuilder *mesh, const Basis &basis, const vec3 &offset, float time) {
        ASSERT(level->extra.muzzleFlash);
        if (time > MUZZLE_FLASH_TIME) return;
        float alpha = min(1.0f, (0.1f - time) * 20.0f);
        float lum   = 3.0f;
        Basis b(basis);
        b.rotate(quat(vec3(1, 0, 0), -PI * 0.5f));
        b.translate(offset);
        if (level->version & (TR::VER_TR2 | TR::VER_TR3))
            lum = alpha;
        Core::active.shader->setParam(uMaterial, vec4(lum, 0.0f, 0.0f, alpha));
        Core::active.shader->setParam(uBasis, b);
        mesh->renderModel(level->extra.muzzleFlash);
    }

    virtual void render(Frustum *frustum, MeshBuilder *mesh, Shader::Type type, bool caustics) { // TODO TR3 render in additive pass
        uint32 visMask = visibleMask;
        if (Core::pass != Core::passShadow && game->getCamera()->firstPerson) // hide head from first person view
            visibleMask &= ~BODY_HEAD;
        Controller::render(frustum, mesh, type, caustics);
        visibleMask = visMask;

        chestOffset = animation.getJoints(getMatrix(), jointChest).pos; // TODO: move to update func

        if (braid)
            braid->render(mesh);

        if (wpnCurrent != Weapon::SHOTGUN && Core::pass != Core::passShadow && (arms[0].shotTimer < MUZZLE_FLASH_TIME || arms[1].shotTimer < MUZZLE_FLASH_TIME)) {
            mat4 matrix = getMatrix();
            game->setShader(Core::pass, Shader::FLASH, false, true);
            
            int meshTransp = mesh->transparent;
            float zOffset;
            if (level->version & (TR::VER_TR2 | TR::VER_TR3)) {
                mesh->transparent = 2;
                Core::setBlending(bmAdd);
                zOffset = 180;
            } else {
                Core::setBlending(bmAlpha);
                zOffset = 150;
            }

            renderMuzzleFlash(mesh, animation.getJoints(matrix, 10, true), vec3(-10, -50, zOffset), arms[0].shotTimer);
            renderMuzzleFlash(mesh, animation.getJoints(matrix, 13, true), vec3( 10, -50, zOffset), arms[1].shotTimer);

            mesh->transparent = meshTransp;
            switch (mesh->transparent) {
                case 0 : Core::setBlending(bmNone);  break;
                case 1 : Core::setBlending(bmAlpha); break;
                case 2 : Core::setBlending(bmAdd);   break;
            }
        }

        if (state == STATE_MIDAS_DEATH /* && Core::pass == Core::passCompose */) {
            game->setRoomParams(getRoomIndex(), Shader::MIRROR, 1.2f, 1.0f, 0.2f, 1.0f, false);
        /* catsuit test
            game->setRoomParams(getRoomIndex(), Shader::MIRROR, 0.3f, 0.3f, 0.3f, 1.0f, false);
            Core::active.shader->setParam(uLightColor, Core::lightColor[0], MAX_LIGHTS);
            Core::active.shader->setParam(uLightPos,   Core::lightPos[0],   MAX_LIGHTS);
        */
            environment->bind(sEnvironment);
            Core::setBlending(bmAlpha);
            visibleMask ^= 0xFFFFFFFF;
            Controller::render(frustum, mesh, type, caustics);
            visibleMask ^= 0xFFFFFFFF;
            Core::setBlending(bmNone);
        }
    }
};

#endif
