#include "Math.hpp"
#include <random>

namespace {
    std::mt19937& GetGenerator()
    {
        static std::random_device rd;
        static std::mt19937 generator(rd());
        return generator;
    }

    template <typename Distribution>
    auto Random(Distribution distribution)
    {
        return distribution(GetGenerator());
    }
}

int Math::RandomInt(int min, int max)
{
    return Random(std::uniform_int_distribution<int>(min, max));
}

float Math::RandomF(float min, float max)
{
    return Random(std::uniform_real_distribution<float>(min, max));
}

double Math::RandomD(double min, double max)
{
    return Random(std::uniform_real_distribution<double>(min, max));
}