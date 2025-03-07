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
// Ref: https://graphicscodex.com/app/app.html?page=_rn_rayCst
// Ref: https://raytracing.github.io/books/RayTracingTheNextWeek.html#quadrilaterals/definingthequadrilateral

NODISCARD bool Triangle::Hit(const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT
{
    const Eigen::Vector3d& v1v0 = u;
    const Eigen::Vector3d& v2v0 = v;
    const Eigen::Vector3d rov0 = ray.origin - origin;
    const Eigen::Vector3d n = v1v0.cross(v2v0);

    const double rdn = ray.direction.dot(n);

    if (FIsZero(rdn)) { return false; }

    const Eigen::Vector3d q = rov0.cross(ray.direction);
    const double d = 1.0 / rdn;
    const double uu = d * -q.dot(v2v0);
    const double vv = d * q.dot(v1v0);
    const double t = d * -n.dot(rov0);

    if (!interval.Contain(t)) { return false; }

    if (FIsNegative(uu) || FIsNegative(vv) || Fgt(uu + vv, 1.0)) { return false; }

    record.t = t;
    record.hit_point = ray.At(t);
    record.hit_normal = n.normalized();
    record.texcoord = Eigen::Vector2d{uu, vv};
    record.material = material;

    return true;
}

NODISCARD bool Quadrangle::Hit(const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT
{
    if (bounding_box && !bounding_box->Hit(ray, interval))
    {
        return false;
    }

    const Eigen::Vector3d n = u.cross(v);

    const double rdn = ray.direction.dot(n);

    if (FIsZero(rdn) || FIsPositive(rdn)) { return false; }

    const double t = (n.dot(origin) - n.dot(ray.origin)) / rdn;

    if (!interval.Contain(t)) { return false; }

    record.t = t;
    record.hit_point = ray.At(t);

    const Eigen::Vector3d p = record.hit_point - origin;
    const Eigen::Vector3d w = n / n.dot(n);

    const double alpha = w.dot(p.cross(v));
    const double beta = w.dot(u.cross(p));

    if (FIsNegative(alpha) || FIsNegative(beta) || Fgt(alpha, 1.0) || Fgt(beta, 1.0)) { return false; }

    record.hit_normal = n.normalized();
    record.texcoord = Texcoord2D(record.hit_point);
    record.material = material;

    return true;
}

NODISCARD bool Sphere::Hit(const Ray &ray, const Interval& interval, HitRecord &record) const NOEXCEPT
{
    if (bounding_box && !bounding_box->Hit(ray, interval))
    {
        return false;
    }

    const Eigen::Vector3d oc = ray.origin - center;
    const double b = oc.dot(ray.direction);
    const Eigen::Vector3d qc = oc - b * ray.direction;
    double h = radius * radius - qc.dot(qc);

    if (FIsNegative(h)) { return false; }

    h = std::sqrt(h);
    const double tmin = - b - h;
    const double tmax = - b + h;

    if (FIsNegative(tmax)) { return false; }

    const double t = FIsNegative(tmin) ? tmax : tmin;
    if (!interval.Contain(t)) { return false; }

    record.t = t;
    record.hit_point = ray.At(t);
    record.hit_normal = (record.hit_point - center).normalized();
    record.texcoord = Texcoord2D(record.hit_point);
    record.material = material;

    return true;
}
