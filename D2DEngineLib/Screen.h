#pragma once

class Screen
{
private:
	int m_width = 0;
	int m_height = 0;

private:
	Screen() = default;
	Screen(const Screen&) = delete;
	Screen operator=(const Screen&) = delete;
	Screen(Screen&&) = delete;
	Screen operator=(Screen&&) = delete;
	~Screen() = default;

public:
	static Screen& Get();

public:
	int GetWidth() const;
	int GetHeight() const;


	void SetWidth(int width);
	void SetHeight(int height);
};