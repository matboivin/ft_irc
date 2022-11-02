# ft_irc

`ft_irc` is a team school project of 42.

> The objective is to implement a simple C++98 IRC server. The list of available commands can be found in the [Wiki](https://gitlab.com/t4778/ft_irc/-/wikis/command-list).

42 is a private, non-profit and tuition-free Computer Science school based on peer-to-peer learning and hands-on projects.

## Requirements

- Git
- Make
- A C++ compiler

## Getting started

1. Clone the repository and its dependencies:

```console
$ git clone https://gitlab.com/t4778/ft_irc
```

2. Change it to your working directory and run:

```console
$ make
```

3. Start the server:

```console
$ ./ircserv <port> <password>
```

- `port`: the port number on which your server will accept incoming connections
- `password`: the password needed by any IRC client that wants to connect to your server

4. Connect to the client using an IRC client such as [`irssi`](https://irssi.org/download/) which is the client used as reference for developping this server.

## Run tests

```console
$ make debug
$ make debug_replies
$ make debug_parsing
```

## Authors

- [nmaccary](https://gitlab.com/Dirty-No)
- [mboivin](https://gitlab.com/mboivin)
