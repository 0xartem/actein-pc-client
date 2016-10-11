#ifndef UUID_GENERATOR_H__
#define UUID_GENERATOR_H__

#include <string>
#include <random>

namespace utils
{
    class UUIDGenerator
    {
        struct UUID
        {
            unsigned Node;//16 bits
            unsigned long long TimeEpoch; //52 bits
            unsigned long long RandomNumber; //60 bits
        };

    public:
        UUIDGenerator();
        ~UUIDGenerator();
        std::string GenerateUUID(bool bPrettyPrint = true);

    protected:
        void SetTimeEpoch();
        void SetRandomNumber(std::mt19937& PRNG);
        void SetNodeNumber(std::mt19937& PRNG);

    private:
        UUID uuid;
    };
}

#endif //UUID_GENERATOR_H__