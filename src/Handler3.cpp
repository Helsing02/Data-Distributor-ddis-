#include "Handler.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>

/**
 * If the packet is a TCP packet, the handler waits for 2 seconds and checks
 * if the current system time (in seconds) is even. If it is even, the packet
 * is written to the output file. 
 * 
 * If the packet is a UDP packet and the source port is equal to the destination port,
 * the packet is written to the output file, and a message is printed to stdout indicating 
 * the matching ports.
 */
void Handler3::handlePckt(PcapPacket& packet) {
    const uint8_t TCP_PROTOCOL = 0x06;

    // Handle TCP packet
    if (packet.ipHdr.protocol == TCP_PROTOCOL) {
        timespec ts;
        ts.tv_sec = 2;       // Seconds to wait
        ts.tv_nsec = 0;      // Nanoseconds to wait
        nanosleep(&ts, nullptr);

        // Check if current system time is even
        if (!(time(NULL) & 1)) {
            // Write packet to output file if time is even
            m_outpFile.write((char*)&packet.pcapHdr, sizeof(packet.pcapHdr));
            m_outpFile.write(reinterpret_cast<const char*>(packet.data.data()), packet.pcapHdr.inclLen);
        }
    } else if (packet.udpHdr.srcPort == packet.udpHdr.destPort) {
        // Handle UDP packets with matching source and destination port
        // Write packet to output file
        m_outpFile.write((char*)&packet.pcapHdr, sizeof(packet.pcapHdr));
        m_outpFile.write(reinterpret_cast<const char*>(packet.data.data()), packet.pcapHdr.inclLen);

        // Print a message indicating matching ports
        std::cout << "\033[32mОбработчик 3:\033[0m Найдено совпадение port = " << packet.udpHdr.srcPort << std::endl;
    }
}