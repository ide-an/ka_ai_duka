#include "ExAttack.h"
#include <cstdlib>
namespace ka_ai_duka{
    namespace managed_types{
        void AddExAttack(
            std::vector<boost::shared_ptr<ExAttack> > &attacks,
            ExAttack* ex_attack
            )
        {
            attacks.push_back(boost::shared_ptr<ExAttack>(ex_attack));
        }
        bool ExAttackFactory(
            raw_types::ExAttack &ex_attack,
            raw_types::ExAttackFuncAddr &ex_func_addr,
            IDGenerator &idgen,
            std::vector<boost::shared_ptr<ExAttack> > &attacks
            )
        {
            int func_addr = ex_attack.update_func_address;
            if(func_addr == ex_func_addr.reimu){
                AddExAttack(attacks, new ExAttackCircle(ex_attack, idgen.NewId(), Reimu, 18.0f));
                return true;
            }else if(func_addr == ex_func_addr.marisa){
                AddExAttack(attacks, new ExAttackUnhittable(ex_attack, idgen.NewId(), Marisa));
                return true;
            }else if(func_addr == ex_func_addr.sakuya_1){
                return false;// ignored
            }else if(func_addr == ex_func_addr.sakuya_2){
                AddExAttack(attacks, new ExAttackSakuya(ex_attack, idgen.NewId(), 0));
                AddExAttack(attacks, new ExAttackSakuya(ex_attack, idgen.NewId(), 1));
                AddExAttack(attacks, new ExAttackSakuya(ex_attack, idgen.NewId(), 2));
                AddExAttack(attacks, new ExAttackSakuya(ex_attack, idgen.NewId(), 3));
                AddExAttack(attacks, new ExAttackSakuya(ex_attack, idgen.NewId(), 4));
                return true;
            }else if(func_addr == ex_func_addr.youmu){
                AddExAttack(attacks, new ExAttackCircle(ex_attack, idgen.NewId(), Youmu, 14.0f));
                return true;
            }else if(func_addr == ex_func_addr.reisen){
                AddExAttack(attacks, new ExAttackReisen(ex_attack, idgen.NewId()));
                return true;
            }else if(func_addr == ex_func_addr.cirno){
                AddExAttack(attacks, new ExAttackCirno(ex_attack, idgen.NewId()));
                return true;
            }else if(func_addr == ex_func_addr.lyrica){
                AddExAttack(attacks, new ExAttackUnhittable(ex_attack, idgen.NewId(), Lyrica));
                return true;
            }else if(func_addr == ex_func_addr.merlin){
                AddExAttack(attacks, new ExAttackUnhittable(ex_attack, idgen.NewId(), Merlin));
                return true;
            }else if(func_addr == ex_func_addr.lunasa){
                AddExAttack(attacks, new ExAttackUnhittable(ex_attack, idgen.NewId(), Lunasa));
                return true;
            }else if(func_addr == ex_func_addr.mystia_1){
                AddExAttack(attacks, new ExAttackMystia(ex_attack, idgen.NewId(), Mystia_Ex));
                return true;
            }else if(func_addr == ex_func_addr.mystia_2){
                AddExAttack(attacks, new ExAttackMystia(ex_attack, idgen.NewId(), Mystia_Charge2));
                return true;
            }else if(func_addr == ex_func_addr.mystia_3){
                AddExAttack(attacks, new ExAttackMystia(ex_attack, idgen.NewId(), Mystia_Charge3));
                return true;
            }else if(func_addr == ex_func_addr.mystia_4){
                AddExAttack(attacks, new ExAttackMystia(ex_attack, idgen.NewId(), Mystia_Boss1));
                return true;
            }else if(func_addr == ex_func_addr.mystia_5){
                AddExAttack(attacks, new ExAttackMystia(ex_attack, idgen.NewId(), Mystia_Boss2));
                return true;
            }else if(func_addr == ex_func_addr.tewi){
                AddExAttack(attacks, new ExAttackCircle(ex_attack, idgen.NewId(), Tewi, 13.0f));
                return true;
            }else if(func_addr == ex_func_addr.aya){
                AddExAttack(attacks, new ExAttackAya(ex_attack, idgen.NewId()));
                return true;
            }else if(func_addr == ex_func_addr.medicine){
                AddExAttack(attacks, new ExAttackNoMove(ex_attack, idgen.NewId(), Medicine, 64.0f));
                return true;
            }else if(func_addr == ex_func_addr.yuuka){
                AddExAttack(attacks, new ExAttackYuuka(ex_attack, idgen.NewId()));
                return true;
            }else if(func_addr == ex_func_addr.komachi){
                AddExAttack(attacks, new ExAttackUnhittable(ex_attack, idgen.NewId(), Komachi));
                return true;
            }else if(func_addr == ex_func_addr.eiki){
                AddExAttack(attacks, new ExAttackNoMove(ex_attack, idgen.NewId(), Eiki, 32.0f));
                return true;
            }
            //UNREACHABLE
            assert(0);
            return false;
        }

        float ExAttack::X(void) const
        {
            if(ex_attack.feature->is_local){
                return ex_attack.position.x;
            }else{
                if(GetPlayerSide(ex_attack) == Side_1P){
                    return ex_attack.position.x - 144.0f - 16.0f;
                }else{
                    return ex_attack.position.x - 144.0f - 336.0f;
                }
            }
        }

        float ExAttack::Y(void) const
        {
            if(ex_attack.feature->is_local){
                return ex_attack.position.y;
            }else{
                return ex_attack.position.y - 16.0f;
            }
        }
        //Circle
        void ExAttackCircle::Update(void)
        {
            hittable_object->SetX(X());
            hittable_object->SetY(Y());
            hittable_object->SetRadius(radius);
        }
        //Sakuya
        float ExAttackSakuya::X() const
        {
            assert(0 <= index && index < 5);
            if(index == 0){
                return ex_attack.position.x;//•â³‚µ‚È‚¢
            }else{
                return ex_attack.feature->knives[index - 1].position.x;
            }
        }
        float ExAttackSakuya::Y() const
        {
            assert(0 <= index && index < 5);
            if(index == 0){
                return ex_attack.position.y;//•â³‚µ‚È‚¢
            }else{
                return ex_attack.feature->knives[index - 1].position.y;
            }
        }
        //Reisen
        void ExAttackReisen::Update(void)
        {
            hittable_object->SetX(X());
            hittable_object->SetY(Y());
            hittable_object->SetRadius(ex_attack.feature->reisen_radius_or_aya_offset * 0.8f);
        }
        //Cirno
        void ExAttackCirno::Update(void)
        {
            hittable_object->SetX(X());
            hittable_object->SetY(Y());
            hittable_object->SetWidth(width);
            hittable_object->SetHeight(height);
        }
        //Aya
        float ExAttackAya::X(void) const
        {
            float offset = ex_attack.feature->reisen_radius_or_aya_offset * cos(ex_attack.feature->yuuka_or_aya_info.ex_aya_offset_angle);
            return ExAttack::X() + offset;
        }
        float ExAttackAya::Y(void) const
        {
            float offset = ex_attack.feature->reisen_radius_or_aya_offset * sin(ex_attack.feature->yuuka_or_aya_info.ex_aya_offset_angle);
            return ExAttack::Y() + offset;
        }
        //Yuuka
        void ExAttackYuuka::Update(void)
        {
            hittable_object->SetX(X());
            hittable_object->SetY(Y());
            hittable_object->SetRadius(ex_attack.feature->yuuka_or_aya_info.ex_yuuka_type >= 2 ? 18.0f : 28.0f);
        }
    }
}
