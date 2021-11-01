/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 18:42:14 by mboivin           #+#    #+#             */
/*   Updated: 2021/11/01 18:33:35 by mboivin          ###   ########.fr       */
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

	class Channel
	{
		public:
			Channel();
			Channel(const std::string& name);
			Channel(const Channel& other);
			Channel&	operator=(const Channel& other);
			~Channel();

			/* Getters and setters */
			std::string						getName() const;
			std::string						getTopic() const;
			std::string						getMode() const;
			const std::list<Client*>&		getClients() const;

			void							setName(const std::string& name);
			void							setTopic(const std::string& topic);
			void							setMode(const std::string& mode);
			void							setClients(const std::list<Client*>& clients);

			/* Helpers */
			bool							isEmpty() const;

			/* Manage clients in channel */
			std::list<Client*>::iterator	findClient(Client& client);
			bool							hasClient(Client& client);
			void							addClient(Client& client);
			void							removeClient(Client& client);

			/* Manage topic */
			void	changeTopic(const std::string& topic, Message& msg);

			// debug
			void	displayClients();

		private:
			std::string						_name;
			std::string						_topic;
			std::string						_mode;
			std::list<Client*>				_clients;
	};
}

#endif