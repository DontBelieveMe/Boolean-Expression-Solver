#pragma once

#include "common.h"

namespace bl {
	enum class token_type {
		kAnd,
		kOr,
		kNot,
		kLiteral,
		kInput,
		kTerminate,
		kOpenBracket,
		kCloseBracket,
		kXor
	};

	struct token {
		token_type type;
		char	   value;	// assume for now that all tokens are single characters
		
		token(token_type type, const char& value)
			: type(type), value(value) {}
		token() :
			type(token_type::kTerminate), value(0) {}

		static token termination;
	};

	enum class node_type {
		kLiteral, 
		kInput, 
		kOperation,
		kNotOperation
	};

	struct ast_node { 
		node_type type;
		ast_node(const node_type& type) : type(type) {}
	};
	
	struct ast_literal : ast_node {
		char value;

		ast_literal(const node_type& type, char c) 
			: ast_node(type), value(c) {}
	};

	struct ast_operation : ast_node {
		ast_node* left;
		token_type operation;
		ast_node* right;

		ast_operation(const node_type& type, ast_node* left, token_type op, ast_node* right) 
			: ast_node(type), left(left), operation(op), right(right) {}
	};
	
	struct ast_input : ast_node {
		char value;
		ast_input(const node_type& type, char c) 
			: ast_node(type), value(c) {}
	};

	struct ast_not_op : ast_node {
		ast_node *expr;
		ast_not_op(const node_type& type, ast_node* expr)
			: ast_node(type), expr(expr) {}
	};

	class parser {
	public:
		parser(const char* expression);
		ast_node* generate_ast();
		
		const flex_array<char>& get_inputs() const {
			return m_inputs;
		};

	private:
		token get_next_token();
		ast_node* parse_factor();
		void eat_token(token_type type);

	private:
		const char* m_expression;
		token m_currentoken;
		flex_array<char> m_inputs;
	};
}