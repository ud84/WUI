//
// config.hpp - Contains the configuration wrappers interface
//
// Copyright (c) 2014-2023 Anton Golovkov (udattsk at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/ud84/wui
//

#pragma once

#include <wui/common/error.hpp>

#include <memory>

#ifdef _WIN32
#include <windows.h>
#endif

namespace wui
{

namespace config
{

/// Initializes the application config from an ini file
bool use_ini_file(const std::string &file_name);

#ifdef _WIN32
/// Initializes the application config from a registry
bool use_registry(const std::string &app_key, HKEY root = HKEY_CURRENT_USER);
#endif

/// Creates a config from the registry on windows or ini file on other systems
bool create_config(const std::string &file_name,
    const std::string &app_key, int64_t root = 0); /// By default root is HKEY_CURRENT_USER

error get_error();

/// Int32 methods
int32_t get_int(const std::string &section, const std::string &entry, int32_t default_);
void set_int(const std::string &section, const std::string &entry, int32_t value);

/// Int64 methods
int64_t get_int64(const std::string &section, const std::string &entry, int64_t default_);
void set_int64(const std::string &section, const std::string &entry, int64_t value);

/// String methods
std::string get_string(const std::string &section, const std::string &entry, const std::string &default_);
void set_string(const std::string &section, const std::string &entry, const std::string value);

/// Delete the value from config
void delete_value(const std::string &section, const std::string &entry);

/// Delete the key from config
void delete_key(const std::string &section);

}

}
