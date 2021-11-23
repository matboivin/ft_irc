/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_replies.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 16:35:17 by mboivin           #+#    #+#             */
/*   Updated: 2021/11/23 16:28:46 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERIC_REPLIES_HPP
# define NUMERIC_REPLIES_HPP

# include <string>

namespace ft_irc
{
	class Message;
	class Channel;
	class Client;

	// command responses
	void	rpl_welcome(Message& msg, bool rewrite = false); // 001
	void	rpl_yourhost(Message& msg, const std::string& version, bool rewrite = false); // 002
	void	rpl_created(Message& msg, const std::string& creation_date, bool rewrite = false); // 003
	void	rpl_myinfo(Message& msg, const std::string& version, bool rewrite = false); // 004
	void	rpl_umodeis(Message& msg, const std::string& user_mode, bool rewrite = false); // 221
	void	rpl_whoisuser(Message& msg, bool rewrite = false); // 311
	void	rpl_whoisuser(Message& msg, const Client& user, bool rewrite = false); // 311
	void	rpl_whoisserver(Message& msg, const std::string& description, bool rewrite = false); // 312
	void	rpl_whoisoperator(Message& msg, const Client& user, bool rewrite = false); // 313
	void	rpl_whoisoperator(Message& msg, bool rewrite = false); // 313
	void	rpl_endofwho(Message& msg, const std::string& name, bool rewrite = false); // 315
	void	rpl_endofwhois(Message& msg, const std::string& nick, bool rewrite = false); // 318
	void	rpl_whoischannels(Message& msg, const std::string& nick, const std::string& chan_name, bool rewrite = false); // 319
	void	rpl_list(Message& msg, Channel& channel, bool rewrite = false); // 322
	void	rpl_listend(Message& msg, bool rewrite = false); // 323
	void	rpl_channelmodeis(Message& msg, Channel& channel, bool rewrite = false); // 324
	void	rpl_notopic(Message& msg, const std::string& chan_name, bool rewrite = false); // 331
	void	rpl_topic(Message& msg, Channel& channel, bool rewrite = false); // 332
	void	rpl_inviting(Message& msg, const std::string& chan_name, const std::string& nick, bool rewrite = false); // 341
	void	rpl_whoreply(Message& msg, const std::string& chan_name, Client& target, bool rewrite = false); // 352
	void	rpl_namreply(Message& msg, bool rewrite = false); // 353
	void	rpl_endofnames(Message& msg, const std::string& chan_name, bool rewrite = false); // 366
	void	rpl_youreoper(Message& msg, bool rewrite = false); // 381

	// error replies
	void	err_nosuchnick(Message& msg, const std::string& nick, bool rewrite = false); // 401
	void	err_nosuchserver(Message& msg, const std::string& serv_name, bool rewrite = false); // 402
	void	err_nosuchchannel(Message& msg, const std::string& chan_name, bool rewrite = false); // 403
	void	err_cannotsendtochan(Message& msg, bool rewrite = false); // 404
	void	err_toomanychannels(Message& msg, const std::string& chan_name, bool rewrite = false); // 405
	void	err_noorigin(Message& msg, bool rewrite = false); // 409
	void	err_norecipient(Message& msg, bool rewrite = false); // 411
	void	err_notexttosend(Message& msg, bool rewrite = false); // 412
	void	err_unknowncommand(Message& msg, bool rewrite = false); // 421
	void	err_nonicknamegiven(Message& msg, bool rewrite = false); // 431
	void	err_erroneusnickname(Message& msg, bool rewrite = false); // 432
	void	err_nicknameinuse(Message& msg, bool rewrite = false); // 433
	void	err_nickcollision(Message& msg, bool rewrite = false); // 436
	void	err_unavailresource(Message& msg, const std::string& target, bool rewrite = false); // 437
	void	err_usernotinchannel(Message& msg, const std::string& nick, const std::string& chan_name, bool rewrite = false); // 441
	void	err_notonchannel(Message& msg, const std::string& chan_name, bool rewrite = false); // 442
	void	err_useronchannel(Message& msg, const std::string& nick, const std::string& chan_name, bool rewrite = false); // 443
	void	err_notregistered(Message& msg, bool rewrite = false); // 451
	void	err_needmoreparams(Message& msg, bool rewrite = false, const std::string& error_string = "Not enough parameters"); // 461
	void	err_syntaxerror(Message& msg, const std::string& cmd = "", bool rewrite = false); // 461
	void	err_alreadyregistered(Message& msg, bool rewrite = false); // 462
	void	err_passwdmismatch(Message& msg, bool rewrite = false); // 464
	void	err_channelisfull(Message& msg, const std::string& chan_name, bool rewrite = false); // 471
	void	err_bannedfromchan(Message& msg, const std::string& chan_name, bool rewrite = false); // 474
	void	err_noprivileges(Message& msg, bool rewrite = false); // 481
	void	err_chanoprivsneeded(Message& msg, const std::string& chan_name, bool rewrite = false); // 482
	void	err_cantkillserver(Message& msg, bool rewrite = false); // 483
	void	err_nooperhost(Message& msg, bool rewrite = false); // 491
	void	err_unknownmodeflag(Message& msg, bool rewrite = false); // 501
	void	err_usersdontmatch(Message& msg, bool rewrite = false); // 502
} // !namespace ft_irc

#endif