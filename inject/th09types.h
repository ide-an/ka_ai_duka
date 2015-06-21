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
            char unknown1[0x4];
            float radius;
            char unknown2[0x10 - 0x4 - sizeof(float)];
            float item_size;
            float speed_fast;
            float speed_slow;
            float speed_fast_div_sqrt2;
            float speed_slow_div_sqrt2;
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
            char unknown1[0x20];
            unsigned int is_ai;
            char unknown2[0xA0 - 0x20 - sizeof(int)];
            unsigned int card_attack_level;
            unsigned int boss_card_attack_level;
            unsigned int life;
            char unknown3[0x1B88 - 0xA8 - 4];
            struct Vector3D position;
            char unknown4[0x1C60 - 0x1B88 - sizeof(Vector3D)];
            struct Rect3D rect1;
            struct Rect3D rect2;
            struct Rect3D rect3; // vs Item
            struct Vector3D rect_size1;//0x1CA8 実際には2倍のサイズ
            struct Vector3D rect_size2;//0x1CB4 実際には2倍のサイズ
            struct Vector3D rect_size3;//0x1CC0 実際には2倍のサイズ
            char unknown5[0x30338 - 0x1CC0 - sizeof(Vector3D)];
            struct PlayerFeature* feature;
            char unknown6[0x30384 - 0x30338 - sizeof(PlayerFeature *)];
            float charge_current;
            float charge_max;
            char unknown7[0x30414 - 0x30388 - sizeof(float)];
            unsigned int combo;
            char unknown8[4];
            unsigned int spell_point;
            char unknown9[0x30454 - 0x3041C - sizeof(int)];
            struct Item items[4];
        };

        enum PlayerCharacter{
            Reimu = 0,
            Marisa,
            Sakuya,
            Youmu,
            Reisen,
            Cirno,
            Lyrica,
            Mystia,
            Tewi,
            Yuuka,
            Aya,
            Medicine,
            Komachi,
            Eiki,
            Merlin,
            Lunasa
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
            char unknown3[0xDBE - 0xD58 - sizeof(Vector3D)];
            unsigned int status;
            char unknown4[0x10BC - 0xDBE - sizeof(int)];
            unsigned int unknown_status;
            //char unknown5[0x10C0 - 0x10BC - sizeof(int)];
            short bullet_type;
            char unknown6[0x10C4 - 0x10C0 - sizeof(short)];
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

        struct BulletContainer{
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

        struct EnemyContainer{
            char unknown1[0x5758];
            struct Enemy enemies[128];
        };

        struct Board{
            struct Player* player;//4A7D94
            struct BulletContainer* bullet_container;//4A7D98
            char unknown1[4];
            struct EnemyContainer* enemy_container;//4A7DA0
            char unknown2[8];
            struct Score* score;//4A7DAC
            PlayerCharacter player_character;//4A7DB0
            char unknown3[0x38 - (0x4A7DB0 - 0x4A7D94) - sizeof(PlayerCharacter)];
        };

        struct ExSakuyaKnife{
            struct Vector3D position;
            char unknown1[0x48 - sizeof(Vector3D)];
        };

        struct ExFeature{
            unsigned int is_local;
            struct Vector3D velocity;
            char unknown1[0x14 - 0x4 - sizeof(Vector3D)];
            struct Vector3D velocity_mystia;
            char unknown2[0x4C - 0x14 - sizeof(Vector3D)];
            union ExFeatureUnion {
                int ex_yuuka_type;
                float ex_aya_offset_angle;
            } yuuka_or_aya_info;
            float reisen_radius_or_aya_offset;//うどんげEXの当たり判定半径
            char unknown3[0x58 - 0x50 - sizeof(float)];
            struct ExSakuyaKnife knives[4];
        };

        struct ExAttack{
            char unknown1[4];
            unsigned int is_2P;
            char unknown6[0xC - 0x4 - sizeof(int)];
            int enabled;
            char unknown2[0x1C - 0xC - sizeof(int)];
            int* hitbodies;//構造体へのポインタ? 結局使う必要なさげ
            struct Vector3D position;
            char unknown3[0x34 - 0x20 - sizeof(Vector3D)];
            struct ExFeature* feature;
            char unknown4[0x40 - 0x34 - sizeof(ExFeature *)];
            int update_func_address;
            char unknown5[0x4C - 0x40 - sizeof(int)];
        };

        struct ExAttackContainer{
            char unknown1[0x1C];
            struct ExAttack attacks[256];
        };

        struct NetInfo{
            char unknown1[0x10];
            void* host_address;//IDirectPlay8Address*
        };

        struct KeyState{
            char unknown1[0x6];
            unsigned short system_keys;
            char unknown1_5[0x2C - 0x6 - sizeof(unsigned short)];
            unsigned short keys;//0x2C
            unsigned short prev_keys;//0x2E
            char unknown2[0x32 - 0x2E - sizeof(unsigned short)];
            unsigned short start_pushing_keys;//0x32
            unsigned short start_leaving_keys;//0x34
            char unknown3[0x8E - 0x34 - sizeof(unsigned short)];
        };

        struct ExAttackFuncAddr{
            int reimu;
            int marisa;
            int sakuya_1;
            int sakuya_2;
            int youmu;
            int reisen;
            int cirno;
            int lyrica;
            int merlin;
            int lunasa;
            int mystia_1;
            int mystia_2;
            int mystia_3;
            int mystia_4;
            int mystia_5;
            int tewi;
            int aya;
            int medicine;
            int yuuka;
            int komachi;
            int eiki;
        };
#pragma pack(pop)
    }
}
