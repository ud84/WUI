﻿// wui.cpp : Defines the entry point for the application.
//

#include "wui.h"

#include <WUI/Theme/Theme.h>
#include <WUI/Window/Window.h>
#include <WUI/Control/Button.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WUI::SetTheme(WUI::Theme::Dark);

	std::shared_ptr<WUI::Window> window(new WUI::Window());
	
	std::shared_ptr<WUI::Window> parentWindow(new WUI::Window());
	window->AddControl(parentWindow, WUI::Rect{ 50, 50, 250, 250 });
	{
		std::shared_ptr<WUI::Button> unplugButton(new WUI::Button(L"Unplug window", [window, parentWindow]() { window->RemoveControl(parentWindow); parentWindow->Init(WUI::WindowType::Dialog, WUI::Rect{ 50, 50, 250, 250 }, L"Child window unplugged!", []() {}); }));
		std::shared_ptr<WUI::Button> plugButton(new WUI::Button(L"Plug window", [window, parentWindow]() { parentWindow->Destroy(); window->AddControl(parentWindow, WUI::Rect{ 50, 50, 250, 250 }); }));
				
		parentWindow->AddControl(unplugButton, WUI::Rect{ 10, 10, 110, 35 });
		parentWindow->AddControl(plugButton, WUI::Rect{ 10, 55, 110, 80 });
	}

	std::shared_ptr<WUI::Button> darkThemeButton(new WUI::Button(L"Set the dark theme", [window, parentWindow]() { WUI::SetTheme(WUI::Theme::Dark); window->UpdateTheme(); parentWindow->UpdateTheme(); }));
	std::shared_ptr<WUI::Button> whiteThemeButton(new WUI::Button(L"Set the white theme", [window, parentWindow]() { WUI::SetTheme(WUI::Theme::White); window->UpdateTheme(); parentWindow->UpdateTheme(); }));

	window->AddControl(darkThemeButton, WUI::Rect{ 140, 350, 150, 375 });
	window->AddControl(whiteThemeButton, WUI::Rect{ 270, 350, 380, 375 });

	std::shared_ptr<WUI::Button> okButton(new WUI::Button(L"OK", [window]() { window->Block(); MessageBox(NULL, L"OK was clicked!", L"Yes", MB_ICONEXCLAMATION); window->Unlock(); }));
	std::shared_ptr<WUI::Button> cancelButton(new WUI::Button(L"Cancel", [window]() { window->Destroy(); }));

	window->AddControl(okButton, WUI::Rect{ 240, 450, 350, 475 });
	window->AddControl(cancelButton, WUI::Rect{ 370, 450, 480, 475 });

	window->Init(WUI::WindowType::Frame, WUI::Rect{ 100, 100, 500, 500 }, L"Welcome to WUI!", []() { PostQuitMessage(IDCANCEL); });
	
	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return (int) msg.wParam;
}
