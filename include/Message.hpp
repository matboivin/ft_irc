/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 18:47:11 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/04 10:51:29 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include <vector>
# include "ft_irc.hpp"

# define MSG_MAX_PARAMS 15

namespace ft_irc
{
	class IRCClient;

	// Parser for IRC protocol messages
	class Message
	{
	private:
		// attributes
		IRCClient&					_sender;
		std::string					_serv_hostname;
		std::string					_response;
		std::string					_command;
		std::vector<std::string>	_params;
		std::vector<IRCClient>		_recipients;

	public:
		// default constructor
					Message(IRCClient& sender);
		// server hostname constructor
					Message(IRCClient& sender, const std::string& serv_hostname);
		// copy constructor
					Message(const Message& other);
		// assignment operator
		Message&	operator=(const Message& other);
		// destructor
					~Message();

		// getters
		IRCClient&	getSender() const;
		std::string	getServHostname() const;
		std::string	getResponse() const;
		std::string	getCommand() const;
		std::string	getParam(int idx) const;
		std::vector<std::string>	getParams() const;
		std::vector<IRCClient>		getRecipients() const;

		// setters
		void		setSender(IRCClient& sender);
		void		setServHostname(const std::string& serv_hostname);
		void		setResponse(const std::string& response);
		void		setCommand(const std::string& command);
		void		setParam(const std::string& param);
		void		setRecipient(const IRCClient& recipient);
		void		setRecipients(const std::vector<IRCClient>& recipients);

		// end message with CRLF
		void		appendSeparator();

		// debug
		void		displayMessage() const;
	};
} // !namespace ft_irc

#endif // !Message