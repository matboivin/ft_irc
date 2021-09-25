/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenjell <mbenjell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 00:44:33 by mbenjell          #+#    #+#             */
/*   Updated: 2021/09/25 01:19:11 by mbenjell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SINGLETON_HPP
# define SINGLETON_HPP

#include "ft_irc.hpp"

class Singleton 
{
	protected:
		static int _len;

	public:
		Singleton( void );
		~Singleton( void );

};

#endif
