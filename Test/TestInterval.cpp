/**
  ******************************************************************************
  * @file           : TestInterval.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-30
  ******************************************************************************
  */



#include <doctest/doctest.h>
#include <Core/Common.h>
#include <Core/Interval.h>

TEST_CASE("Interval")
{
    SUBCASE("EMPTY && UNIVERSE")
    {
        CONSTEXPR double x = 0.0;
        CHECK(Interval::EMPTY.IsEmpty());
        CHECK(!Interval::EMPTY.IsUniverse());
        CHECK(!Interval::UNIVERSE.IsEmpty());
        CHECK(Interval::UNIVERSE.IsUniverse());
        CHECK(!Interval::EMPTY.Contain(x));
        CHECK(!Interval::EMPTY.Surround(x));
        CHECK(Interval::UNIVERSE.Contain(x));
        CHECK(Interval::UNIVERSE.Surround(x));
        CHECK(!Interval::EMPTY.Contain(Interval::UNIVERSE));
        CHECK(!Interval::EMPTY.Surround(Interval::UNIVERSE));
        CHECK(Interval::UNIVERSE.Contain(Interval::EMPTY));
        CHECK(Interval::UNIVERSE.Surround(Interval::EMPTY));
        CHECK_THROWS(Interval::EMPTY.Clamp(x));
        CHECK(Feq(Interval::UNIVERSE.Clamp(x), x));
        CHECK(!Interval::OverLap(Interval::EMPTY, Interval::UNIVERSE));
        CHECK(!Interval::OverLap(Interval::UNIVERSE, Interval::EMPTY));
        CHECK(Interval::Intersection(Interval::EMPTY, Interval::UNIVERSE).IsEmpty());
        CHECK(Interval::Intersection(Interval::UNIVERSE, Interval::EMPTY).IsEmpty());
    }
    SUBCASE("Contain")
    {
        Interval interval
        {
            .imin = -1.0,
            .imax = 1.0,
        };
        for (const double dmin : {-EPS, 0.0, EPS})
        {
            for (const double dmax : {-EPS, 0.0, EPS})
            {
                CHECK(interval.Contain(Interval
                {
                    .imin = interval.imin + dmin,
                    .imax = interval.imax + dmax,
                }));
            }
        }
        CHECK(!interval.Contain(Interval
        {
            .imin = interval.imin - 2 * EPS,
            .imax = interval.imax - 2 * EPS,
        }));
        CHECK(!interval.Contain(Interval
        {
            .imin = interval.imin - 2 * EPS,
            .imax = interval.imax,
        }));
        CHECK(!interval.Contain(Interval
        {
            .imin = interval.imin - 2 * EPS,
            .imax = interval.imax + 2 * EPS,
        }));
        CHECK(!interval.Contain(Interval
        {
            .imin = interval.imin,
            .imax = interval.imax + 2 * EPS,
        }));
        CHECK(!interval.Contain(Interval
        {
            .imin = interval.imin + 2 * EPS,
            .imax = interval.imax + 2 * EPS,
        }));
    }
    SUBCASE("Surround")
    {
        Interval interval
        {
            .imin = -1.0,
            .imax = 1.0,
        };
        for (const double dmin : {-EPS, 0.0, EPS})
        {
            for (const double dmax : {-EPS, 0.0, EPS})
            {
                CHECK(!interval.Surround(Interval
                {
                    .imin = interval.imin + dmin,
                    .imax = interval.imax + dmax,
                }));
            }
        }
        CHECK(interval.Surround(Interval
        {
            .imin = interval.imin + 2 * EPS,
            .imax = interval.imax - 2 * EPS,
        }));
    }
    SUBCASE("Clamp")
    {
        Interval interval
        {
            .imin = -1.0,
            .imax = 1.0,
        };
        CHECK(Feq(0.0, interval.Clamp(0.0)));
        CHECK_THROWS(Interval::EMPTY.Clamp(0.0));
        CHECK(Feq(0.0, Interval::UNIVERSE.Clamp(0.0)));
        CHECK(Feq(-1.0, interval.Clamp(-1.5)));
        CHECK(Feq(1.0, interval.Clamp(1.5)));
    }
    SUBCASE("OverLap")
    {
        Interval interval0
        {
            .imin = -1.0,
            .imax = 1.0,
        };
        Interval interval1
        {
            .imin = 1.0 + EPS,
            .imax = 2.0,
        };
        CHECK(Interval::OverLap(interval0, interval1));
    }
    SUBCASE("Intersection")
    {
        Interval interval0
        {
            .imin = -1.0,
            .imax = 1.0,
        };
        Interval interval1
        {
            .imin = 1.0 + EPS,
            .imax = 2.0,
        };
        auto inter = Interval::Intersection(interval0, interval1);
        CHECK(Fle(inter.imin, inter.imax));
        CHECK(Feq(inter.imin, 1.0));
        CHECK(Feq(inter.imax, 1.0 + EPS));
    }
}
