# Local Socket Communication

This repository contains C programs for local socket communication between a client and a server on Unix-like systems.

## Files

- **client.c**: Sends messages to the server. Type `quit` to exit.
- **server.c**: Receives and displays messages from the client. Exits on `quit`.

## Prerequisites

- Unix-like OS (Linux, macOS)
- C compiler (e.g., GCC)

## Compilation

```sh
gcc -o client client.c
gcc -o server server.c
```

1. **Start the Server:**

    ``sh
    ./server
    ```

2.**Start the Client:**

    ``sh
    ./client
    ```
3.**Communication:**
 - Type a message in the client terminal.
 - The server displays the message.
 - Type quit to stop.