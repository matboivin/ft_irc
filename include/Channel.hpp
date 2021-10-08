/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 18:42:14 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/08 15:24:19 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <list>
# include <string>
# include "Client.hpp"

namespace ft_irc
{
	class Message;

	// IRC channel
	class Channel
	{
	private:
		// attributes
		std::string			_name;
		std::string			_topic;
		std::string			_mode;
		std::list<Client*>	_clients;

	public:
		// default constructor
					Channel();
		// name constructor
					Channel(const std::string& name);
		// copy constructor
					Channel(const Channel& other);
		// assignment operator
		Channel&	operator=(const Channel& other);
		// destructor
					~Channel();

		// getters
		std::string			getName() const;
		std::string			getTopic() const;
		std::string			getMode() const;
		const std::list<Client*>&	getClients() const;

		// setters
		void	setName(const std::string& name);
		void	setTopic(const std::string& topic);
		void	setMode(const std::string& mode);
		void	setClients(const std::list<Client*>& clients);

		// helpers
		bool	isEmpty() const;

		// manage clients in channel
		std::list<Client*>::iterator	findClient(Client& client);
		bool	hasClient(Client& client);
		void	addClient(Client& client);
		void	removeClient(Client& client);

		// send a message to all clients in channel
		void	broadcastMessage(const Message& msg);

		// debug
		void	displayClients();
	};
} // namespace ft_irc

#endif // !CHANNEL_HPP