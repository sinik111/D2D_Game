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
		const auto& iter = std::find(container.begin(), container.end(), target);

		if (iter != container.end())
		{
			std::iter_swap(iter, container.end() - 1);

			container.pop_back();
		}
	}

	// std::vector ���� ��� ������ �����
	// ���� ������
	// < �����ڷ� �� ������ ��� ��� ����
	// ������ �����ε� �ؾ���
	template<typename T>
	void OptimizedErase(std::vector<T>& container, std::vector<T>& targetContainer, bool isTargetSorted = false)
	{
		for (auto& target : targetContainer)
		{
			const auto& iter = std::find(container.begin(), container.end(), target);

			if (iter != container.end())
			{
				std::iter_swap(iter, container.end() - 1);

				container.pop_back();
			}
		}

		/*if (!isTargetSorted)
		{
			std::sort(targetContainer.begin(), targetContainer.end());
		}

		const auto& newEnd = std::remove_if(
			container.begin(),
			container.end(),
			[&](const T& target) {
				return std::binary_search(targetContainer.begin(), targetContainer.end(), target);
			});

		container.erase(newEnd, container.end());*/
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
}