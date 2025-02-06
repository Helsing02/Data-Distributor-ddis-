#include <iostream>
#include <fstream>
#include <cstring>
#include "pcap_structs.h"
#include "Distributor.h"

#include "Utilities.h"

const uint8_t TCP_PROTOCOL = 0x06;
const uint8_t UDP_PROTOCOL = 0x11;

/**
 * @brief Parses command-line arguments and returns the file path.
 * @param argc Number of arguments.
 * @param argv Argument values.
 * @return Path to the PCAP file.
 */
std::string argParse(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "USAGE: " << argv[0] << " <pathToFile>\n";
        exit(1);
    }

    return std::string(argv[1]);
}

/**
 * @brief Validates if the file has a .pcap suffix.
 * @param pathToFile The file path to check.
 * @return True if the file has a valid .pcap suffix.
 */  
bool hasPcapSuffix(std::string pathToFile) {
    return pathToFile.size() >= 5 && 
           pathToFile.substr(pathToFile.size() - 5) == ".pcap";
}

std::string getDirectory(const std::string& pathToFile) {
    size_t lastSlash = pathToFile.find_last_of("/");
    if (lastSlash == std::string::npos) {
        return "."; // file in current directory
    }
    return pathToFile.substr(0, lastSlash);
}

/**
 * @brief Reads and validates the PCAP global header.
 * @param pcapFs Input file stream.
 * @return The read PcapGlobalHdr.
 */
PcapGlobalHdr readPcapHeader(std::ifstream& pcapFs) {
    PcapGlobalHdr globalHdr;
    pcapFs.read((char*)&globalHdr, sizeof(globalHdr));

    if (globalHdr.magicNumber != 0xA1B2C3D4) {
        std::cerr << "\033[31mОшибка формата:\033[0m Некорректная структура заголовка pcap.\n";
        pcapFs.close();
        exit(1);
    }
    return globalHdr;
}

/**
 * @brief Parses a single packet from the file stream.
 * @param pcapFs Input file stream.
 * @return Parsed PcapPacket.
 */
PcapPacket readPacket(std::ifstream& pcapFs) {
    PcapPacket packet;
    pcapFs.read((char *)&packet.pcapHdr, sizeof(packet.pcapHdr));

    packet.data.resize(packet.pcapHdr.inclLen);
    pcapFs.read(reinterpret_cast<char*>(packet.data.data()), packet.pcapHdr.inclLen);

    memcpy(&packet.ethHdr, &packet.data, sizeof(packet.ethHdr));
    memcpy(&packet.ipHdr, &packet.data[sizeof(packet.ethHdr)], sizeof(packet.ipHdr));

    if (packet.ipHdr.protocol == TCP_PROTOCOL) { 
        memcpy(&packet.tcpHdr, &packet.data[sizeof(packet.ethHdr) + sizeof(packet.ipHdr)], sizeof(packet.tcpHdr));
    } else if (packet.ipHdr.protocol == UDP_PROTOCOL) {
        memcpy(&packet.udpHdr, &packet.data[sizeof(packet.ethHdr) + sizeof(packet.ipHdr)], sizeof(packet.udpHdr));
    } else {
        std::cerr << "\033[31mОшибка протокола:\033[0m Неподдерживаемый протокол (номер протокола: " << std::hex << "0x" << (int) packet.ipHdr.protocol << std::dec << "). Ожидался TCP (0x06) или UDP (0x11)." << std::endl;
        pcapFs.close();
        exit(1);
    }
    return packet;
}

/**
 * @brief Reads and processes packets from a PCAP file.
 * @param pcapFs Input file stream.
 * @param distributor Distributor instance.
 */
void processPcapFile(std::ifstream& pcapFs, Distributor& distributor) {
    while (pcapFs.peek() != EOF) {
        distributor.distrPacket(readPacket(pcapFs));
    }
}

int main(int argc, char* argv[]) {
    std::string pathToFile = argParse(argc, argv);
    if (!hasPcapSuffix(pathToFile)) {
        std::cerr << "\033[31mОшибка файла:\033[0m Неверный суффикс. Ожидался .pcap.\n";
        return 1;
    }    
    
    std::ifstream pcapFs;
    pcapFs.open(pathToFile, std::fstream::in | std::fstream::binary);

    if (!pcapFs.is_open()) {
        std::cout << "\033[31mОшибка файла:\033[0m Не удалось открыть файл " << pathToFile << std::endl;
        return 1;
    }

    std::string fileDir = getDirectory(pathToFile);

    Distributor distributor(readPcapHeader(pcapFs), fileDir);
    distributor.start();

    processPcapFile(pcapFs, distributor);

    pcapFs.close();
    return 0;
}
