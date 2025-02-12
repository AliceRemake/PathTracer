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
    record.t = INF;
    for (const auto& hittable : hittable_list)
    {
        if (HitRecord temp_record; hittable->Hit(ray, interval, temp_record))
        {
            if (Flt(temp_record.t, record.t))
            {
                record = temp_record;
            }
        }
    }
    return !FIsInfinity(record.t);
}
