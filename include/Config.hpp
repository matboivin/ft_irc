/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 16:41:45 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/19 17:08:13 by mboivin          ###   ########.fr       */
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
		std::string		_hostname;
		std::string		_bind_address;
		std::string		_port;
		std::string		_connection_password;
		t_oper_block	_oper_blocks;

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

		// oper operations
		void		addOperBlock(const std::string& name, const std::string& password);
		bool		operBlockIsValid(const std::string& name, const std::string& password);
	};
} // !namespace ft_irc

#endif // !Config