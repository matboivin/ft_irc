/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_replies.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 17:01:20 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/30 18:33:05 by mboivin          ###   ########.fr       */
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
			);// need host (ip only if couldn't resolve)
	}

	// numeric replies
	void	rpl_welcome(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 001 Welcome to the Internet Relay Network "
			+ build_full_client_id(msg.getSender())
			);
		msg.appendSeparator();
	}

	void	rpl_umodeis(Message& msg, const std::string& user_mode)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 221 " + user_mode
			);
		msg.appendSeparator();
	}

	void	rpl_whoisuser(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 311 " + msg.getSender().getNick() + " " + "getUsername()" + " "
			+ msg.getSender().getIpAddressStr() + " :" + msg.getSender().getRealName()
			);
		msg.appendSeparator();
	}

	void	rpl_whoisoperator(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 313 " + msg.getSender().getNick() + " :is an IRC operator"
			);
		msg.appendSeparator();
	}

	void	rpl_endofwho(Message& msg, const std::string& name)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(build_prefix(msg.getServHostname()) + " 315 " + name + " :End of WHO list");
		msg.appendSeparator();
	}

	void	rpl_endofwhois(Message& msg, const std::string& nick)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(build_prefix(msg.getServHostname()) + " 318 " + nick + " :End of WHOIS list");
		msg.appendSeparator();
	}

	void	rpl_whoischannels(Message& msg, const std::string& nick, const std::string& chan_name)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(build_prefix(
			msg.getServHostname())
			+ " 319 " + nick + " :*" + chan_name // todo
			);
		msg.appendSeparator();
	}

	void	rpl_list(Message& msg, const std::string& chan_name, const std::string& chan_topic)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 322 " + chan_name + " :" + chan_topic
			);
		msg.appendSeparator();
	}

	void	rpl_listend(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(build_prefix(msg.getServHostname()) + " 323 :End of LIST");
		msg.appendSeparator();
	}

	void	rpl_notopic(Message& msg, const std::string& chan_name)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 331 " + chan_name + " :No topic is set"
			);
		msg.appendSeparator();
	}

	void	rpl_topic(Message& msg, const std::string& chan_name, const std::string& chan_topic)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 332 " + chan_name + " :" + chan_topic
			);
		msg.appendSeparator();
	}

	void	rpl_youreoper(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 381 :You are now an IRC operator"
			);
		msg.appendSeparator();
	}

	void	err_nosuchnick(Message& msg, const std::string& nick)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 401 " + nick + " :No such nick/channel"
			);
		msg.appendSeparator();
	}

	void	err_nosuchchannel(Message& msg, const std::string& chan_name)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 403 " + chan_name + " :No such channel"
			);
		msg.appendSeparator();
	}

	// error replies
	void	err_unknowncommand(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 421 " + msg.getCommand() + " :Unknown command"
			);
		msg.appendSeparator();
	}

	void	err_nonicknamegiven(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(build_prefix(msg.getServHostname()) + " 431 :No nickname given");
		msg.appendSeparator();
	}

	void	err_erroneusnickname(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 432 " + msg.getSender().getNick() + " :Erroneous nickname"
			);
		msg.appendSeparator();
	}

	void	err_nicknameinuse(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 433 " + msg.getSender().getNick() + " :Nickname is already in use"
			);
		msg.appendSeparator();
	}

	void	err_nickcollision(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 436 " + msg.getSender().getNick()
			+ " :Nickname collision KILL from " + "usertmp"
			+ "@" + msg.getSender().getIpAddressStr()
			);
		msg.appendSeparator();
	}

	void	err_unavailresource(Message& msg, const std::string& target)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 437 " + target + " :Nick/channel is temporarily unavailable"
			);
		msg.appendSeparator();
	}

	void	err_notonchannel(Message& msg, const std::string& chan_name)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 442 " + chan_name + " :You're not on that channel");
		msg.appendSeparator();
	}

	void	err_notregistered(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(build_prefix(msg.getServHostname()) + " 451 :You have not registered");
		msg.appendSeparator();
	}

	void	err_needmoreparams(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 461 " + msg.getCommand() + ":Not enough parameters"
			);
		msg.appendSeparator();
	}

	void	err_alreadyregistered(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 462 :Unauthorized command (already registered)"
			);
		msg.appendSeparator();
	}

	void	err_passwdmismatch(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(build_prefix(msg.getServHostname()) + " 464 :Password incorrect");
		msg.appendSeparator();
	}

	void	err_restricted(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(build_prefix(msg.getServHostname()) + " 484 :Your connection is restricted!");
		msg.appendSeparator();
	}

	void	err_nooperhost(Message& msg)
	{
		msg.setType(reply_to_cli);
		msg.setResponse(build_prefix(msg.getServHostname()) + " 491 :No O-lines for your host");
		msg.appendSeparator();
	}
} // !namespace ft_irc