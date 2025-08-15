#pragma once

#include "../D2DEngineLib/Script.h"

class BitmapRenderer;

class VolumeControllerBase :
	public Script
{
protected:
	BitmapRenderer* m_bitmapRenderer = nullptr;

	float m_originalVolume = 0.0f;

private:
	void Initialize() override;

public:
	virtual void VolumeUp() = 0;
	virtual void VolumeDown() = 0;
	virtual void SetVolume(float volume) = 0;
	virtual void Reset() = 0;
	virtual void Cancel() = 0;
	virtual void Save() = 0;
};