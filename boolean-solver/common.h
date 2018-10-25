#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace bl {
	typedef std::string string;
	typedef const char* cstring;
	
	template <typename T>
	using flex_array = std::vector<T>;

	template <typename T, typename U>
	using dictionary = std::unordered_map<T, U>;
}