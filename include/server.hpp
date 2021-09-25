/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenjell <mbenjell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 00:56:41 by mbenjell          #+#    #+#             */
/*   Updated: 2021/09/25 00:54:07 by mbenjell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "ft_irc.hpp"

template< typename T >

class Server : public Singleton
{
	private:
		std::string		_name;
		Array<T>		_users;

	public:
		Server<T>( void );
		~Server<T>( void );
};

#endif
