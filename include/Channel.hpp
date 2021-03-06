/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 18:42:14 by mboivin           #+#    #+#             */
/*   Updated: 2021/12/14 20:43:00 by mboivin          ###   ########.fr       */
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

	/*
	 * IRC Channel
	 * Attribute of the Server
	 */
	class Channel
	{
	public:
		/* Aliases */
		typedef std::list<Client*>	t_clients;

		/* Default constructor */
							Channel();

		/* Name constructor */
							Channel(const std::string& name);

		/* Copy constructor */
							Channel(const Channel& other);

		/* Copy assignment operator */
		Channel&			operator=(const Channel& other);

		/* Destructor */
							~Channel();

		/* Getters */
		std::string			getName() const;
		std::string			getTopic() const;
		std::string			getMode() const;
		const t_clients&	getClients() const;
		const t_clients&	getChanOps() const;

		/* Setters */
		void				setName(const std::string& name);
		void				setTopic(const std::string& topic);
		void				setMode(const std::string& mode);
		void				setClients(const t_clients& clients);
		void				setChanOps(const t_clients& chan_ops);

		/* Helpers */
		bool				isEmpty() const;

		/* Manage clients in channel */
		t_clients::iterator	findClient(Client& client);
		bool				hasClient(Client& client);
		void				addClient(Client& client);
		void				removeClient(Client& client);

		/* Manage chan ops */
		t_clients::iterator	findChanOp(const Client& chan_op);
		bool				hasChanOp(const Client& chan_op);
		void				addChanOp(Client& chan_op);
		void				removeChanOp(Client& chan_op);

		/* Manage topic */
		void				changeTopic(const std::string& topic, Message& msg);

		/* Mode operations */
		bool				hasMode(const char& mode_char) const;
		int					addMode(const char& mode_char);
		int					removeMode(const char& mode_char);

		/* debug */
		void				displayClients();
		void				displayChanOps();

	private:
		/* Attributes */
		std::string			_name;
		std::string			_topic;
		std::string			_mode;
		t_clients			_clients;
		t_clients			_chan_ops;
	};
}

#endif // !CHANNEL_HPP
