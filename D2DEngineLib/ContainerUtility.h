#pragma once

#include <vector>
#include <algorithm>
#include <functional>

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

	// hash가 필요한 자료구조에 Pair를 쓰기 위한 커스텀 Hash 구조체
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