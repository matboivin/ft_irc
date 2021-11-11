#ifndef LOGGER_HPP
# define LOGGER_HPP
# include <string>
# include <iostream>
# define CRLF "\n"
# define CRLF_PRINTABLE "\\n"

enum LogLevel
{
	LOG_LEVEL_DEBUG = 0,
	LOG_LEVEL_INFO = 1,
	LOG_LEVEL_WARNING = 2,
	LOG_LEVEL_ERROR = 3,
	LOG_LEVEL_FATAL = 4
};

namespace ft_irc
{
	class Logger
	{
		private:
			int		_log_level;

		public:
					Logger(void);
					Logger(int log_level);
					Logger(const Logger& src);
					Logger&	operator=(const Logger& src);
					~Logger(void);
			void	log(int level, const std::string& message) const;
	};
}

#endif
