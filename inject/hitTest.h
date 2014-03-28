#pragma once
namespace ka_ai_duka {
    namespace managed_types{
        enum HitType{
            Hit_Rect = 0,
            Hit_Circle,
            Hit_RotatableRect
        };

        class HittableObject{
        private:
            float x;
            float y;
        public:
            HittableObject(float x, float y) : x(x), y(y)
            {}
            virtual HitType Type() const = 0;
            float X() const
            {
                return x;
            }
            float Y() const
            {
                return y;
            }
            void SetX(float x)
            {
                this->x = x;
            }
            void SetY(float y)
            {
                this->y = y;
            }
            virtual float Width() const = 0;
            virtual float Height() const = 0;
            virtual float Radius() const = 0;
            virtual float Angle() const = 0;
            //virtualでコピーメソッドを用意するか?
            //各種コンストラクタをLua側に提供していればいらない気もする
        };

        class HittableRect : public HittableObject{
        private:
            float width;
            float height;
        public:
            HittableRect(float x, float y, float width, float height)
                : HittableObject(x, y), width(width), height(height)
            {
            }
            HitType Type() const
            {
                return Hit_Rect;
            }
            float Width() const;
            float Height() const;
            float Radius() const
            {
                return Width() / 2;
            }
            float Angle() const
            {
                return 0.0f;
            }
        };

        class HittableCircle : public HittableObject{
        private:
            float radius;
        public:
            HittableCircle(float x, float y, float radius)
                : HittableObject(x, y), radius(radius)
            {
            }
            HitType Type() const
            {
                return Hit_Circle;
            }
            float Width() const
            {
                return Radius() * 2;
            }
            float Height() const
            {
                return Radius() * 2;
            }
            float Radius() const;
            float Angle() const
            {
                return 0.0f;
            }
        };

        class HittableRotatableRect : public HittableObject{
        private:
            float width;
            float height;
            float angle;
        public:
            HittableRotatableRect(float x, float y, float length, float thickness, float angle)
                : HittableObject(x, y), width(length), height(thickness), angle(angle)
            {
            }
            HitType Type() const
            {
                return Hit_RotatableRect;
            }
            float Width() const;
            float Height() const;
            float Radius() const
            {
                return 0.0;
            }
            float Angle() const;
        };

        bool HitTest(HittableObject* a, HittableObject* b);
    }
}