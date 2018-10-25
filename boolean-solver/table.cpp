#include "table.h"

using namespace bl;

void table::add_column(const string& header) {
	m_headers[header] = m_headers.size();
}

table::row& table::add_row() {
	row row;
	row.m_data.resize(m_headers.size());
	row.m_parenttable = this;
	m_rows.push_back(row);
	return m_rows.back();
}