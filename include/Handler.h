#pragma once

#include <fstream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include "pcap_structs.h"

/**
 * @class IHandler
 * @brief Abstract base class for handling packets.
 *
 * @details This class defines the common interface and shared
 *          resources for packet handlers. It ensures 
 *          thread-safe access to the packet queue and 
 *          provides an entry point for handler threads.
 */
class IHandler {
protected:
    std::ofstream m_outpFile; ///< Output File stream for writing processed packets.
    std::mutex& m_mtx; ///< Mutex for synchronizing access to the queue.
    std::condition_variable& m_cv; ///< Condition variable for signaling new packets.
    std::queue<PcapPacket>& m_pcktQueue; ///< Queue holding packets for processing.
    std::atomic<bool>& m_stopFlag; ///< Atomic flag to signal handler termination.
    
    /// @brief Processing loop for handling packets. 
    virtual void process();
    
    /**
     * @brief Processes an individual packet.
     *
     * @param packet The packet to be processed.
     */
    virtual void handlePckt(PcapPacket&) = 0;
    
public:
    /**
     * @brief Constructor for IHandler.
     *
     * @param mtx Reference to a mutex for synchronization.
     * @param cv Reference to a condition variable for thread signaling.
     * @param pcktQueue Reference to the queue containing packets.
     * @param stopFlag Reference to an atomic flag for stopping execution.
     * @param globalHdr Global header for the output file.
     * @param filePath The path to file, that should be open by handler.
     */
    IHandler(std::mutex&, 
             std::condition_variable&,
             std::queue<PcapPacket>&,
             std::atomic<bool>&,
             PcapGlobalHdr,
             std::string&);
    /**
     * @brief Virtual destructor to ensure proper cleanup.
     */
    virtual ~IHandler();
    
    /**
     * @brief Entry point for handler threads.
     *
     * @param arg Pointer to the handler instance.
     * @return Always returns nullptr.
     */
    static void* threadFunc(void* arg);
};

/**
 * @class Handler1
 * @brief Specific handler implementation.
 *
 * @details Handler1 processes packets based on predefined 
 *          criteria and writes the results to an output file.
 */
class Handler1 : public IHandler {
    using IHandler::IHandler;
private:
    size_t m_packetNum = 0; ///< Counter for processed packets.
    
protected:
    /**
     * @brief Handles a specific packet.
     *
     * @param packet The packet to process.
     */
    void handlePckt(PcapPacket&) override;
};

/**
 * @class Handler2
 * @brief Specific handler implementation.
 *
 * @details Handler2 processes packets based on predefined 
 *          criteria and writes the results to an output file.
 */
class Handler2 : public IHandler {
    using IHandler::IHandler;
protected:
    /**
     * @brief Handles a specific packet.
     *
     * @param packet The packet to process.
     */
    void handlePckt(PcapPacket&)override;
};

/**
 * @class Handler3
 * @brief Specific handler implementation.
 *
 * @details Handler3 processes packets based on predefined 
 *          criteria and writes the results to an output file.
 */
class Handler3 : public IHandler {
    using IHandler::IHandler;
protected:
    /**
     * @brief Handles a specific packet.
     *
     * @param packet The packet to process.
     */
    void handlePckt(PcapPacket&)override;
};

