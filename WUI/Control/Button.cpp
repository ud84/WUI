
#include <WUI/Control/Button.h>

#include <WUI/Window/Window.h>

#include <WUI/Theme/Theme.h>

namespace WUI
{

Button::Button(const std::string &caption_, std::function<void(void)> clickCallback_)
	: caption(caption_),
	clickCallback(clickCallback_),
	position(),
	parent(nullptr),
	active(false)
#ifdef _WIN32
	,calmBrush(0), activeBrush(0),
	calmPen(0), activePen(0),
	font(0)
#endif
{
#ifdef _WIN32
	MakePrimitives();
#endif
}

Button::~Button()
{
#ifdef _WIN32
	DestroyPrimitives();
#endif

	if (parent)
	{
		parent->RemoveControl(*this);
	}
}

void Button::Draw(Graphic &gr)
{
#ifdef _WIN32
	SelectObject(gr.dc, active ? activePen : calmPen);
	SelectObject(gr.dc, active ? activeBrush : calmBrush);

	RoundRect(gr.dc, position.left, position.top, position.right, position.bottom, 5, 5);
#endif
}

void Button::ReceiveEvent(Event &ev)
{
}

void Button::SetPosition(const Rect &position_)
{
	position = position_;
}

void Button::SetParent(Window *window)
{
	parent = window;
}

void Button::UpdateTheme()
{
#ifdef _WIN32
	DestroyPrimitives();
	MakePrimitives();
#endif
}

void Button::SetCaption(const std::string &caption_)
{
	caption = caption_;
}

void Button::SetCallback(std::function<void(void)> clickCallback_)
{
	clickCallback = clickCallback_;
}

#ifdef _WIN32
void Button::MakePrimitives()
{
	calmBrush = CreateSolidBrush(ThemeColor(ThemeValue::Button_Calm));
	activeBrush = CreateSolidBrush(ThemeColor(ThemeValue::Button_Active));
	calmPen = CreatePen(PS_SOLID, 1, ThemeColor(ThemeValue::Button_Calm));
	activePen = CreatePen(PS_SOLID, 1, ThemeColor(ThemeValue::Button_Active));
	font = CreateFont(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
}

void Button::DestroyPrimitives()
{
	DeleteObject(calmBrush);
	DeleteObject(activeBrush);
	DeleteObject(calmPen);
	DeleteObject(activePen);
	DeleteObject(font);
}
#endif

}
