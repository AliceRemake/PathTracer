/**
  ******************************************************************************
  * @file           : Primitive.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-27
  ******************************************************************************
  */



#include <Core/Primitive.h>
#include <Core/Ray.h>
#include <Core/Interval.h>

// Ref: https://iquilezles.org/articles/intersectors/
NODISCARD bool Sphere::Hit(const Ray &ray, const Interval& interval, HitRecord &record) NOEXCEPT
{
    const Eigen::Vector3d oc = ray.origin - center;
    const double b = oc.dot(ray.direction);
    const Eigen::Vector3d qc = oc - b * ray.direction;
    double h = radius * radius - qc.dot(qc);

    if (FIsNegative(h)) {return false;}

    h = std::sqrt(h);
    const double tmin = -b - h;
    const double tmax = -b + h;

    if (FIsNegative(tmax)) {return false;}

    const double t = FIsNegative(tmin) ? tmax : tmin;
    if (!interval.Contain(t)) {return false;}

    record.t = t;
    record.hit_point = ray.At(t);
    record.hit_normal = (record.hit_point - center).normalized();
    record.material = material;

    return true;
}
