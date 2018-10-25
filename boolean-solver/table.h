#pragma once

#include "common.h"

#include <any>
#include <assert.h>

namespace bl {
	typedef std::any table_value;

	template <typename T>
	inline T table_value_cast(const table_value& v) { return std::any_cast<T>(v); }

	class table {
	public:
		class row {
			friend class table;

			flex_array<table_value> m_data;
			table* m_parenttable;

		public:
			table_value& operator[](const string& header) {
				int i = m_parenttable->m_headers[header];
				return m_data[i];
			}

			const flex_array<table_value>& cols() const { return m_data; }

			template <typename T>
			T& get(const string& header) {
				int i = m_parenttable->m_headers[header];
				std::any& a = m_data[i];
				assert(a.has_value());
				return std::any_cast<T&>(a);
			}
		};

		void add_column(const string& header);

		template <typename T, typename... Args>
		void add_columns(const T& colname, const Args&... names) {
			add_column(colname);
			add_columns(names...);
		}

		void add_columns() {}

		// todo: need someway to verify that all column names are of string type.
		template <typename... Args>
		typename void add_columns(const Args&... names) {
			add_columns(names...);
		}

		const flex_array<row>& rows() const { return m_rows; }

		row& add_row();

	private:
		dictionary<string, int> m_headers;
		flex_array<row> m_rows;
	};
}