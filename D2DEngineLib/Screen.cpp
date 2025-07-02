#include "pch.h"
#include "Screen.h"

Screen& Screen::Get()
{
	static Screen s_instance;

	return s_instance;
}

int Screen::GetWidth() const
{
	return m_width;
}

int Screen::GetHeight() const
{
	return m_height;
}

void Screen::SetWidth(int width)
{
	m_width = width;
}

void Screen::SetHeight(int height)
{
	m_height = height;
}
