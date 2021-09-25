/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenjell <mbenjell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 23:53:21 by mbenjell          #+#    #+#             */
/*   Updated: 2021/09/25 03:36:45 by mbenjell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ft_irc.hpp"
#include "UserInterface.hpp"

class Client : public UserInterface 
{
	public:
		Client( void );
		~Client( void );

		void sendCommand( void );

	private:
};

#endif
