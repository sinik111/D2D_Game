#pragma once

#include <vector>
#include <algorithm>

namespace Util
{
	// std::vector ��� ������ �����
	// ������ ��� ���� ��� ��� ����
	// == �����ڷ� �� ������ ��� ��� ����
	// ������ �����ε� �ؾ���
	template<typename T>
	void OptimizedErase(std::vector<T>& container, const T& target)
	{
		auto iter = std::find(container.begin(), container.end(), target);

		if (iter != container.end())
		{
			std::iter_swap(iter, container.end() - 1);

			container.pop_back();
		}
	}

	// std::vector ��� �Ϲ����� ����
	// ���� ����
	// == �����ڷ� �� ������ ��� ��� ����
	// ������ �����ε� �ؾ���
	template<typename T>
	void StableErase(std::vector<T>& container, const T& target)
	{
		auto iter = std::find(container.begin(), container.end(), target);

		if (iter != container.end())
		{
			container.erase(iter);
		}
	}
}