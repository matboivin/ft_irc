/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_operations.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 19:37:26 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/03 19:38:27 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "client.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "server_operations.hpp"

namespace ft_irc
{
	// helpers
	std::string	build_prefix(const std::string& sender)
	{
		return (":" + sender);
	}

	std::string	build_full_client_id(const IRCClient& client)
	{
		return (
			client.getNick() + "!" + client.getUsername()
			+ "@" + client.getIpAddressStr() // need host (ip only if couldn't resolve)
			);
	}
} // namespace ft_irc