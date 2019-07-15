/**
 * @file System.h
 * @date 19.4.2017
 * @author Matej Karas, FIT
 */

#pragma once
#include <string>
#include <queue>
#include <exception>

/**
 * Exceptions from parser
 */
class ParserException : public std::exception
{
public:
	ParserException(std::string&& except)
		: mExcept(except)
	{

	}

	const char* what() const throw ()
	{
		return mExcept.c_str();
	}

private:
	std::string mExcept;		//!< string holding exception message
};

/**
 * Operators used in parser
 */
enum class operator_t
{
	ISNUMBER,

	UNARYMINUS,	//@ character

	LEFTPAR, // precedence 1
	RIGHTPAR,
	FUN_SEP,  
	
	GROOT,
	PI,
	FIBO,


	ADD, // precedence 2
	SUB,

	MUL, // precedence 3 
	DIV,
	MOD,

	POW, // precedence 4  // also only 2 right associative operator
	FACT, 
};

/**
 * Parsers token
 */
struct token_t
{
	operator_t op;  //!< define operation of token
	double num;		//!< value of number if the token is number
};

/**
 * System class, connecting UI and math library
 */
class System
{
public:
	/**
	 * Instance of system
	 * @ret pointer to the system
	 */
	static System* getInstance();
	~System();

	/**
	 * Evaluate given expression. Parse string and convert it to 
	 * postfix notation.
	 *
	 * @ret value of expression
	 */
	double evaluate();

	/**
	 * Helper function, for setting expression
	 * 
	 * @param s string containing expression
	 */
	void setExpression(std::string&& s);

private:
	/**
	 * Converts infix notation to postfix.
	 */
	void toPostfix();

	/**
	 * Correction for unary minus
	 */
	void unaryMinusCorrection();

	/** 
	 * Get operator type from character
	 * @param c operator character
	 * @ret operator token
	 */
	operator_t		getOperator(char c);

	/**
	 * Get precedence of given operator
	 * @param op operator
	 * @ret precedence of operator
	 */
	unsigned int	getPrecedence(operator_t op);

	/**
	 * Reads one token from expression
	 * @ret read token
	 */
	token_t			readToken();

private:
	/**
	 * Defaul constructor hidden (MSVC doesn't support delete keyword)
	 */
	System();

	static System*	instance;	//!< Pointer containing instance of system class
	
private:
	std::string				mExpression;	//!< expression to evaluate
	std::deque<token_t>		mOutput;		//!< output queue holding postfix notation
};
