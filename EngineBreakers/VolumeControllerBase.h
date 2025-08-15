#pragma once

#include "../D2DEngineLib/Script.h"

class BitmapRenderer;

class VolumeControllerBase :
	public Script
{
protected:
	BitmapRenderer* m_bitmapRenderer = nullptr;
	GameObject* m_volumeIndicator = nullptr;
	GameObject* m_volumeSliderFrame = nullptr;

	float m_originalVolume = 0.0f;
	float m_barWidth = 0.0f;

private:
	void Initialize() override;

public:
	virtual void VolumeUp() = 0;
	virtual void VolumeDown() = 0;
	virtual void SetVolume(float volume) = 0;
	virtual void Reset() = 0;
	virtual void Cancel(bool destroy = false) = 0;
	virtual void Save() = 0;
	virtual void SetSelected(bool selected) = 0;
};