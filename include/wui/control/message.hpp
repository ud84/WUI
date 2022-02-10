//
// Copyright (c) 2021-2022 Anton Golovkov (udattsk at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/ud84/wui
//

#pragma once

#include <wui/window/window.hpp>

#include <wui/control/image.hpp>
#include <wui/control/text.hpp>
#include <wui/control/button.hpp>

namespace wui
{

enum class message_icon
{
    information,
    question,
    alert,
    stop
};

enum class message_button : uint32_t
{
    ok,
    ok_cancel,
    abrot_retry_ignore,
    yes_no,
    yes_no_cancel,
    retry_cancel,
    cancel_try_continue
};

enum class message_result
{
    ok,
    cancel,
    yes,
    no,
    abort,
    retry,
    ignore
};

class message
{
public:
    message(const std::string &message_,
        const std::string &title_,
        message_icon icon_,
        message_button button_,
        std::function<void(message_result)> result_callback,
        std::shared_ptr<window> transient_window_, bool docked_ = true,
        std::shared_ptr<i_theme> theme_ = nullptr);
    ~message();

public:
    /// Control name in theme
    static constexpr const char *tc = "message";

    /// Used theme values
    static constexpr const char *tv_background = "background";
    static constexpr const char *tv_button_calm = "calm";
    static constexpr const char *tv_button_active = "active";
    static constexpr const char *tv_border = "border";
    static constexpr const char *tv_border_width = "border_width";
    static constexpr const char *tv_text = "text";
    static constexpr const char *tv_round = "round";
    static constexpr const char *tv_font = "font";

private:
    std::shared_ptr<window> window, transient_window;
};

message_result show_message(const std::string &message_,
    const std::string &title_,
    message_icon icon_,
    message_button button_,
    std::shared_ptr<window> transient_window_, bool docked_ = true,
    std::shared_ptr<i_theme> theme_ = nullptr);

}
