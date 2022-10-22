/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_operations.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 19:35:18 by mboivin           #+#    #+#             */
/*   Updated: 2022/10/22 22:04:48 by mboivin          ###   ########.fr       */
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
	std::string	buildPrefix(const std::string& sender);
	std::string	buildFullClientId(const Client& client);

	/* Secondary parsing helpers and checkers */
	bool		isSpecial(const char& c);
	bool		nickIsValid(const std::string& newnick);
	bool		channelIsValid(const std::string& chan_name);
	bool		matchNick(const std::string& to_match, const std::string& nick);
	bool		isStringInMsgParams(const Message& msg, const std::string& str);
	int			getModePrefix(const char& prefix, char& mode_operator);
	bool		usermodeCharIsValid(const char& c);
	bool		chanmodecharisvalid(const char& c);

	/* Remove duplicates from lists */
	template<typename T>
		std::list<T*>	removeDuplicates(const std::list<T*>& src, T* elem=0);
} // !namespace ft_irc

# include "server_operations.tpp"

#endif // !SERVER_OPERATIONS_HPP
