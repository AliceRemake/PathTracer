/**
  ******************************************************************************
  * @file           : Interval.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-30
  ******************************************************************************
  */



#include <Core/Interval.h>

const Interval Interval::EMPTY = Interval{INF, -INF};
const Interval Interval::UNIVERSE = Interval{-INF, INF};
