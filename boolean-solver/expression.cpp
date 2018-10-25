#include "expression.h"

#include <algorithm>
#include <assert.h>
#include <cmath>

using namespace bl;

static int number_of_bits_needed_for_number(int num) {
	if (num == 0) return 0;
	int count = 32;

	// https://stackoverflow.com/questions/8991024/find-out-how-many-binary-digits-a-particular-integer-has
	for (int i = 1 << 31; i != 0; i >>= 1, count--)
		if ((num & i) != 0)
			break;

	return count;
}

static flex_array<bool> number_to_binary(int num, int bits_to_fill) {
	flex_array<bool> bools;
	bools.resize(bits_to_fill);

	for (int i = 0; i < bits_to_fill; i++) {
		int b = (num & (1 << i)) >> i;
		bool bb = b != 0;
		bools[i] = bb;
	}

	return bools;
}

expression::expression(parser& parser) :
	m_parser(parser), m_currentindex(0) {
	ast_node *expr_node = parser.generate_ast();
	
	auto inputs = parser.get_inputs();
	
	size_t n = inputs.size();
	for (size_t i = 0; i < n; ++i) {
		m_input_lookup[inputs[i]] = i;
	}

	int s = static_cast<int>(std::pow(2, inputs.size()));
	for (int i = 0; i < s; ++i) {
		m_inputs.push_back(number_to_binary(i, n));
	}

	for (int i = 0; i < s; ++i) {
		m_outputs.push_back(visit(expr_node));
		m_currentindex++;
	}

	// output
	printf("| ");
	for (char c : inputs) {
		printf("%c ", c);
	}
	printf("|   |\n| ");
	int dashes_num = inputs.size() * 2 + 5;
	for (int i = 0; i < dashes_num; i++)printf("-");
	printf("\n");
	for (int i = 0; i < s; i++) {
		auto& inputs = m_inputs[i];
		printf("| ");
		for (size_t j = 0; j < inputs.size(); ++j) {
			printf("%i ", inputs[j] ? 1 : 0);
		}
		printf("| %i |\n", m_outputs[i] ? 1 : 0);
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
