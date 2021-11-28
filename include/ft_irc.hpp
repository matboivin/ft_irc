/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 13:07:04 by mboivin           #+#    #+#             */
/*   Updated: 2021/11/28 16:56:19 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

# include "Channel.hpp"
# include "Client.hpp"
# include "CLIParser.hpp"
# include "Config.hpp"
# include "Logger.hpp"
# include "Message.hpp"
# include "numeric_replies.hpp"
# include "Parser.hpp"
# include "Server.hpp"
# include "server_operations.hpp"

enum	e_error_code
{
	ERR_SUCCESS = 0,
	ERR_NOSUCHNICK,
	ERR_NOSUCHSERVER,
	ERR_NOSUCHCHANNEL,
	ERR_CANNOTSENDTOCHAN,
	ERR_TOOMANYCHANNELS,
	ERR_WASNOSUCHNICK,
	ERR_TOOMANYTARGETS,
	ERR_NOORIGIN,
	ERR_NORECIPIENT,
	ERR_NOTEXTTOSEND,
	ERR_NOTOPLEVEL,
	ERR_WILDTOPLEVEL,
	ERR_UNKNOWNCOMMAND,
	ERR_NOMOTD,
	ERR_NOADMININFO,
	ERR_FILEERROR,
	ERR_NONICKNAMEGIVEN,
	ERR_ERRONEUSNICKNAME,
	ERR_NICKNAMEINUSE,
	ERR_NICKCOLLISION,
	ERR_USERNOTINCHANNEL,
	ERR_NOTONCHANNEL,
	ERR_USERONCHANNEL,
	ERR_NOLOGIN,
	ERR_SUMMONDISABLED,
	ERR_USERSDISABLED,
	ERR_NOTREGISTERED,
	ERR_NEEDMOREPARAMS,
	ERR_ALREADYREGISTRED,
	ERR_NOPERMFORHOST,
	ERR_PASSWDMISMATCH,
	ERR_YOUREBANNEDCREEP,
	ERR_KEYSET,
	ERR_CHANNELISFULL,
	ERR_UNKNOWNMODE,
	ERR_INVITEONLYCHAN,
	ERR_BANNEDFROMCHAN,
	ERR_BADCHANNELKEY,
	ERR_NOPRIVILEGES,
	ERR_CHANOPRIVSNEEDED	
};

extern const char *error_msg[];
int		setNonblocking(int fd);
void	displayUsage(const char* program_name);
int		test_parsing();
int		test_modes();

# ifndef nullptr
#  define nullptr NULL
# endif

#ifndef DEBUG
# define DEBUG 0

#endif // !DEBUG

#endif // FT_IRC_HPP
