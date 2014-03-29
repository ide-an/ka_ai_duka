#pragma once
namespace ka_ai_duka{
    class IDGenerator{
    private:
        unsigned int count;
    public:
        IDGenerator(void) : count(0)
        {}
        unsigned int NewId(void)
        {
            count++;
            return count;
        }
        void Reset(unsigned int count = 0)
        {
            this->count = count;
        }
    };
}