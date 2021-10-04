/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 13:07:04 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/04 13:30:20 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
#define FT_IRC_HPP

# include <string>
# include <iostream>
# include <cstring>
# include <cstdlib>
# include <iostream>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <fcntl.h>
# include <unistd.h>
# include "numeric_replies.hpp"
# include "client.hpp"
# include "server.hpp"
# include "Channel.hpp"
# include "Parser.hpp"
# include "Message.hpp"

void	displayUsage(const char* program_name);
int		test_parsing();

enum Status
{
	e_admin,
	e_ircop,
	e_member
};

#ifndef nullptr
# define nullptr NULL
#endif // !nullpt

#endif
