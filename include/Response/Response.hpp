#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>

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

	private:

};

#endif
