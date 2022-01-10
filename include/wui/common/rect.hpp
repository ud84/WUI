//
// Copyright (c) 2021-2022 Anton Golovkov (udattsk at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/ud84/WUI
//

#pragma once

#include <cstdint>

namespace wui
{

struct rect
{
    int32_t left, top, right, bottom;

    inline bool operator==(const rect &lv)
    {
        return left == lv.left && top == lv.top && right == lv.right && bottom == lv.bottom;
    }

    inline rect operator+(const rect &lv)
    {
        return rect{ left + lv.left, top + lv.top, left + lv.right, top + lv.bottom };
    }

    inline bool in(int32_t x, int32_t y) const
    {
        return x >= left && x <= right && y >= top && y <= bottom;
    }

    inline bool in(const rect &outer) const
    {
        return !((outer.right <= left || right <= outer.left || outer.bottom <= top || bottom <= outer.top));
    }

    inline int32_t width() const
    {
        return right - left;
    }

    inline int32_t height() const
    {
        return bottom - top;
    }
};

}
