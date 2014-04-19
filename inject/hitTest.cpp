#include "hitTest.h"
#include <cmath>
namespace ka_ai_duka{
    namespace managed_types{
        bool HitTestRectVsRect(HittableRect* a, HittableRect* b)
        {
            float adx = a->Width() / 2;
            float ady = a->Height() / 2;
            float bdx = b->Width() / 2;
            float bdy = b->Height() / 2;
            if(a->X() - adx <= b->X() + bdx && b->X() - bdx <= a->X() + adx 
                && a->Y() - ady <= b->Y() + bdy && b->Y() - bdy <= a->Y() + ady){
                return true;
            }
            return false;
        }
        bool HitTestCircleVsCircle(HittableCircle* a, HittableCircle* b)
        {
            float dx = a->X() - b->X();
            float dy = a->Y() - b->Y();
            float r = a->Radius() + b->Radius();
            if(dx*dx+dy*dy <= r*r){
                return true;
            }
            return false;
        }
        bool HitTestRectVsRotatableRect(HittableRect* a, HittableRotatableRect* b)
        {
            float cos_ = cos(-1 * a->Angle());
            float sin_ = sin(-1 * a->Angle());
            float dx = b->X() - a->X();
            float dy = b->Y() - a->Y();
            float bx = b->X() + cos_ * dx - sin_ * dy;
            float by = b->Y() + sin_ * dx + cos_ * dy;
            float bdx = b->Width() / 2;
            float bdy = b->Height() / 2;
            if(a->X() <= bx + bdx && bx - bdx <= a->X() + a->Width() 
                && a->Y() <= by + bdy && by - bdy <= a->Y() + a->Height()){
                return true;
            }
            return false;
        }
        bool HitTest(HittableObject* a, HittableObject* b)
        {
            if(a->Type() == Hit_Rect && b->Type() == Hit_Rect){
                return HitTestRectVsRect(reinterpret_cast<HittableRect*>(a), reinterpret_cast<HittableRect*>(b));
            }else if(a->Type() == Hit_Circle && b->Type() == Hit_Circle){
                return HitTestCircleVsCircle(reinterpret_cast<HittableCircle*>(a), reinterpret_cast<HittableCircle*>(b));
            }else if(a->Type() == Hit_RotatableRect && b->Type() == Hit_Rect){
                return HitTestRectVsRotatableRect(reinterpret_cast<HittableRect*>(a), reinterpret_cast<HittableRotatableRect*>(b));
            }else if(b->Type() == Hit_RotatableRect && a->Type() == Hit_Rect){
                return HitTestRectVsRotatableRect(reinterpret_cast<HittableRect*>(b), reinterpret_cast<HittableRotatableRect*>(a));
            }else{
                return false;
            }
        }

        std::ostream& operator<<(std::ostream& os, const HittableObject& hittable)
        {
            os << "{ x:" << hittable.X() << ", y:" << hittable.Y()
                << ", type:" << hittable.Type() 
                << ", width:" << hittable.Width() << ", height:" << hittable.Height()
                << ", radius:" << hittable.Radius() << ", angle:" << hittable.Angle() << " }";
            return os;
        }
    }
}
