﻿#include <WUI/Window/Window.h>

#include <WUI/Graphic/Graphic.h>

#include <WUI/Theme/Theme.h>

#include <WUI/Control/Button.h>

#include <algorithm>

#ifdef _WIN32
#include <windowsx.h>
#endif

namespace WUI
{

Window::Window()
	: controls(),
	activeControl(),
	windowType(WindowType::Frame),
	position(),
	caption(),
	closeCallback(),
	showed(true), enabled(true),
	parent(),
	movingMode(MovingMode::Move)
#ifdef _WIN32
	, hWnd(0),
	backgroundBrush(0),
	font(0),
	xClick(0), yClick(0)
#endif
{
#ifdef _WIN32
	MakePrimitives();
#endif
}

Window::~Window()
{
#ifdef _WIN32
	DestroyPrimitives();
#endif
}

void Window::AddControl(std::shared_ptr<IControl> control, const Rect &controlPosition)
{
	if (std::find(controls.begin(), controls.end(), control) == controls.end())
	{
		control->SetPosition(!parent ? controlPosition : position + controlPosition);
		control->SetParent(shared_from_this());
		controls.emplace_back(control);

		Redraw(controlPosition);
	}
}

void Window::RemoveControl(std::shared_ptr<IControl> control)
{
	auto exist = std::find(controls.begin(), controls.end(), control);
	if (exist != controls.end())
	{
		(*exist)->ClearParent();
		Redraw((*exist)->GetPosition());
		controls.erase(exist);
	}
}

void Window::Redraw(const Rect &redrawPosition)
{
	if (parent)
	{
		parent->Redraw(redrawPosition);
	}
	else
	{
#ifdef _WIN32
		RECT invalidatingRect = { redrawPosition.left, redrawPosition.top, redrawPosition.right, redrawPosition.bottom };
		InvalidateRect(hWnd, &invalidatingRect, TRUE);
#endif
	}
}

void Window::Draw(Graphic &gr)
{
	if (!showed)
	{
		return;
	}

	for (auto &control : controls)
	{
		control->Draw(gr);
	}
}

void Window::ReceiveEvent(const Event &ev)
{
	if (!showed)
	{
		return;
	}

	if (ev.type == EventType::Mouse)
	{
		SendMouseEvent(ev.mouseEvent);
	}
}

void Window::SetPosition(const Rect &position_)
{
	position = position_;
}

Rect Window::GetPosition() const
{
	return position;
}

void Window::SetParent(std::shared_ptr<Window> window)
{
	parent = window;
}

void Window::ClearParent()
{
	parent.reset();
}

void Window::UpdateTheme()
{
#ifdef _WIN32
	DestroyPrimitives();
	MakePrimitives();
#endif
	if (!parent)
	{
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		InvalidateRect(hWnd, &clientRect, TRUE);
	}

	for (auto &control : controls)
	{
		control->UpdateTheme();
	}
}

void Window::Show()
{
	showed = true;

	for (auto &control : controls)
	{
		control->Show();
	}

#ifdef _WIN32
	if (!parent)
	{
		ShowWindow(hWnd, SW_SHOW);
	}
#endif
}

void Window::Hide()
{
	showed = false;

	for (auto &control : controls)
	{
		control->Hide();
	}

#ifdef _WIN32
	if (!parent)
	{
		ShowWindow(hWnd, SW_HIDE);
	}
#endif
}

bool Window::Showed() const
{
	return showed;
}

void Window::Enable()
{
	enabled = true;

	for (auto &control : controls)
	{
		control->Enable();
	}
}

void Window::Disable()
{
	enabled = false;

	for (auto &control : controls)
	{
		control->Disable();
	}
}

bool Window::Enabled() const
{
	return enabled;
}

void Window::Minimize()
{

}

void Window::Expand()
{

}

void Window::Block()
{
#ifdef _WIN32
	EnableWindow(hWnd, FALSE);
#endif
}

void Window::Unlock()
{
#ifdef _WIN32
	EnableWindow(hWnd, TRUE);
	SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
#endif
}

void Window::SendMouseEvent(const MouseEvent &ev)
{
	if (activeControl && !activeControl->GetPosition().In(ev.x, ev.y))
	{
		MouseEvent me{ MouseEventType::Leave, 0, 0 };
		activeControl->ReceiveEvent({ EventType::Mouse, me });

		activeControl.reset();
	}

	for (auto &control : controls)
	{
		if (control->GetPosition().In(ev.x, ev.y))
		{
			if (activeControl == control)
			{
				control->ReceiveEvent({ EventType::Mouse, ev });
			}
			else
			{
				activeControl = control;

				MouseEvent me{ MouseEventType::Enter, 0, 0 };
				control->ReceiveEvent({ EventType::Mouse, me });
			}

			break;
		}
	}
}

/// Windows specified code
#ifdef _WIN32

bool Window::Init(WindowType type, const Rect &position_, const std::wstring &caption_, std::function<void(void)> closeCallback_)
{
	windowType = type;
	position = position_;
	caption = caption_;
	closeCallback = closeCallback_;

	if (parent)
	{
		showed = true;
		parent->Redraw(position);

		return true;
	}

	if (type == WindowType::Frame)
	{
		std::shared_ptr<Button> minimizeButton(new Button(L"🗕", std::bind(&Window::Minimize, this), ButtonType::WindowControl));
		AddControl(minimizeButton, { position.right - 78, 2, position.right - 54, 26 });
		std::shared_ptr<Button> expandButton(new Button(L"🗖", std::bind(&Window::Expand, this), ButtonType::WindowControl));
		AddControl(expandButton, { position.right - 52, 2, position.right - 28, 26 });
	}
	std::shared_ptr<Button> closeButton(new Button(L"❌", std::bind(&Window::Destroy, this), ButtonType::WindowControl));
	AddControl(closeButton, { position.right - 26, 2, position.right - 2, 26 });

	WNDCLASSEXW wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Window::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(this);
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hbrBackground = backgroundBrush;
	wcex.lpszClassName = L"WUI Window";
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);

	RegisterClassExW(&wcex);

	hWnd = CreateWindowW(wcex.lpszClassName, L"", WS_POPUP,
		position.left, position.top, position.right, position.bottom, nullptr, nullptr, GetModuleHandle(NULL), this);

	if (!hWnd)
	{
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return true;
}

void Window::Destroy()
{
	if (parent)
	{
		showed = false;
		parent->Redraw(position);

		if (closeCallback)
		{
			closeCallback();
		}

		return;
	}
	DestroyWindow(hWnd);
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams));
		}
		break;
		case WM_PAINT:
		{
			Window* wnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			Graphic gr{ hdc };

			SelectObject(hdc, wnd->font);

			SetTextColor(hdc, ThemeColor(ThemeValue::Window_Text));
			SetBkColor(hdc, ThemeColor(ThemeValue::Window_Background));
			TextOutW(hdc, 5, 5, wnd->caption.c_str(), (int32_t)wnd->caption.size());
		
			for (auto &control : wnd->controls)
			{
				control->Draw(gr);
			}

			EndPaint(hWnd, &ps);
		}
		break;
		case WM_ERASEBKGND:
		{
			HDC hdc = (HDC)wParam;
			RECT clientRect;
			GetClientRect(hWnd, &clientRect);
			SetMapMode(hdc, MM_ANISOTROPIC);

			Window* wnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			FillRect(hdc, &clientRect, wnd->backgroundBrush);
		}
		break;
		case WM_MOUSEMOVE:
		{
			Window* wnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

			RECT windowRect;
			GetWindowRect(hWnd, &windowRect);

			int16_t xMouse = GET_X_LPARAM(lParam);
			int16_t yMouse = GET_Y_LPARAM(lParam);

			if (wnd->windowType == WindowType::Frame)
			{
				if ((xMouse > windowRect.right - windowRect.left - 5 && yMouse > windowRect.bottom - windowRect.top - 5) ||
					(xMouse < 5 && yMouse < 5))
				{
					SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
				}
				else if ((xMouse > windowRect.right - windowRect.left - 5 && yMouse < 5) ||
					(xMouse < 5 && yMouse > windowRect.bottom - windowRect.top - 5))
				{
					SetCursor(LoadCursor(NULL, IDC_SIZENESW));
				}
				else if (xMouse > windowRect.right - windowRect.left - 5 || xMouse < 5)
				{
					SetCursor(LoadCursor(NULL, IDC_SIZEWE));
				}
				else if (yMouse > windowRect.bottom - windowRect.top - 5 || yMouse < 5)
				{
					SetCursor(LoadCursor(NULL, IDC_SIZENS));
				}
			}

			if (GetCapture() == hWnd)
			{
				switch (wnd->movingMode)
				{
					case MovingMode::Move:
					{
						int32_t xWindow = windowRect.left + xMouse - wnd->xClick;
						int32_t yWindow = windowRect.top + yMouse - wnd->yClick;

						SetWindowPos(hWnd, NULL, xWindow, yWindow, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
					}
					break;
					case MovingMode::SizeWELeft:
					{
						POINT scrMouse = { 0 };
						GetCursorPos(&scrMouse);

						int32_t width = windowRect.right - windowRect.left - xMouse;
						int32_t height = windowRect.bottom - windowRect.top;
						SetWindowPos(hWnd, NULL, scrMouse.x, windowRect.top, width, height, SWP_NOZORDER);
					}
					break;
					case MovingMode::SizeWERight:
					{
						int32_t width = xMouse;
						int32_t height = windowRect.bottom - windowRect.top;
						SetWindowPos(hWnd, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
					}
					break;
					case MovingMode::SizeNSTop:
					{
						POINT scrMouse = { 0 };
						GetCursorPos(&scrMouse);

						int32_t width = windowRect.right - windowRect.left;
						int32_t height = windowRect.bottom - windowRect.top - yMouse;
						SetWindowPos(hWnd, NULL, windowRect.left, scrMouse.y, width, height, SWP_NOZORDER);
					}
					break;
					case MovingMode::SizeNSBottom:
					{
						int32_t width = windowRect.right - windowRect.left;
						int32_t height = yMouse;
						SetWindowPos(hWnd, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
					}
					break;
					case MovingMode::SizeNESWTop:
					{
						POINT scrMouse = { 0 };
						GetCursorPos(&scrMouse);

						int32_t width = xMouse;
						int32_t height = windowRect.bottom - windowRect.top - yMouse;
						SetWindowPos(hWnd, NULL, windowRect.left, scrMouse.y, width, height, SWP_NOZORDER);
					}
					break;
					case MovingMode::SizeNWSEBottom:
					{
						int32_t width = xMouse;
						int32_t height = yMouse;
						SetWindowPos(hWnd, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
					}
					break;
					case MovingMode::SizeNWSETop:
					{
						POINT scrMouse = { 0 };
						GetCursorPos(&scrMouse);

						int32_t width = windowRect.right - windowRect.left - xMouse;
						int32_t height = windowRect.bottom - windowRect.top - yMouse;
						SetWindowPos(hWnd, NULL, scrMouse.x, scrMouse.y, width, height, SWP_NOZORDER);
					}
					break;
					case MovingMode::SizeNESWBottom:
					{
						POINT scrMouse = { 0 };
						GetCursorPos(&scrMouse);

						int32_t width = windowRect.right - windowRect.left - xMouse;
						int32_t height = yMouse;
						SetWindowPos(hWnd, NULL, scrMouse.x, windowRect.top, width, height, SWP_NOZORDER);
					}
					break;
				}
			}
			else
			{
				wnd->SendMouseEvent({ MouseEventType::Move, xMouse, yMouse });
			}
		}
		break;
		case WM_LBUTTONDOWN:
		{
			Window* wnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

			SetCapture(hWnd);

			RECT windowRect;
			GetWindowRect(hWnd, &windowRect);

			wnd->xClick = GET_X_LPARAM(lParam);
			wnd->yClick = GET_Y_LPARAM(lParam);

			if (wnd->windowType == WindowType::Frame)
			{
				if (wnd->xClick > windowRect.right - windowRect.left - 5 && wnd->yClick > windowRect.bottom - windowRect.top - 5)
				{
					wnd->movingMode = MovingMode::SizeNWSEBottom;
				}
				else if (wnd->xClick < 5 && wnd->yClick < 5)
				{
					wnd->movingMode = MovingMode::SizeNWSETop;
				}
				else if (wnd->xClick > windowRect.right - windowRect.left - 5 && wnd->yClick < 5)
				{
					wnd->movingMode = MovingMode::SizeNESWTop;
				}
				else if (wnd->xClick < 5 && wnd->yClick > windowRect.bottom - windowRect.top - 5)
				{
					wnd->movingMode = MovingMode::SizeNESWBottom;
				}
				else if (wnd->xClick > windowRect.right - windowRect.left - 5)
				{
					wnd->movingMode = MovingMode::SizeWERight;
				}
				else if(wnd->xClick < 5)
				{
					wnd->movingMode = MovingMode::SizeWELeft;
				}
				else if (wnd->yClick > windowRect.bottom - windowRect.top - 5)
				{
					wnd->movingMode = MovingMode::SizeNSBottom;
				}
				else if (wnd->yClick < 5)
				{
					wnd->movingMode = MovingMode::SizeNSTop;
				}
			}

			wnd->SendMouseEvent({ MouseEventType::LeftDown, wnd->xClick, wnd->yClick });
		}
		break;
		case WM_LBUTTONUP:
		{
			ReleaseCapture();

			Window* wnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

			wnd->movingMode = MovingMode::Move;

			wnd->SendMouseEvent({ MouseEventType::LeftUp, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
		}
		break;
		case WM_DESTROY:
		{
			Window* wnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			if (wnd->closeCallback)
			{
				wnd->closeCallback();
			}
		}
		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void Window::MakePrimitives()
{
	backgroundBrush = CreateSolidBrush(ThemeColor(ThemeValue::Window_Background));
	font = CreateFont(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
}

void Window::DestroyPrimitives()
{
	DeleteObject(backgroundBrush);
	DeleteObject(font);
}

#endif

}
