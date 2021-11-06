/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 14:46:41 by root              #+#    #+#             */
/*   Updated: 2021/11/06 17:42:19 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

namespace ft_irc
{

	Logger::Logger(void)
	{
		_log_level = 0;
	}

	Logger::Logger(int log_level)
	{	
		_log_level = log_level;
	}

	Logger::Logger(const Logger &src)
	{
		*this = src;
	}
	
	Logger::~Logger(void)
	{
	}

	Logger &Logger::operator=(const Logger &rhs)
	{
		if (this != &rhs)
		{
			_log_level = rhs._log_level;
		}
		return *this;
	}

	
	std::string	str_replace(std::string subject, const std::string &search, const std::string &replace)
	{
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos)
		{
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
		return (subject);
	}

	void Logger::log(int level, std::string message) const
	{
		const char				*log_level_str[] = {"DEBUG", "INFO", "WARNING", "ERROR", "FATAL"};
		const char				*log_level_color[] = {"\033[0;36m", "\033[0;32m", "\033[0;33m", "\033[0;31m", "\033[0;35m"};
		const char				*toReplace = "\n\r\t";
		const char				*replaceWith[] = {"\\n", "\\r", "\\t"};
		std::string				copy;

		if (level < _log_level)
			return ;
		copy = message;
		for (int i = 0; i < 3; i++)
		{
			copy = str_replace(copy, toReplace, replaceWith[i]);
		}
		std::cout << log_level_color[level] << "[" << log_level_str[level] << "] " << copy << "\033[0m" << std::endl;
	}
}
