/**
  ******************************************************************************
  * @file           : Hittable.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-27
  ******************************************************************************
  */



#ifndef HITTABLE_H
#define HITTABLE_H

#include <Core/Common.h>
#include <Core/Bounds.h>

struct Material;

struct HitRecord
{
    enum ScatterType
    {
        SCATTER_TYPE_REFLECT,
        SCATTER_TYPE_REFRACT,
    };

    double t;
    ScatterType scatter_type;
    Ref<Material> material;
    Eigen::Vector2d texcoord2d;
    Eigen::Vector3d hit_point;
    Eigen::Vector3d hit_normal;
};

struct Hittable
{
protected:
    const enum HittableKind
    {
        HITTABLE_KIND_PRIMITIVE_START,
        HITTABLE_KIND_QUADRANGLE, // TODO
        HITTABLE_KIND_SPHERE,
        HITTABLE_KIND_PRIMITIVE_END,

        HITTABLE_KIND_MESH,

        HITTABLE_KIND_LIST,
        HITTABLE_KIND_BVH,
    } kind;

public:
    NODISCARD CONSTEXPR FORCE_INLINE HittableKind Kind() const NOEXCEPT { return kind; }
    NODISCARD explicit Hittable(const HittableKind kind) NOEXCEPT : kind(kind) {}
    virtual ~Hittable() NOEXCEPT = default;

    // Interface.
    NODISCARD virtual const Ref<BoundingBox>& GetBoundingBox() const NOEXCEPT = 0;
    NODISCARD virtual bool Hit(const Ray& ray, const Interval& interval, HitRecord& record) const NOEXCEPT = 0;
};

struct BVH;

struct HittableList final : Hittable
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Hittable* ptr) NOEXCEPT {return ptr->Kind() == HITTABLE_KIND_LIST;}

    std::vector<Ref<Hittable>> data;
    Ref<BVH> bvh;

    NODISCARD HittableList() NOEXCEPT : Hittable(HITTABLE_KIND_LIST) {}

    void PushBack(const Ref<Hittable>& hittable) NOEXCEPT { data.push_back(hittable); }
    void PopBack() NOEXCEPT { data.pop_back(); }
    void InitializeBVH() NOEXCEPT { bvh = MakeRef<BVH>(*this); }

    NODISCARD const Ref<BoundingBox>& GetBoundingBox() const NOEXCEPT OVERRIDE;
    NODISCARD bool Hit(const Ray& ray, const Interval& interval, HitRecord& record) const NOEXCEPT OVERRIDE;
};

struct BVH final : Hittable
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Hittable* ptr) NOEXCEPT {return ptr->Kind() == HITTABLE_KIND_BVH;}

    Ref<Hittable> left;
    Ref<Hittable> right;
    Ref<BoundingBox> bounding_box;

    NODISCARD BVH() NOEXCEPT
    : Hittable(HITTABLE_KIND_BVH), left(nullptr), right(nullptr)
    {}

    NODISCARD explicit BVH(HittableList& hittable_list, Eigen::Index start, Eigen::Index end) NOEXCEPT
    : Hittable(HITTABLE_KIND_BVH), left(nullptr), right(nullptr)
    {
        ASSERT(start < end);

        if (start + 1 == end)
        {
            left = right = hittable_list.data[start];
        }
        else if (start + 2 == end)
        {
            left = hittable_list.data[start];
            right = hittable_list.data[start + 1];
        }
        else
        {
            Eigen::Index mid = (start + end) / 2;
            left = MakeRef<BVH>(hittable_list, start, mid);
            right = MakeRef<BVH>(hittable_list, mid, end);
        }

        bounding_box = MakeRef<AABB>(
            *Cast<AABB>(left->GetBoundingBox().get()),
            *Cast<AABB>(right->GetBoundingBox().get())
        );
    }

    NODISCARD explicit BVH(HittableList& hittable_list) NOEXCEPT
    : Hittable(HITTABLE_KIND_BVH), left(nullptr), right(nullptr)
    {
        static Eigen::Index axis = 2;
        axis = (axis + 1) % 3;
        std::vector hittables(hittable_list.data);
        std::ranges::sort(hittables, [](const Ref<Hittable>& lhs, const Ref<Hittable>& rhs) -> bool
        {
            if (axis == 0)
            {
                return lhs->GetBoundingBox()->Center().x() < rhs->GetBoundingBox()->Center().x();
            }
            else if (axis == 1)
            {
                return lhs->GetBoundingBox()->Center().y() < rhs->GetBoundingBox()->Center().y();
            }
            else // if (axis == 2)
            {
                return lhs->GetBoundingBox()->Center().z() < rhs->GetBoundingBox()->Center().z();
            }
        });

        if (hittable_list.data.empty()) UNLIKELY
        {
            return;
        }
        else if (hittable_list.data.size() == 1) UNLIKELY
        {
            left = right = hittable_list.data[0];
        }
        else if (hittable_list.data.size() == 2) UNLIKELY
        {
            left = hittable_list.data[0];
            right = hittable_list.data[1];
        }
        else
        {
            left = MakeRef<BVH>(hittable_list, 0, hittable_list.data.size() / 2);
            right = MakeRef<BVH>(hittable_list, hittable_list.data.size() / 2, hittable_list.data.size());
        }

        bounding_box = MakeRef<AABB>(
            *Cast<AABB>(left->GetBoundingBox().get()),
            *Cast<AABB>(right->GetBoundingBox().get())
        );
    }

    NODISCARD const Ref<BoundingBox>& GetBoundingBox() const NOEXCEPT OVERRIDE { return bounding_box; }
    NODISCARD bool Hit(const Ray &ray, const Interval &interval, HitRecord& record) const NOEXCEPT OVERRIDE // NOLINT(*-no-recursion)
    {
        if (!bounding_box->Hit(ray, interval)) { return false; }
        const bool hit_left = left && left->Hit(ray, interval, record);
        const bool hit_right = right && right->Hit(ray, hit_left ? Interval(interval.imin, record.t) : interval, record);
        return hit_left || hit_right;
    }
};

NODISCARD FORCE_INLINE const Ref<BoundingBox>& HittableList::GetBoundingBox() const NOEXCEPT { return bvh->GetBoundingBox(); }

#endif //HITTABLE_H
