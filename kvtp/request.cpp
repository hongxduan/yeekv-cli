//
// Created by HongXing on 1/10/25.
//

#include "request.h"

#include <iostream>
#include <ostream>

std::vector<uint8_t> kvtp::encodeRequest(InputData data) {
    std::vector<uint8_t> request;


    // protocal
    for (auto c: PROTOCOL) {
        if (c != ZERO) {
            request.push_back(c);
        }
    }
    request.push_back(LINE_FEED);

    // CMD: line
    for (auto c: CMD_PREFIX) {
        if (c != ZERO) {
            request.push_back(c);
        }
    }

    for (auto c: data.cmd) {
        if (c != ZERO) {
            request.push_back(c);
        }
    }
    request.push_back(LINE_FEED);

    // KEY: line
    for (auto c: KEY_PREFIX) {
        if (c != ZERO) {
            request.push_back(c);
        }
    }

    for (auto c: data.key) {
        if (c != ZERO) {
            request.push_back(c);
        }
    }
    request.push_back(LINE_FEED);

    // ARGS: line
    if (data.args.size()>0) {
        for (auto c: ARG_PREFIX) {
            if (c != ZERO) {
                request.push_back(c);
            }
        }

        for (auto arg: data.args) {
            for (auto c:arg) {
                if (c != ZERO) {
                    request.push_back(c);
                }
            }
            request.push_back(SPACE_CHAR);
        }
        // remove the last SPACE_CHAR
        request.pop_back();

        request.push_back(LINE_FEED);
    }

    // TTL: line
    for (auto c: TTL_PREFIX) {
        if (c != ZERO) {
            request.push_back(c);
        }
    }
    for (auto c: std::to_string(data.ttl)) {
        if (c != ZERO) {
            request.push_back(c);
        }
    }
    request.push_back(LINE_FEED);

    // body
    // header/body seperator, i.e. empty line
    request.push_back(LINE_FEED);

    for (auto c: data.value) {
        if (c != ZERO) {
            request.push_back(c);
        }
    }

    // ending zero \0
    request.push_back(ZERO);

    return request;
}
