//
// Copyright (c) 2021-2022 Anton Golovkov (udattsk at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/ud84/wui
//

#include <wui/theme/theme_str.hpp>

namespace wui
{

std::string theme_control_to_str(theme_control tc)
{
    switch (tc)
    {
        case theme_control::window:
            return "window";
        break;
        case theme_control::image:
            return "image";
        break;
        case theme_control::button:
            return "button";
        break;
        case theme_control::input:
            return "input";
        break;
        case theme_control::tooltip:
            return "tooltip";
        break;
    }
    return "";
}

std::string theme_value_to_str(theme_value tv)
{
    switch (tv)
    {
        case theme_value::background:
            return "background";
        break;
        case theme_value::text:
            return "text";
        break;
        case theme_value::calm:
            return "calm";
        break;
        case theme_value::active:
            return "active";
        break;
        case theme_value::active_button:
            return "active_button";
        break;
        case theme_value::border:
            return "border";
        break;
        case theme_value::focused_border:
            return "focused_border";
        break;
        case theme_value::disabled:
            return "disabled";
        break;
        case theme_value::selection:
            return "selection";
        break;
        case theme_value::cursor:
            return "cursor";
        break;
        case theme_value::path:
            return "path";
        break;
        case theme_value::round:
            return "round";
        break;
        case theme_value::text_indent:
            return "text_indent";
        break;
        case theme_value::font:
            return "font";
        break;
        case theme_value::caption_font:
            return "caption_font";
        break;
    }
    return "";
}

}
