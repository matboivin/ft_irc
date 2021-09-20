/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:20:57 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/20 17:27:53 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCPARSER_HPP
# define IRCPARSER_HPP

# include "ft_irc.hpp"

namespace ft_irc
{
	// Parser for IRC protocol messages
	class IRCParser
	{
		protected:
			//

		public:
			IRCParser();
			IRCParser(const IRCParser& other);
			IRCParser	&operator=(const IRCParser& other);
			~IRCParser();
	};
}

#endif // !IRCParser