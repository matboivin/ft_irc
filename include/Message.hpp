/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 18:47:11 by mboivin           #+#    #+#             */
/*   Updated: 2021/11/09 16:31:16 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <list>
# include <string>
# include <vector>

# define MSG_MAX_PARAMS 15

namespace ft_irc
{
	class Client;

	/*
	 * Parser for IRC protocol messages
	 */
	class Message
	{
	public:
		/* Aliases */
		typedef std::vector<std::string>	t_params;
		typedef std::list<Client*>			t_clients;

		/* Default constructor */
							Message(Client& sender);

		/* Server name constructor */
							Message(Client& sender, const std::string& serv_hostname);

		/* Copy constructor */
							Message(const Message& other);

		/* Copy assignment operator */
		Message&			operator=(const Message& other);

		/* Destructor */
							~Message();

		/* Getters */
		Client&				getSender() const;
		std::string			getServHostname() const;
		std::string			getResponse() const;
		std::string			getCommand() const;
		const t_params&		getParams() const;
		const t_clients&	getRecipients() const;
		/* Setters */
		void				setSender(const Client& sender);
		void				setServHostname(const std::string& serv_hostname);
		void				setResponse(const std::string& response);
		void				appendResponse(const std::string& response);
		void				setCommand(const std::string& command);
		void				setParam(const std::string& param);
		void				setRecipients(const t_clients& recipients);
		void				setRecipient(Client& recipient);

		/* Helpers */
		void				appendSeparator();
		void				clearParams();
		void				clearResponse();

		/* Clients operations */
		void				clearRecipients();
		void				addRecipients(const t_clients& recipients);

		/* debug */
		void				displayMessage() const;

	private:
		Client&				_sender;
		std::string			_serv_hostname;
		std::string			_response;
		std::string			_command;
		t_params			_params;
		t_clients			_recipients;
	};
} // !namespace ft_irc

#endif // !Message