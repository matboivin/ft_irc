/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_replies.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 17:01:20 by mboivin           #+#    #+#             */
/*   Updated: 2021/11/06 16:37:52 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "numeric_replies.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "server_operations.hpp"

namespace ft_irc
{
	// command responses
	void	rpl_welcome(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 001 Welcome to the Internet Relay Network ");
		msg.appendResponse(build_full_client_id(msg.getSender()));
		msg.appendSeparator();
	}

	void	rpl_yourhost(Message& msg, const std::string& version)
	{
		msg.setRecipient(msg.getSender());
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 002 ");
		msg.appendResponse(msg.getSender().getNick());
		msg.appendResponse(" :Your host is ");
		msg.appendResponse(", running version ");
		msg.appendResponse(version);
		msg.appendSeparator();
	}

	void	rpl_umodeis(Message& msg, const std::string& user_mode)
	{
		msg.setRecipient(msg.getSender());
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 221 ");
		msg.appendResponse(user_mode);
		msg.appendSeparator();
	}

	void	rpl_whoisuser(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 311 ");
		msg.appendResponse(msg.getSender().getNick());
		msg.appendResponse(" ");
		msg.appendResponse(msg.getSender().getUsername());
		msg.appendResponse(" ");
		msg.appendResponse(msg.getSender().getIpAddressStr());
		msg.appendResponse(" :");
		msg.appendResponse(msg.getSender().getRealName());
		msg.appendSeparator();
	}

	void	rpl_whoisserver(Message& msg, const std::string& description)
	{
		msg.setRecipient(msg.getSender());
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 312 ");
		msg.appendResponse(msg.getSender().getNick());
		msg.appendResponse(" ");
		msg.appendResponse(msg.getServHostname());
		msg.appendResponse(" :");
		msg.appendResponse(description);
		msg.appendSeparator();
	}

	void	rpl_whoisoperator(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 313 ");
		msg.appendResponse(msg.getSender().getNick());
		msg.appendResponse(" :is an IRC operator");
		msg.appendSeparator();
	}

	void	rpl_endofwho(Message& msg, const std::string& name)
	{
		msg.setRecipient(msg.getSender());
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 315 ");
		msg.appendResponse(name);
		msg.appendResponse(" :End of WHO list");
		msg.appendSeparator();
	}

	void	rpl_endofwhois(Message& msg, const std::string& nick)
	{
		msg.setRecipient(msg.getSender());
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 318 ");
		msg.appendResponse(nick);
		msg.appendResponse(" :End of WHOIS list");
		msg.appendSeparator();
	}

	void	rpl_whoischannels(Message& msg, const std::string& nick, const std::string& chan_name)
	{
		msg.setRecipient(msg.getSender());
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 319 ");
		msg.appendResponse(nick);
		msg.appendResponse(" :");
		msg.appendResponse(chan_name);
		msg.appendSeparator();
	}

	void	rpl_list(Message& msg, Channel& channel)
	{
		msg.setRecipient(msg.getSender());
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 322 ");
		msg.appendResponse(channel.getName());
		msg.appendResponse(" :");
		msg.appendResponse(channel.getTopic());
		msg.appendSeparator();
	}

	void	rpl_listend(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 323 :End of LIST");
		msg.appendSeparator();
	}

	void	rpl_channelmodeis(Message& msg, Channel& channel)
	{
		msg.setRecipient(msg.getSender());
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 324 ");
		msg.appendResponse(channel.getName());
		msg.appendResponse(" ");
		msg.appendResponse(channel.getMode());
		msg.appendSeparator();
	}

	void	rpl_notopic(Message& msg, const std::string& chan_name)
	{
		msg.setRecipient(msg.getSender());
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 331 ");
		msg.appendResponse(chan_name);
		msg.appendResponse(" :No topic is set");
		msg.appendSeparator();
	}

	void	rpl_topic(Message& msg, Channel& channel)
	{
		msg.setRecipient(msg.getSender());
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 332 ");
		msg.appendResponse(channel.getName());
		msg.appendResponse(" :");
		msg.appendResponse(channel.getTopic());
		msg.appendSeparator();
	}

	void	rpl_inviting(Message& msg, const std::string& chan_name, const std::string& nick)
	{
		msg.setRecipient(msg.getSender());
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 341 ");
		msg.appendResponse(chan_name);
		msg.appendResponse(" ");
		msg.appendResponse(nick);
		msg.appendSeparator();
	}

	void	rpl_whoreply(Message& msg, const std::string& chan_name, Client& target)
	{
		msg.setRecipient(msg.getSender());
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 352 ");
		msg.appendResponse(chan_name);
		msg.appendResponse(" ");
		msg.appendResponse(target.getUsername());
		msg.appendResponse(" ");
		msg.appendResponse(target.getIpAddressStr());
		msg.appendResponse(" ");
		//msg.appendResponse();  todo server
		msg.appendResponse(" ");
		msg.appendResponse(target.getNick());
		msg.appendResponse(" :");
		msg.appendResponse(target.getRealName());
		msg.appendSeparator();
	}

	void	rpl_namreply(Message& msg, Channel& channel)
	{
		msg.setRecipient(msg.getSender());
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 353 ");
		msg.appendResponse(channel.getName());
		// todo
		msg.appendSeparator();
	}

	void	rpl_endofnames(Message& msg, const std::string& chan_name)
	{
		msg.setRecipient(msg.getSender());
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 366 ");
		msg.appendResponse(chan_name);
		msg.appendResponse(" :End of NAMES list");
		msg.appendSeparator();
	}

	void	rpl_youreoper(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 381 :You are now an IRC operator");
		msg.appendSeparator();
	}

	// error replies

	void	err_nosuchnick(Message& msg, const std::string& nick)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 401 " + nick + " :No such nick/channel"
			);
		msg.appendSeparator();
	}

	void	err_nosuchchannel(Message& msg, const std::string& chan_name)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 403 " + chan_name + " :No such channel"
			);
		msg.appendSeparator();
	}

	void	err_cannotsendtochan(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 404 " + msg.getParams().front() + " :Cannot send to channel"
			);
		msg.appendSeparator();
	}

	void	err_toomanychannels(Message& msg, const std::string& chan_name)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 405 " + chan_name + " :You have joined too many channels"
			);
		msg.appendSeparator();
	}

	void	err_toomanytargets(Message& msg, const std::string& target) // necessary?
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 407 " + target + " :<error code> recipients. <abort message>" // todo
			);
		msg.appendSeparator();
	}

	void	err_noorigin(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(build_prefix(msg.getServHostname()) + " 409 :No origin specified");
		msg.appendSeparator();
	}

	void	err_norecipient(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 411 :No recipient given (" + msg.getCommand() + ")"
			);
		msg.appendSeparator();
	}

	void	err_notexttosend(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(build_prefix(msg.getServHostname()) + " 412 :No text to send");
		msg.appendSeparator();
	}

	void	err_unknowncommand(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 421 " + msg.getCommand() + " :Unknown command"
			);
		msg.appendSeparator();
	}

	void	err_nonicknamegiven(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(build_prefix(msg.getServHostname()) + " 431 :No nickname given");
		msg.appendSeparator();
	}

	void	err_erroneusnickname(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 432 " + msg.getParams().front() + " :Erroneous nickname"
			);
		msg.appendSeparator();
	}

	void	err_nicknameinuse(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 433 * " + msg.getParams().front() + " :Nickname already in use"
			);
		msg.appendSeparator();
	}

	void	err_unavailresource(Message& msg, const std::string& target)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 437 " + target + " :Nick/channel is temporarily unavailable"
			);
		msg.appendSeparator();
	}

	void	err_usernotinchannel(Message& msg, const std::string& nick, const std::string& chan_name)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 441 " + nick + " " + chan_name + " :They aren't on that channel");
		msg.appendSeparator();
	}

	void	err_notonchannel(Message& msg, const std::string& chan_name)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 442 " + chan_name + " :You're not on that channel");
		msg.appendSeparator();
	}

	void	err_useronchannel(Message& msg, const std::string& nick, const std::string& chan_name)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 443 " + nick + " " + chan_name + " :is already on channel");
		msg.appendSeparator();
	}

	void	err_notregistered(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(build_prefix(msg.getServHostname()) + " 451 :You have not registered");
		msg.appendSeparator();
	}

	void	err_needmoreparams(Message& msg, const std::string& error_string)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 461 " + msg.getCommand() + " :" + error_string
			);
		msg.appendSeparator();
	}

	void	err_syntaxerror(Message& msg, std::string cmd)
	{
		Client& sender = msg.getSender();
		msg.setRecipient(sender);
		//:public-irc.w3.org 461 ezakjhzkjehkjzehdk WHO :Syntax error
		msg.setResponse(
			build_prefix(msg.getServHostname()) 
			+ " 461 " + sender.getNick() + " " + cmd + " :Syntax error"
			);
		msg.appendSeparator();
	}

	void	err_alreadyregistered(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 462 :Unauthorized command (already registered)"
			);
		msg.appendSeparator();
	}

	void	err_passwdmismatch(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(build_prefix(msg.getServHostname()) + " 464 :Password incorrect");
		msg.appendSeparator();
	}

	void	err_channelisfull(Message& msg, const std::string& chan_name)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 471 " + chan_name + " :Cannot join channel (+l)"
			);
		msg.appendSeparator();
	}

	void	err_bannedfromchan(Message& msg, const std::string& chan_name)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 474 " + chan_name + " :Cannot join channel (+b)"
			);
		msg.appendSeparator();
	}

	void	err_noprivileges(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(build_prefix(
			msg.getServHostname()) + " 481 :Permission Denied- You're not an IRC operator"
			);
		msg.appendSeparator();
	}

	void	err_chanoprivsneeded(Message& msg, const std::string& chan_name)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 482 " + chan_name + " :You're not channel operator"
			);
		msg.appendSeparator();
	}

	void	err_cantkillserver(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(build_prefix(msg.getServHostname()) + " 483 :You can't kill a server!");
		msg.appendSeparator();
	}

	void	err_nooperhost(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(build_prefix(msg.getServHostname()) + " 491 :No O-lines for your host");
		msg.appendSeparator();
	}

	void	err_unknownmodeflag(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(build_prefix(msg.getServHostname()) + " 501 :Unknown MODE flag");
		msg.appendSeparator();
	}

	void	err_usersdontmatch(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(build_prefix(msg.getServHostname()) + " 502 :Cannot change mode for other users");
		msg.appendSeparator();
	}
} // !namespace ft_irc