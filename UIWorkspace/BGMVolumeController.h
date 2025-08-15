#pragma once

#include "VolumeControllerBase.h"

class BitmapRenderer;

class BGMVolumeController :
	public VolumeControllerBase
{
private:
	void Start() override;

public:
	void VolumeUp() override;
	void VolumeDown() override;
	void SetVolume(float volume) override;
	void Reset() override;
	void Cancel() override;
	void Save() override;
};