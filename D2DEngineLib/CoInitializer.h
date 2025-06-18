#pragma once

#include <cassert>

class CoInitializer
{
private:
	CoInitializer(DWORD dwCoInit = COINIT_APARTMENTTHREADED)
	{
		HRESULT hr = CoInitializeEx(nullptr, dwCoInit);
		assert(SUCCEEDED(hr) && "Com �ʱ�ȭ ����");
	}

	~CoInitializer()
	{
		CoUninitialize();
	}

	CoInitializer(const CoInitializer&) = delete;
	CoInitializer& operator=(const CoInitializer&) = delete;

	friend class WinApp;
};