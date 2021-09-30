/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_replies.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 16:35:17 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/30 16:34:26 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERIC_REPLIES_HPP
# define NUMERIC_REPLIES_HPP

# include <string>

namespace ft_irc
{
	class Message;

	// numeric replies
	void	rpl_welcome(Message& msg); // 001
	void	rpl_youreoper(Message& msg); // 381

	// error replies
	void	err_nosuchnick(Message& msg, const std::string& nick); // 401
	void	err_nosuchchannel(Message& msg, const std::string& channel); // 403
	void	err_unknowncommand(Message& msg); // 421
	void	err_nonicknamegiven(Message& msg); // 431
	void	err_erroneusnickname(Message& msg); // 432
	void	err_nicknameinuse(Message& msg); // 433
	void	err_nickcollision(Message& msg); // 436
	void	err_unavailresource(Message& msg, const std::string& target); // 437
	void	err_notonchannel(Message& msg, const std::string& channel); // 442
	void	err_notregistered(Message& msg); // 451
	void	err_needmoreparams(Message& msg); // 461
	void	err_alreadyregistered(Message& msg); // 462
	void	err_passwdmismatch(Message& msg); // 464
	void	err_badchannelkey(Message& msg, const std::string& channel); // 475
	void	err_restricted(Message& msg); // 484
	void	err_nooperhost(Message& msg); // 491
} // !namespace ft_irc

#endif