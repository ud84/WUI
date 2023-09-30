// wui.cpp : Defines the entry point for the application.
//

#include <wui/theme/theme.hpp>
#include <wui/locale/locale.hpp>

#include <MainFrame/MainFrame.h>

#include <Resource.h>

#ifdef _WIN32
#include <gdiplus.h>
#else
#include <iostream>
#endif

#ifdef _WIN32
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
#elif __linux__
int main(int argc, char *argv[])
{
    if (setlocale(LC_ALL, "") == NULL)
    {
        std::cerr << "warning: could not set default locale"  << std::endl;
    }

#endif

#ifdef _WIN32
    auto ok = wui::set_default_theme_from_resource("dark", TXT_DARK_THEME, "JSONS");
    if (!ok)
    {
        printf("can't load theme\n");
        return -1;
    }

    ok = wui::set_locale_from_resource("en", TXT_LOCALE_EN, "JSONS");
    if (!ok)
    {
        printf("can't load locale\n");
        return -1;
    }
#elif __linux__
    auto ok = wui::set_default_theme_from_file("dark", dark_theme_json_file);
    if (!ok)
    {
        printf("can't load theme\n");
        return -1;
    }

    ok = wui::set_locale_from_file("en", en_locale_json_file);
    if (!ok)
    {
        printf("can't load locale\n");
        return -1;
    }
#endif

    MainFrame mainFrame;
    mainFrame.Run();

#ifdef _WIN32
    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    //Gdiplus::GdiplusShutdown(gdiplusToken);
    return (int) msg.wParam;
#elif __linux__
    // Wait for main window
    while (mainFrame.Runned())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
#endif
}
