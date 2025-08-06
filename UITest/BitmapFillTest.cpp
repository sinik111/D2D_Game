#include "../D2DEngineLib/framework.h"
#include "BitmapFillTest.h"

#include "../D2DEngineLib/BitmapRenderer.h"

float fillRatio = 1.0f;
float floatingRatio = 0.0f;

void BitmapFillTest::Start()
{
	m_bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();
}

void BitmapFillTest::Update()
{
	if (Input::IsKeyHeld('1'))
	{
		fillRatio -= 0.01f;

		fillRatio = MyMath::Clamp(fillRatio, 0.0f, 1.0f);

		m_bitmapRenderer->SetFill(BitmapDirectionType::RightToLeft, fillRatio);
	}

	if (Input::IsKeyHeld('2'))
	{
		fillRatio += 0.01f;

		fillRatio = MyMath::Clamp(fillRatio, 0.0f, 1.0f);

		m_bitmapRenderer->SetFill(BitmapDirectionType::RightToLeft, fillRatio);
	}

	floatingRatio += 0.001f;

	m_bitmapRenderer->SetFloating(BitmapDirectionType::RightToLeft, std::fmodf(floatingRatio, 1.0f));
}