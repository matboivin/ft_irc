/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserInterface.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenjell <mbenjell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 23:05:18 by mbenjell          #+#    #+#             */
/*   Updated: 2021/09/25 03:30:07 by mbenjell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_INTERFACE_HPP
# define USER_INTERFACE_HPP

#include "ft_irc.hpp"

class UserInterface
{
	public:
		UserInterface( void ){}
		virtual ~UserInterface( void ){}
		virtual void sendCommand( void ) = 0;
};

#endif
