/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:28:44 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/20 19:46:41 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCParser.hpp"

namespace ft_irc
{
	// Parser for IRC protocol messages

	// default constructor
	IRCParser::IRCParser()
	{
	}

	// copy constructor
	IRCParser::IRCParser(const IRCParser& other)
	{
		static_cast<void>(other);// tmp
	}

	// assignment operator
	IRCParser&	IRCParser::operator=(const IRCParser& other)
	{
		static_cast<void>(other);// tmp

		return *this;
	}

	// destructor
	IRCParser::~IRCParser()
	{
	}

	// main parsing function
	void	IRCParser::parse(const std::string& packet)
	{
		static_cast<void>(packet); // tmp
	}
}