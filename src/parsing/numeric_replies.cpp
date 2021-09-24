/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_replies.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 17:01:20 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/24 17:37:00 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <string>
# include "ft_irc.hpp"
# include "numeric_replies.hpp"
# include "Message.hpp"

namespace ft_irc
{
	// numeric replies
	void	rpl_welcome(Message& msg)
	{
		msg.setOutput(
			":" + msg.getServHostname()
			+ " 001 Welcome to the Internet Relay Network "
			+ msg.getSender().getNick() + "!" + "usertmp" // need username
			+ "@" + msg.getSender().getIpAddressStr()); // need host (ip only if couldn't resolve)
	}

	// error replies
	void	err_unknowncommand(Message& msg)
	{
		msg.setOutput(
			":" + msg.getServHostname()
			+ " 421 " + msg.getCommand() + " :Unknown command");
	}
}