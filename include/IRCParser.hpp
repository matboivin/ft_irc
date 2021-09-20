/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:20:57 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/20 19:46:08 by mboivin          ###   ########.fr       */
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
			IRCParser(const IRCParser& other);
			IRCParser	&operator=(const IRCParser& other);
			~IRCParser();

			void	parse(const std::string& packet); // + client sender info
	};
}

#endif // !IRCParser