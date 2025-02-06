#pragma once

#include <fstream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include "pcap_structs.h"

class IHandler;

/**
 * @class Distributor
 * @brief Distributes network packets to appropriate handlers.
 *
 * @details This class is responsible for receiving packets
 *          and distributing them among different handlers 
 *          based on predefined rules.
 */
class Distributor {
public:
    /**
     * @brief Constructs a Distributor instance.
     *
     * @param globalHdr The global PCAP header for output 
     *                  files.
     * @param fileDir The directory containing the input file.
     */
     
    Distributor(PcapGlobalHdr, std::string);
    /**
     * @brief Destructor for the Distributor.
     *
     * @details Ensures proper cleanup by stopping all handlers
     *          and releaseng allocated memory.
     */
    ~Distributor();
    
    /**
     * @brief Distributes a given packet to the appropriate 
     *        handler.
     *
     * @param packet The packet to be distributed to the handlers.
     */
    void distrPacket(PcapPacket);
    
    /// Launches handlers threats.
    void start();
    /**
     * @brief Joins handlers threats.
     *
     * @details This function signals all handlers that no more
     *          new packets will be received. Then waits 
     *          for the handlers to finish processing 
     *          the remaining packets.
     */
    void stop();
    
private:
    IHandler* m_handlers[3]; ///< Array of handler objects.
    pthread_t m_threads[3];  ///< Array of threads for handlers.
    
    std::mutex m_handler1_mtx, m_handler2_mtx, m_handler3_mtx; ///< Mutexes for thread synchronization.
    std::condition_variable m_handler1_cv, m_handler2_cv, m_handler3_cv; ///< Condition variables for synchronization.
    std::queue<PcapPacket> m_handler1_queue, m_handler2_queue, m_handler3_queue; ///< Queues for packet transmission.
    std::atomic<bool> m_stopFlag; ////< Flag to signal handlers that no new packets will arrive.
};

