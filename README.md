# Data Distributor (ddist)

This project implements a data distribution system that processes packets from a `.pcap` file and distributes them to different handlers based on certain criteria. The handlers process the packets and write them to separate output files. The handlers also have specific rules for processing packets, including ignoring certain packets, modifying packet data, and writing only certain packets based on system time.

## Features

- **Packet Processing**: Reads packets from a `.pcap` file and distributes them to one of three handlers based on destination IP and port.
- **Multithreading**: Uses POSIX threads (pthread) to handle packet processing concurrently.
- **Packet Handling Rules**:
  - **Handler 1**: Ignores packets with destination port `7070` and writes the rest to `result_1.pcap`.
  - **Handler 2**: Modifies the packet if the L4 (Transport Layer) data contains the character `x` and writes the modified packet to `result_2.pcap`.
  - **Handler 3**: Writes TCP packets to `result_3.pcap` only if the current system time (in seconds) is even. For UDP packets, if the source port equals the destination port, the packet is written, and a log is printed.
- **Output**: Three output `.pcap` files are generated: `result_1.pcap`, `result_2.pcap`, `result_3.pcap`.

## Requirements

- Linux operating system
- C++ compiler (GCC)
- Make for building
- `pcap` file containing UDP or TCP packets for input (otherwise, undefined behavior will occur)

## Installation

### Prerequisites

Before you begin, ensure you have the following tools and libraries installed on your system:

- **C++ Compiler GCC**
- **Make**

To install these prerequisites, follow the instructions below for your Linux distribution.

### Cloning the Repository

```bash
git clone https://github.com/Helsing02/Data-Distributor-ddis-.git
cd Data-Distributor-ddis-
```

### Building the Project

Simply run:
```bash
make
```

## Running the Program

Once the build is complete, you can run the program with the following command (уou must be in the directory containing the binary file):

```bash
./ddist /path/to/your/input.pcap
```

The program will process the packets from the provided .pcap file and generate three output .pcap files: result_1.pcap, result_2.pcap, and result_3.pcap. These files will be placed in the same directory as the provided file.

## Documentation

To generate and view the documentation for this project, follow the steps below:

### 1. Generate Documentation

To generate the documentation using Doxygen, run the following command in your terminal:

```bash
make docs
```

This will process the Doxyfile configuration file located in the configs directory and generate the documentation in the docs directory.

### 2. Accessing the Documentation

Once the documentation is generated, you can access it.
Open the index.html file located in the docs/html directory in your web browser. You can do this by navigating to the directory and opening the file:

```bash
cd documentation/html
open index.html   # Or just double-click on the file
```

