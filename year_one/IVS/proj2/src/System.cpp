/**
 * @file System.cpp
 * @date 19.4.2017
 * @author Matej Karas, FIT
 */

#include "System.h"
#include "mathematics.h"
#include <deque>
#include <stack>
#include <algorithm>
#include <regex>

System* System::instance = nullptr;

System::System()
	//: mExpression("groot(1- 2 /3, (3!))^ (2 + 1!)")
{
}

System::~System()
{

}

System* System::getInstance()
{
	if (!instance)
		instance = new System();

	return instance;
}

namespace //helper functions
{
	inline token_t pop(std::stack<operator_t>& opstack)
	{
		operator_t temp = opstack.top();
		opstack.pop();
		return token_t{ temp, 0.f };
	}

	inline double pop(std::deque<token_t>& queue)
	{
		double temp = queue.begin()->num;
		queue.pop_front();
		return temp;
	}

	inline double pop(std::stack<double>& stack)
	{
		if (stack.empty())
			throw(ParserException("Syntax error"));

		double temp = stack.top();
		stack.pop();
		return temp;
	}
}

void System::toPostfix()
{
	std::stack<operator_t> opStack;
	mOutput.clear();
	unaryMinusCorrection();

	// Get rid of spaces
	mExpression.erase(std::remove_if(mExpression.begin(), mExpression.end(), isspace), mExpression.end());

	mExpression = std::regex_replace(mExpression, std::regex("groot"),	"s");
	mExpression = std::regex_replace(mExpression, std::regex("fibo"),	"f");
	mExpression = std::regex_replace(mExpression, std::regex("pi"),		"p");

	// Go through expresion and pop tokens
	while (mExpression.begin() != mExpression.end())
	{
		token_t token = readToken();

		switch (token.op)
		{
			// If the token is an unary minus sign, correct number and write is as number
		case operator_t::UNARYMINUS:
			token = readToken();

			if (token.op != operator_t::ISNUMBER)
				throw(ParserException("Syntax error"));

			token.num = -token.num;

			// If the token is a number
		case operator_t::ISNUMBER:
		case operator_t::PI:
			mOutput.emplace_back(token);
			break;

			// If the token is a function token
		case operator_t::GROOT:
		case operator_t::FIBO:
			opStack.emplace(token.op);
			mExpression.erase(0, 1); //get rid of left parenthesis
			break;

			// If the token is a function argument separator
		case operator_t::FUN_SEP:
			while (!opStack.empty() && getPrecedence(opStack.top()) > 1) // pop OP stack till we get same precedence
				mOutput.emplace_back(pop(opStack));

			if (!opStack.empty() && opStack.top() != operator_t::GROOT) // only root function hase more than 1 parameters
				throw(ParserException("Syntax error"));

			break;

			// If the token is an operator
		case operator_t::ADD:	// Left associative
		case operator_t::SUB:
		case operator_t::MUL:
		case operator_t::DIV:
		case operator_t::MOD:
			while (!opStack.empty() && getPrecedence(token.op) <= getPrecedence(opStack.top()))
				mOutput.emplace_back(pop(opStack));

			opStack.emplace(token.op);
			break;

		case operator_t::POW:	// Right associative
		case operator_t::FACT:
			if (!opStack.empty())
				while (getPrecedence(token.op) < getPrecedence(opStack.top()))
					mOutput.emplace_back(pop(opStack));

			opStack.emplace(token.op);
			break;

			// If the token is a left parenthesis
		case operator_t::LEFTPAR:
			opStack.emplace(token.op);
			break;

			// If the token is a right parenthesis
		case operator_t::RIGHTPAR:
			operator_t found = operator_t::ISNUMBER; // some crap value

			// pop the stack unless left paranthesis or function is encountered
			while (!opStack.empty())
			{
				if (opStack.top() != operator_t::LEFTPAR	&& 
					opStack.top() != operator_t::GROOT		&& 
					opStack.top() != operator_t::FIBO		
					)
					mOutput.emplace_back(pop(opStack));
				else
				{
					found = opStack.top();
					break;
				}
			}

			if (found == operator_t::ISNUMBER)
				throw(ParserException("Syntax error"));

			if (found != operator_t::LEFTPAR)
				mOutput.emplace_back(pop(opStack));
			else
				opStack.pop();

			break;
		}
	} // While end

	// While there are still operator tokens in the stack, pop the operator onto the output queue
	while (!opStack.empty())
		// If the operator token on the top of the stack is a parenthesis, 
		// then there are mismatched parentheses
		if (opStack.top() == operator_t::LEFTPAR || opStack.top() == operator_t::RIGHTPAR)
			throw(ParserException("Syntax error"));
		else
			mOutput.emplace_back(pop(opStack));
}

double System::evaluate()
{
	toPostfix();
	std::stack<double> numberStack;
	
	// While there are input tokens left
	while (!mOutput.empty())
	{
		// If the token is a value, push it onto the stack
		if (mOutput.begin()->op == operator_t::ISNUMBER)
		{
			numberStack.emplace(pop(mOutput));
			continue;
		}

		// The token is an operator now 
		double operandA, operandB;
		if (mOutput.begin()->op != operator_t::PI)
		{
			operandB = pop(numberStack);
			if (mOutput.begin()->op != operator_t::FIBO && mOutput.begin()->op != operator_t::FACT)
				operandA = pop(numberStack);
		}

		switch (mOutput.begin()->op)
		{
		case operator_t::GROOT:
			numberStack.emplace(groot(operandB, operandA));
			break;

		case operator_t::FIBO:
			numberStack.emplace(fibonacci(operandB));
			break;

		case operator_t::ADD:
			numberStack.emplace(addition(operandA, operandB));
			break;

		case operator_t::SUB:
			numberStack.emplace(subtract(operandA, operandB));
			break;

		case operator_t::MUL:
			numberStack.emplace(multiplication(operandA, operandB));
			break;

		case operator_t::DIV:
			numberStack.emplace(divide(operandA, operandB));
			break;

		case operator_t::MOD:
			numberStack.emplace(modulo(operandA, operandB));
			break;

		case operator_t::POW:
			numberStack.emplace(power(operandA, operandB));
			break;

		case operator_t::FACT:
			numberStack.emplace(factorial(operandB));
			break;

		case operator_t::PI:
			numberStack.emplace(3.141592653589);
			break;
		}

		mOutput.pop_front();
	}

	if (numberStack.size() != 1)
		throw(ParserException("Syntax error"));

	return numberStack.top();
}

void System::unaryMinusCorrection()
{
	//a minus sign at the beginning is also for a negative number
	//a minus sign immediately after a parentheses is for a negative number as well

	if (*mExpression.cbegin() == '-')
		*mExpression.begin() = '@';

	mExpression = std::regex_replace(mExpression, std::regex("\\(-"), "(@");
	mExpression = std::regex_replace(mExpression, std::regex("\\,-"), ",@");
}

void System::setExpression(std::string&& s)
{
	mExpression = std::move(s);
}

operator_t System::getOperator(char c)
{
	switch (c)
	{
	case '@':
		return operator_t::UNARYMINUS;
	case '(':
		return operator_t::LEFTPAR;
	case ')':
		return operator_t::RIGHTPAR;
	case ',':
		return operator_t::FUN_SEP;
	case '+':
		return operator_t::ADD;
	case '-':
		return operator_t::SUB;
	case '*':
		return operator_t::MUL;
	case '/':
		return operator_t::DIV;
	case '%':
		return operator_t::MOD;
	case '^':
		return operator_t::POW;
	case '!':
		return operator_t::FACT;
	case 's':
		return operator_t::GROOT;
	case 'f':
		return operator_t::FIBO;
	case 'p':
		return operator_t::PI;
	}
}

unsigned int System::getPrecedence(operator_t op)
{
	switch (op)
	{
	case operator_t::FIBO:
	case operator_t::GROOT:
	case operator_t::PI:

	case operator_t::LEFTPAR:
	case operator_t::RIGHTPAR:
	case operator_t::FUN_SEP:
		return 1;

	case operator_t::ADD:
	case operator_t::SUB:
		return 2;

	case operator_t::MUL:
	case operator_t::DIV:
	case operator_t::MOD:
		return 3;

	case operator_t::POW:
	case operator_t::FACT:
		return 4;
	}
}

token_t System::readToken()
{
	if (!isdigit(*mExpression.begin()))
	{
		operator_t op = getOperator(*mExpression.begin());
		mExpression.erase(0, 1);
		return token_t{ op, 0 };
	}
	else
	{
		size_t it = mExpression.find_first_of("+-*/%^!sf(),");
		double num = std::stod(mExpression.substr(0, it));
		mExpression.erase(0, it);
		return token_t{ operator_t::ISNUMBER, num };
	}
}
