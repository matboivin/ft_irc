/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 18:47:11 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/24 17:14:09 by mboivin          ###   ########.fr       */
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

	enum reply_type { undefined = 0, reply_to_cli, forward, broadcast };
	// broadcast is tmp

	// Parser for IRC protocol messages
	class Message
	{
	private:

		// private alias
		typedef std::vector<std::string>	str_vec;

		// attributes
		IRCClient	_sender;
		reply_type	_type;
		std::string	_output;
		std::string	_command;
		str_vec		_params;

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
		reply_type	getType() const;
		std::string	getOutput() const;
		std::string	getCommand() const;
		str_vec		getParams() const;

		// Message setters
		void		setSender(const IRCClient& sender);
		void		setType(reply_type type);
		void		setOutput(const std::string& output);
		void		setCommand(const std::string& command);
		void		setParam(const std::string& param);

		// end message with CRLF
		void		appendSeparator();

		// debug
		void		displayMessage() const;
		void		displayReply() const;
	};
}

#endif // !Message