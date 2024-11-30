/**
  ******************************************************************************
  * @file           : TestCommon.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-30
  ******************************************************************************
  */



#include <doctest/doctest.h>
#include <Core/Common.h>

struct Base
{
protected:
    enum BaseType
    {
        BASE_TYPE_DERIVE_0,
        BASE_TYPE_DERIVE_1,
    };

    const BaseType kind;

public:
    NODISCARD CONSTEXPR FORCE_INLINE BaseType Kind() const NOEXCEPT
    {
        return kind;
    }

    NODISCARD explicit Base(const BaseType kind) NOEXCEPT : kind(kind) {}
};

struct Derive0 : Base
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Base* ptr) NOEXCEPT
    {
        return ptr->Kind() == BASE_TYPE_DERIVE_0;
    }

    NODISCARD explicit Derive0() : Base(BASE_TYPE_DERIVE_0) {}
};

struct Derive1 : Base
{
    NODISCARD CONSTEXPR FORCE_INLINE static bool ClassOf(const Base* ptr) NOEXCEPT
    {
        return ptr->Kind() == BASE_TYPE_DERIVE_1;
    }

    NODISCARD explicit Derive1() : Base(BASE_TYPE_DERIVE_1) {}
};

TEST_CASE("Common")
{
    SUBCASE("RTTI")
    {
        Derive0 derive0 = Derive0();
        Derive1 derive1 = Derive1();

        Base* ptr = &derive0;

        CHECK(IsA<Derive0>(ptr));
        CHECK(!IsA<Derive1>(ptr));
        CHECK((DynCast<Derive0>(ptr) != nullptr));
        CHECK((DynCast<Derive1>(ptr) == nullptr));

        ptr = &derive1;

        CHECK(!IsA<Derive0>(ptr));
        CHECK(IsA<Derive1>(ptr));
        CHECK((DynCast<Derive0>(ptr) == nullptr));
        CHECK((DynCast<Derive1>(ptr) != nullptr));
    }

    SUBCASE("Fops")
    {
        CHECK(Feq(0.0, 0.0-EPS));
        CHECK(Feq(0.0, 0.0+EPS));

        CHECK(Fne(0.0, 0.0-2.0*EPS));
        CHECK(Fne(0.0, 0.0+2.0*EPS));

        CHECK(!Fgt(0.0, 0.0-EPS));
        CHECK(Fge(0.0, 0.0-EPS));
        CHECK(Fgt(0.0, 0.0-2*EPS));

        CHECK(!Flt(0.0, 0.0+EPS));
        CHECK(Fle(0.0, 0.0+EPS));
        CHECK(Flt(0.0, 0.0+2*EPS));

        CHECK(FIsZero(0.0-EPS));
        CHECK(FIsZero(0.0+EPS));
        CHECK(FIsNegative(-2.0*EPS));
        CHECK(FIsPositive(+2.0*EPS));

        CHECK(FIsInfinity(std::numeric_limits<double>::infinity()));
        CHECK(FIsInfinity(INF));
        CHECK(FIsInfinity(INF + 100.0));
        CHECK(FIsInfinity(INF - 100.0));
        CHECK(!FIsInfinity(1E100));
        CHECK(!FIsInfinity(-1E100));
    }
}
