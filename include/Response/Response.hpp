#pragma once

# include <iostream>
# include <string>

struct Header {
	// Status-Line
	std::string	protocol;
	int			statusCode;
	std::string	reasonPhrase;
};

class Response
{
	public:
		// Constructors
		Response();
		Response(const Response &copy);

		// Destructor
		~Response();

		// Operators
		Response & operator=(const Response &assign);

		// Methods
		void setStatusCode(int statusCode);

		// Accessors
		int getStatusCode() const;

	private:
		Header		_header;

};

