#pragma once

#include "VolumeControllerBase.h"

class BitmapRenderer;

class SFXVolumeController :
	public VolumeControllerBase
{
private:
	GameObject* m_sfxVolumeName = nullptr;

private:
	void Start() override;

public:
	void VolumeUp() override;
	void VolumeDown() override;
	void SetVolume(float volume) override;
	void Reset() override;
	void Cancel(bool destroy = false) override;
	void Save() override;
	void SetSelected(bool selected) override;
};