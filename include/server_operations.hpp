/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_operations.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 19:35:18 by mboivin           #+#    #+#             */
/*   Updated: 2021/12/12 13:27:22 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_OPERATIONS_HPP
# define SERVER_OPERATIONS_HPP

# include <list>
# include <string>
# include <vector>

/* Operations helpers needed for building responses, checking channels, ... */

namespace ft_irc
{
	class Message;
	class Client;

	/* Aliases */
	typedef std::vector<std::string>	t_params;

	/* Response helpers */
	std::string	build_prefix(const std::string& sender);
	std::string	build_full_client_id(const Client& client);

	/* Secondary parsing helpers and checkers */
	bool		is_special(char c);
	bool		nick_is_valid(const std::string& newnick);
	bool		channel_is_valid(const std::string& chan_name);
	bool		match_nick(const std::string& to_match, const std::string& nick);
	bool		is_string_in_msg_params(const Message& msg, const std::string& str);
	int			verify_mode_prefix(const char& prefix, bool& add_mode);

	/* Remove duplicates from lists */
	template<typename T>
		std::list<T*>	removeDuplicates(const std::list<T*>& src, T* elem=0);
} // !namespace ft_irc

# include "server_operations.tpp"

#endif // !SERVER_OPERATIONS_HPP
