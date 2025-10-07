//
// Created by HongXing on 1/10/25.
//

#include <bit>
#include "byte_util.h"

#include <iostream>
#include <ostream>


void util::int32_to_bytes(int32_t value, uint8_t bytes[4]) {
    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = value & 0xFF;
}

void util::uint32_to_bytes(uint32_t value, uint8_t bytes[4]) {
    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = value & 0xFF;
}

void util::uint16_to_bytes(uint16_t value, uint8_t bytes[2]) {
    bytes[0] = (value >> 8) & 0xFF;
    bytes[1] = value & 0xFF;
}

uint32_t util::bytes_to_uint32(const uint8_t *bytes) {
    // convert bytes to int first
    const uint32_t result = bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3];
    //std::memcpy(&reuslt, bytes, sizeof(result));

    return result;
}

int32_t util::bytes_to_int32(const uint8_t *bytes) {
    // convert bytes to int first
    const uint32_t result = bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3];
    //std::memcpy(&reuslt, bytes, sizeof(result));

    return result;
}

