#include "../D2DEngineLib/framework.h"
#include "VolumeControllerBase.h"

#include "../D2DEngineLib/BitmapRenderer.h"

void VolumeControllerBase::Initialize()
{
	m_bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();
}