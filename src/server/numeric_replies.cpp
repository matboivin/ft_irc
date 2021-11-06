/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_replies.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 17:01:20 by mboivin           #+#    #+#             */
/*   Updated: 2021/11/06 17:36:06 by root             ###   ########.fr       */
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
	void	rpl_welcome(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 001 Welcome to the Internet Relay Network ");
		msg.appendResponse(build_full_client_id(msg.getSender()));
		msg.appendSeparator();
	}

	void	rpl_yourhost(Message& msg, const std::string& version, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 002 ");
		msg.appendResponse(msg.getSender().getNick());
		msg.appendResponse(" :Your host is ");
		msg.appendResponse(", running version ");
		msg.appendResponse(version);
		msg.appendSeparator();
	}

	void	rpl_umodeis(Message& msg, const std::string& user_mode, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 221 ");
		msg.appendResponse(user_mode);
		msg.appendSeparator();
	}

	void	rpl_whoisuser(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
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

	void	rpl_whoisserver(Message& msg, const std::string& description, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 312 ");
		msg.appendResponse(msg.getSender().getNick());
		msg.appendResponse(" ");
		msg.appendResponse(msg.getServHostname());
		msg.appendResponse(" :");
		msg.appendResponse(description);
		msg.appendSeparator();
	}

	void	rpl_whoisoperator(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 313 ");
		msg.appendResponse(msg.getSender().getNick());
		msg.appendResponse(" :is an IRC operator");
		msg.appendSeparator();
	}

	void	rpl_endofwho(Message& msg, const std::string& name, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 315 ");
		msg.appendResponse(name);
		msg.appendResponse(" :End of WHO list");
		msg.appendSeparator();
	}

	void	rpl_endofwhois(Message& msg, const std::string& nick, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 318 ");
		msg.appendResponse(nick);
		msg.appendResponse(" :End of WHOIS list");
		msg.appendSeparator();
	}

	void	rpl_whoischannels(Message& msg, const std::string& nick, const std::string& chan_name, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 319 ");
		msg.appendResponse(nick);
		msg.appendResponse(" :");
		msg.appendResponse(chan_name);
		msg.appendSeparator();
	}

	void	rpl_list(Message& msg, Channel& channel, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 322 ");
		msg.appendResponse(channel.getName());
		msg.appendResponse(" :");
		msg.appendResponse(channel.getTopic());
		msg.appendSeparator();
	}

	void	rpl_listend(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 323 :End of LIST");
		msg.appendSeparator();
	}

	void	rpl_channelmodeis(Message& msg, Channel& channel, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 324 ");
		msg.appendResponse(channel.getName());
		msg.appendResponse(" ");
		msg.appendResponse(channel.getMode());
		msg.appendSeparator();
	}

	void	rpl_notopic(Message& msg, const std::string& chan_name, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 331 ");
		msg.appendResponse(chan_name);
		msg.appendResponse(" :No topic is set");
		msg.appendSeparator();
	}

	void	rpl_topic(Message& msg, Channel& channel, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 332 ");
		msg.appendResponse(channel.getName());
		msg.appendResponse(" :");
		msg.appendResponse(channel.getTopic());
		msg.appendSeparator();
	}

	void	rpl_inviting(Message& msg, const std::string& chan_name, const std::string& nick, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 341 ");
		msg.appendResponse(chan_name);
		msg.appendResponse(" ");
		msg.appendResponse(nick);
		msg.appendSeparator();
	}

	void	rpl_whoreply(Message& msg, const std::string& chan_name, Client& target, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
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

	void	rpl_namreply(Message& msg, Channel& channel, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 353 ");
		msg.appendResponse(channel.getName());
		// todo
		msg.appendSeparator();
	}

	void	rpl_endofnames(Message& msg, const std::string& chan_name, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 366 ");
		msg.appendResponse(chan_name);
		msg.appendResponse(" :End of NAMES list");
		msg.appendSeparator();
	}

	void	rpl_youreoper(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 381 :You are now an IRC operator");
		msg.appendSeparator();
	}

	// error replies

	void	err_nosuchnick(Message& msg, const std::string& nick, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 401 ");
		msg.appendResponse(nick);
		msg.appendResponse(" :No such nick/channel");
		msg.appendSeparator();
	}

	void	err_nosuchchannel(Message& msg, const std::string& chan_name, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 403 ");
		msg.appendResponse(chan_name);
		msg.appendResponse(" :No such channel");
		msg.appendSeparator();
	}

	void	err_cannotsendtochan(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 404 ");
		msg.appendResponse(msg.getParams().at(0));
		msg.appendResponse(" :Cannot send to channel");
		msg.appendSeparator();
	}

	void	err_toomanychannels(Message& msg, const std::string& chan_name, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 405 ");
		msg.appendResponse(chan_name);
		msg.appendResponse(" :You have joined too many channels");
		msg.appendSeparator();
	}

	void	err_toomanytargets(Message& msg, const std::string& target, bool rewrite) // maybe not necessary?
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 407 ");
		msg.appendResponse(target);
		msg.appendResponse(" :<error code> recipients. <abort message>"); // todo
		msg.appendSeparator();
	}

	void	err_noorigin(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 409 :No origin specified");
		msg.appendSeparator();
	}

	void	err_norecipient(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 411 :No recipient given (");
		msg.appendResponse(msg.getCommand());
		msg.appendResponse(")");
		msg.appendSeparator();
	}

	void	err_notexttosend(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 412 :No text to send");
		msg.appendSeparator();
	}

	void	err_unknowncommand(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 421 ");
		msg.appendResponse(msg.getCommand());
		msg.appendResponse(" :Unknown command");
		msg.appendSeparator();
	}

	void	err_nonicknamegiven(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 431 :No nickname given");
		msg.appendSeparator();
	}

	void	err_erroneusnickname(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 432 ");
		msg.appendResponse(msg.getParams().at(0));
		msg.appendResponse(" :Erroneous nickname");
		msg.appendSeparator();
	}

	void	err_nicknameinuse(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 433 ");
		msg.appendResponse(msg.getParams().at(0));
		msg.appendResponse(" :Nickname already in use");
		msg.appendSeparator();
	}

	void	err_unavailresource(Message& msg, const std::string& target, bool rewrite) // maybe not necessary?
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 437 ");
		msg.appendResponse(target);
		msg.appendResponse(" :Nick/channel is temporarily unavailable");
		msg.appendSeparator();
	}

	void	err_usernotinchannel(Message& msg, const std::string& nick, const std::string& chan_name, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 441 ");
		msg.appendResponse(nick);
		msg.appendResponse(" ");
		msg.appendResponse(chan_name);
		msg.appendResponse(" :They aren't on that channel");
		msg.appendSeparator();
	}

	void	err_notonchannel(Message& msg, const std::string& chan_name, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 442 ");
		msg.appendResponse(chan_name);
		msg.appendResponse(" :You're not on that channel");
		msg.appendSeparator();
	}

	void	err_useronchannel(Message& msg, const std::string& nick, const std::string& chan_name, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 443 ");
		msg.appendResponse(nick);
		msg.appendResponse(" ");
		msg.appendResponse(chan_name);
		msg.appendResponse(" :is already on channel");
		msg.appendSeparator();
	}

	void	err_notregistered(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 451 :You have not registered");
		msg.appendSeparator();
	}

	void	err_needmoreparams(Message& msg, bool rewrite, const std::string& error_string)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 461 ");
		msg.appendResponse(msg.getCommand());
		msg.appendResponse(" :");
		msg.appendResponse(error_string);
		msg.appendSeparator();
	}

	void	err_syntaxerror(Message& msg, std::string cmd, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		//:public-irc.w3.org 461 ezakjhzkjehkjzehdk WHO :Syntax error
		msg.setResponse(
			build_prefix(msg.getServHostname()) 
			+ " 461 " + sender.getNick() + " " + cmd + " :Syntax error"
			);
		msg.appendSeparator();
	}

	void	err_alreadyregistered(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 462 :Unauthorized command (already registered)");
		msg.appendSeparator();
	}

	void	err_passwdmismatch(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 464 :Password incorrect");
		msg.appendSeparator();
	}

	void	err_channelisfull(Message& msg, const std::string& chan_name, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 471 ");
		msg.appendResponse(chan_name);
		msg.appendResponse(" :Cannot join channel (+l)");
		msg.appendSeparator();
	}

	void	err_bannedfromchan(Message& msg, const std::string& chan_name, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 474 ");
		msg.appendResponse(chan_name);
		msg.appendResponse(" :Cannot join channel (+b)");
		msg.appendSeparator();
	}

	void	err_noprivileges(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 481 :Permission Denied- You're not an IRC operator");
		msg.appendSeparator();
	}

	void	err_chanoprivsneeded(Message& msg, const std::string& chan_name, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 482 ");
		msg.appendResponse(chan_name);
		msg.appendResponse(" :You're not channel operator");
		msg.appendSeparator();
	}

	void	err_cantkillserver(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 483 :You can't kill a server!");
		msg.appendSeparator();
	}

	void	err_nooperhost(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 491 :No O-lines for your host");
		msg.appendSeparator();
	}

	void	err_unknownmodeflag(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 501 :Unknown MODE flag");
		msg.appendSeparator();
	}

	void	err_usersdontmatch(Message& msg, bool rewrite)
	{
		msg.setRecipient(msg.getSender());
		if (rewrite)
			msg.clearResponse();
		msg.appendResponse(build_prefix(msg.getServHostname()));
		msg.appendResponse(" 502 :Cannot change mode for other users");
		msg.appendSeparator();
	}
} // !namespace ft_irc