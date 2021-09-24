/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_replies.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 16:35:17 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/24 17:06:22 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERIC_REPLIES_HPP
# define NUMERIC_REPLIES_HPP

# include <string>
# include "Message.hpp"

namespace ft_irc
{
	class Message;

	// numeric replies
	void	rpl_welcome(const std::string& nick, const std::string& user,
						const::std::string& host);

	// error replies
	void	err_unknowncommand(Message& msg);
}

#endif