#ifndef LOGGER_HPP
# define LOGGER_HPP
# include <string>
# include <iostream>
# define CRLF "\n"
# define CRLF_PRINTABLE "\\n"

enum LogLevel {
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_INFO,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_FATAL
};

namespace ft_irc
{
	class Logger
	{
		private:
			int _log_level;
		public:
					Logger(void);
					Logger(int log_level);
					Logger(const Logger &src);
					Logger &operator=(const Logger &src);
					~Logger(void);
			void	log(int level, std::string message) const;
	};
}

#endif
