#pragma once

#include "math.hpp"

template <std::size_t N, typename T>
struct  AABB
{
    vec<N, T>   min;
    vec<N, T>   max;

    static bool    intersects(const AABB<N, T>& a, const AABB<N, T>& b)
    {
        for (std::size_t i = 0; i < N; i++)
            if (a.min.data[i] > b.max.data[i] || a.max.data[i] < b.min.data[i])
                return (false);
        return (true);
    }

    static bool    contains(const AABB<N, T>& box, const vec<N, T>& point)
    {
        for (std::size_t i = 0; i < N; i++)
            if (point.data[i] < box.min.data[i] || point.data[i] > box.max.data[i])
                return (false);
        return (true);
    }
};

using aabb2i = AABB<2, int>;
using aabb3i = AABB<3, int>;

using aabb2f = AABB<2, float>;
using aabb3f = AABB<3, float>;
