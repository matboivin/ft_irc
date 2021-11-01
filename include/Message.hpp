/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 18:47:11 by mboivin           #+#    #+#             */
/*   Updated: 2021/11/01 18:50:54 by mboivin          ###   ########.fr       */
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
	private:
		Client&						_sender;
		std::string					_serv_hostname;
		std::string					_response;
		std::string					_command;
		std::vector<std::string>	_params;
		std::list<Client*>			_recipients;

	public:
		Message(Client& sender);
		Message(Client& sender, const std::string& serv_hostname);
		Message(const Message& other);
		Message&	operator=(const Message& other);
		~Message();

		/* Getters and setters */
		Client&							getSender() const;
		std::string						getServHostname() const;
		std::string						getResponse() const;
		std::string						getCommand() const;
		const std::vector<std::string>&	getParams() const;
		const std::list<Client*>&		getRecipients() const;

		void	setSender(const Client& sender);
		void	setServHostname(const std::string& serv_hostname);
		void	setResponse(const std::string& response);
		void	appendResponse(const std::string& response);
		void	setCommand(const std::string& command);
		void	setParam(const std::string& param);
		void	setRecipients(const std::list<Client*>& recipients);
		void	setRecipient(Client& recipient);

		/* Helpers */
		void	appendSeparator();

		/* Clients operations */
		void	clearRecipients();
		void	addRecipients(const std::list<Client*>& recipients);
		void	setRecipientsFromChannels(const Client& client);

		// debug
		void	displayMessage() const;
	};
} // !namespace ft_irc

#endif // !Message