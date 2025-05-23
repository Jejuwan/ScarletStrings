﻿// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#include "Engine_Settings.h"

#ifndef ENGINE_PCH_H
#define ENGINE_PCH_H

// add headers that you want to pre-compile here

#include <time.h>
#include <process.h>

#include <cmath>
#include <random>
#include <memory>
#include <fstream>
#include <iostream>

#include <typeinfo>
#include <typeindex>
#include <type_traits>

using std::type_info;
using std::type_index;

using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;

using std::static_pointer_cast;
using std::dynamic_pointer_cast;
using std::const_pointer_cast;
using std::reinterpret_pointer_cast;

using std::make_unique;
using std::make_shared;

using std::cout;
using std::wcout;
using std::endl;

namespace filesystem = std::filesystem;

#include <utility>
#include <optional>

using std::pair;
using std::make_pair;

#include <string>
#include <locale>
#include <codecvt>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <deque>
#include <stack>
#include <bitset>

#include <algorithm>
#include <functional>
#include <tuple>
#include <any>
#include <variant>
#include <optional>
#include <initializer_list>
#include <numeric>

#include <io.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include <filesystem>
namespace fs = std::filesystem;

#include <random>

using std::string;
using std::wstring;
using std::array;
using std::vector;
using std::list;
using std::map;
using std::multimap;
using std::unordered_map;
using std::set;
using std::multiset;
using std::unordered_set;
using std::queue;
using std::deque;

using std::bitset;
using std::function;
using std::tuple;
using std::any;
using std::optional;

using std::nullopt;

using std::make_tuple;
using std::any_cast;
using std::initializer_list;

#if ACTIVATE_TOOL
#pragma warning(push)
#pragma warning(disable:26451)
#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"
#pragma warning(pop)
#endif
#include "DirectXTK/DDSTextureLoader.h"
#include "DirectXTK/WICTextureLoader.h"
#include "DirectXTK/ScreenGrab.h"
#include "DirectXTK/Effects.h"
#include "DirectXTK/PrimitiveBatch.h"
#include "DirectXTK/VertexTypes.h"
#include "DirectXTK/SpriteBatch.h"
#include "DirectXTK/SpriteFont.h"
#include "FX11/d3dx11effect.h"

#include "FMOD/fmod.h"
#include "FMOD/fmod.hpp"
#include "FMOD/fmod_codec.h"
#include "FMOD/fmod_common.h"
#include "FMOD/fmod_dsp.h"
#include "FMOD/fmod_dsp_effects.h"
#include "FMOD/fmod_errors.h"
#include "FMOD/fmod_output.h"

#include "physx/PxPhysicsAPI.h"

#undef PX_RELEASE
#define PX_RELEASE(x)                                                          \
    if (x) {                                                                   \
        x->release();                                                          \
        x = NULL;                                                              \
    }

#define PVD_HOST "127.0.0.1"
#define MAX_NUM_ACTOR_SHAPES 128

using namespace physx;

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dtypes.h>

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

using namespace DirectX;

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

#endif // ENGINE_PCH_H

// Boost
