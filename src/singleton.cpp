/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenjell <mbenjell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 00:48:32 by mbenjell          #+#    #+#             */
/*   Updated: 2021/09/25 01:30:10 by mbenjell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "singleton.hpp"

Singleton::Singleton( void )
{
	if (_len)
		throw "Error - Object already created.";
	_len++;
}

Singleton::~Singleton( void )
{
	_len = 0;
}

int	Singleton::_len = 0;
