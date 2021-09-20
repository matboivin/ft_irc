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
		CLIParser(int argc, char **argv)
		{
			if (argc == 2 && std::strcmp(argv[1], "--help") == 0)
			{
				displayUsage(argv[0]);
				exit(EXIT_SUCCESS);
			}
			if (argc < 4)
				throw std::runtime_error("Not enough arguments");
			if (argc > 4)
				throw std::runtime_error("Too many arguments");
			parse(argv[1], argv[2], argv[3]);
		}
		std::string getBindAddress() const
		{
			return this->bind_address;
		}
		std::string getPort() const
		{
			return this->port;
		}
		std::string getPassword() const
		{
			return this->password;
		}
		
		//default constructor
		CLIParser()
		{
			this->argv = nullptr;
			this->argc = 0;
			this->bind_address = "";
			this->port = "";
			this->password = "";
		}
		//copy constructor
		CLIParser(const CLIParser &other)
		{
			this->argv = nullptr;
			this->argc = other.argc;
			this->bind_address = other.bind_address;
			this->port = other.port;
			this->password = other.password;
		}
		//assignment operator
		CLIParser &operator=(const CLIParser &other)
		{
			this->argc = other.argc;
			this->bind_address = other.bind_address;
			this->port = other.port;
			this->password = other.password;
			return *this;
		}
		//destructor
		~CLIParser()
		{
		}
		//parse the command line arguments
		void parse(char const *bind_address, char const *port, char const *password)
		{
			this->bind_address = bind_address;
			this->port = port;
			this->password = password;
		}
	};
}

#endif // !CLIParser_
