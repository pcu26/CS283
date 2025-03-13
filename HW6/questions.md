1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client knows when a command's output is received from the server when it checks for a special EOF character. In this case, the server will send RDSH_EOF_CHAR to the end of every response, which signals that the output is finished. Since data over network doesn't arrive all at once all the time, the client has to keep reading in a loop until the special character is seen. To handle partial reads, the client will store received data in a buffer and will process it in chunks until the full message is received. This ensures that the command output is large or split into multiple packets.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

Since TCP is a stream-based protocol and it does not automatically separate messages, a networked shell has to define start and end markers for every command that is sent. If the message boundaries are not handled correctly, commands may be split the wrong way across multiple reads, which causes incorrect execution. It also might execute incomplete commands or fail to recognize when one command ends and when another begins.

3. Describe the general differences between stateful and stateless protocols.

Stateful and stateless protocols differ in how they handle and maintain information about past interactions with the server. A stateful protocol keeps track of previous requests and responses. That maintains a session with the client, allowing for continuous interactions. A stateless protocol acts as if each request is independent, with no memory of past interactions.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

Even though UDP is considered "unreliable", it is still widely used because it is fast and efficient compared to TCP. It has much lower latency, making it ideal for applications like video streaming and online gaming, where speed is more important and making sure that the packets arrive.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The operating system provides sockets as the main interface for applications to use network communications. Sockets allow programs to send and receive data over networks using standard protocols. It provides a way for applications to communicate over the internet or local networks without the need to manage low-level details of the network hardware.