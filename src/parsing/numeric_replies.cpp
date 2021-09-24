/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_replies.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 17:01:20 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/24 19:04:00 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <string>
# include "ft_irc.hpp"
# include "numeric_replies.hpp"
# include "client.hpp"
# include "Message.hpp"

namespace ft_irc
{
	class IRCClient;

	// helpers
	static std::string	build_prefix(const std::string& from)
	{
		return (":" + from);
	}

	static std::string	build_full_client_id(const IRCClient& client)
	{
		return (
			client.getNick() + "!" + "usertmp" // need username
			+ "@" + client.getIpAddressStr()
			); // need host (ip only if couldn't resolve)
	}

	// numeric replies
	void	rpl_welcome(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 001 Welcome to the Internet Relay Network "
			+ build_full_client_id(msg.getSender())
			); 
	}

	void	rpl_youreoper(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 381 :You are now an IRC operator"
			); 
	}

	// error replies
	void	err_unknowncommand(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 421 " + msg.getCommand() + " :Unknown command"
			);
	}

	void	err_nonicknamegiven(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 431 :No nickname given"
			);
	}

	void	err_erroneusnickname(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 432 " + msg.getSender().getNick() + " :Erroneous nickname"
			);
	}

	void	err_nicknameinuse(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 433 " + msg.getSender().getNick() + " :Nickname is already in use"
			);
	}

	void	err_nickcollision(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 436 " + msg.getSender().getNick()
			+ " :Nickname collision KILL from " + "usertmp"
			+ "@" + msg.getSender().getIpAddressStr()
			);
	}

	void	err_unavailresource(const std::string& target)
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 437 " + target + " :Nick/channel is temporarily unavailable"
			);
	}

	void	err_notregistered(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 451 :You have not registered"
			);
	}

	void	err_needmoreparams(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 461 " + msg.getCommand() + ":Not enough parameters"
			);
	}

	void	err_alreadyregistered(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 462 :Unauthorized command (already registered)"
			);
	}

	void	err_restricted(Message& msg);
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " :Your connection is restricted!");
	}
}