#pragma once

#include <vector>
#include <algorithm>

namespace Util
{
	// std::vector 요소 빠르게 지우기
	// 순서가 상관 없을 경우 사용 가능
	// == 연산자로 비교 가능한 경우 사용 가능
	// 없으면 오버로딩 해야함
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

	// std::vector 여러 요소 빠르게 지우기
	// 순서 유지됨
	// < 연산자로 비교 가능한 경우 사용 가능
	// 없으면 오버로딩 해야함
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

	// std::vector 요소 일반적인 삭제
	// 순서 유지
	// == 연산자로 비교 가능한 경우 사용 가능
	// 없으면 오버로딩 해야함
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