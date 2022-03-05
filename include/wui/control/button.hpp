//
// Copyright (c) 2021-2022 Anton Golovkov (udattsk at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/ud84/wui
//

#pragma once

#include <wui/control/i_control.hpp>
#include <wui/graphic/graphic.hpp>
#include <wui/event/event.hpp>
#include <wui/common/rect.hpp>
#include <wui/common/color.hpp>

#include <string>
#include <functional>
#include <memory>
#include <vector>

namespace wui
{

class image;
class tooltip;

enum class button_view
{
    text,
    image,
    image_right_text,
    image_bottom_text,
    image_right_text_no_frame,
    switcher,
    anchor
};

class button : public i_control, public std::enable_shared_from_this<button>
{
public:
    button(const std::string &caption, std::function<void(void)> click_callback, const std::string &theme_control_name = tc, std::shared_ptr<i_theme> theme_ = nullptr);
    button(const std::string &caption, std::function<void(void)> click_callback, button_view button_view_, const std::string &theme_control_name = tc, std::shared_ptr<i_theme> theme_ = nullptr);

#ifdef _WIN32
    button(const std::string &caption, std::function<void(void)> click_callback, button_view button_view_, int32_t image_resource_index, int32_t image_size, const std::string &theme_control_name = tc, std::shared_ptr<i_theme> theme_ = nullptr);
#endif
    button(const std::string &caption, std::function<void(void)> click_callback, button_view button_view_, const std::string &image_file_name, int32_t image_size, const std::string &theme_control_name = tc, std::shared_ptr<i_theme> theme_ = nullptr);
    button(const std::string &caption, std::function<void(void)> click_callback, button_view button_view_, const std::vector<uint8_t> &image_data, int32_t image_size, const std::string &theme_control_name = tc, std::shared_ptr<i_theme> theme_ = nullptr);
    ~button();

    /// i_control impl
    virtual void draw(graphic &gr, const rect &);

    virtual void set_position(const rect &position, bool redraw = true);
    virtual rect position() const;

    virtual void set_parent(std::shared_ptr<window> window_);
    virtual void clear_parent();

    virtual bool topmost() const;

    virtual void update_theme(std::shared_ptr<i_theme> theme_ = nullptr);

    virtual void show();
    virtual void hide();
    virtual bool showed() const;

    virtual void enable();
    virtual void disable();
    virtual bool enabled() const;

    virtual bool focused() const;
    virtual bool focusing() const;

public:
    /// Button's interface
    void set_caption(const std::string &caption);

    void set_button_view(button_view button_view_);
#ifdef _WIN32
    void set_image(int32_t resource_index);
#endif
    void set_image(const std::string &file_name);
    void set_image(const std::vector<uint8_t> &image_data);

    void enable_focusing();
    void disable_focusing();

    void switch_(bool on);
    bool switched() const;

    void set_callback(std::function<void(void)> click_callback);

public:
    /// Possible control names in theme
    static constexpr const char *tc = "button";
    static constexpr const char *tc_tool = "tool_button";
    static constexpr const char *tc_tool_red = "red_tool_button";

    /// Used theme values
    static constexpr const char *tv_calm = "calm";
    static constexpr const char *tv_active = "active";
    static constexpr const char *tv_border = "border";
    static constexpr const char *tv_border_width = "border_width";
    static constexpr const char *tv_focused_border = "focused_border";
    static constexpr const char *tv_text = "text";
    static constexpr const char *tv_disabled = "disabled";
    static constexpr const char *tv_anchor = "anchor";
    static constexpr const char *tv_round = "round";
    static constexpr const char *tv_font = "font";

    ///Used theme images
    static constexpr const char *ti_switcher_off = "button_switcher_off";
    static constexpr const char *ti_switcher_on = "button_switcher_on";

private:
    button_view button_view_;
    std::string caption;
    std::string tcn; /// control name in theme

    std::shared_ptr<image> image_;
    int32_t image_size;
    
    std::shared_ptr<tooltip> tooltip_;
    
    std::function<void(void)> click_callback;

    std::shared_ptr<i_theme> theme_;

    rect position_;

    std::weak_ptr<window> parent;
    std::string my_subscriber_id;

    bool showed_, enabled_;
    bool active, focused_;
    bool focusing_;

    bool switched_;

    rect text_rect;

    void receive_event(const event &ev);

    void redraw();
};

}
