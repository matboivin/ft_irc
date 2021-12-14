#ifndef CLIPARSER_HPP
# define CLIPARSER_HPP

# include <string>

namespace ft_irc
{
	/*
	 * Parser for command line arguments
	 * • Your executable will be used as follows:
	 * ./ircserv <port> <password>
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

		/* Setters */
		void		setBindAddress(const std::string& bind_address);
		void		setPort(const std::string& port);
		void		setPassword(const std::string& password);

		/* Parses the command line arguments */
		void		parse(const char* port, const char* password);
	
	private:
		/* Attributes */
		int			_argc;
		char**		_argv;
		std::string	_bind_address;
		std::string	_port;
		std::string	_password;
	};
} // namespace ft_irc

#endif // !CLIPARSER_HPP
