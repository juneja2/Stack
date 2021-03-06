#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <iomanip>
#include <cstdlib>

using namespace std;

double valueOf(char s);
bool does_x_have_higherOrEqualPrecidence_than_c(char x, char c);
string infixToPostFix(string exp);
double evaluatePostFix(string exp);
string postfixToPrefix(string exp);

int main()
{
	try {
		ifstream infile;
		infile.open("data.txt");
		if (!infile)
			throw("Wasn't able to open the data file\n");
		cout << left << setw(30) << "infix" << setw(25) << "prefix" << setw(25) << "postfix" << "value\n";
		while (!infile.eof()) {
			string infix;
			getline(infile, infix);
			string postFix = infixToPostFix(infix);
			string preFix = postfixToPrefix(postFix);
			cout << setw(30) << infix << setw(25) << preFix << setw(25) << postFix << evaluatePostFix(postFix) << endl;
		}
	}
	catch (char * error) {
		cout << error << endl;
		exit(EXIT_FAILURE);
	}
}
double valueOf(char s) {
	switch (s) {
	case 'A': return 2.0;
	case 'B': return 3.0;
	case 'C': return 4.0;
	case 'D': return 5.0;
	case 'E': return 6.0;
	default: throw("Not a valid symbol");
	}
}
bool does_x_have_higherOrEqualPrecidence_than_c(char x, char c) {
	if (x == '/' || x == '*')
		return true;
	else if ((x == '+' || x == '-') && (c == '+' || c == '-'))
		return true;
	else
		return false;
}
string infixToPostFix(string exp) {
	string postFixExp;
	stack<char> operators;
	unsigned expLength = exp.length();
	for (unsigned i = 0; i < expLength; i++) {
		char c = exp[i];
		if (c == ' ')
			continue;
		else if (c == '(')
			operators.push(c);
		else if (c == ')') {
			char d = operators.top();
			operators.pop();
			while (d != '(') {
				postFixExp += d;
				d = operators.top();
				operators.pop();
			}
		}
		else if (c == '*' || c == '/' || c == '+' || c == '-') {
			char x;
			while (!operators.empty() && (x = operators.top()) != '(' && does_x_have_higherOrEqualPrecidence_than_c(x, c)) {
				postFixExp += x;
				operators.pop();
			}
			operators.push(c);
		}
		else
			postFixExp += c;
	}
	while (!operators.empty()) {
		postFixExp += operators.top();
		operators.pop();
	}
	return postFixExp;
}
double evaluatePostFix(string exp) {
	stack<double> operand_stack;
	for (unsigned i = 0; i < exp.length(); i++) {
		char c = exp[i];
		if (c == '*' || c == '/' || c == '+' || c == '-') {
			double A[2];
			for (unsigned i = 0; i < 2; i++) {
				A[i] = operand_stack.top();
				operand_stack.pop();
			}
			switch (c) {
			case '*': operand_stack.push(A[1] * A[0]);
				break;
			case '/': operand_stack.push(A[1] / A[0]);
				break;
			case '+': operand_stack.push(A[1] + A[0]);
				break;
			case '-': operand_stack.push(A[1] - A[0]);
			}
		}
		else operand_stack.push(valueOf(c));
	}
	return operand_stack.top();
}
string postfixToPrefix(string exp) {
	stack<string> preFixExp;
	string temp = "";
	for (unsigned i = 0; i < exp.length(); i++) {
		string token(1, exp[i]);
		if (token == "*" || token == "/" || token == "+" || token == "-") {
			for (int i = 0; i < 2; i++) { // i < 2 because we are dealing with binary 
					 //operators which means this loop will run 2 times
				temp = preFixExp.top() + temp;
				preFixExp.pop();
			}
			preFixExp.push(token + temp);
			temp = "";
		}
		else preFixExp.push(token);
	}
	return preFixExp.top();
}