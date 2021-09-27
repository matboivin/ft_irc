/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_replies.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 17:01:20 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/27 16:45:58 by mboivin          ###   ########.fr       */
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

	void	err_nosuchnick(Message& msg, const std::string& nick)
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 401 " + nick + " :No such nick/channel"
			); 
	}

	void	err_nosuchchannel(Message& msg, const std::string& channel)
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 403 " + channel + " :No such channel"
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

	void	err_unavailresource(Message& msg, const std::string& target)
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 437 " + target + " :Nick/channel is temporarily unavailable"
			);
	}

	void	err_notonchannel(Message& msg, const std::string& channel)
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 442 " + channel + " :You're not on that channel"
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

	void	err_passwdmismatch(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 464 :Password incorrect"
			);
	}

	void	err_badchannelkey(Message& msg, const std::string& channel)
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 475 " + channel + " :Cannot join channel (+k)"
			);
	}

	void	err_restricted(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 484 :Your connection is restricted!");
	}

	void	err_nooperhost(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 491 :No O-lines for your host");
	}
} // !namespace ft_irc