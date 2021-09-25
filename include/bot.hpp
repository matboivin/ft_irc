/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenjell <mbenjell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 01:31:09 by mbenjell          #+#    #+#             */
/*   Updated: 2021/09/25 03:39:35 by mbenjell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

#include "ft_irc.hpp"
#include "UserInterface.hpp"

class Bot : public UserInterface
{
	public:
		Bot( void );
		~Bot( void );

		void sendCommand( void );

	private:
};

#endif
