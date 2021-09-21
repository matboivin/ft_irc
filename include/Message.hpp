/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 18:47:11 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/21 14:10:20 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include <vector>
# include "ft_irc.hpp"

namespace ft_irc
{
	// Parser for IRC protocol messages
	class Message
	{
		private:

			// private alias
			typedef std::vector<std::string>	str_vec;

			std::string	_prefix;
			std::string	_command;
			str_vec		_params;

		public:

			Message();
			// copy constructor
			Message(const Message& other);
			// assignment operator
			Message	&operator=(const Message& other);
			// destructor
			~Message();

			// Messagegetters
			std::string	getPrefix(void) const;
			std::string	getCommand(void) const;
			str_vec		getParams(void) const;

			// Message setters
			void	setPrefix(const std::string& prefix);
			void	setCommand(const std::string& command);
			void	setParam(const std::string& param);
	};
}

#endif // !Message