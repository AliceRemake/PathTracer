/**
  ******************************************************************************
  * @file           : Interval.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-30
  ******************************************************************************
  */



#ifndef INTERVAL_H
#define INTERVAL_H

#include <Core/Common.h>

// Close Interval [xmin, xmax]
struct Interval
{
    double imin, imax;

    static const Interval EMPTY;
    static const Interval UNIVERSE;

    NODISCARD CONSTEXPR FORCE_INLINE bool IsEmpty() const NOEXCEPT
    {
        return imin == INF && imax == -INF;
    }

    NODISCARD CONSTEXPR FORCE_INLINE bool IsUniverse() const NOEXCEPT
    {
        return imin == -INF && imax == INF;
    }

    NODISCARD CONSTEXPR FORCE_INLINE bool Contain(const double x) const NOEXCEPT
    {
        return Fle(imin, x) && Fle(x, imax);
    }

    NODISCARD CONSTEXPR FORCE_INLINE bool Contain(const Interval& oth) const NOEXCEPT
    {
        return Fle(imin, oth.imin) && Fle(oth.imax, imax);
    }

    NODISCARD CONSTEXPR FORCE_INLINE bool Surround(const double x) const NOEXCEPT
    {
        return Flt(imin, x) && Flt(x, imax);
    }

    NODISCARD CONSTEXPR FORCE_INLINE bool Surround(const Interval& oth) const NOEXCEPT
    {
        return Flt(imin, oth.imin) && Flt(oth.imax, imax);
    }

    NODISCARD CONSTEXPR FORCE_INLINE double Clamp(const double x) const NOEXCEPT
    {
        ASSERT(!IsEmpty());
        if (IsUniverse()) {return x;}
        if (Flt(x, imin)) {return imin;}
        if (Fgt(x, imax)) {return imax;}
        return x;
    }

    NODISCARD CONSTEXPR FORCE_INLINE static bool OverLap(const Interval& lhs, const Interval& rhs) NOEXCEPT
    {
        if (lhs.IsEmpty() || rhs.IsEmpty()) {return false;}
        if (lhs.IsUniverse() || rhs.IsUniverse()) {return true;}
        return Fle(lhs.imin, rhs.imax) && Fle(rhs.imin, lhs.imax);
    }

    NODISCARD CONSTEXPR FORCE_INLINE static Interval Intersection(const Interval& lhs, const Interval& rhs) NOEXCEPT
    {
        if (!OverLap(lhs, rhs)) {return EMPTY;}
        Interval interval
        {
            .imin = std::max(lhs.imin, rhs.imin),
            .imax = std::min(lhs.imax, rhs.imax),
        };
        if (Flt(interval.imin, interval.imax)) std::swap(interval.imin, interval.imax);
        return interval;
    }
 };

#endif //INTERVAL_H
