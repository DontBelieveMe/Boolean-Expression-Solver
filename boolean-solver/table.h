#pragma once

#include "common.h"

#include <any>
#include <assert.h>
#include <iostream>
#include <iomanip>

namespace bl {
	typedef std::any table_value;

	template <typename T>
	inline T table_value_cast(const table_value& v) { return std::any_cast<T>(v); }

	class table {
	private:
		size_t index_of_header(const string& str) {
			size_t pos = static_cast<size_t>(std::find(m_headers.begin(), m_headers.end(), str) - m_headers.begin());
			assert(pos < m_headers.size());
			return pos;
		}
	public:
		class row {
			friend class table;

			flex_array<table_value> m_data;
			table* m_parenttable;

		public:
			table_value& operator[](const string& header) {
				size_t i = m_parenttable->index_of_header(header);
				return m_data[i];
			}

			const flex_array<table_value>& cols() const { return m_data; }

			template <typename T>
			T& get(const string& header) {
				int i = m_parenttable->index_of_header(header);
				std::any& a = m_data[pos];
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
		const flex_array<string>& headers() const;

		row& add_row();

	private:
		flex_array<string> m_headers;
		flex_array<row> m_rows;
	};

	enum class align {
		left,
		right
	};

	// todo: fix assumes all components in the table are of the type T
	template <typename ValueType, typename Stream>
	class truth_table_pretty_printer{
	public:
		truth_table_pretty_printer(Stream& stream) :
			m_stream(stream) {}

		// not pretty code (read: terrible code), but hey it does it's job
		void print(const table& table) {
			int table_w = 1;
			auto headers = table.headers();
			for (const string& str : headers) {
				table_w += str.size() + 1;
			}
			auto full_width_bar = [&]() {
				m_stream << '+' << std::setfill('-') << std::setw(table_w) << "+\n";
			};
			full_width_bar();
			m_stream << '|';
			int i = 1;
			for (const string& str : headers) {
				m_stream << str;
				if (i++ != headers.size()) 
					m_stream << ' ';
			}
			
			m_stream << "|\n";
			full_width_bar();
			for (const table::row& row : table.rows()) {
				m_stream << '|';
				int i = 1;
				for (const table_value& v : row.cols()) {
					if (i == headers.size()) {
						if (final_row_alignment == align::left) {
							m_stream << table_value_cast<ValueType>(v) << ' ';
						}
						m_stream << std::setfill(' ') << std::setw(headers[i - 1].size() - 1);
						if (final_row_alignment == align::left) {
							m_stream << "|";
						}
						if (final_row_alignment == align::right) {
							m_stream << table_value_cast<ValueType>(v) << " |";
						}
					}
					else {
						m_stream << table_value_cast<ValueType>(v) << " ";
					}
					i++;
				}
				m_stream << "\n";
			}
			full_width_bar();
			std::flush(m_stream);
		}

		align final_row_alignment = align::left;

	private:
		Stream& m_stream;
	};
}