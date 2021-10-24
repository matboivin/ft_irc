/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 16:41:45 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/24 11:56:20 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <map>
# include <string>

namespace ft_irc
{
	class Client;
	class Message;

	// Config for IRC server
	class Config
	{
	public:
		// aliases
		typedef std::map<std::string, std::string>	t_oper_block; // <name,password>

	private:
		// attributes
		std::string		_bind_address;
		std::string		_port;
		std::string		_connection_password;
		std::string		_hostname;
		t_oper_block	_oper_blocks;

	public:
		// default constructor
				Config();
		// copy constructor
				Config(const Config& other);
		// constructor
				Config(std::string bind_address="0.0.0.0",
					   std::string port="6697",
					   std::string password="",
					   std::string hostname="irc.42.fr");
		// assignment operator
		Config&	operator=(const Config& other);
		// destructor
				~Config();

		// getters
		std::string	getBindAddress() const;
		std::string	getPort() const;
		std::string	getPassword() const;
		std::string	getHostname() const;

		// setters
		void		setBindAddress(const std::string& bind_address);
		void		setPort(const std::string& port);
		void		setPassword(const std::string& password);
		void		setHostname(const std::string& hostname);

		// oper operations
		void		addOperBlock(const std::string& name, const std::string& password);
		bool		operBlockIsValid(const std::string& name, const std::string& password);
	};
} // !namespace ft_irc

#endif // !Config