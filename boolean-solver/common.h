#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
namespace bl {
	typedef std::string string;
	
	template <typename T>
	using flex_array = std::vector<T>;

	template <typename T, typename U>
	using dictionary = std::unordered_map<T, U>;

	template <typename T, typename U>
	using ordered_dictionary = std::map<T, U>;

	// goddammit std::vector<bool>...
	// for the uninitiated (or sensible and logical thinkers) this may be confusing
	// that's cus std::vector<bool> isn't actually a vector of bools... i know right.
	// c++ trys to be clever and turns std::vector<bool> into what is essensially a bitset
	// (meaning you cannot get bool& references out of it, but some silly proxy type)
	// which, whilst it may be more space efficent, when I want a vector of bools
	// i want a vector of bools, otherwise i'd use std::bitset
	// so here I basically try to emulate correct behaviour of std::vector<bool>
	// ontop of std::vector<char>... 
	class bool_iterator {
	private:
		std::vector<char>::iterator m_it;
	public:
		bool_iterator(const std::vector<char>::iterator& it) : m_it(it) {}

		bool_iterator& operator++() { m_it++; return *this; }
		
		bool operator==(bool_iterator other) const { return m_it == other.m_it; }
		bool operator!=(bool_iterator other) const { return m_it != other.m_it; }
		bool& operator*() const { return (bool&) (*m_it); }
	};

	class flex_array_bool {
	public:
		void push_back(const bool& b) { m_data.push_back((char)b); }
		bool& operator[](int index) { return (bool&)m_data[index]; }
		
		bool_iterator begin() { return bool_iterator(m_data.begin()); }
		bool_iterator end() { return bool_iterator(m_data.end()); }

		bool& back() { return (bool&) m_data.back(); }
		size_t size() const { return m_data.size(); }
		void resize(size_t n) { m_data.resize(n); }
	private:
		flex_array<char> m_data;
	};
}