#pragma once

#include <cassert>

class CoInitializer
{
private:
	CoInitializer(DWORD dwCoInit = COINIT_APARTMENTTHREADED)
	{
		HRESULT hr = CoInitializeEx(nullptr, dwCoInit);
		assert(SUCCEEDED(hr) && "Com 초기화 실패");
	}

	~CoInitializer()
	{
		CoUninitialize();
	}

	CoInitializer(const CoInitializer&) = delete;
	CoInitializer& operator=(const CoInitializer&) = delete;

	friend class WinApp;
};