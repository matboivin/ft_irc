#ifndef CLI_PARSER_HPP
# define CLI_PARSER_HPP

# include <string>

namespace ft_irc
{
	/*
	 * Parser for command line arguments
	 * • Your executable will be used as follows:
	 * ./ircserv bind_address <port> <password>
	 */
	class CLIParser
	{
	public:
		/* Default constructor */
					CLIParser();

		/* Arguments constructor */
					CLIParser(int argc, char** argv);

		/* Copy constructor */
					CLIParser(const CLIParser& other);

		/* Copy assignment operator */
		CLIParser&	operator=(const CLIParser& other);

		/* Destructor */
					~CLIParser();

		/* Getters */
		std::string	getBindAddress() const;
		std::string	getPort() const;
		std::string	getPassword() const;

		/* Parses the command line arguments */
		void		parse(const char* bind_address, const char* port, const char* password);
	
	private:
		char**		_argv;
		int			_argc;
		std::string	_bind_address;
		std::string	_port;
		std::string	_password;
	};
} // namespace ft_irc

#endif // !CLIParser
