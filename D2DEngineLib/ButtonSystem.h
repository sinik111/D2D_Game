#pragma once

class Button;
class D2DRenderer;

class ButtonSystem
{
private:
	std::vector<Button*> m_buttons;
	Button* m_pressedButton = nullptr;

public:
	void RegisterButton(Button* button);
	void UnregisterButton(Button* button);

public:
	void Update();
};