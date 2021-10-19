/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 16:41:45 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/19 16:54:43 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>

namespace ft_irc
{
	class Client;
	class Message;

	// Config for IRC server
	class Config
	{
	private:
		std::string	_hostname;
		std::string	_bind_address;
		std::string	_port;
		std::string	_connection_password;

	public:
		// default constructor
				Config();
		// copy constructor
				Config(const Config& other);
		// constructor
				Config(std::string hostname="irc.42.fr",
					   std::string bind_address="0.0.0.0",
					   std::string port="6697",
					   std::string password="");
		// assignment operator
		Config&	operator=(const Config& other);
		// destructor
				~Config();

		// getters
		std::string	getHostname() const;
		std::string	getBindAddress() const;
		std::string	getPort() const;
		std::string	getPassword() const;

		// setters
		void		setHostname(const std::string& hostname);
		void		setBindAddress(const std::string& bind_address);
		void		setPort(const std::string& port);
		void		setPassword(const std::string& password);
	};
} // !namespace ft_irc

#endif // !Config