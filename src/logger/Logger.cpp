/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 14:46:41 by root              #+#    #+#             */
/*   Updated: 2021/12/02 18:02:59 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

namespace ft_irc
{
	/* Default constructor */
	Logger::Logger(void)
	: _log_level(0)
	{
	}

	/* Level constructor */
	Logger::Logger(int log_level)
	: _log_level(log_level)
	{
	}

	/* Copy constructor */
	Logger::Logger(const Logger& other)
	{
		*this = other;
	}

	/* Copy assignment operator */
	Logger&	Logger::operator=(const Logger& other)
	{
		if (this != &other)
			_log_level = other._log_level;
		return (*this);
	}

	/* Destructor */
	Logger::~Logger(void)
	{
	}

	/* Helper */
	std::string	str_replace(std::string subject, const std::string& search, const std::string& replace)
	{
		size_t	pos = 0;

		while ((pos = subject.find(search, pos)) != std::string::npos)
		{
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
		return (subject);
	}

	/* Display log message */
	void	Logger::log(int level, const std::string& message) const
	{
		const char*	log_level_str[] = {"DEBUG", "INFO", "WARNING", "ERROR", "FATAL"};
		const char*	log_level_color[] = {"\033[0;36m", "\033[0;32m", "\033[0;33m", "\033[0;31m", "\033[0;35m"};
		const char*	toReplace[] = {"\n", "\r", "\t"};
		const char*	replaceWith[] = {"\\n", "\\r", "\\t"};
		std::string	copy;

		if (level < _log_level)
			return ;

		copy = message;
		for (int j = 0 ; j < 3 ; j++)
		{
			for (int i = 0; i < 3; i++)
				copy = str_replace(copy, toReplace[i], replaceWith[i]);
		}
		std::cerr << log_level_color[level] << "[" << log_level_str[level] << "] "
				  << copy << "\033[0m" << std::endl;
	}

	/* Call overload */
	void	Logger::operator()(int level, const std::string& message) const
	{
		this->log(level, message);
	}
} // namespace ft_irc
