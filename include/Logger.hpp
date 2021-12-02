#ifndef LOGGER_HPP
# define LOGGER_HPP
# include <string>
# include <iostream>
# define CRLF "\r\n"
# define CRLF_PRINTABLE "\\r\\n"

enum	LogLevel
{
	LOG_LEVEL_DEBUG = 0,
	LOG_LEVEL_INFO = 1,
	LOG_LEVEL_WARNING = 2,
	LOG_LEVEL_ERROR = 3,
	LOG_LEVEL_FATAL = 4
};

namespace ft_irc
{
	/*
	 * Logging helper class
	 */
	class Logger
	{
		private:
			int		_log_level;

		public:
			/* Default constructor */
					Logger(void);
			/* Level constructor */
					Logger(int log_level);
			/* Copy constructor */
					Logger(const Logger& other);
			/* Copy assignment operator */
					Logger&	operator=(const Logger& other);
			/* Destructor */
					~Logger(void);

			/* Display log message */
			void	log(int level, const std::string& message) const;
			/* Call overload */
			void	operator()(int level, const std::string& message) const;
	};
} // !namespace ft_irc

#endif // !LOGGER_HPP
