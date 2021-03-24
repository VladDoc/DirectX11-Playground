#pragma once

#include <utility>
#include <array>
#include <cstdlib>

#include "constants.h"

namespace util 
{
	template<typename T>
	constexpr void check()
	{
		static_assert(std::is_arithmetic<T>::value,
					  "Works only with arithmetic types.");
	}

	template <typename T, typename Time>
	inline T interpolateValue(Time frametime, T value)
	{
		check<Time>();
		return value * (static_cast<T>(frametime) / static_cast<T>(Constants::targetSpeed));
	}

	template<typename ID3D>
	void safeRelease(ID3D*& ptr)
	{
		if (ptr) { ptr->Release(); ptr = nullptr; }
	}

	inline auto to_wchar(const char* str) -> std::pair<std::array<wchar_t, MAX_PATH>, size_t>
	{
		std::array<wchar_t, MAX_PATH> wstr;

		size_t charsConverted = 0;

		mbstowcs_s(&charsConverted, wstr.data(), std::size(wstr), str, strlen(str));

		return {wstr, charsConverted};
	}

	template<typename Container>
	constexpr inline size_t elem_size(Container&& c) { return sizeof(decltype(*std::begin(c)));}
}