#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

// General
#include <windows.h>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <queue>
#include <map>

// Com
#include <wrl.h>

// DirectX
#include <d3d11.h>
#include <d2d1_3.h>
#include <dxgi1_6.h>
#include <wincodec.h>
#include <dwrite_3.h>

// Math
#include "MyMath.h"
#include "Vector2.h"
#include "Matrix3x2.h"
#include "Bounds.h"
#include "Random.h"

// System
#include "MyTime.h"
#include "Input.h"
#include "Debug.h"

// Util
#include "ContainerUtility.h"