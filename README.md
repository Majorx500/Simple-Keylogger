# Simple Keylogger

This is a simple linux keylogger that uses the client-server architecture.

The client process will find the keyboard device and read the keys it presses. Then it will use linux sockets to send the input events to the server.

The server receives the input events, parses them, and both displays and saves them to a file. The output from the server should match the string typed on the client's device.


---

This repository is for educational purposes only. No contributors are to fault for any actions done by this program.
