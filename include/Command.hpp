/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 19:09:34 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/03 15:34:37 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <string>

namespace ft_irc
{
	class Message;
	class IRCClient;

	// Command interface
	class ICommand
	{
	public:
		virtual ~ICommand() {}

		virtual void	execute() = 0;
	};

	// Concrete commands

	// NICK: Change a user nickname
	class NickCommand : public ICommand
	{
	private:
		// attributes
		std::string	_newnick;
		Message&	_msg;

		bool		_nick_is_valid();

	public:
		// message constructor
					NickCommand(Message& msg);
		// copy constructor
					NickCommand(const NickCommand& other);
		// assignment operator
		NickCommand&	operator=(const NickCommand& other);
		// destructor
					virtual ~NickCommand();

		// getters
		std::string	getNewNick() const;
		Message		getMsg() const;
		// setters
		void	setNewNick(const std::string& newnick);
		void	setMsg(const Message& msg);

		void	execute();
	};

	// PASS: set a connection password
	class PassCommand : public ICommand
	{
	private:
		// attributes
		Message&	_msg;

	public:
		// message constructor
					PassCommand(Message& msg);
		// copy constructor
					PassCommand(const PassCommand& other);
		// assignment operator
		PassCommand&	operator=(const PassCommand& other);
		// destructor
					virtual ~PassCommand();
		
		// getters
		Message	getMsg() const;
		// setters
		void	setMsg(const Message& msg);

		void	execute();
	};
} // namespace ft_irc

#endif // !COMMAND_HPP