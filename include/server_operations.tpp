/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_operations.tpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 19:35:18 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/18 19:33:37 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_OPERATIONS_TPP
# define SERVER_OPERATIONS_TPP

# include <string>

// Operations helpers, template implementation

namespace ft_irc
{
	// Remove duplicates from lists and the optional element if specified
	template<typename T>
	std::list<T*>	removeDuplicates(const std::list<T*>& src, T* elem)
	{
		std::list<T*>	lst(src);

		lst.sort();
		lst.unique();

		if (elem)
			lst.remove(elem);

		return (lst);
	}

} // !namespace ft_irc

#endif