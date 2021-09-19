/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 13:07:21 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/19 14:13:50 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>


#ifndef nullptr
# define nullptr NULL
#endif // !nullpt

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

	class IRCClient
	{
	private:
		std::string nick;
		std::string user_agent;
		std::string joined_channels;
		std::string password;
		struct sockaddr_in client_address;	//IPv4 address		
		std::string client_address_str;		//IPv4 address as string
	public:
		IRCClient(std::string nick="", std::string user_agent="", std::string password="")
		{
			this->nick = nick;
			this->user_agent = user_agent;
			this->password = password;
			this->client_address_str = "";
		}
		//IRCClient getters
		std::string getNick() const
		{
			return this->nick;
		}
		std::string getUserAgent() const
		{
			return this->user_agent;
		}
		std::string getJoinedChannels() const
		{
			return this->joined_channels;
		}
		std::string getPassword() const
		{
			return this->password;
		}
		std::string getIpAddressStr() const
		{
			return this->client_address_str;
		}
		struct sockaddr_in getClientAddress() const
		{
			return this->client_address;
		}
		//IRCClient setters
		void setNick(std::string nick)
		{
			this->nick = nick;
		}
		void setUserAgent(std::string user_agent)
		{
			this->user_agent = user_agent;
		}
		void setJoinedChannels(std::string joined_channels)
		{
			this->joined_channels = joined_channels;
		}
		void setPassword(std::string password)
		{
			this->password = password;
		}
		void setAddress(std::string ipaddress)
		{
			this->client_address_str = ipaddress;
		}
		//copy constructor
		IRCClient(const IRCClient &other)
		{
			this->nick = other.nick;
			this->user_agent = other.user_agent;
			this->joined_channels = other.joined_channels;
			this->password = other.password;
			this->client_address_str = other.client_address_str;
			this->client_address = other.client_address;
		}
		//assignment operator
		IRCClient &operator=(const IRCClient &other)
		{
			this->nick = other.nick;
			this->user_agent = other.user_agent;
			this->joined_channels = other.joined_channels;
			this->password = other.password;
			this->client_address_str = other.client_address_str;
			return *this;
		}
		//destructor
		~IRCClient()
		{
		}
	};
		

	class IRCServer
	{
	private:
		std::string			bind_address;
		std::string			port;
		std::string			password;
		//Structure describing an Internet socket address.
		struct sockaddr_in	address;
		//Socket descriptor.
		int					sockfd;
		int					backlog_max;
	public:
		IRCServer(std::string const &bind_address, std::string const &port, std::string const &password)
		{
			this->bind_address = bind_address;
			this->port = port;
			this->password = password;
			this->sockfd = -1;
			this->backlog_max = 5;
		}
		
		//IRCServer getters
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

		//IRCServer setters
		void setBindAddress(std::string bind_address)
		{
			this->bind_address = bind_address;
		}
		void setPort(std::string port)
		{
			this->port = port;
		}
		void setPassword(std::string password)
		{
			this->password = password;
		}

		//default constructor
		IRCServer()
		{
			this->bind_address = "";
			this->port = "";
			this->password = "";
			this->sockfd = -1;
			this->backlog_max = 5;
		}
		//copy constructor
		IRCServer(const IRCServer &other)
		{
			this->bind_address = other.bind_address;
			this->port = other.port;
			this->password = other.password;
			this->sockfd = other.sockfd;
			this->backlog_max = other.backlog_max;
		}
		//assignment operator
		IRCServer &operator=(const IRCServer &other)
		{
			this->bind_address = other.bind_address;
			this->port = other.port;
			this->password = other.password;
			this->sockfd = other.sockfd;
			this->backlog_max = other.backlog_max;
			return *this;
		}
		//destructor
		~IRCServer()
		{
		}

		int run()
		{
			//create a new listening tcp socket and bind it to the given address and port
			//https://www.geeksforgeeks.org/socket-programming-cc/

			//create a new socket
			//AF_INET: IPv4
			//SOCK_STREAM: TCP
			//IPPROTO_TCP: TCP protocol
			sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (sockfd < 0)
			{
				throw std::runtime_error("socket() failed");
			}
			
			//create a new address struct
			address.sin_family = AF_INET;
			address.sin_port = htons(atoi(this->port.c_str()));
			address.sin_addr.s_addr = inet_addr(this->bind_address.c_str());
			
			//bind the socket to the given address
			if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
			{
				throw std::runtime_error("bind() failed");
			}
			
			//listen for incoming connections
			if (listen(sockfd, 5) < 0)
			{
				throw std::runtime_error("listen() failed");
			}
			
			//accept incoming connections
			while (true)
			{
				//create new Client object
				IRCClient client;
				//declare a new client address
				struct sockaddr_in client_address;
				//declare a new client address size
				socklen_t client_address_size = sizeof(client_address);	
				//accept a new connection
				int new_sockfd = accept(sockfd, (struct sockaddr *)&client_address, &client_address_size);
				if (new_sockfd < 0)
				{
					throw std::runtime_error("accept() failed");
				}				
				//log the clients IP address
				std::cout << "Client " << inet_ntoa(client_address.sin_addr) << " connected" << std::endl;

				//read the client's nick
				std::string input;
	
				if (!sockGetLine(new_sockfd, input))
				{
					throw std::runtime_error("sockGetLine() failed");
				}
				client.setNick(input);
				std::cout << "Nick: " << input << std::endl;
				input.clear();
				//read the client's user agent
				if (!sockGetLine(new_sockfd, input))
				{
					throw std::runtime_error("sockGetLine() failed");
				}
				client.setUserAgent(input);
				std::cout << "User-Agent: " << input << std::endl;
				input.clear();
		
				//read the client's password
				if (!sockGetLine(new_sockfd, input))
				{
					throw std::runtime_error("sockGetLine() failed");
				}
				client.setPassword(input);
				std::cout << "Password: " << input << std::endl;
				input.clear();
				
				//send the client's nick and user agent back to him (for logging purposes)
				std::string response = ":";
				response += client.getNick();
				response += "!";
				response += client.getUserAgent();
				response += " ";
				response += client.getPassword();
				response += "\r\n";
				//log the response
				std::cout << "Sending: " << response << "to " << inet_ntoa(client_address.sin_addr) << std::endl;
				if (send(new_sockfd, response.c_str(), response.size(), 0) < 0)
				{
					throw std::runtime_error("send() failed");
				}
				//log the closing of the connection
				std::cout << "Closing connection to " << inet_ntoa(client_address.sin_addr) << std::endl
				<< "----------------------------------------------------------------" << std::endl;
				//send EOF to the client
				//close the connection
				close(new_sockfd);
			}
		}

			private:
				int sockGetLine(int sockfd, std::string &line)
				{
					//read char by char
					char c;
					line = "";
					while (true)
					{
						if (recv(sockfd, &c, 1, 0) < 0)
						{
							return false;
						}
						if (c == '\n')
						{
							break;
						}
						line += c;
					}
					return true;
				}

				int sockGetLine(int sockfd, std::string &line, std::size_t max_bytes)
				{
					//read char by char
					char c;
					line = "";
					while (true)
					{
						if (recv(sockfd, &c, 1, 0) < 0)
						{
							return false;
						}
						if (c == '\n')
						{
							break;
						}
						line += c;
						if (line.size() > max_bytes)
						{
							throw std::runtime_error("line too long");
						}
					}
					return true;
				}
	};

}
	

int	main( int argc, char** argv )
{
	try
	{
		ft_irc::CLIParser CLI_parser(argc, argv);
		try
		{
			ft_irc::IRCServer server(CLI_parser.getBindAddress(), CLI_parser.getPort(), CLI_parser.getPassword());
			server.run();
		}
		catch (std::runtime_error &e)
		{
			std::cerr << e.what() << std::endl;
			return EXIT_FAILURE;
		}
	}
	catch (std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
		displayUsage(argv[0]);
		return EXIT_FAILURE;
	}
	return (EXIT_SUCCESS);
}
