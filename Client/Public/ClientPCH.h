﻿// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#include "Client_Settings.h"

#ifndef CLIENT_PCH_H
#define CLIENT_PCH_H

#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

// add headers that you want to pre-compile here

#if ACTIVATE_IMGUI
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "ImGuizmo.h"
#include "ImGuiFileDialog.h"
#include "implot.h"
#endif

#include "EnginePCH.h"

#include "framework.h"

#endif // CLIENT_PCH_H
