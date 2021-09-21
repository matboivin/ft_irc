/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:20:57 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/21 14:11:53 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCPARSER_HPP
# define IRCPARSER_HPP

# include <string>
# include "ft_irc.hpp"

namespace ft_irc
{
	// Parser for IRC protocol messages
	class IRCParser
	{
		protected:
			// next workers in pipeline?

		public:
			IRCParser();
			// copy constructor
			IRCParser(const IRCParser& other);
			// assignment operator
			IRCParser	&operator=(const IRCParser& other);
			~IRCParser();
			// destructor
			void	parse(const std::string& packet); // + client sender info
	};
}

#endif // !IRCParser