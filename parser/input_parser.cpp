//
// Created by HongXing on 1/10/25.
//


#include <vector>

#include "input_parser.h"

#include <iostream>
#include <ostream>

#include "../util/string_util.h"

InputData parseInput(std::string input) {
    InputData data;
    std::vector<std::string> pieces;

    bool quoted = false; // check double-quoted or not during walk through input
    uint si = 0; // start index
    uint ei = 0; // end index

    char pre, cur;
    uint len = input.size();
    for (uint i = 0; i < len; i++) {
        cur = input[i];
        // meet space
        if (cur == SPACE_CHAR || i == len - 1) {
            ei = i;
            // the last char
            if (cur == DQUOTE_CHAR) {
                if (pre != BSLASH_CHAR) {
                    quoted = !quoted;
                }
            }

            if (i == len - 1 && quoted) {
                data.error = "Quote not closed";
                return data;
            }

            // if not quoted, then push to pieces
            if (!quoted) {
                std::string piece;
                if (i == len - 1) {
                    piece = input.substr(si, ei - si + 1);
                } else {
                    if (ei > si) {
                        piece = input.substr(si, ei - si);
                    }
                }
                if (piece.size() > 0) {
                    // remove the leading and trailing Double Quote
                    if (piece[0] == DQUOTE_CHAR) {
                        piece = piece.substr(1, piece.size() - 2);
                    }
                }
                //std::cout << piece << std::endl;
                pieces.push_back(piece);
                si = i + 1;
            }
        } else if (cur == DQUOTE_CHAR) {
            if (pre != BSLASH_CHAR) {
                quoted = !quoted;
            }
        }
        pre = cur;
    }

    /* print pieces for debug */
    /*
    for (uint i = 0; i < pieces.size(); i++) {
        std::cout  << pieces[i]  << std::endl;
    }*/

    int i = 0;
    while (i < pieces.size()) {
        std::string piece = pieces[i];
        if (i == 0) {
            data.cmd = util::toUpper(piece);
        } else {
            // process commands with KEY
            if (data.cmd == "GET" || data.cmd == "GET" || data.cmd == "SET") {
                if (piece == ARG_DEL || piece == ARG_EX || piece == ARG_NX) {
                    data.args.push_back(piece);
                } else if (piece == ARG_TTL) {
                    if (data.cmd == "SET") {
                        if (i + 1 < pieces.size()) {
                            try {
                                data.ttl = std::stoi(pieces[i + 1]);
                            } catch (const std::invalid_argument &e) {
                                data.error = "invalid ttl";
                                return data;
                            }
                        } else {
                            data.error = "invalid ttl";
                            return data;
                        }
                    } else {
                        // For get key -ttl, the -ttl don't have value, just set 0
                        // -1: never expire
                        // -2: expired
                        data.ttl = 0;
                    }
                } else {
                    if (data.key.size() == 0) {
                        data.key = piece;
                    } else if (data.value.size() == 0) {
                        data.value = piece;
                    }
                }
            }
            // Process commands without KEY
            else {
            }
        }
        i++;
    }

    //std::cout << data.cmd << " " << data.key << " " << data.value << std::endl;
    return data;
}
