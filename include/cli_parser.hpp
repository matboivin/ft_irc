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
		char			**argv;
		int				argc;
		std::string		bind_address;
		std::string		port;
		std::string		password;
	public:
		CLIParser(int argc, char **argv);
		std::string getBindAddress() const;
		std::string getPort() const;
		std::string getPassword() const;
		CLIParser();
		CLIParser(const CLIParser &other);
		CLIParser &operator=(const CLIParser &other);
		~CLIParser();
		//parse the command line arguments
		void parse(char const *bind_address, char const *port, char const *password);
	};
}

#endif // !CLIParser_
