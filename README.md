```markdown
*This project has been created as part of the 42 curriculum by rmarrero, aybelhaj.*

# ft_irc

## Overview

A robust, fully non-blocking IRC (Internet Relay Chat) server engineered from scratch in C++98. Built around a single `poll()` event loop, the server efficiently manages concurrent client connections, standard IRC channel operations, privilege controls, and network I/O without blocking or process forking. Fully compliant with RFC specifications and validated against reference clients like **irssi** and standard TCP utilities (`nc`).

---

## Core Requirements & Architecture

- **Standard & Compilation:** Strict C++98 compliance. Clean compilation under `-Wall -Wextra -Werror`.
- **I/O Multiplexing:** 100% non-blocking sockets using `fcntl(..., O_NONBLOCK)`. A single `poll()` loop handles `accept`, read, write, and disconnect events simultaneously.
- **Zero Process Overhead:** Single-threaded design; never forks processes or spawns additional threads.
- **Protocol & Transport:** Operates over TCP/IPv4, handling stream fragmentation, partial buffer reads, and queued output buffers smoothly.

---

## IRC Command Suite

### Connection & Registration
- `CAP` — Capability negotiation for client compatibility (irssi/HexChat).
- `PASS` — Authenticates the connection using the server password.
- `NICK` — Assigns or updates a unique user nickname.
- `USER` — Completes registration by setting username and real name.
- `QUIT` — Gracefully closes the connection with an optional quit message.
- `PING` / `PONG` — Keep-alive heartbeat mechanism.

### Channel Management & Messaging
- `JOIN` — Joins an existing channel or creates one (granting initial Operator privileges).
- `PART` — Leaves one or multiple specified channels.
- `PRIVMSG` — Transmits private messages to individual users or entire channels.
- `TOPIC` — Queries or updates the current channel topic.
- `LIST` — Lists visible channels alongside their user counts and topics.
- `WHO` / `WHOIS` — Retrieves detailed user and channel membership metadata.

### Operator Control (`MODE` & Channel Admin)
- `KICK` — Ejects a targeted user from a channel (Ops only).
- `INVITE` — Grants a user entry to an invite-only channel.
- `MODE` — Configures channel parameters and access controls:
  - `i` — Invite-only toggle.
  - `t` — Restrict `TOPIC` changes to channel operators.
  - `k` — Set/remove channel join password (key).
  - `o` — Grant or revoke operator status for a user.
  - `l` — Set/remove user limits for a channel.

---

## Code Optimizations & Bonus Features

- **Robust Buffer Management:** Custom input/output buffer queues per socket to handle incomplete incoming frames and prevent partial write blocks.
- **Enhanced Signal Handling:** Graceful shutdown on `SIGINT` / `SIGQUIT` ensuring clean resource deallocation and proper client notification.
- **Memory & Resource Leak Safety:** Zero leaks verified with `Valgrind`; complete cleanup of disconnected client objects, channels, and file descriptors.
- **File Transfer Support (DCC):** Direct Client-to-Client message handling enabling direct file sharing between users across IRC clients.
- **Bot Integration:** Dedicated automated IRC bot endpoint built-in for utility commands and live server status queries.

---

## Getting Started

### Compilation
```bash
make

```

*(Available targets: `make`, `make clean`, `make fclean`, `make re`)*

### Execution

Launch the server specifying the target listening port and access password:

```bash
./ircserv <port> <password>

```

### Connecting

**Via Netcat (`nc`):**

```bash
nc <IP> <PORT>

```

**Via Irssi Client:**

```bash
irssi
/connect <IP> <PORT> <password>

```

---

## Resources & Documentation

RFC 1459 — Internet Relay Chat Protocol — Original IRC protocol architecture and command specs.

RFC 2812 — Internet Relay Chat: Client Protocol — Updated specifications for registration, channel management, and messages.

RFC 2813 — Internet Relay Chat: Server Protocol — Details on server state, privilege flags, and error numeric responses.

Modern IRC Client Protocol Documentation — Comprehensive breakdown of real-world IRC behavior, client capability negotiation (CAP), and client quirks.

**How AI was used:** AI tools served as pair-programming assistants throughout development. Key contributions included auditing socket state handling, debugging edge cases (such as unhandled `SIGPIPE` crashes, `QUIT` use-after-free conditions, and uncleaned channel references upon sudden disconnects), clarifying ambiguous RFC behaviors, and generating stress-test shell scripts (`valgrind`, raw sockets, parallel `nc` clients). All generated code and suggestions were manually reviewed, adapted, and tested prior to integration.

---

## Contributors

* [rmarrero](https://www.google.com/search?q=https://profile.intra.42.fr/users/rmarrero)
* [aybelhaj](https://www.google.com/search?q=https://profile.intra.42.fr/users/aybelhaj)

```

```