#include "expression.h"
#include "table.h"

#include <algorithm>
#include <assert.h>
#include <cmath>
#include <iostream>

using namespace bl;

static flex_array_bool number_to_binary(int num, int bits_to_fill) {
	flex_array_bool bools;
	bools.resize(bits_to_fill);

	for (int i = 0; i < bits_to_fill; i++) {
		int b = (num >> i) & 1;
		bool bb = b != 0;
		bools[i] = bb;
	}

	return bools;
}
expression::expression(parser& parser) :
	m_parser(parser), m_currentindex(0) {
	ast_node *expr_node = parser.generate_ast();
	
	auto inputs = parser.get_inputs();
	
	bl::table truth_table;

	size_t n = inputs.size();
	for (size_t i = 0; i < n; ++i) {
		m_input_lookup[inputs[i]] = i;
		truth_table.add_column(string(1, inputs[i]));
	}
	truth_table.add_column("answer");

	int s = static_cast<int>(std::pow(2, inputs.size()));
	for (int i = 0; i < s; ++i) {
		m_inputs.push_back(number_to_binary(i, n));
	}

	for (int i = 0; i < s; ++i) {
		table::row& row = truth_table.add_row();
		for (auto& pair : m_input_lookup) {
			bool b = m_inputs[m_currentindex][m_input_lookup[pair.first]];
			row[string(1, pair.first)] = b;
		}
			
		row["answer"] = visit(expr_node);
		m_currentindex++;
	}

	for (const table::row& row : truth_table.rows()) {
		for (const table_value& v : row.cols()) {
			std::cout << table_value_cast<bool>(v) << " ";
		}
		std::cout << std::endl;
	}
}

bool expression::visit(ast_node * node) {
	switch (node->type) {
	case node_type::kOperation:
		return visit_op((ast_operation*)node);
	case node_type::kInput:
		return visit_input((ast_input*)node);
	case node_type::kLiteral:
		return visit_literal((ast_literal*)node);
	case node_type::kNotOperation:
		return visit_not_op((ast_not_op*)node);
	}
	assert(false);
	return false;
}

bool expression::visit_op(ast_operation * node) {
	bool left = visit(node->left);
	bool right = visit(node->right);

	switch (node->operation) {
	case token_type::kAnd:
		return left && right;
	case token_type::kOr:
		return left || right;
	case token_type::kXor:
		return (!left && right) || (left && !right);
	}
	assert(false);
	return false;
}

bool expression::visit_input(ast_input* input) {
	return m_inputs[m_currentindex][m_input_lookup[input->value]];
}

bool expression::visit_literal(ast_literal * literal) {
	return literal->value == '1';
}

bool expression::visit_not_op(ast_not_op * op) {
	return !visit(op->expr);
}
