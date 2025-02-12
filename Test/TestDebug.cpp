/**
  ******************************************************************************
  * @file           : TestDebug.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 25-2-12
  ******************************************************************************
  */

#include <doctest/doctest.h>

#include <Core/Debug.h>


TEST_CASE("Debug")
{
    const auto st = Debug::Now();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    const auto ed = Debug::Now();
    Debug::Print("{}", Debug::MilliSeconds(ed - st));
}
