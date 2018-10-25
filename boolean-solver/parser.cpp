#include "parser.h"

#include <ctype.h>
#include <stdio.h>
#include <assert.h>

using namespace bl;

token token::termination = token::token(token_type::kTerminate, 0);

/*
 * factor     : literal | input | OPEN_BRACKET expression CLOSE_BRACKET
 * expression : factor ((AND | OR) factor)*
 */

parser::parser(const char* expression) {
	m_expression = expression;
	m_currentoken = get_next_token();
}

ast_node* parser::parse_factor() {
	token tok = m_currentoken;
	if (tok.type == token_type::kLiteral) {
		this->eat_token(token_type::kLiteral);
		return new ast_literal(node_type::kLiteral, tok.value);
	}
	else if (tok.type == token_type::kInput) {
		this->eat_token(token_type::kInput);
		return new ast_input(node_type::kInput, tok.value);
	}
	else if (tok.type == token_type::kOpenBracket) {
		this->eat_token(token_type::kOpenBracket);
		ast_node* node = this->generate_ast();
		this->eat_token(token_type::kCloseBracket);
		return node;
	}
	else if (tok.type == token_type::kNot) {
		this->eat_token(token_type::kNot);
		ast_node* node = new ast_not_op(node_type::kNotOperation, this->parse_factor());
		return node;
	}

	assert(false);
	return nullptr;
}

void parser::eat_token(token_type type) {
	if (m_currentoken.type == type)
		m_currentoken = this->get_next_token();
	else
		printf("error: invalid syntax!");
}

ast_node* parser::generate_ast() {
	ast_node* node = this->parse_factor();
	while (m_currentoken.type == token_type::kAnd || 
		m_currentoken.type == token_type::kOr || 
		m_currentoken.type == token_type::kXor) {
		token_type op_type = m_currentoken.type;
		if (op_type == token_type::kAnd) {
			this->eat_token(token_type::kAnd);
		}
		else if (op_type == token_type::kOr) {
			this->eat_token(token_type::kOr);
		}
		else if (op_type == token_type::kXor) {
			this->eat_token(token_type::kXor);
		}
		node = new ast_operation(node_type::kOperation, node, op_type, this->parse_factor());
	}
	return node;
}

token parser::get_next_token() {
	char c = *m_expression;

	if (c == '\0')
		return token::termination;
	m_expression++;

	start_switch:
	switch (c) {
	case ' ':
		do {
			c = *(m_expression++);
		} while (c == ' ');
		goto start_switch;	// kinda messy (read i know it's bad practice), but it works. todo: fix
	case '0':
	case '1':
		return token(token_type::kLiteral, c);
	case '+':
		return token(token_type::kOr, c);
	case '*':
		return token(token_type::kAnd, c);
	case '~':
		return token(token_type::kNot, c);
	case '^':
		return token(token_type::kXor, c);
	case '(':
		return token(token_type::kOpenBracket, c);
	case ')':
		return token(token_type::kCloseBracket, c);
	default:
		if (isalpha((unsigned char)c)) {
			auto it = std::find(m_inputs.begin(), m_inputs.end(), c);
			if(it == m_inputs.end())
				m_inputs.push_back(c);
			return token(token_type::kInput, c);
		}
		else {
			printf("error: unknown character '%c'\n", c);
			return token::termination;
		}
	}
	return token::termination;
}