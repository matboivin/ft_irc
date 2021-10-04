/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_operations.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 19:35:18 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/04 10:18:47 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_OPERATIONS_HPP
# define SERVER_OPERATIONS_HPP

# include <string>

// Operations helpers needed for building responses, checking channels, ...

namespace ft_irc
{
	class Message;
	class IRCClient;

	// helpers
	std::string	build_prefix(const std::string& sender);
	std::string	build_full_client_id(const IRCClient& client);

} // !namespace ft_irc

#endif