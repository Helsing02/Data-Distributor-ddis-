#include "Handler.h"
#include <iostream>

IHandler::IHandler(std::mutex& mtx, 
                   std::condition_variable& cv,
             	   std::queue<PcapPacket>& pcktQueue,
             	   std::atomic<bool>& stopFlag,
             	   PcapGlobalHdr globalHdr,
             	   std::string& filePath)
    : m_mtx(mtx), m_cv(cv), 
    m_pcktQueue(pcktQueue), 
    m_stopFlag(stopFlag) {

   	// Open the output file for writing in binary mode
    m_outpFile.open(filePath, std::fstream::out | std::fstream::binary);

    if (!m_outpFile.is_open()) {
        std::cerr << "\033[31mОшибка файла:\033[0m Не удалось открыть файл для записи результата \"" << filePath << "\"\n";
        exit(1);
    }

    // Write the global header to the file
    m_outpFile.write((char*)&globalHdr, sizeof(globalHdr));
}

/**
 * Closes the output file if it is open.
 */
IHandler::~IHandler() {
    if (m_outpFile.is_open()) {
        m_outpFile.close();
    }
}

/**
 * This function is executed in a separate thread and calls the `process` method.
 * It is intended to be passed as an argument to `pthread_create`.
 */
void* IHandler::threadFunc(void* arg) {
    IHandler* self = static_cast<IHandler*>(arg);
    self->process();
    return nullptr;
}

/**
 * This method waits for packets to be available in the queue or for the stop flag to be set.
 * It extracts packets from the queue and calls `handlePckt` to process them.
 * 
 * The thread continues processing until the stop flag is set and the queue is empty.
 */
void IHandler::process() {
	while (!m_stopFlag || !m_pcktQueue.empty()) {
        std::unique_lock<std::mutex> lock(m_mtx);
        m_cv.wait(lock, [this]{ return !m_pcktQueue.empty() || m_stopFlag; });

        if (m_pcktQueue.empty()) {
            break;
        }

        PcapPacket packet = std::move(m_pcktQueue.front());
        m_pcktQueue.pop();
        lock.unlock();

        handlePckt(packet);
    }    
}