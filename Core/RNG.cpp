/**
  ******************************************************************************
  * @file           : Rng.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-30
  ******************************************************************************
  */



#include <Core/RNG.h>

std::vector<std::mt19937> RNG::rngs(std::thread::hardware_concurrency());
