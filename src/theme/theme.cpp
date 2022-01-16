//
// Copyright (c) 2021-2022 Anton Golovkov (udattsk at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/ud84/wui
//

#include <wui/theme/theme.hpp>
#include <wui/theme/theme_impl.hpp>

namespace wui
{

static std::shared_ptr<i_theme> instance = nullptr;

/// Interface

void set_default_theme_from_json(const std::string &theme_, const std::string &json)
{
    instance.reset();
    instance = std::shared_ptr<i_theme>(new theme_impl(theme_));
    instance->load_json(json);
}

void set_default_theme_from_file(const std::string &theme_, const std::string &file_name)
{
    instance.reset();
    instance = std::shared_ptr<i_theme>(new theme_impl(theme_));
    instance->load_file(file_name);
}

void set_default_theme_empty(const std::string &theme_)
{
    instance.reset();
    instance = std::shared_ptr<i_theme>(new theme_impl(theme_));
}

std::shared_ptr<i_theme> get_default_theme()
{
    return instance;    
}

std::shared_ptr<i_theme> make_custom_theme(const std::string &name)
{
    return std::shared_ptr<i_theme>(new theme_impl(name));
}

std::shared_ptr<i_theme> make_custom_theme(const std::string &name, const std::string &json)
{
    auto ct = std::shared_ptr<i_theme>(new theme_impl(name));
    ct->load_json(json);
    return ct;
}

color theme_color(const std::string &control, const std::string &value, std::shared_ptr<i_theme> theme_)
{
    if (theme_)
    {
        return theme_->get_color(control, value);
    }
    else if (instance)
    {
        return instance->get_color(control, value);
    }
    return 0;
}

int32_t theme_dimension(const std::string &control, const std::string &value, std::shared_ptr<i_theme> theme_)
{
    if (theme_)
    {
        return theme_->get_dimension(control, value);
    }
    else if (instance)
    {
        return instance->get_dimension(control, value);
    }
    return 0;
}

std::string theme_string(const std::string &control, const std::string &value, std::shared_ptr<i_theme> theme_)
{
    if (theme_)
    {
        return theme_->get_string(control, value);
    }
    else if (instance)
    {
        return instance->get_string(control, value);
    }
    return 0;
}

font theme_font(const std::string &control, const std::string &value, std::shared_ptr<i_theme> theme_)
{
    if (theme_)
    {
        return theme_->get_font(control, value);
    }
    else if (instance)
    {
        return instance->get_font(control, value);
    }
    return font();
}

}
