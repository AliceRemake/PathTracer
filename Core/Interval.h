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

struct Interval // (imin, imax)
{
    double imin, imax;

    NODISCARD Interval() NOEXCEPT
    : imin(0), imax(0)
    {}

    NODISCARD Interval(const double a, const double b) NOEXCEPT
    : imin(std::min(a, b)), imax(std::max(a, b))
    {}

    NODISCARD FORCE_INLINE bool IsEmpty() const NOEXCEPT
    {
        return Fge(imin, imax);
    }

    NODISCARD FORCE_INLINE bool IsUniversal() const NOEXCEPT
    {
        return imin == -INF && imax == INF;
    }

    NODISCARD double Center() const NOEXCEPT
    {
        return (imin + imax) / 2.0;
    }

    NODISCARD FORCE_INLINE bool Contain(const double x) const NOEXCEPT
    {
        return Flt(imin, x) && Flt(x, imax);
    }

    NODISCARD FORCE_INLINE bool Contain(const Interval& oth) const NOEXCEPT
    {
        return Flt(imin, oth.imin) && Flt(oth.imax, imax);
    }

    NODISCARD FORCE_INLINE bool Overlap(const Interval& oth) const NOEXCEPT
    {
        if (IsEmpty() || oth.IsEmpty()) { return false; }
        if (IsUniversal() || oth.IsUniversal()) { return true; }
        return Fle(imin, oth.imax) && Fle(oth.imin, imax);
    }

    NODISCARD FORCE_INLINE double Clamp(const double x) const NOEXCEPT
    {
        ASSERT(!IsEmpty());
        if (Flt(x, imin)) UNLIKELY { return imin; }
        if (Fgt(x, imax)) UNLIKELY { return imax; }
        return x;
    }

    NODISCARD FORCE_INLINE Interval Expand(const double padding) const NOEXCEPT
    {
        return { imin - padding, imax + padding };
    }

    NODISCARD FORCE_INLINE static Interval Intersection(const Interval& lhs, const Interval& rhs) NOEXCEPT
    {
        return { std::max(lhs.imin, rhs.imin), std::min(lhs.imax, rhs.imax) };
    }

    NODISCARD FORCE_INLINE static Interval Union(const Interval& lhs, const Interval& rhs) NOEXCEPT
    {
        return { std::min(lhs.imin, rhs.imin), std::max(lhs.imax, rhs.imax) };
    }
 };

#endif //INTERVAL_H
