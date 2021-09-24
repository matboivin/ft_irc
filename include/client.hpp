/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 16:55:22 by root              #+#    #+#             */
/*   Updated: 2021/09/24 14:38:27 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_CLIENT_HPP
# define IRC_CLIENT_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <fcntl.h>
# include <unistd.h>
# include <string>
# include <sys/time.h>
# include <stdexcept>


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
		struct timeval		timeout;			//timeout for select()
		int					socket_fd;			//socket file descriptor
		bool				is_connected;		//is the client connected to the server?
	public:
		IRCClient(struct sockaddr_in address=(struct sockaddr_in){0,0,{0},{0}},
		std::string nick="", std::string user_agent="", std::string password="");
		IRCClient(const IRCClient &other);
		IRCClient &operator=(const IRCClient &other);
		~IRCClient();
		//IRCClient ge/tters
		std::string			getNick() const;
		std::string			getUserAgent() const;
		std::string			getJoinedChannels() const;
		std::string			getPassword() const;
		std::string			getIpAddressStr() const;
		int 				getSocketFd() const;
		struct sockaddr_in	&getAddress();
		socklen_t 			&getAddressSize();
		//IRCClient se/tters
		void				setNick(std::string nick);
		void				setUserAgent(std::string user_agent);
		void				setJoinedChannels(std::string joined_channels);
		void				setPassword(std::string password);
		bool				isRegistered() const;
		bool				isConnected() const;
		bool				awaitConnection();
	};
}

#endif // !IRC_CLIENT
