/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 16:41:45 by mboivin           #+#    #+#             */
/*   Updated: 2021/11/27 17:47:59 by mboivin          ###   ########.fr       */
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

	/*
	 * Configuration for IRC server
	 * Attribute of the Server
	 */
	class Config
	{
	public:
		/* Aliases */
		typedef std::map<std::string, std::string>	t_oper_block;

		/* Default constructor */
						Config();

		/* Copy constructor */
						Config(const Config& other);

		/* Settings constructor */
						Config(const std::string& bind_address="0.0.0.0",
							   const std::string& port="6697",
							   const std::string& password="",
							   const std::string& hostname="irc.42.fr"
							   );

		/* Copy assignment operator */
		Config&			operator=(const Config& other);

		/* Destructor */
						~Config();

		/* Getters */
		std::string		getBindAddress() const;
		std::string		getPort() const;
		std::string		getPassword() const;
		std::string		getHostname() const;
		/* Setters */
		void			setBindAddress(const std::string& bind_address);
		void			setPort(const std::string& port);
		void			setPassword(const std::string& password);
		void			setHostname(const std::string& hostname);

		/* Oper operations */
		void			addOperBlock(const std::string& name, const std::string& password);
		bool			operBlockIsValid(const std::string& name, const std::string& password);

	private:
		/* Attributes */
		std::string		_bind_address;
		std::string		_port;
		std::string		_connection_password;
		std::string		_hostname;
		t_oper_block	_oper_blocks;
	};
} // !namespace ft_irc

#endif // !CONFIG_HPP
