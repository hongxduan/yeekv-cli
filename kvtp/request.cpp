//
// Created by HongXing on 1/10/25.
//

#include "request.h"

#include <iostream>
#include <ostream>
#include <list>

#include "../util/byte_util.h"

std::vector<uint8_t> kvtp::encode_request(const InputData &data) {
    std::list<uint8_t> kvtp_bytes;
    std::vector<uint8_t> request;

    // Protocol
    // Cast the #define string to string is to remove trailing \0 char
    kvtp_bytes.append_range(static_cast<std::string>(PROTOCOL));
    kvtp_bytes.push_back(LINE_FEED);

    // CMD: line
    kvtp_bytes.append_range(static_cast<std::string>(CMD_PREFIX));
    for (const auto c: data.cmd) {
        if (c != ZERO) {
            kvtp_bytes.push_back(c);
        }
    }
    kvtp_bytes.push_back(LINE_FEED);

    // KEY: line
    kvtp_bytes.append_range(static_cast<std::string>(KEY_PREFIX));
    for (auto c: data.key) {
        if (c != ZERO) {
            kvtp_bytes.push_back(c);
        }
    }
    kvtp_bytes.push_back(LINE_FEED);

    // ARGS: line
    if (!data.args.empty()) {
        kvtp_bytes.append_range(static_cast<std::string>(ARG_PREFIX));

        for (const auto &arg: data.args) {
            for (const auto c: arg) {
                if (c != ZERO) {
                    kvtp_bytes.push_back(c);
                }
            }
            kvtp_bytes.push_back(SPACE_CHAR);
        }
        // remove the last SPACE_CHAR
        kvtp_bytes.pop_back();

        kvtp_bytes.push_back(LINE_FEED);
    }

    if (!data.ttl.empty()) {
        // TTL: line
        kvtp_bytes.append_range(static_cast<std::string>(TTL_PREFIX));

        for (auto c: data.ttl) {
            if (c != ZERO) {
                kvtp_bytes.push_back(c);
            }
        }
        kvtp_bytes.push_back(LINE_FEED);
    }

    if (!data.inc.empty()) {
        // INC: line
        kvtp_bytes.append_range(static_cast<std::string>(INC_PREFIX));

        for (const auto c: data.inc) {
            if (c != ZERO) {
                kvtp_bytes.push_back(c);
            }
        }
        kvtp_bytes.push_back(LINE_FEED);
    }

    // -id for shard command
    // shard -id shard_id
    if (!data.id.empty()) {
        // ID: line
        kvtp_bytes.append_range(static_cast<std::string>(ID_PREFIX));
        for (const auto c: data.id) {
            kvtp_bytes.push_back(c);
        }
        kvtp_bytes.push_back(LINE_FEED);
    }

    // Empty line: header body separator
    kvtp_bytes.push_back(LINE_FEED);

    ///
    /// encode body
    ///
    std::list<uint8_t> body_bytes;
    //uint16_t key_len;
    //uint8_t key_len_bytes[2];

    ////push key bytes
    //for (auto c: data.key) {
    //    body_bytes.push_back(c);
    //}

    //// key length to byte[2]
    //key_len = body_bytes.size();
    //util::uint16_to_bytes(key_len, key_len_bytes);

    //// push key length bytes in body bytes front
    //body_bytes.push_front(key_len_bytes[1]);
    //body_bytes.push_front(key_len_bytes[0]);

    // push value bytes
    for (auto c: data.value) {
        if (c != ZERO) {
            body_bytes.push_back(c);
        }
    }

    kvtp_bytes.append_range(body_bytes);

    //byte_list.push_back(ZERO);

    // prepend request length
    uint32_t request_len = kvtp_bytes.size();
    uint8_t len_bytes[4];

    //
    util::uint32_to_bytes(request_len, len_bytes);
    request.push_back(len_bytes[0]);
    request.push_back(len_bytes[1]);
    request.push_back(len_bytes[2]);
    request.push_back(len_bytes[3]);

    request.append_range(kvtp_bytes);

    // assign to vector
    //request.assign(byte_list.begin(), byte_list.end());
    // ending zero \0
    //request.push_back(ZERO);

    return request;
}
