/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 18:47:11 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/21 14:08:31 by mboivin          ###   ########.fr       */
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

			// TODO: sender info + target info
			std::string	_command;
			str_vec		_params;

		public:
			Message();
			Message(const Message& other);
			Message	&operator=(const Message& other);
			~Message();
			// getters
			std::string	getCommand(void) const;
			str_vec		getParams(void) const;
			//setters
			void	setCommand(const std::string& command);
			void	setParam(const std::string& param);
	};
}

#endif // !Message