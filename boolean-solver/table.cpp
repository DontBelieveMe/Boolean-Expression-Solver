#include "table.h"

using namespace bl;

void table::add_column(const string& header) {
	m_headers.push_back(header);
}

const flex_array<string>& table::headers() const {
	return m_headers;
}

table::row& table::add_row() {
	row row;
	row.m_data.resize(m_headers.size());
	row.m_parenttable = this;
	m_rows.push_back(row);
	return m_rows.back();
}