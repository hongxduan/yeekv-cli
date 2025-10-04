//
// Created by HongXing on 1/10/25.
//

#include <bit>
#include "byte_util.h"


void util::uint32_to_le_bytes(uint32_t value, uint8_t bytes[4]) {
    if constexpr (std::endian::big == std::endian::native) {
        value = std::byteswap(value);
    }
    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = value & 0xFF;
}

void util::uint16_to_le_bytes(uint16_t value, uint8_t bytes[2]) {
    if constexpr (std::endian::big == std::endian::native) {
        value = std::byteswap(value);
    }
    bytes[0] = (value >> 8) & 0xFF;
    bytes[1] = value & 0xFF;
}

uint32_t util::convertBeBytesToUint32(const uint8_t bytes[4]) {
    return bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3];
}
