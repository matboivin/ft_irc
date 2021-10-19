/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 18:47:11 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/19 17:12:41 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <list>
# include <string>

# define MSG_MAX_PARAMS 15

namespace ft_irc
{
	class Client;

	// Parser for IRC protocol messages
	class Message
	{
	private:
		// attributes
		Client&					_sender;
		std::string				_serv_hostname;
		std::string				_response;
		std::string				_command;
		std::list<std::string>	_params;
		std::list<Client*>		_recipients;

	public:
		// default constructor
					Message(Client& sender);
		// server hostname constructor
					Message(Client& sender, const std::string& serv_hostname);
		// copy constructor
					Message(const Message& other);
		// assignment operator
		Message&	operator=(const Message& other);
		// destructor
					~Message();

		// getters
		Client&							getSender() const;
		std::string						getServHostname() const;
		std::string						getResponse() const;
		std::string						getCommand() const;
		std::string						getParam(int idx) const;
		const std::list<std::string>&	getParams() const;
		const std::list<Client*>&		getRecipients() const;

		// setters
		void	setSender(const Client& sender);
		void	setServHostname(const std::string& serv_hostname);
		void	setResponse(const std::string& response);
		void	setCommand(const std::string& command);
		void	setParam(const std::string& param);
		void	setRecipients(const std::list<Client*>& recipients);
		void	setRecipient(Client& recipient);

		void	addRecipients(const std::list<Client*>& recipients);
		void	setRecipientsFromChannels(const Client& client);

		// end message with CRLF
		void	appendSeparator();

		// debug
		void	displayMessage() const;
	};
} // !namespace ft_irc

#endif // !Message