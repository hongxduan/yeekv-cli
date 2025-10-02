//
// Created by HongXing on 1/10/25.
//

#ifndef YEEKV_CLI_BYTE_UTIL_H
#define YEEKV_CLI_BYTE_UTIL_H
#include <cstdint>


namespace util {

    //
    // convert uint32_t to byte array in Little Endian
    //
    void uint32_to_le_bytes(uint32_t value, uint8_t bytes[4]);

    //
    // convert Big Endian byte array to uint32_t
    //
    uint32_t convertBeBytesToUint32(const uint8_t bytes[4]);

}

#endif //YEEKV_CLI_BYTE_UTIL_H