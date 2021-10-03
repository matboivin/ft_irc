/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 18:42:14 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/30 18:56:13 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>

namespace ft_irc
{
	// IRC channel
	class Channel
	{
	private:
		std::string	_name;
		std::string	_topic;
		std::string	_mode;
		// users in the channel, etc

	public:
		// default constructor
				Channel();
		// name constructor
				Channel(const std::string& name);
		// copy constructor
				Channel(const Channel& other);
		// assignment operator
		Channel	&operator=(const Channel& other);
		// destructor
				~Channel();

		// Channel getters
		std::string	getName() const;
		std::string	getTopic() const;
		std::string	getMode() const;
		// Channel setters
		void	setName(const std::string& name);
		void	setTopic(const std::string& topic);
		void	setMode(const std::string& mode);
	};
}

#endif // !CHANNEL_HPP