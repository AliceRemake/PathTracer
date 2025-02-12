/**
  ******************************************************************************
  * @file           : Scene.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-27
  ******************************************************************************
  */



#include <Core/Scene.h>

NODISCARD bool Scene::Hit(const Ray &ray, const Interval &interval, HitRecord &record) const NOEXCEPT
{
    record.t = INF;
    for (const auto& object : objects)
    {
        if (HitRecord t_record = {}; object->Hit(ray, interval, t_record))
        {
            if (Fgt(record.t, t_record.t))
            {
                record = t_record;
            }
        }
    }
    return !FIsInfinity(record.t);
}
