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

