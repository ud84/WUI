﻿//
// Copyright (c) 2023 Anton Golovkov (udattsk at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/ud84/wui
//

#include <wui/config/config_impl_ini.hpp>
#include <wui/system/tools.hpp>
#include <wui/system/path_tools.hpp>
#include <wui/system/string_tools.hpp>

#include <fstream>

namespace wui
{

namespace config
{

config_impl_ini::config_impl_ini(const std::string &file_name_)
    : file_name(wui::real_path(file_name_)),
    values(),
    ok(false), err_msg()
{
    ok = load_values();
}

int32_t config_impl_ini::get_int(const std::string &section, const std::string &entry, int32_t default_)
{
    return static_cast<int32_t>(get_int64(section, entry, default_));
}

void config_impl_ini::set_int(const std::string &section, const std::string &entry, int32_t value)
{
    set_int64(section, entry, value);
}

int64_t config_impl_ini::get_int64(const std::string &section, const std::string &entry, int64_t default_)
{
    auto s = values.find({ section, entry });
    if (s != values.end() && s->second.type == value_type::int64)
    {
        return s->second.int_val;
    }
    return default_;
}

void config_impl_ini::set_int64(const std::string &section, const std::string &entry, int64_t value)
{
    auto &s = values[{ section, entry }];
    s.type = value_type::int64;
    s.int_val = value;

    save_values();
}

std::string config_impl_ini::get_string(const std::string &section, const std::string &entry, const std::string &default_)
{
    auto s = values.find({ section, entry });
    if (s != values.end())
    {
        return s->second.str_val;
    }
    return default_;
}

void config_impl_ini::set_string(const std::string &section, const std::string &entry, const std::string value)
{
    auto &s = values[{ section, entry }];
    s.type = value_type::string;
    s.str_val = value;

    save_values();
}

void config_impl_ini::delete_value(const std::string &section, const std::string &entry)
{
    auto s = values.find({ section, entry });
    if (s != values.end())
    {
        values.erase(s);
    }

    save_values();
}

void config_impl_ini::delete_key(const std::string &section)
{
    // todo
}

bool config_impl_ini::is_ok() const
{
    return ok;
}

std::string config_impl_ini::get_error() const
{
    return err_msg;
}

bool config_impl_ini::load_values()
{
    std::ifstream f(file_name, std::ios::in);
	if (!f)
	{
        err_msg = "WUI Error [config_impl_ini::load_values] :: Error opening config file: " + file_name;
        ok = false;
		return false;
	}

    std::string section;
	for (std::string line; std::getline(f, line); )
	{
        auto comment_pos = line.find(";");
        auto beg_bracket = line.find("["), end_bracket = line.find("]");
        if (beg_bracket != std::string::npos && end_bracket != std::string::npos &&
            comment_pos > beg_bracket && comment_pos > end_bracket)
		{
			section = trim_copy(line.substr(beg_bracket + 1, end_bracket - 1));
			continue;
		}

        auto eq_pos = line.find("=");		
        if (eq_pos != std::string::npos && eq_pos < comment_pos)
		{
            std::string entry = trim_copy(line.substr(0, eq_pos));
            std::string value = trim_copy(line.substr(eq_pos + 1, comment_pos - eq_pos - 1));

            auto &v = values[{section, entry}];
			v.type = value_type::string;
			v.str_val = value;

            if (is_number(value))
            {
                v.type = value_type::int64;
				v.str_val = value;

                try
                {
					std::size_t pos{};
                    v.int_val = std::stoll(value, &pos);
                }
                catch (std::invalid_argument const& ex)
                {
                    err_msg = "WUI Error [config_impl_ini::load_values] :: std::invalid_argument::what(): " + std::string(ex.what());
                    ok = false;
                    return false;
                }
                catch (std::out_of_range const& ex)
                {
                    err_msg = "WUI Error [config_impl_ini::load_values] :: std::out_of_range::what(): " + std::string(ex.what());
                    ok = false;
                    return false;
                }
            }

            if (comment_pos != std::string::npos)
            {
                v.comment = trim_copy(line.substr(comment_pos + 1, std::string::npos));
            }
        }
        else if (comment_pos != std::string::npos)
        {
            auto &v = values[{section, ""}];
            v.type = value_type::only_comment;
            v.comment = trim_copy(line.substr(comment_pos + 1, std::string::npos));
        }
    }

    f.close();

    ok = true;
    return true;
}

bool config_impl_ini::save_values()
{
    std::ofstream f(file_name, std::ios::out | std::ios::trunc);
	if (!f)
	{
        err_msg = "WUI Error [config_impl_ini::save_values] :: Error write to config file: " + file_name;
        ok = false;
		return false;
	}

    std::string current_section;
    bool first_section = true;

    for (auto &v : values)
    {
        if (v.first.first != current_section)
        {
            if (!first_section)
            {
                f << std::endl;
            }

            f << "[" << v.first.first << "]" << std::endl;
                        
            if (first_section)
            {
                first_section = false;
            }
            
            current_section = v.first.first;
        
            for (auto &s : values)
            {
                if (s.first.first == current_section)
                {
                    switch (s.second.type)
                    {
                        case value_type::string:
                            f << s.first.second << " = " << s.second.str_val;
                        break;
                        case value_type::int64:
                            f << s.first.second << " = " << s.second.int_val;
                        break;
                    }

                    if (!s.second.comment.empty())
                    {
                        f << (s.second.type == value_type::only_comment ? "; " : " ; ") << s.second.comment;
                    }

                    f << std::endl;
                }
            }
        }
    }

    f.close();
    
    return true;
}

}

}
