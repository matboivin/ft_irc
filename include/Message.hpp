/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 18:47:11 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/24 15:51:47 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include <vector>
# include "ft_irc.hpp"
# include "IRCParser.hpp"
# include "client.hpp"

# define MSG_MAX_PARAMS 15

namespace ft_irc
{
	class IRCClient;

	enum reply_type { undefined = 0, serv_to_cli, cli_to_cli, broadcast };
	// broadcast is tmp

	// Parser for IRC protocol messages
	class Message
	{
	private:

		// private alias
		typedef std::vector<std::string>	str_vec;

		// attributes
		IRCClient	_sender;
		std::string	_prefix;
		std::string	_command;
		std::string	_content;
		str_vec		_params;
		reply_type	_type;

	public:

		// default constructor
					Message();
		// copy constructor
					Message(const Message& other);
		// assignment operator
		Message		&operator=(const Message& other);
		// destructor
					~Message();

		// Message getters
		IRCClient	getSender() const;
		std::string	getPrefix() const;
		std::string	getCommand() const;
		std::string	getContent() const;
		str_vec		getParams() const;
		reply_type	getType() const;

		// Message setters
		void		setSender(const IRCClient& sender);
		void		setPrefix(const std::string& prefix);
		void		setCommand(const std::string& command);
		void		setContent(const std::string& content);
		void		setParam(const std::string& param);
		void		setType(reply_type type);

		// end message with CRLF
		void		appendSeparator();

		// debug
		void		displayMessage() const;
	};
}

#endif // !Message