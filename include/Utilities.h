#pragma once

#include <cstdint>

/**
 * @brief Changes the endianness of a 32-bit value.
 * 
 * @param value The 32-bit value whose byte order is to be 
 *              changed.
 * @return The value with its byte order changed.
 *
 * @details This function swaps the byte order of a 32-bit 
 *          integer, converting it from little-endian to 
 *          big-endian or vice versa.
 */
inline uint32_t changeEndian(uint32_t value) {
    return ((value << 24) & 0xFF000000) |
           ((value << 8) & 0x00FF0000) |
           ((value >> 8) & 0x0000FF00) |
           ((value >> 24) & 0x000000FF);
}


/**
 * @brief Changes the endianness of a 16-bit value.
 * 
 * @param value The 16-bit value whose byte order is to be 
 *              changed.
 * @return The value with its byte order changed.
 *
 * @details This function swaps the byte order of a 16-bit 
 *          integer, converting it from little-endian to 
 *          big-endian or vice versa.
 */
uint16_t inline changeEndian(uint16_t value) {
    return (value << 8) | (value >> 8);
}

