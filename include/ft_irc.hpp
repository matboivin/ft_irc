/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 13:07:04 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/20 16:51:49 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP
# include <cstring>
# include <cstdlib>
# include <iostream>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <fcntl.h>
# include <unistd.h>

void	displayUsage( const char* program_name );

#ifndef nullptr
# define nullptr NULL
#endif // !nullpt

namespace ft_irc
{
	class IRCClient
	{
	private:
		std::string			nick;
		std::string			user_agent;
		std::string			joined_channels;
		std::string			password;
		struct sockaddr_in	address;			//IPv4 address		
		socklen_t 			address_size;		//IPv4 address size
		std::string			address_str;		//IPv4 address as string
	public:
		IRCClient(struct sockaddr_in address=(struct sockaddr_in){0,0,{0},{0}},
		std::string nick="", std::string user_agent="", std::string password="")
		{
			this->nick = nick;
			this->user_agent = user_agent;
			this->password = password;
			this->address = address;
			this->address_str = inet_ntoa(address.sin_addr);
			this->address_size = sizeof(address);
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
			return this->address_str;
		}
		struct sockaddr_in &getAddress()
		{
			return this->address;
		}
		//getAdressSize
		socklen_t &getAddressSize()
		{
			return this->address_size;
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
		//copy constructor
		IRCClient(const IRCClient &other)
		{
			this->nick = other.nick;
			this->user_agent = other.user_agent;
			this->joined_channels = other.joined_channels;
			this->password = other.password;
			this->address_str = other.address_str;
			this->address = other.address;
			this->address_size = other.address_size;
		}
		//assignment operator
		IRCClient &operator=(const IRCClient &other)
		{
			this->nick = other.nick;
			this->user_agent = other.user_agent;
			this->joined_channels = other.joined_channels;
			this->password = other.password;
			this->address_str = other.address_str;
			this->address = other.address;
			this->address_size = other.address_size;
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
		IRCServer(std::string bind_address="0.0.0.0", std::string port="6697",
			std::string password="", int backlog_max=5)
		{
			this->bind_address = bind_address;
			this->port = port;
			this->password = password;
			this->sockfd = -1;
			this->backlog_max = backlog_max;
			//create a new address struct
			address.sin_family = AF_INET;
			address.sin_port = htons(atoi(this->port.c_str()));
			address.sin_addr.s_addr = inet_addr(this->bind_address.c_str());
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
	private:
		//Function to create a socket.
		//create a new listening tcp s	ocket and bind it to the given address and port
		//https://www.geeksforgeeks.org/socket-programming-cc/

		//create a new socket
		//AF_INET: IPv4
		//SOCK_STREAM: TCP
		//IPPROTO_TCP: TCP protocol
		bool createSocket()
		{
			//Create a socket.
			this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if (this->sockfd < 0)
			{
				std::cerr << "Error: Could not create socket." << std::endl;
				return false;
			}
			//Set socket options.
			int optval = 1;
			if (setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
			{
				std::cerr << "Error: Could not set socket options." << std::endl;
				return false;
			}
			//Bind the socket to the address.
			if (bind(this->sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
			{
				std::cerr << "Error: Could not bind socket." << std::endl;
				return false;
			}
			//Listen for connections.
			if (listen(this->sockfd, this->backlog_max) < 0)
			{
				std::cerr << "Error: Could not listen on socket." << std::endl;
				return false;
			}
			return true;
		}

	public:	
		int run()
		{
			if (createSocket() == false)
			{
				return (-1);
			}
			//accept incoming connections
			while (true)
			{
				IRCClient client;
				//accept a new connection
				int new_sockfd = accept(sockfd, (struct sockaddr *)&(client.getAddress()), &(client.getAddressSize()));
				if (new_sockfd < 0)
				{
					throw std::runtime_error("accept() failed");
				}				
				//log the clients IP address
				std::cout << "Client " << client.getIpAddressStr() << " connected" << std::endl;

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
				std::cout << "Sending: " << response << "to " << inet_ntoa(address.sin_addr) << std::endl;
				if (send(new_sockfd, response.c_str(), response.size(), 0) < 0)
				{
					throw std::runtime_error("send() failed");
				}
				//log the closing of the connection
				std::cout << "Closing connection to " << inet_ntoa(address.sin_addr) << std::endl
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

#endif
