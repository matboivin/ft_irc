/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenjell <mbenjell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 19:00:39 by mbenjell          #+#    #+#             */
/*   Updated: 2019/10/24 14:36:54 by mbenjell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

template< typename T >

class	Array
{
	public:

		Array<T>( void )
		{
			this->_array = new T [0];
			this->_length = 0;
		}
		~Array<T>( void ){}

		Array<T>( unsigned int i )
		{
			this->_length = i;
			this->_array = new T [i];
		}

		Array<T>(Array<T> const & src){ *this = src; }

		Array<T>	&operator=( Array<T> const & src )
		{
			unsigned int	n;
			T				*src_array;

			src_array = src.getArray();
			n = src.size();
			this->_length = n;
			this->_array = new T [n];
			while (n--)
				   this->_array[n] = src_array[n];
			return *this;
		}

		T 				*getArray( void ) const { return this->_array; } ;
		unsigned int 	size( void ) const { return this->_length; } ;

		T	&operator[](unsigned int const i)
		{
			if (i < 0 || i >= this->_length)
				throw std::exception();
			return this->_array[i];
		}

	private:
		unsigned int	_length;
		T				*_array;
};

template< typename T >

std::ostream & operator<<( std::ostream & o, Array<T> & src)
{
	unsigned int i = 0;

	if (src.size() == 0)
	{
		o << "(null)" << std::endl;
		return o;
	}
	while (i < src.size() - 1)
		o << src.getArray()[i++] << ", ";
	o << src.getArray()[i] << std::endl;

	return o;
}
