/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_replies.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 17:01:20 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/03 15:37:50 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <string>
# include "numeric_replies.hpp"
# include "client.hpp"
# include "Channel.hpp"
# include "Message.hpp"

namespace ft_irc
{
	class IRCClient;
	class Channel;
	class Message;

	// helpers
	static std::string	build_prefix(const std::string& sender)
	{
		return (":" + sender);
	}

	static std::string	build_full_client_id(const IRCClient& client)
	{
		return (
			client.getNick() + "!" + client.getUsername()
			+ "@" + client.getIpAddressStr() // need host (ip only if couldn't resolve)
			);
	}

	// command responses
	void	rpl_welcome(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 001 Welcome to the Internet Relay Network "
			+ build_full_client_id(msg.getSender())
			);
		msg.appendSeparator();
	}

	void	rpl_umodeis(Message& msg, const std::string& user_mode)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 221 " + user_mode
			);
		msg.appendSeparator();
	}

	void	rpl_whoisuser(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 311 " + msg.getSender().getNick() + " " + msg.getSender().getUsername() + " "
			+ msg.getSender().getIpAddressStr() + " :" + msg.getSender().getRealName()
			);
		msg.appendSeparator();
	}

	void	rpl_whoisoperator(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 313 " + msg.getSender().getNick() + " :is an IRC operator"
			);
		msg.appendSeparator();
	}

	void	rpl_endofwho(Message& msg, const std::string& name)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(build_prefix(msg.getServHostname()) + " 315 " + name + " :End of WHO list");
		msg.appendSeparator();
	}

	void	rpl_endofwhois(Message& msg, const std::string& nick)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(build_prefix(msg.getServHostname()) + " 318 " + nick + " :End of WHOIS list");
		msg.appendSeparator();
	}

	void	rpl_whoischannels(Message& msg, const std::string& nick, const std::string& chan_name)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(build_prefix(
			msg.getServHostname())
			+ " 319 " + nick + " :*" + chan_name // todo
			);
		msg.appendSeparator();
	}

	void	rpl_list(Message& msg, Channel& channel)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 322 " + channel.getName() + " :" + channel.getTopic()
			);
		msg.appendSeparator();
	}

	void	rpl_channelmodeis(Message& msg, Channel& channel)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 322 " + channel.getName() + " " + channel.getMode() // todo + <mode params>
			);
		msg.appendSeparator();
	}

	void	rpl_listend(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(build_prefix(msg.getServHostname()) + " 323 :End of LIST");
		msg.appendSeparator();
	}

	void	rpl_notopic(Message& msg, const std::string& chan_name)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 331 " + chan_name + " :No topic is set"
			);
		msg.appendSeparator();
	}

	void	rpl_topic(Message& msg, Channel& channel)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 332 " + channel.getName() + " :" + channel.getTopic()
			);
		msg.appendSeparator();
	}

	void	rpl_inviting(Message& msg, const std::string& chan_name, const std::string& nick)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 341 " + chan_name + " " + nick
			);
		msg.appendSeparator();
	}

	void	rpl_whoreply(Message& msg, const std::string& chan_name, IRCClient& target)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 352 " + chan_name + " " + target.getUsername() + " " + target.getIpAddressStr()
			+ " " + "getServer" + " " + target.getNick() + " :" + target.getRealName()
			);
		msg.appendSeparator();
	}

	void	rpl_namreply(Message& msg, Channel& channel)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 353 " + channel.getName() // todo
			);
		msg.appendSeparator();
	}

	void	rpl_endofnames(Message& msg, const std::string& chan_name)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 366 " + chan_name + " :End of NAMES list"
			);
		msg.appendSeparator();
	}

	void	rpl_youreoper(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 381 :You are now an IRC operator"
			);
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

	void	err_cannotsendtochan(Message& msg, const std::string& chan_name)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 404 " + chan_name + " :Cannot send to channel"
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

	void	err_erroneusnickname(Message& msg, const std::string& nick)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 432 " + nick + " :Erroneous nickname"
			);
		msg.appendSeparator();
	}

	void	err_nicknameinuse(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 433 " + msg.getSender().getNick() + " :Nickname is already in use"
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

	void	err_userinchannel(Message& msg, const std::string& nick, const std::string& chan_name)
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

	void	err_needmoreparams(Message& msg)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 461 " + msg.getCommand() + " :Not enough parameters"
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
			+ " 471 " + chan_name + " :Cannot join channel (+l)");
		msg.appendSeparator();
	}

	void	err_bannedfromchan(Message& msg, const std::string& chan_name)
	{
		msg.setRecipient(msg.getSender());
		msg.setResponse(
			build_prefix(msg.getServHostname())
			+ " 474 " + chan_name + " :Cannot join channel (+b)");
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
			+ " 482 " + chan_name + " :You're not channel operator");
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