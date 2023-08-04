#include "Response.hpp"

// Constructors
Response::Response()
{
	std::cout << "\e[0;33mDefault Constructor called of Response\e[0m" << std::endl;
}

Response::Response(const Response &copy)
{
	(void) copy;
	std::cout << "\e[0;33mCopy Constructor called of Response\e[0m" << std::endl;
}


// Destructor
Response::~Response()
{
	std::cout << "\e[0;31mDestructor called of Response\e[0m" << std::endl;
}


// Operators
Response & Response::operator=(const Response &assign)
{
	(void) assign;
	return *this;
}

