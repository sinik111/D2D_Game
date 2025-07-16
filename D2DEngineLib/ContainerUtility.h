#pragma once

#include <vector>
#include <algorithm>
#include <functional>

namespace Util
{
	// std::vector ��� ������ �����
	// ������ ��� ���� ��� ��� ����
	// == �����ڷ� �� ������ ��� ��� ����
	// ������ �����ε� �ؾ���
	template<typename T>
	void OptimizedErase(std::vector<T>& container, const T& target)
	{
		const auto& iter = std::find(container.begin(), container.end(), target);

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
		const auto& iter = std::find(container.begin(), container.end(), target);

		if (iter != container.end())
		{
			container.erase(iter);
		}
	}

	// hash�� �ʿ��� �ڷᱸ���� Pair�� ���� ���� Ŀ���� Hash ����ü
	struct PairHash
	{
		template <class T1, class T2>
		std::size_t operator()(const std::pair<T1, T2>& p) const
		{
			auto h1 = std::hash<T1>{}(p.first);
			auto h2 = std::hash<T2>{}(p.second);

			return h1 ^ (h2 << 1);
		}
	};
}