#pragma once

namespace ka_ai_duka {
    namespace raw_types {
#pragma pack(push, 1)
        struct Vector3D{
            float x,y,z;
        };

        struct Vector2D{
            float x,y;
        };

        struct Rect3D{
            struct Vector3D ru,ld;
        };

        struct Rect2D{
            struct Vector2D ru,ld;
        };

        struct Size2D{
            float width, height;
        };

        struct PlayerFeature{
            char unknown1[0x24];
            float charge_speed;
        };

        struct Item{
            unsigned int type;
            struct Vector3D position;
            struct Vector3D velocity;
            unsigned int enabled;
            char unknown1[0x2C4 - 0x1C - sizeof(int)];
        };

        struct Player{
            char unknown1[0xA0];
            unsigned int card_attack_level;
            unsigned int boss_card_attack_level;
            unsigned int life;
            char unknown2[0x1B88 - 0xA8 - 4];
            struct Vector3D position;
            char unknown3[0x1C60 - 0x1B88 - sizeof(Vector3D)];
            struct Rect3D rect1;
            struct Rect3D rect2;
            struct Rect3D rect3;
            char unknown4[0x30338 - 0x1C90 - sizeof(Rect3D)];
            struct PlayerFeature* feature;
            char unknown5[0x30384 - 0x30338 - sizeof(PlayerFeature *)];
            float charge_current;
            float charge_max;
            char unknown6[0x30454 - 0x30388 - sizeof(int)];
            struct Item items[4];
        };

        enum PlayerCharacter{
            Reimu = 0
        };

        struct Score{
            char unknown1[4];
            unsigned int score;
        };

        struct Bullet{
            char unknown1[0xD38];
            struct Size2D size;
            char unknown2[0xD4C - 0xD38 - sizeof(Size2D)];
            struct Vector3D position;
            struct Vector3D velocity;
            char unknown3[0x10BC - 0xD58 - sizeof(Size2D)];
            unsigned int status;
            char unknown4[0x10C4 - 0x10BC - sizeof(int)];
        };

        struct Laser{
            char unknown1[0x548];
            struct Vector3D position;
            float angle;
            float length_1;
            float length_2;
            float unknown2;
            float thickness;
            char unknown3[0x584 - 0x564 - sizeof(float)];
            unsigned int enabled;
            char unknown4[0x59C - 0x584 - sizeof(float)];
        };

        struct BulletsLasers{
            char unknown1[0x1A900];
            struct Bullet bullets[536];
            char unknown2[4292];
            struct Laser lasers[48];
        };

        struct Enemy{
            char unknown1[0x2D74];
            struct Vector3D position;
            char unknown2[0x2D8C - 0x2D74 - sizeof(Vector3D)];
            struct Vector3D velocity;
            struct Vector3D velocity2;
            char unknown3[0x2DBC - 0x2D98 - sizeof(Vector3D)];
            struct Size2D size;
            char unknown4[0x2DD4 - 0x2DBC - sizeof(Size2D)];
            struct Vector3D position_copy;
            char unknown5[0x337C - 0x2DD4 - sizeof(Vector3D)];
            unsigned int status;
            unsigned int status2;//ghost activation
            char unknown6[0x3398 - 0x3380 - sizeof(int)];
            struct Rect2D appear_area;
            char unknown7[0x5430 - 0x3398 - sizeof(Rect2D)];
        };

        struct Enemies{
            char unknown1[0x5758];
            struct Enemy enemies[128];
        };

        struct Board{
            struct Player* player;//4A7D94
            struct BulletsLasers* bullets_lasers;//4A7D98
            char unknown1[4];
            struct Enemies* enemies;//4A7DA0
            char unknown2[8];
            struct Score* score;//4A7DAC
            PlayerCharacter player_character;//4A7DB0
            char unknown3[0x38 - (0x4A7DB0 - 0x4A7D94) - sizeof(PlayerCharacter)];
        };

        struct ExFeature{
            unsigned int is_local;
            struct Vector3D velocity;
        };

        struct ExAttack{
            char unknown1[4];
            unsigned int is_2P;
            char unknown2[0x20 - 8];
            struct Vector3D position;
            char unknown3[0x34 - 0x20 - sizeof(Vector3D)];
            struct ExFeature* feature;
            char unknown4[0x4C - 0x34 - sizeof(ExFeature *)];
        };

        struct ExAttacks{
            char unknown1[0x1C];
            struct ExAttack attacks[256];
        };

        struct NetInfo{
            char unknown1[0x10];
            void* host_address;//IDirectPlay8Address*
        };

        struct KeyState{
            char unknown1[0x2C];
            unsigned short keys;
            char unknown2[0x8E - 0x2C - sizeof(unsigned short)];
        };
#pragma pack(pop)
    }
}
