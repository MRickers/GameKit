#pragma once
#include <random>

namespace gk
{
  class RandomIntGenerator
  {
  public:
    RandomIntGenerator(int lower, int upper)
        : distribution(lower, upper)
        , generator(rd())
    {
    }

    int generate()
    {
      return distribution(generator);
    }

  private:
    std::random_device rd;
    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution;
  };

} // namespace gk
