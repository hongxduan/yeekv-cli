//
// Created by HongXing on 1/10/25.
//

#ifndef YEEKV_CLI_BYTE_UTIL_H
#define YEEKV_CLI_BYTE_UTIL_H
#include <cstdint>


namespace util {
    void int32_to_bytes(int32_t value, uint8_t bytes[4]);

    //
    // convert uint32_t to byte array in Little Endian
    //
    void uint32_to_bytes(uint32_t value, uint8_t bytes[4]);

    //
    // convert uint16_t to byte array in Little Endian
    //
    void uint16_to_bytes(uint16_t value, uint8_t bytes[2]);

    //
    // convert little endian bytes to uint32
    //
    uint32_t bytes_to_uint32(const uint8_t *bytes);

    //
    // convert little endian bytes to uint32
    //
    int32_t bytes_to_int32(const uint8_t *bytes);
}

#endif //YEEKV_CLI_BYTE_UTIL_H
