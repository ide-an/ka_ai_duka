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
                AddExAttack(attacks, new ExAttackReimu(ex_attack, idgen.NewId()));
                return true;
            }else if(func_addr == ex_func_addr.marisa){
                AddExAttack(attacks, new ExAttackMarisa(ex_attack, idgen.NewId()));
                return true;
            }else if(func_addr == ex_func_addr.sakuya_1){
                return false;//ignored
            }else if(func_addr == ex_func_addr.sakuya_2){
                AddExAttack(attacks, new ExAttackSakuya(ex_attack, idgen.NewId(), 0));
                AddExAttack(attacks, new ExAttackSakuya(ex_attack, idgen.NewId(), 1));
                AddExAttack(attacks, new ExAttackSakuya(ex_attack, idgen.NewId(), 2));
                AddExAttack(attacks, new ExAttackSakuya(ex_attack, idgen.NewId(), 3));
                AddExAttack(attacks, new ExAttackSakuya(ex_attack, idgen.NewId(), 4));
                return true;
            }else if(func_addr == ex_func_addr.youmu){
                AddExAttack(attacks, new ExAttackYoumu(ex_attack, idgen.NewId()));
                return true;
            }else if(func_addr == ex_func_addr.reisen){
            }else if(func_addr == ex_func_addr.lyrica){
            }else if(func_addr == ex_func_addr.merlin){
            }else if(func_addr == ex_func_addr.lunasa){
            }else if(func_addr == ex_func_addr.mystia_1){
            }else if(func_addr == ex_func_addr.mystia_2){
            }else if(func_addr == ex_func_addr.mystia_3){
            }else if(func_addr == ex_func_addr.mystia_4){
            }else if(func_addr == ex_func_addr.mystia_5){
            }else if(func_addr == ex_func_addr.tewi){
            }else if(func_addr == ex_func_addr.aya){
            }else if(func_addr == ex_func_addr.yuuka){
            }else if(func_addr == ex_func_addr.komachi){
            }else if(func_addr == ex_func_addr.eiki){
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
        //Reimu
        void ExAttackReimu::Update(void)
        {
            hittable_object->SetX(X());
            hittable_object->SetY(Y());
            hittable_object->SetRadius(radius);
        }
        //Marisa
        void ExAttackMarisa::Update(void)
        {
            //–‚—¹EX‚Í—LˆÓ‚È“–‚½‚è”»’è‚ðŽ‚½‚È‚¢
            hittable_object->SetX(X());
            hittable_object->SetY(Y());
            hittable_object->SetRadius(0);
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
        void ExAttackSakuya::Update(void)
        {
            hittable_object->SetX(X());
            hittable_object->SetY(Y());
            hittable_object->SetRadius(radius);
        }
        //Youmu
        void ExAttackYoumu::Update(void)
        {
            hittable_object->SetX(X());
            hittable_object->SetY(Y());
            hittable_object->SetRadius(radius);
        }
    }
}