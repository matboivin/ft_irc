#ifndef CLI_PARSER_HPP
# define CLI_PARSER_HPP

# include "ft_irc.hpp"

namespace ft_irc
{
	//Parser for command line arguments
	// • Your executable will be used as follows:
	// ./ircserv bind_address <port> <password>
	class CLIParser
	{
	private:
		// attributes
		char**			argv;
		int				argc;
		std::string		bind_address;
		std::string		port;
		std::string		password;

	public:
		// default constructor
					CLIParser();
		// copy constructor
					CLIParser(const CLIParser& other);
		// arg constructor
					CLIParser(int argc, char** argv);
		// assignment operator
		CLIParser	&operator=(const CLIParser& other);
		// destructor
					~CLIParser();

		// getters
		std::string	getBindAddress() const;
		std::string	getPort() const;
		std::string	getPassword() const;

		//parse the command line arguments
		void		parse(const char* bind_address, const char* port,
						  const char* password);
	};
} // namespace ft_irc

#endif // !CLIParser
