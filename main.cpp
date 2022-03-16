#include "Main.h"

#include <Windows.h>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <winbase.h>
#include <tchar.h>
#include <WinInet.h>


#pragma comment(lib,"Wininet.lib")

#pragma comment(lib, "winmm.lib")

#define _WIN32_WINNT 0x0500


using namespace std;

namespace BasePlayer {
        float Sensitivity = 0.2;
        int FOV = 120;
        bool Enabled = true;

        //Visuals
        bool ESP = false;
        bool FullBox = false;
        bool CornerBox = false;
        bool Skeleton = false;
        bool PName = false;
        bool PHealth = false;
        bool PWeapon = false;
        bool PDistance = false;
        bool Chams = true;

        //Weapon
        bool NoSpread = false;
        bool NoRecoil = false;
        float NoRecoilValue = 0;
        float NoSpreadValue = 0;


        //Aimbot
        bool Aimbot = false;
        bool SuperFlick = false;
        bool FOVCircle = false;
        float FOVSize = false;
        bool Visible = false;
        bool TeamCheck = false;
        bool Smooth = false;
        float SmoothValue = 5.0f;
}

namespace Variables {
    int ActiveTab = 1;
}

void Overlay()
{
    float ScreenX = GetSystemMetrics(SM_CXSCREEN);
    float ScreenY = GetSystemMetrics(SM_CYSCREEN);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ScreenX, ScreenY);
    ImGui::Begin("#overlay", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_Nobackground | ImGuiWindowFlags_NoCollapse | 
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMouseInputs);
    auto draw = ImGui::GetBackgroundDrawList();
    if (BasePlayer::FOVCircle)
    { 
        draw->AddCircle(ImVec2(ScreenX / 2, ScreenY / 2), BasePlayer::FOVSize, IM_COL32(255, 0, 0, 255), 100, 1.0f);
    }
    ImGui::End();
}

static const char* Bones[]{"Head","Neck","Body","Legs"};
static int SelectedBone = 0;

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{






    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, LOADER_BRAND, NULL };
    RegisterClassEx(&wc);
    main_hwnd = CreateWindow(wc.lpszClassName, LOADER_BRAND, WS_POPUP, 0, 0, 5, 5, NULL, NULL, wc.hInstance, NULL);


    if (!CreateDeviceD3D(main_hwnd)) {
        CleanupDeviceD3D();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }


    ShowWindow(main_hwnd, SW_HIDE);
    UpdateWindow(main_hwnd);


    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();



    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        void Theme(); {



            ImGui::GetStyle().FrameRounding = 4.0f;
            ImGui::GetStyle().GrabRounding = 4.0f;

            ImVec4* colors = ImGui::GetStyle().Colors;
            colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
            colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
            colors[ImGuiCol_WindowBg] = ImColor(10, 10, 10, 255);
            colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
            colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
            colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
            colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
            colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
            colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
            colors[ImGuiCol_TitleBg] = ImColor(30, 30, 160, 255);
            colors[ImGuiCol_TitleBgActive] = ImColor(30, 30, 160, 255);
            colors[ImGuiCol_TitleBgCollapsed] = ImColor(0, 0, 0, 130);
            colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);

            colors[ImGuiCol_ScrollbarBg] = ImColor(31, 30, 31, 255);
            colors[ImGuiCol_ScrollbarGrab] = ImColor(41, 40, 41, 255);
            colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(31, 30, 31, 255);
            colors[ImGuiCol_ScrollbarGrabActive] = ImColor(41, 40, 41, 255);
            colors[ImGuiCol_CheckMark] = ImColor(10, 10, 200, 255);
            colors[ImGuiCol_SliderGrab] = ImColor(180, 30, 31, 255);
            colors[ImGuiCol_SliderGrabActive] = ImColor(185, 35, 35, 255);

            colors[ImGuiCol_Button] = ImColor(31, 30, 31, 255);
            colors[ImGuiCol_ButtonHovered] = ImColor(31, 30, 31, 255);
            colors[ImGuiCol_ButtonActive] = ImColor(41, 40, 41, 255);

            colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
            colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
            colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

            colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
            colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
            colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);

            colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
            colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
            colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);

            colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
            colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
            colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
            colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
            colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);

            colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
            colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
            colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);

            colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
            colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
            colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
            colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
            colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);











        }
    }


    ImGui_ImplWin32_Init(main_hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);



    DWORD window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

    RECT screen_rect;
    GetWindowRect(GetDesktopWindow(), &screen_rect);
    auto screenwidth = float(screen_rect.right - WINDOW_WIDTH) / 2.f;
    auto screenheight = float(screen_rect.bottom - WINDOW_HEIGHT) / 2.f;

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }





        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(400, 250));
            ImGui::SetNextWindowBgAlpha(1.0f);
            ImGui::Begin("                  External Menu Example", &loader_active, window_flags);
            {



                ImGui::Columns(2);
                ImGui::SetColumnOffset(1, 120);

                ImGui::Spacing();
                if (ImGui::Button("Aimbot", ImVec2(100, 40)))
                    Variables::ActiveTab = 1;

                ImGui::Spacing();
                if (ImGui::Button("Visuals", ImVec2(100, 40)))
                    Variables::ActiveTab = 2;

                ImGui::Spacing();
                if (ImGui::Button("Extras", ImVec2(100, 40)))
                    Variables::ActiveTab = 3;

                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Text("   Made By");
                ImGui::Text("NotSlater#0999");
                
                ImGui::NextColumn();
                {
                    if (Variables::ActiveTab == 1)
                    {
                        ImGui::Text("             -=Aimbot=-");
                        ImGui::Spacing();
                        ImGui::Checkbox("Aimbot", &BasePlayer::Aimbot);
                        ImGui::Checkbox("Smoothing Mode", &BasePlayer::Smooth);
                        if (BasePlayer::Smooth)
                        {
                            ImGui::SliderFloat(" ", &BasePlayer::SmoothValue, 1, 10, "% .2f");
                        }
                        ImGui::Checkbox("Enable FOV", &BasePlayer::FOVCircle);
                        if (BasePlayer::FOVCircle)
                        {
                            Overlay();
                            ImGui::SliderFloat("", &BasePlayer::FOVSize, 0, 240, "% .2f");
                        }
                        ImGui::Checkbox("Visible Check", &BasePlayer::Visible);
                        ImGui::Checkbox("Team Check", &BasePlayer::TeamCheck);
                    }
                    if (Variables::ActiveTab == 2)
                    {
                        ImGui::Text("             -=Visuals=-");
                        ImGui::Spacing();
                        ImGui::Checkbox("ESP", &BasePlayer::ESP);
                        ImGui::Checkbox("Full Box", &BasePlayer::FullBox);
                        ImGui::Checkbox("Corner Box", &BasePlayer::CornerBox);
                        ImGui::Checkbox("Skeleton", &BasePlayer::Skeleton);
                        ImGui::Checkbox("Player Name", &BasePlayer::PName);
                        ImGui::Checkbox("Player Health", &BasePlayer::PHealth);
                        ImGui::Checkbox("Player Weapon", &BasePlayer::PWeapon);
                    }
                    if (Variables::ActiveTab == 3)
                    {
                        ImGui::Text("             -=Extras=-");
                        ImGui::Checkbox("No Recoil", &BasePlayer::NoRecoil);
                        if (BasePlayer::NoRecoil)
                        {
                            ImGui::SliderFloat("  ", &BasePlayer::NoRecoilValue, 0, 100, "% .2f");
                        }
                        ImGui::Checkbox("NoNoSpreadSpread", &BasePlayer::NoSpread);
                        if (BasePlayer::NoRecoil)
                        {
                            ImGui::SliderFloat("   ", &BasePlayer::NoSpreadValue, 0, 100, "% .2f");
                        }
                        ImGui::Text("        ...Coming soon...");
                        ImGui::Text("");
                        ImGui::Text("");
                        ImGui::Text("");
                        ImGui::Text("             -=Credits=-");
                        ImGui::Text("       Discord - NotSlater#0999");
                        ImGui::Text("         Github.com/NotSlater");
                    }
                }
            }
            ImGui::End();
        }
        ImGui::EndFrame();

        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }


        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);


        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
            ResetDevice();
        }
        if (!loader_active) {
            msg.message = WM_QUIT;
        }
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    DestroyWindow(main_hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
