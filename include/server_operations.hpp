/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_operations.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 19:35:18 by mboivin           #+#    #+#             */
/*   Updated: 2021/11/01 17:19:26 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_OPERATIONS_HPP
# define SERVER_OPERATIONS_HPP

# include <list>
# include <string>

// Operations helpers needed for building responses, checking channels, ...

namespace ft_irc
{
	class Message;
	class Client;

	// Response helpers
	std::string	build_prefix(const std::string& sender);
	std::string	build_full_client_id(const Client& client);

	// Late parsing helpers
	bool	is_special(char c);
	bool	nick_is_valid(const std::string& newnick);
	bool	channel_is_valid(const std::string& chan_name);

	// Remove duplicates from lists
	template<typename T>
	std::list<T*>	removeDuplicates(const std::list<T*>& src, T* elem=0);

	bool	match_nick(const std::string& to_match, const std::string& nick);

} // !namespace ft_irc

# include "server_operations.tpp"

#endif