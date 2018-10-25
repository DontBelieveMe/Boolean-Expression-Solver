#pragma once

#include "common.h"
#include "parser.h"

namespace bl {
	class expression {
	public:
		expression(parser& parser);

	private:
		bool visit(ast_node* node);
		bool visit_op(ast_operation* node);
		bool visit_input(ast_input* input);
		bool visit_literal(ast_literal* literal);
		bool visit_not_op(ast_not_op* op);
	private:
		parser & m_parser;

		dictionary<char, int> m_input_lookup;
		flex_array<flex_array<bool>> m_inputs;
		flex_array<bool> m_outputs;
		int m_currentindex;
	};
}