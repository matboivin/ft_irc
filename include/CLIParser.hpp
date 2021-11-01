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
	private:
		char**		_argv;
		int			_argc;
		std::string	_bind_address;
		std::string	_port;
		std::string	_password;

	public:
		CLIParser();
		CLIParser(const CLIParser& other);
		CLIParser(int argc, char** argv);
		CLIParser&	operator=(const CLIParser& other);
		~CLIParser();

		/* Getters */
		std::string	getBindAddress() const;
		std::string	getPort() const;
		std::string	getPassword() const;

		/* Parses the command line arguments */
		void	parse(const char* bind_address, const char* port, const char* password);
	};
} // namespace ft_irc

#endif // !CLIParser
