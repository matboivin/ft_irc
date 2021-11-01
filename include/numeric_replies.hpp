/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_replies.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 16:35:17 by mboivin           #+#    #+#             */
/*   Updated: 2021/11/01 13:50:08 by root             ###   ########.fr       */
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
	void	rpl_welcome(Message& msg); // 001
	void	rpl_umodeis(Message& msg, const std::string& user_mode); // 221
	void	rpl_whoisuser(Message& msg); // 311
	void	rpl_whoisoperator(Message& msg); // 313
	void	rpl_endofwho(Message& msg, const std::string& name); // 315
	void	rpl_endofwhois(Message& msg, const std::string& nick); // 318
	void	rpl_whoischannels(Message& msg, const std::string& nick, const std::string& chan_name); // 319
	void	rpl_list(Message& msg, Channel& channel); // 322
	void	rpl_listend(Message& msg); // 323
	void	rpl_channelmodeis(Message& msg, Channel& channel); // 324
	void	rpl_notopic(Message& msg, const std::string& chan_name); // 331
	void	rpl_topic(Message& msg, Channel& channel); // 332
	void	rpl_inviting(Message& msg, const std::string& chan_name, const std::string& nick); // 341
	void	rpl_whoreply(Message& msg, const std::string& chan_name, Client& target); // 352
	void	rpl_namreply(Message& msg); // 353
	void	rpl_endofnames(Message& msg, const std::string& chan_name); // 366
	void	rpl_youreoper(Message& msg); // 381

	// error replies
	void	err_nosuchnick(Message& msg, const std::string& nick); // 401
	void	err_nosuchchannel(Message& msg, const std::string& chan_name); // 403
	void	err_cannotsendtochan(Message& msg); // 404
	void	err_toomanychannels(Message& msg, const std::string& chan_name); // 405
	void	err_toomanytargets(Message& msg, const std::string& target); // 407
	void	err_noorigin(Message& msg); // 409
	void	err_norecipient(Message& msg); // 411
	void	err_notexttosend(Message& msg); // 412
	void	err_unknowncommand(Message& msg); // 421
	void	err_nonicknamegiven(Message& msg); // 431
	void	err_erroneusnickname(Message& msg); // 432
	void	err_nicknameinuse(Message& msg); // 433
	void	err_nickcollision(Message& msg); // 436
	void	err_unavailresource(Message& msg, const std::string& target); // 437
	void	err_usernotinchannel(Message& msg, const std::string& nick, const std::string& chan_name); // 441
	void	err_notonchannel(Message& msg, const std::string& chan_name); // 442
	void	err_useronchannel(Message& msg, const std::string& nick, const std::string& chan_name); // 443
	void	err_notregistered(Message& msg); // 451
	void	err_needmoreparams(Message& msg); // 461
	void	err_needmoreparams(Message& msg, std::string error_string); // 461
	void	err_alreadyregistered(Message& msg); // 462
	void	err_passwdmismatch(Message& msg); // 464
	void	err_channelisfull(Message& msg, const std::string& chan_name); // 471
	void	err_bannedfromchan(Message& msg, const std::string& chan_name); // 474
	void	err_noprivileges(Message& msg); // 481
	void	err_chanoprivsneeded(Message& msg, const std::string& chan_name); // 482
	void	err_cantkillserver(Message& msg); // 483
	void	err_nooperhost(Message& msg); // 491
	void	err_unknownmodeflag(Message& msg); // 501
	void	err_usersdontmatch(Message& msg); // 502
	void	err_syntaxerror(Message& msg, std::string cmd = ""); // 461
} // !namespace ft_irc

#endif