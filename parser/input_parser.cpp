//
// Created by HongXing on 1/10/25.
//


#include <vector>

#include "input_parser.h"

#include <iostream>
#include <ostream>

#include "../util/string_util.h"

InputData parse_input(std::string input) {
    InputData data{
        "",
        "",
        "",
        {},
        "",
        "",
        ""
    };

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
                piece = util::trim(piece);
                if (piece.size() > 0) {
                    // remove the leading and trailing Double Quote
                    if (piece[0] == DQUOTE_CHAR) {
                        piece = piece.substr(1, piece.size() - 2);
                    }
                    pieces.push_back(piece);
                }
                //std::cout << piece << std::endl;
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
        std::cout << piece << std::endl;
        if (i == 0) {
            data.cmd = util::to_upper(util::trim(piece));
        } else if (i == 1) {
            data.key = util::trim(piece);
        } else {
            // process commands with KEY
            if (data.cmd == "GET" || data.cmd == "SET" || data.cmd == "DEL") {
                if (piece == ARG_DEL || piece == ARG_EX || piece == ARG_NX) {
                    data.args.push_back(piece);
                } else if (piece == ARG_TTL) {
                    if (data.cmd == "SET") {
                        if (i + 1 < pieces.size()) {
                            try {
                                auto ttl_str = util::trim(pieces[i + 1]);
                                // validate ttl:
                                //  ttl must be a positive number
                                //  ttl cannot be 0 or negative number
                                auto ttl = std::stoll(ttl_str);
                                if (ttl <= 0) {
                                    data.error = "ttl must be greater than zero";
                                    return data;
                                }
                                data.ttl = ttl_str;
                                // i need forward one step, because consumed 2 pieces
                                // piece 1: -ttl, piece 2: the ttl
                                i++;
                            } catch (const std::invalid_argument &e) {
                                data.error = "invalid ttl";
                                return data;
                            }
                        } else {
                            data.error = "invalid ttl";
                            return data;
                        }
                    } else if (data.cmd == GET) {
                        // if GET key -ttl, then take ttl as args
                        data.args.push_back(piece);
                        // For get key -ttl, the -ttl don't have value, just set 0
                        // -1: never expire
                    }
                } else if (piece == ARG_INC) {
                    if (i + 1 < pieces.size()) {
                        try {
                            std::cout << "data.inc" << util::trim(pieces[i + 1]) << std::endl;
                            auto inc_str = util::trim(pieces[i + 1]);
                            auto inc = std::stoll(inc_str);
                            if (inc == 0) {
                                data.error = "INC value cannot be 0";
                                return data;
                            }
                            data.inc = inc_str;
                            std::cout << "data.inc" << data.inc << std::endl;
                            // i need forward one step, because consumed 2 pieces
                            // piece 1: -inc, piece 2: the inc number
                            i++;
                        } catch (const std::invalid_argument &e) {
                            data.error = "INC value must be a number";
                            return data;
                        }
                    } else {
                        data.error = "INC value must be a number";
                        return data;
                    }
                } else if (piece == ARG_DEL || piece == ARG_EX || piece == ARG_NX) {
                    data.args.push_back(piece);
                } else if (data.value.size() == 0) {
                    data.value = piece;
                } else {
                    data.error = "invalid command format";
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
