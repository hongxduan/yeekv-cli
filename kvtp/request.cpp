//
// Created by HongXing on 1/10/25.
//

#include "request.h"

#include <iostream>
#include <ostream>
#include <list>

#include "../util/byte_util.h"

std::vector<uint8_t> kvtp::encodeRequest(InputData data) {
    std::list<uint8_t> byte_list;
    std::vector<uint8_t> request;


    // protocal
    for (auto c: PROTOCOL) {
        if (c != ZERO) {
            byte_list.push_back(c);
        }
    }
    byte_list.push_back(LINE_FEED);

    // CMD: line
    for (auto c: CMD_PREFIX) {
        if (c != ZERO) {
            byte_list.push_back(c);
        }
    }

    for (auto c: data.cmd) {
        if (c != ZERO) {
            byte_list.push_back(c);
        }
    }
    byte_list.push_back(LINE_FEED);

    // KEY: line
    for (auto c: KEY_PREFIX) {
        if (c != ZERO) {
            byte_list.push_back(c);
        }
    }

    for (auto c: data.key) {
        if (c != ZERO) {
            byte_list.push_back(c);
        }
    }
    byte_list.push_back(LINE_FEED);

    // ARGS: line
    if (data.args.size() > 0) {
        for (auto c: ARG_PREFIX) {
            if (c != ZERO) {
                byte_list.push_back(c);
            }
        }

        for (auto arg: data.args) {
            for (auto c: arg) {
                if (c != ZERO) {
                    byte_list.push_back(c);
                }
            }
            byte_list.push_back(SPACE_CHAR);
        }
        // remove the last SPACE_CHAR
        byte_list.pop_back();

        byte_list.push_back(LINE_FEED);
    }

    // TTL: line
    for (auto c: TTL_PREFIX) {
        if (c != ZERO) {
            byte_list.push_back(c);
        }
    }
    for (auto c: std::to_string(data.ttl)) {
        if (c != ZERO) {
            byte_list.push_back(c);
        }
    }
    byte_list.push_back(LINE_FEED);

    // body
    // header/body seperator, i.e. empty line
    byte_list.push_back(LINE_FEED);

    for (auto c: data.value) {
        if (c != ZERO) {
            byte_list.push_back(c);
        }
    }

    byte_list.push_back(ZERO);

    // prepend request length
    uint32_t request_len = byte_list.size();
    uint8_t len_bytes[4];

    //
    util::uint32_to_le_bytes(request_len, len_bytes);
    request.push_back(len_bytes[0]);
    request.push_back(len_bytes[1]);
    request.push_back(len_bytes[2]);
    request.push_back(len_bytes[3]);

    request.append_range(byte_list);

    // assign to vector
    //request.assign(byte_list.begin(), byte_list.end());
    // ending zero \0
    //request.push_back(ZERO);

    return request;
}
