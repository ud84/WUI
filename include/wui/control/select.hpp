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

class list;

struct select_item;

typedef std::vector<select_item> select_items_t;

struct select_item
{
    int32_t id;

    std::string text;

    inline bool operator==(const int32_t id_)
    {
        return id == id_;
    }
};

class select : public i_control, public std::enable_shared_from_this<select>
{
public:
    select(std::shared_ptr<i_theme> theme_ = nullptr);
    ~select();

    virtual void draw(graphic &gr, const rect &);

    virtual void set_position(const rect &position, bool redraw = true);
    virtual rect position() const;

    virtual void set_parent(std::shared_ptr<window> window_);
    virtual void clear_parent();

    virtual bool topmost() const;

    virtual void set_focus();
    virtual bool remove_focus();
    virtual bool focused() const;
    virtual bool focusing() const;

    virtual void update_theme(std::shared_ptr<i_theme> theme_ = nullptr);

    virtual void show();
    virtual void hide();
    virtual bool showed() const;

    virtual void enable();
    virtual void disable();
    virtual bool enabled() const;

    void set_items(const select_items_t &items);
    void update_item(const select_item &mi);
    void swap_items(int32_t first_item_id, int32_t second_item_id);
    void delete_item(int32_t id);

    void set_item_height(int32_t item_height);

    select_item selected_item() const;

    void set_change_callback(std::function<void(int32_t, const std::string&)> change_callback);

public:
    /// Control name in theme
    static constexpr const char *tc = "select";

    /// Used theme values
    static constexpr const char *tv_background = "background";
    static constexpr const char *tv_border = "border";
    static constexpr const char *tv_border_width = "border_width";
    static constexpr const char *tv_focused_border = "focused_border";
    static constexpr const char *tv_button_calm = "button_calm";
    static constexpr const char *tv_button_active = "button_active";
    static constexpr const char *tv_text = "text";
    static constexpr const char *tv_scrollbar = "scrollbar";
    static constexpr const char *tv_scrollbar_slider = "scrollbar_slider";
    static constexpr const char *tv_scrollbar_slider_acive = "scrollbar_slider_active";
    static constexpr const char *tv_selected_item = "selected_item";
    static constexpr const char *tv_active_item = "active_item";
    static constexpr const char *tv_round = "round";
    static constexpr const char *tv_font = "font";

private:
    std::vector<select_item> items;

    std::shared_ptr<list> list_;

    std::function<void(int32_t, const std::string&)> change_callback;
    std::shared_ptr<i_theme> theme_;

    rect position_;;
    
    std::weak_ptr<window> parent;
    std::string my_subscriber_id;

    bool showed_, enabled_, active;
    bool focused_;
    bool focusing_;
    
    int32_t left_shift;

    void receive_event(const event &ev);

    void redraw();
};

}
