/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 19:09:34 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/03 19:38:43 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include <string>
# include <map>

namespace ft_irc
{
	class Message;

	typedef void (*cmd_fun)(Message& msg);
	typedef std::map<std::string, cmd_fun>	cmds_map;

	void	init_commands_map(cmds_map& m);
	void	exec_cmd(const cmds_map& m, Message& msg);

	// One helper function per command
	void	exec_pass_cmd(Message& msg);
	void	exec_nick_cmd(Message& msg);
	void	exec_quit_cmd(Message& msg);
	void	exec_notice_cmd(Message& msg);
	void	exec_privmsg_cmd(Message& msg);
} // namespace ft_irc

#endif // !COMMAND_HPP