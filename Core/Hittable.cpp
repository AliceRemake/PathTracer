/**
  ******************************************************************************
  * @file           : Hittable.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-27
  ******************************************************************************
  */



#include <Core/Hittable.h>

NODISCARD bool HittableList::Hit(const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT
{
    if (bvh == nullptr)
    {
        record.t = INF;
        for (const auto& hittable : data)
        {
            if (HitRecord t_record; hittable->Hit(ray, interval, t_record))
            {
                if (Flt(t_record.t, record.t))
                {
                    record = t_record;
                }
            }
        }
        return !FIsInfinity(record.t);
    }

    return bvh->Hit(ray, interval, record);
}
