#include "Response/Response.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Response::Response() { }

Response::Response(const Response &copy)
{
	if (this != &copy)
		*this = copy;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Response::~Response() { }

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Response & Response::operator=(const Response &assign)
{
	if (this != &assign)
		_header = assign._header;
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void Response::setStatusCode(int statusCode) { _header.statusCode = statusCode; }

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int Response::getStatusCode() const { return _header.statusCode ; }
