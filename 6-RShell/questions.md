1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client determines when a command's output is fully recieved from the server by using the EOF character. Another techinique that could be used to handle partial reads is by confirming receipt of completed messages, also called application-layer acknowledgments.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

A networked shell protocol has to define boundaries explicitly using the EOF character and newline separation. If this is not handled correctly it could lead to partially recieved commands or multiple commands being treated as one.

3. Describe the general differences between stateful and stateless protocols.

Stateful protocols maintain a session state and the server tracks client interactions. Whereas stateless protocols does not maintain a session state and instead each request is independent and contains full context. Stateful is harder to scale than stateless.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

We use UDP when we are prioritizing speed over reliability, like when live-streaming or gaming.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The operating system provides the socket API for network communications.