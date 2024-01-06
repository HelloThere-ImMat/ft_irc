## ft_irc

### 🗺️ Project Overview
The ft_irc project involves building an IRC server. The server manages connections between clients, facilitates communication, and handles user interactions within channels. Clients connect to the server to join channels, exchange messages, and interact with other users.

### 🌟 Key Features
Server-Client Architecture: Implements a server-client model for managing connections and facilitating communication between users.

IRC Protocol: Adheres to the IRC protocol for handling client-server interactions, commands, and message formats.

Multiple Channels: Supports the creation and management of multiple chat channels where users can join, send messages, and interact.

User Authentication: Implements user authentication to ensure secure access to the server and channel communication.

Command Handling: Processes various IRC commands that users can send to the server to perform actions, here the list:

- NICK (set the nickname of the user) 
- PRIVMSG (send messages to client(s) or channel(s))
- TOPIC (set the topic of a channel)
- INVITE (invite specific user for joining a channel)
- KICK (kick specific user from a channel)
- MODE (only for channels) :
  -  i (set/unset invite-only)
  -  k (set/unset password)
  -  l (set/unset user limit)
  -  o (set/unset a user as operator)
  -  t (set/unset topic restriction)

### 🚀 How to Use
To compile and run the ft_irc server and client, follow these steps:

#### Compilation:

```shell
make
```

#### Start Server:

```shell
./ircserv [port] [password]
```

Replace [port] with the port number on which the server will listen for incoming connections and [password] with the password that clients will need to enter to connect to it.

It is recommended to use irssi.
