/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 16:56:54 by root              #+#    #+#             */
/*   Updated: 2021/12/19 22:58:46 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <list>
#include <string>
#include "ft_irc.hpp"

namespace ft_irc
{
	/* Constructor */
	Client::Client(struct sockaddr_in address,
				   std::string nick,
				   std::string realname,
				   std::string username,
				   std::string hostname)
	: _in_buffer(),
	  _out_buffer(),
	  _max_cmd_length(512),
	  _nick(nick),
	  _realname(realname),
	  _username(username),
	  _hostname(hostname),
	  _mode("i"),
	  _enteredPass(),
	  _enteredNick(false),
	  _enteredUser(false),
	  _registered(false),
	  _alive(true),
	  _pinged(false),
	  _address(address),
	  _address_size(sizeof(address)),
	  _address_str(inet_ntoa(address.sin_addr)),
	  _timeout(),
	  _keep_alive(),
	  _last_event_time(),
	  _joined_channels(),
	  _kick_reason()
	{
		this->_timeout.tv_sec = 0;
		this->_timeout.tv_usec = 50;
		this->_keep_alive.tv_sec = 30;
		this->_keep_alive.tv_usec = 0;

		if (gettimeofday(&this->_last_event_time, NULL))
			throw std::runtime_error("gettimeofday() failed");
	}

	/* Constructor */
	Client::Client(std::string nick,
				   std::string realname,
				   std::string username,
				   std::string hostname)
	: _in_buffer(),
	  _out_buffer(),
	  _max_cmd_length(512),
	  _nick(nick),
	  _realname(realname),
	  _username(username),
	  _hostname(hostname),
	  _mode("i"),
	  _enteredPass(),
	  _enteredNick(false),
	  _enteredUser(false),
	  _registered(false),
	  _alive(true),
	  _pinged(false),
	  _address(),
	  _address_size(),
	  _address_str(),
	  _timeout(),
	  _keep_alive(),
	  _last_event_time(),
	  _joined_channels(),
	  _kick_reason()
	{
		this->_timeout.tv_sec = 0;
		this->_timeout.tv_usec = 50;
		this->_keep_alive.tv_sec = 30;
		this->_keep_alive.tv_usec = 0;

		if (gettimeofday(&this->_last_event_time, NULL))
			throw std::runtime_error("gettimeofday() failed");
	}

	/* Copy constructor */
	Client::Client(const Client& other)
	: _in_buffer(other._in_buffer),
	  _out_buffer(other._out_buffer),
	  _max_cmd_length(other._max_cmd_length),
	  _nick(other._nick),
	  _realname(other._realname),
	  _username(other._username),
	  _hostname(other._hostname),
	  _mode(other._mode),
	  _enteredPass(other._enteredPass),
	  _enteredNick(other._enteredNick),
	  _enteredUser(other._enteredUser),
	  _registered(other._registered),
	  _alive(other._alive),
	  _pinged(other._pinged),
	  _address(other._address),
	  _address_size(other._address_size),
	  _address_str(other._address_str),
	  _timeout(other._timeout),
	  _keep_alive(other._keep_alive),
	  _last_event_time(other._last_event_time),
	  _joined_channels(other._joined_channels),
	  _kick_reason(other._kick_reason)
	{
	}

	/* Copy assignment operator */
	Client&	Client::operator=(const Client& other)
	{
		if (this != &other)
		{
			this->_in_buffer = other._in_buffer;
			this->_out_buffer = other._out_buffer;
			this->_nick = other._nick;
			this->_realname = other._realname;
			this->_username = other._username;
			this->_hostname = other._hostname;
			this->_mode = other._mode;
			this->_enteredPass = other._enteredPass;
			this->_enteredNick = other._enteredNick;
			this->_enteredUser = other._enteredUser;
			this->_registered = other._registered;
			this->_alive = other._alive;
			this->_pinged = other._pinged;
			this->_address = other._address;
			this->_address_str = other._address_str;
			this->_address_size = other._address_size;
			this->_timeout = other._timeout;
			this->_keep_alive = other._keep_alive;
			this->_last_event_time = other._last_event_time;
			this->_joined_channels = other._joined_channels;
			this->_kick_reason = other._kick_reason;
		}
		return (*this);
	}

	/* Destructor */
	Client::~Client()
	{
	}

	void Client::setAddressStr(const std::string& address)
	{
		this->_address_str = address;
	}

	/* Getters ****************************************************************** */

	std::string	Client::getNick() const
	{
		if (this->isRegistered() && !this->_nick.empty())
			return (this->_nick);
		return ("*");
	}

	std::string	Client::getRealName() const
	{
		if (this->isRegistered() && !this->_realname.empty())
			return (this->_realname);
		return ("*");
	}

	std::string	Client::getUsername() const
	{
		if (this->isRegistered() && !this->_username.empty())
			return (this->_username);
		return ("*");
	}

	std::string	Client::getHostname() const
	{
		// return IP address if no host
		if (this->_hostname.empty())
			return (this->_address_str);
		return (this->_hostname);
	}

	std::string	Client::getMode() const
	{
		return (this->_mode);
	}

	std::string	Client::getEnteredPass() const
	{
		return (this->_enteredPass);
	}

	bool	Client::enteredNick() const
	{
		return (this->_enteredNick);
	}

	bool	Client::enteredUser() const
	{
		return (this->_enteredUser);
	}

	bool	Client::isRegistered() const
	{
		return (this->_registered);
	}

	bool	Client::isAlive() const
	{
		return (this->_alive);
	}

	bool	Client::isPinged() const
	{
		return (this->_pinged);
	}

	struct sockaddr_in&	Client::getAddress()
	{
		return (this->_address);
	}

	std::string	Client::getIpAddressStr() const
	{
		return (this->_address_str);
	}

	socklen_t&	Client::getAddressSize()
	{
		return (this->_address_size);
	}

	const Client::t_channels&	Client::getJoinedChannels() const
	{
		return (this->_joined_channels);
	}

	struct timeval&	Client::getLastEventTime()
	{
		return (this->_last_event_time);
	}

	Client::t_clients	Client::getAllContacts()
	{
		t_clients	contacts;

		for (t_channels::iterator it = this->_joined_channels.begin();
			 it != this->_joined_channels.end();
			 ++it)
		{
			contacts.insert(contacts.end(), (*it)->getClients().begin(), (*it)->getClients().end());
		}
		return (removeDuplicates(contacts, this));
	}

	const std::string&	Client::getKickReason() const
	{
		return (this->_kick_reason);
	}

	/* Setters ****************************************************************** */

	void	Client::setNick(const std::string& nick)
	{
		this->_nick = nick;
	}

	void	Client::setRealName(const std::string& realname)
	{
		this->_realname = realname;
	}

	void	Client::setHostname(const std::string& hostname)
	{
		this->_hostname = hostname;
	}

	void	Client::setUsername(const std::string& username)
	{
		this->_username = username;
	}

	void	Client::setEnteredPass(const std::string& enteredPass)
	{
		this->_enteredPass = enteredPass;
	}

	void	Client::setEnteredNick(bool enteredNick)
	{
		this->_enteredNick = enteredNick;
	}

	void	Client::setEnteredUser(bool enteredUser)
	{
		this->_enteredUser = enteredUser;
	}

	void	Client::setRegistered(bool registered)
	{
		this->_registered = registered;
	}

	void	Client::setAlive(bool alive)
	{
		this->_alive = alive;
	}

	void	Client::setPinged(bool pinged)
	{
		this->_pinged = pinged;
	}

	void	Client::setJoinedChannels(const t_channels& joined_channels)
	{
		this->_joined_channels = joined_channels;
	}

	/* Helpers ****************************************************************** */

	bool	Client::isTimeouted() const
	{
		struct timeval	now;

		gettimeofday(&now, NULL);
		return ((now.tv_sec - this->_last_event_time.tv_sec) > this->_keep_alive.tv_sec);
	}

	bool	Client::isOper() const
	{
		return (this->_mode.find("o") != std::string::npos);
	}

	bool	Client::isChanOp(Channel& channel)
	{
		return (channel.hasChanOp(*this));
	}

	bool	Client::isInvisible() const
	{
		return (this->_mode.find('i') != std::string::npos);
	}

	void	Client::kick(const std::string& reason)
	{
		this->_alive = false;
		this->_kick_reason = reason;
	}

	/* Connection handling ****************************************************** */

	/* Resets timeout and pinged */
	void Client::updateLastEventTime()
	{
		if (gettimeofday(&this->_last_event_time, NULL))
			throw std::runtime_error("gettimeofday() failed");
		this->_pinged = false;
	}

	/* Buffer operations ******************************************************** */

	bool	Client::hasUnprocessedCommands()
	{
		//if \r\n is in the in_buffer, return true
		return (this->_in_buffer.find("\n") != std::string::npos);
	}

	std::string	Client::popUnprocessedCommand()
	{
		std::string				endofline = CRLF;
		std::string::size_type	found = this->_in_buffer.find(CRLF);

		if (found == std::string::npos)
		{
			// found = this->_in_buffer.find("\n");
			endofline = "\n";
		}
		std::string	cmd = this->_in_buffer.substr(0, this->_in_buffer.find(endofline) + endofline.size());

		Logger	logger(DEBUG);

		logger.log(LOG_LEVEL_DEBUG, "Received: " + cmd);
		this->_in_buffer.erase(0, this->_in_buffer.find(endofline) + endofline.size());
		return (cmd);
	}

	int	Client::addToInBuffer(const std::string& str)
	{
		this->_in_buffer += str;
		return (0);
	}

	int Client::addToOutBuffer(const std::string& str)
	{
		this->_out_buffer += str;
		return (0);
	}

/*
	int	Client::updateOutBuffer()
	{
		int		ret;
		size_t	size = std::min(this->_out_buffer.size(), this->_max_cmd_length);

		if (!size)
			return (0);
		// write 512 bytes at most and removes them from the out_buffer
		ret = send(this->_socket_fd, this->_out_buffer.c_str(), size, 0);
		if (ret == -1)
		{
			std::cerr << __FILE__ ":" <<__LINE__ << " : " << "FATAL: poll()" << std::endl;
			throw std::runtime_error("send() failed");
		}
		this->_out_buffer.erase(0, ret);
		return (ret);
	}*/

	void	Client::sendCommand(std::string cmd)
	{
		this->_out_buffer += cmd;
	}

	/* Channel operations ******************************************************* */

	void	Client::joinChannel(Channel& channel)
	{
		channel.addClient(*this);
		this->_joined_channels.push_back(&channel);
	}

	void	Client::partChannel(Channel& channel)
	{
		channel.removeClient(*this);
		this->_joined_channels.remove(&channel);
	}

	/* The client quits all channels they joined */
	void	Client::quitAllChannels()
	{
		for (t_channels::iterator it = this->_joined_channels.begin();
			 it != this->_joined_channels.end();
			 ++it)
		{
			(*it)->removeClient(*this);
		}
		this->_joined_channels.clear();
	}

	/* Mode operations ********************************************************** */

	/* Adds the mode passed as parameter to the client mode string */
	int	Client::addMode(char mode_char)
	{
		std::string	valid_modes = "io";

		if (valid_modes.find(mode_char) != std::string::npos)
		{
			if (this->_mode.find(mode_char) == std::string::npos)
			{
				this->_mode += mode_char;
				return (1);
			}
			return (0);
		}
		return (ERR_UNKNOWNMODE);
	}

	/* Removes the mode passed as parameter from the client mode string */
	int	Client::removeMode(char mode_char)
	{
		std::string	valid_modes = "io";

		if (valid_modes.find(mode_char) != std::string::npos)
		{
			if (this->_mode.find(mode_char) != std::string::npos)
			{
				this->_mode.erase(this->_mode.find(mode_char), 1);
				return (1);
			}
			return (0);
		}
		return (ERR_UNKNOWNMODE);
	}

	/* ************************************************************************** */

	/* friend operator == */
	bool	operator==(const Client& lhs, const Client& rhs)
	{
		return (lhs._nick == rhs._nick && lhs._username == rhs._username && lhs._hostname == rhs._hostname
		&& lhs._alive == rhs._alive && lhs._last_event_time.tv_sec == rhs._last_event_time.tv_sec &&
		lhs._last_event_time.tv_usec == rhs._last_event_time.tv_usec);
	}
} // namespace ft_irc
