#include "Distributor.h"
#include "Handler.h"
#include "Utilities.h"

#include <iostream>
#include <pthread.h>

Distributor::Distributor(PcapGlobalHdr globalHdr, std::string fileDir)
    : m_stopFlag(false) {
    std::string result1Path = fileDir + "/result_1.pcap";
    std::string result2Path = fileDir + "/result_2.pcap";
    std::string result3Path = fileDir + "/result_3.pcap";

    m_handlers[0] = new Handler1(m_handler1_mtx, m_handler1_cv, 
                                 m_handler1_queue, m_stopFlag, 
                                 globalHdr, result1Path);
    m_handlers[1] = new Handler2(m_handler2_mtx, m_handler2_cv, 
                                 m_handler2_queue, m_stopFlag, 
                                 globalHdr, result2Path);
    m_handlers[2] = new Handler3(m_handler3_mtx, m_handler3_cv, 
                                 m_handler3_queue, m_stopFlag, 
                                 globalHdr, result3Path);
}

Distributor::~Distributor() {
    if (!m_stopFlag) {
        stop();
    }

    for (size_t i = 0; i < 3; i++) {
        delete m_handlers[i];
    }
}

/**
 * @details This method checks the destination IP and port of the packet and forwards it
 *          to one of the three handlers. The decision is based on:
 *          - Handler 1: destination IP in the range 11.0.0.3 to 11.0.0.200.
 *          - Handler 2: destination IP in the range 12.0.0.3 to 12.0.0.200 and destination port 8080.
 *          - Handler 3: all other packets.
 */
void Distributor::distrPacket(struct PcapPacket packet) {
    uint32_t destIp = changeEndian(packet.ipHdr.destIp);

    if ( destIp >= 0xB000003 && destIp <= 0xB0000C9) { // Handler 1
        std::unique_lock<std::mutex> lock(m_handler1_mtx);  
        m_handler1_queue.push(packet);    // Adds packet to the queue of Handler 1.
        m_handler1_cv.notify_one();       // Notifies the handler that a new packet is available.
        lock.unlock();
    } else if (destIp >= 0xC000003 && destIp <= 0xC0000C9 && 
               changeEndian(packet.tcpHdr.destPort) == 8080) {// Handler 2
        std::unique_lock<std::mutex> lock(m_handler2_mtx);
        m_handler2_queue.push(packet);    // Adds packet to the queue of Handler 2.
        m_handler2_cv.notify_one();       // Notifies the handler that a new packet is available.
        lock.unlock();
    } else { // Handler 3
        std::unique_lock<std::mutex> lock(m_handler3_mtx);
        m_handler3_queue.push(packet);    // Adds packet to the queue of Handler 3.
        m_handler3_cv.notify_one();       // Notifies the handler that a new packet is available.
        lock.unlock();
    }
}

/**
 * Creates and launches threads for all three handlers to begin processing
 * packets concurrently. Each handler runs in its own thread.
 */
void Distributor::start() {
    for (size_t i = 0; i < 3; i++) {
        pthread_create(&m_threads[i], nullptr, 
                       IHandler::threadFunc, 
                       m_handlers[i]);            
    }
}

/** 
 * Sets the stop flag, notifies the handlers to wake up, and waits for all
 * handler threads to finish processing before returning.
 */
void Distributor::stop() {
    m_stopFlag.store(true);        // Set the stop flag to true, signaling handlers to stop.
    m_handler1_cv.notify_one();    // Notifies Handler 1 to wake up and check for the stop flag.
    m_handler2_cv.notify_one();    // Notifies Handler 2 to wake up and check for the stop flag.
    m_handler3_cv.notify_one();    // Notifies Handler 3 to wake up and check for the stop flag.

    for (size_t i = 0; i < 3; i++) {
        pthread_join(m_threads[i], nullptr);  // Waits for all handler threads to finish.
    }
}