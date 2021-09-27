/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:39:18 by root              #+#    #+#             */
/*   Updated: 2021/09/27 18:05:59 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <string>

namespace ft_irc
{
	IRCServer::IRCServer(std::string bind_address, std::string port,
		std::string password, int backlog_max)
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
	std::string IRCServer::getBindAddress() const
	{
		return (this->bind_address);
	}
	std::string IRCServer::getPort() const
	{
		return (this->port);
	}
	std::string IRCServer::getPassword() const
	{
		return (this->password);
	}

	//IRCServer setters
	void IRCServer::setBindAddress(std::string bind_address)
	{
		this->bind_address = bind_address;
	}
	void IRCServer::setPort(std::string port)
	{
		this->port = port;
	}
	void IRCServer::setPassword(std::string password)
	{
		this->password = password;
	}

	//copy constructor
	IRCServer::IRCServer(const IRCServer &other)
	{
		this->bind_address = other.bind_address;
		this->port = other.port;
		this->password = other.password;
		this->sockfd = other.sockfd;
		this->backlog_max = other.backlog_max;
		this->address = other.address;
	}
	//assignment operator
	IRCServer &IRCServer::operator=(const IRCServer &other)
	{
		this->bind_address = other.bind_address;
		this->port = other.port;
		this->password = other.password;
		this->sockfd = other.sockfd;
		this->backlog_max = other.backlog_max;
		return (*this);
	}
	//destructor
	IRCServer::~IRCServer()
	{
	}
	//Function to create a socket.
	//create a new listening tcp s	ocket and bind it to the given address and port
	//https://www.geeksforgeeks.org/socket-programming-cc/

	//create a new socket
	//AF_INET: IPv4
	//SOCK_STREAM: TCP
	//IPPROTO_TCP: TCP protocol
	bool IRCServer::createSocket()
	{
		//Create a socket.
		this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (this->sockfd < 0)
		{
			std::cerr << "Error: Could not create socket." << std::endl;
			return (false);
		}
		//Set socket options.
		int optval = 1;
		if (setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		{
			std::cerr << "Error: Could not set socket options." << std::endl;
			return (false);
		}
		//Bind the socket to the address.
		if (bind(this->sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
		{
			std::cerr << "Error: Could not bind socket." << std::endl;
			return (false);
		}
		//Listen for connections.
		if (listen(this->sockfd, this->backlog_max) < 0)
		{
			std::cerr << "Error: Could not listen on socket." << std::endl;
			return (false);
		}
		return (true);
	}
	bool				IRCServer::hasPendingConnections()
	{
		//Check if there are pending connections. (poll)
		struct pollfd		poll_fd = {this->sockfd, POLLIN, 0};
		int					poll_result = poll(&poll_fd, 1, 0);
		if (poll_result < 0)
		{
			std::cerr << "Error: Could not poll socket." << std::endl;
			return (false);
		}
		if (poll_result == 0)
			return (false);
		return (true);
	}

	int IRCServer::run()
	{
		if (createSocket() == false)
		{
			return (-1);
		}
		//accept incoming connections
		while (true)
		{
			if (hasPendingConnections() == true)
				awaitNewConnection();
			processClients();
		}
	}
	int IRCServer::sockGetLine(int sockfd, std::string &line)
	{
		//read char by char
		char c;
		line = "";
		while (true)
		{
			if (recv(sockfd, &c, 1, 0) < 0)
			{
				return (false);
			}
			if (c == '\n')
			{
				break;
			}
			line += c;
		}
		return (true);
	}

	int IRCServer::sockGetLine(int sockfd, std::string &line, std::size_t max_bytes)
	{
		//read char by char
		char c;
		line = "";
		while (true)
		{
			if (recv(sockfd, &c, 1, 0) < 0)
			{
				return (false);
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
		return (true);
	}
	//awaitNewConnection
	//accepts a new connection
	bool				IRCServer::awaitNewConnection()
	{
		IRCClient new_client;

		//accept a new connection
		
		new_client.awaitConnection(this->sockfd);
		if (new_client.getSocketFd() < 0)
		{
			throw std::runtime_error("accept() failed");
		}
		//log the clients IP address
		std::cout << "Client " << new_client.getIpAddressStr()
		<< " connected" << std::endl;
		this->clients.push_back(new_client);
		return (true);
	}
	
	bool				IRCServer::processClients()
	{
		//process all clients
		for (std::list<IRCClient>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
		{
			if (it->getSocketFd() < 0 || it->hasNewEvents() == false)
			{
				continue;
			}
			//read the client's nick	
			std::string input;
			if (!sockGetLine(it->getSocketFd(), input))
			{
				throw std::runtime_error("sockGetLine() failed");
			}
			it->setNick(input);
			std::cout << "Nick: " << input << std::endl;

			//read the client's user agent
			if (!sockGetLine(it->getSocketFd(), input))
			{
				throw std::runtime_error("sockGetLine() failed");
			}
			it->setUserAgent(input);
			std::cout << "User-Agent: " << input << std::endl;

			//read the client's password
			if (!sockGetLine(it->getSocketFd(), input))
			{
				throw std::runtime_error("sockGetLine() failed");
			}
			it->setPassword(input);
			std::cout << "Password: " << input << std::endl;

			//send the client's nick and user agent back to him (for logging purposes)
			std::string response = ":";
			response += it->getNick();
			response += "!";
			response += it->getUserAgent();
			response += " ";
			response += it->getPassword();
			response += "\r\n";
			//log the response
			std::cout << "Sending: " << response << "to " << it->getIpAddressStr() << std::endl;
			if (send(it->getSocketFd(), response.c_str(), response.size(), 0) < 0)
			{
				throw std::runtime_error("send() failed");
			}
/*			//log the closing of the connection
			std::cout << "Closing connection to " << it->getIpAddressStr() << std::endl
			<< "---------------------------------------------------------" << std::endl;
			//send EOF to the client
			//close the connection
			close(it->getSocketFd());
			//delete the client
			it = this->clients.erase(it);
*/
		}
		return (true);
	}
}
