/**
  ******************************************************************************
  * @file           : TestBounds.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-27
  ******************************************************************************
  */



#include <doctest/doctest.h>
#include <Core/Common.h>
#include <Core/Bounds.h>

TEST_CASE("Bounds")
{
    // SUBCASE("2D")
    // {
    //     AABB2D a = { {0.0, 0.0}, {1.0, 1.0} };
    //     AABB2D b = { {0.5, 0.5}, {1.5, 1.5} }; // a b Overlap
    //     AABB2D c = { {0.1, 0.1}, {0.9, 0.9} }; // a Contain c
    //     AABB2D d = { {0.0, 0.0}, {1.5, 1.5} }; // Union a b
    //     AABB2D e = { {0.0, 0.0}, {1.0, 1.0} }; // Union a c
    //     AABB2D f = { {0.1, 0.1}, {1.5, 1.5} }; // Union b c
    //     SUBCASE("Center")
    //     {
    //         CHECK((a.Center() == Eigen::Vector2d{0.5, 0.5}));
    //         CHECK((b.Center() == Eigen::Vector2d{1.0, 1.0}));
    //         CHECK((c.Center() == Eigen::Vector2d{0.5, 0.5}));
    //         CHECK((d.Center() == Eigen::Vector2d{0.75, 0.75}));
    //         CHECK((e.Center() == Eigen::Vector2d{0.5, 0.5}));
    //         CHECK((f.Center() == Eigen::Vector2d{0.8, 0.8}));
    //     }
    //     SUBCASE("Radius")
    //     {
    //         CHECK((a.Radius() == Eigen::Vector2d{0.5, 0.5}));
    //         CHECK((b.Radius() == Eigen::Vector2d{0.5, 0.5}));
    //         CHECK((c.Radius() == Eigen::Vector2d{0.4, 0.4}));
    //         CHECK((d.Radius() == Eigen::Vector2d{0.75, 0.75}));
    //         CHECK((e.Radius() == Eigen::Vector2d{0.5, 0.5}));
    //         CHECK((f.Radius() == Eigen::Vector2d{0.7, 0.7}));
    //     }
    //     SUBCASE("Overlap")
    //     {
    //         CHECK(AABB2D::OverLap(a, b));
    //     }
    //     SUBCASE("Contain")
    //     {
    //         CHECK(AABB2D::Contain(a, c));
    //     }
    //     SUBCASE("Union")
    //     {
    //         CHECK((AABB2D::Union(a, b) == d));
    //         CHECK((AABB2D::Union(a, c) == e));
    //         CHECK((AABB2D::Union(b, c) == f));
    //     }
    // }
    //
    // SUBCASE("3D")
    // {
    //     AABB3D a = { {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0} };
    //     AABB3D b = { {0.5, 0.5, 0.5}, {1.5, 1.5, 1.5} }; // a b Overlap
    //     AABB3D c = { {0.1, 0.1, 0.1}, {0.9, 0.9, 0.9} }; // a Contain c
    //     AABB3D d = { {0.0, 0.0, 0.0}, {1.5, 1.5, 1.5} }; // Union a b
    //     AABB3D e = { {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0} }; // Union a c
    //     AABB3D f = { {0.1, 0.1, 0.1}, {1.5, 1.5, 1.5} }; // Union b c
    //     SUBCASE("Center")
    //     {
    //         CHECK((a.Center() == Eigen::Vector3d{0.5, 0.5, 0.5}));
    //         CHECK((b.Center() == Eigen::Vector3d{1.0, 1.0, 1.0}));
    //         CHECK((c.Center() == Eigen::Vector3d{0.5, 0.5, 0.5}));
    //         CHECK((d.Center() == Eigen::Vector3d{0.75, 0.75, 0.75}));
    //         CHECK((e.Center() == Eigen::Vector3d{0.5, 0.5, 0.5}));
    //         CHECK((f.Center() == Eigen::Vector3d{0.8, 0.8, 0.8}));
    //     }
    //     SUBCASE("Radius")
    //     {
    //         CHECK((a.Radius() == Eigen::Vector3d{0.5, 0.5, 0.5}));
    //         CHECK((b.Radius() == Eigen::Vector3d{0.5, 0.5, 0.5}));
    //         CHECK((c.Radius() == Eigen::Vector3d{0.4, 0.4, 0.4}));
    //         CHECK((d.Radius() == Eigen::Vector3d{0.75, 0.75, 0.75}));
    //         CHECK((e.Radius() == Eigen::Vector3d{0.5, 0.5, 0.5}));
    //         CHECK((f.Radius() == Eigen::Vector3d{0.7, 0.7, 0.7}));
    //     }
    //     SUBCASE("Overlap")
    //     {
    //         CHECK(AABB3D::OverLap(a, b));
    //     }
    //     SUBCASE("Contain")
    //     {
    //         CHECK(AABB3D::Contain(a, c));
    //     }
    //     SUBCASE("Union")
    //     {
    //         CHECK((AABB3D::Union(a, b) == d));
    //         CHECK((AABB3D::Union(a, c) == e));
    //         CHECK((AABB3D::Union(b, c) == f));
    //     }
    // }
}
