#ifndef COMMAND_HPP
# define COMMAND_HPP

namespace ft_irc
{
	class Message;
	//command interface
	class Command
	{
		public:
		virtual ~Command() {}
		virtual void execute(Message &msg) = 0;
	};
}
#endif // !COMMAND_HPP
