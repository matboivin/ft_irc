/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 13:07:04 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/05 14:05:23 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

# include <iostream>
# include <cstdlib>
# include <cstring>
# include <fcntl.h>
# include <string>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netdb.h>
# include <unistd.h>

# include "Channel.hpp"
# include "Client.hpp"
# include "CLIParser.hpp"
# include "Message.hpp"
# include "numeric_replies.hpp"
# include "Parser.hpp"
# include "Server.hpp"
# include "server_operations.hpp"
# include "UserInterface.hpp"

void	displayUsage(const char* program_name);
int		test_parsing();

enum Status
{
	e_admin,
	e_ircop,
	e_member
};

# ifndef nullptr
#  define nullptr NULL
# endif // !nullpt

#endif
