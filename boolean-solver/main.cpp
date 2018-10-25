#include "parser.h"
#include "expression.h"
#include "table.h"

#include <iostream>

int main(int argc, char** argv) {
	while (1) {
		std::cout << "> ";
		std::string expression;
		std::getline(std::cin, expression);
		if (expression == "clear") {
			system("cls");
			continue;
		}
		else if (expression == "exit" || expression == "quit" || expression.empty()) {
			exit(0);
		}

		bl::parser parser(expression.data());
		bl::expression expr(parser);
	}
	system("pause");
}