# Bare-NS, a bare-metal network stack.

- [Overview](#overview)
- [Limitations](#limitations)
- [Architecture](#architecture)

## Overview

Bare-NS is a simple network stack intended for baremetal targets. It requires some memory and string related standard library functions such as *memcpy* and *strlen*, but aside from that it is self-contained. An upshot of this is that it doesn't use a heap. This requires that many features such as frame and packet size be upper-bounded to avoid buffer overflows. 

Currently the stack supports the following protocols:

### Data Link Layer
- ARP

### Network Layer
- IPv4

### Transport Layer
- TCP
- UDP

### Application Layer
- HTTP 1.0

## Limitations

This is toy network stack, so it doesn't support many of the features that real network stacks would support. Here is an incomplete list of some of the limitations, organized by module.

### TCP
- retransmission not implemented.
- segments must be received in order, potentially forcing retransmission from sender.

### IPv4
- packet fragmentation not implemented.

## Architecture

Protocol modules in the stack implement a common interface. Every module added the the stack implements a `deliver` function to handle the receipt of data of that particular protocol. Most modules will also implement a `send` function or something similar for transmitting data of that particular protocol. To distinguish these functions across modules, the protocol name is prefixed to its respective function, e.g, `tcp_deliver`.

Each protocol implementation is typically split into 1-3 sources: 

1. `<protocol name>_in.c`
2. `<protocol name>_out.c`
3. `<protocol name>.c`

With this kind of separation, modules for protocols that encapsulate or are encapsulated by other protocols form two separate dependency chains--a chain of modules that implement receipt of data and a chain of modules that implement sending of data-- rather than a single chain full of circular dependencies. The first file contains the implementation of `deliver`, while the second contains the implementation of `send`. The third is included as needed to implement more complex behavior or to share logic across the first two sources.

Received data propogates up the stack through subsequent `deliver` function calls until it reaches its destination. Application layer users can access delivered data using the familiar socket interface.

To integrate this network stack into a system, implement the functions in `net_interface.c` to interact with an NIC.

- `net_rx` should receive a pointer to data received by the NIC and deliver that data to a module for a layer 2 or layer 3 protocol. All other modules implement delivery.

- `net_tx` should transmit a buffer of data using the NIC.

- `net_mac_address` should return the NIC's MAC address.




