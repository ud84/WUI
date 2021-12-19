#pragma once

#include <WUI/Window/Type.h>
#include <WUI/Window/IWindow.h>
#include <WUI/Common/Rect.h>
#include <WUI/Control/IControl.h>

#include <vector>
#include <memory>

#ifdef _WIN32
#include <windows.h>
#endif

namespace WUI
{

class Window : public IWindow
{
public:
	Window();
	~Window();

	virtual bool Init(WindowType type, const Rect &position, const std::string &caption);

	virtual void AddControl(IControl &control, const Rect &position);
	virtual void RemoveControl(IControl &control);
	virtual void Redraw(const Rect &position);

	virtual void UpdateTheme();

	virtual void Show();
	virtual void Hide();

private:
	std::vector<IControl*> controls, activeControls;

#ifdef _WIN32
	HWND hWnd;

	HBRUSH backgroundBrush;
	HFONT font;

	int16_t xClick, yClick;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void MakePrimitives();
	void DestroyPrimitives();
#endif

	void SendMouseEvent(const MouseEvent &ev);
};

}
