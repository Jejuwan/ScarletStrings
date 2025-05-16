#pragma once
#pragma region Pre-Settings
#ifndef FALSE
#define FALSE					0
#endif
#ifndef TRUE
#define TRUE					1
#endif
#pragma endregion

#define	ACTIVATE_CONSOLE		TRUE
#define ACTIVATE_IMGUI			TRUE
#define ACTIVATE_TOOL			TRUE
#define ACTIVATE_PHYSX_DEBUG    TRUE

#define FORCE_CONSOLE			TRUE

#define WIP_FRUSTRUM_CULLING	FALSE

#pragma region Post-Settings
#ifndef _DEBUG
#ifndef FORCE_CONSOLE
#undef ACTIVATE_CONSOLE
#define ACTIVATE_CONSOLE		FALSE
#endif
#undef ACTIVATE_IMGUI
#define ACTIVATE_IMGUI			FALSE
#endif
#if !ACTIVATE_IMGUI
#undef ACTIVATE_TOOL
#define ACTIVATE_TOOL			FALSE
#endif
#pragma endregion

#pragma region TODO
#define TODO_MOVE_PIVOT_TO_MODEL
#pragma endregion
