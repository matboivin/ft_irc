/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 18:42:14 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/04 16:39:25 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <list>

namespace ft_irc
{
	class IRCClient;

	// IRC channel
	class Channel
	{
	private:
		std::string				_name;
		std::string				_topic;
		std::string				_mode;
		std::list<IRCClient>	_clients;

	public:
		// default constructor
					Channel();
		// name constructor
					Channel(const std::string& name);
		// copy constructor
					Channel(const Channel& other);
		// assignment operator
		Channel		&operator=(const Channel& other);
		// destructor
					~Channel();

		// getters
		std::string	getName() const;
		std::string	getTopic() const;
		std::string	getMode() const;
		std::list<IRCClient>	getClients() const;
		// setters
		void		setName(const std::string& name);
		void		setTopic(const std::string& topic);
		void		setMode(const std::string& mode);
		void		setClients(const std::list<IRCClient>& clients);

		// manage clients in channel
		std::list<IRCClient>::iterator	findClient(IRCClient& client);
		bool		hasClient(IRCClient& client);
		void		addClient(IRCClient& client);
		void		removeClient(IRCClient& client);

		// debug
		void		displayClients();
	};
}

#endif // !CHANNEL_HPP