#pragma once
#ifndef __ENGINE_INCLUDE_H__
#define __ENGINE_INCLUDE_H__

#pragma warning ( disable : 4251 )

// Standard
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <math.h>

using namespace std;

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

// DirectX
#include "../../Reference/DirectX9/Header/d3d9.h"
#include "../../Reference/DirectX9/Header/d3dx9.h"
#pragma comment(lib,"../../Reference/DirectX9/Library/d3d9.lib")
#pragma comment(lib,"../../Reference/DirectX9/Library/d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

// 고수준 입출력
#include <iostream>

// fmod 사용을 위한 헤더와 라이브러리 포함
#include <io.h>
#include "fmod.h"
#include "fmod.hpp"
#pragma comment(lib, "../../Reference/Library/fmodex_vc.lib")

// Custom
#include "Engine_Define.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_TypeDef.h"

using namespace Engine;

#endif // !__ENGINE_INCLUDE_H__
