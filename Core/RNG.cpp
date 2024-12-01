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

THREAD_LOCAL std::mt19937 RNG::rng;
