//
// Created by HongXing on 1/10/25.
//

#include "request.h"

#include <iostream>
#include <ostream>
#include <list>

#include "../util/byte_util.h"

std::vector<uint8_t> kvtp::encode_request(InputData data) {
    std::list<uint8_t> kvtp_bytes;
    std::vector<uint8_t> request;


    // protocol
    for (auto c: PROTOCOL) {
        if (c != ZERO) {
            kvtp_bytes.push_back(c);
        }
    }
    kvtp_bytes.push_back(LINE_FEED);

    // CMD: line
    for (auto c: CMD_PREFIX) {
        if (c != ZERO) {
            kvtp_bytes.push_back(c);
        }
    }

    for (auto c: data.cmd) {
        if (c != ZERO) {
            kvtp_bytes.push_back(c);
        }
    }
    kvtp_bytes.push_back(LINE_FEED);

    // KEY: line
    for (auto c: KEY_PREFIX) {
        if (c != ZERO) {
            kvtp_bytes.push_back(c);
        }
    }

    for (auto c: data.key) {
        if (c != ZERO) {
            kvtp_bytes.push_back(c);
        }
    }
    kvtp_bytes.push_back(LINE_FEED);

    // ARGS: line
    if (data.args.size() > 0) {
        for (auto c: ARG_PREFIX) {
            if (c != ZERO) {
                kvtp_bytes.push_back(c);
            }
        }

        for (auto arg: data.args) {
            for (auto c: arg) {
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

    if (data.ttl != "") {
        // TTL: line
        for (auto c: TTL_PREFIX) {
            if (c != ZERO) {
                kvtp_bytes.push_back(c);
            }
        }

        for (auto c: data.ttl) {
            if (c != ZERO) {
                kvtp_bytes.push_back(c);
            }
        }
        kvtp_bytes.push_back(LINE_FEED);
    }

    if (data.inc != "") {
        // TTL: line
        for (auto c: INC_PREFIX) {
            if (c != ZERO) {
                kvtp_bytes.push_back(c);
            }
        }

        for (auto c: data.inc) {
            if (c != ZERO) {
                kvtp_bytes.push_back(c);
            }
        }
        kvtp_bytes.push_back(LINE_FEED);
    }

    // empty line
    kvtp_bytes.push_back(LINE_FEED);

    /**************body start*****************/
    std::list<uint8_t> body_bytes;
    uint16_t key_len;
    uint8_t key_len_bytes[2];

    //push key bytes
    for (auto c: data.key) {
        body_bytes.push_back(c);
    }

    // key length to byte[2]
    key_len = body_bytes.size();
    util::uint16_to_bytes(key_len, key_len_bytes);

    // push key length bytes in body bytes front
    body_bytes.push_front(key_len_bytes[1]);
    body_bytes.push_front(key_len_bytes[0]);

    // push value bytes
    for (auto c: data.value) {
        if (c != ZERO) {
            body_bytes.push_back(c);
        }
    }
    /**************body end*****************/

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
