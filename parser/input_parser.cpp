//
// Created by HongXing on 1/10/25.
//


#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>

#include "input_parser.h"
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

    data.cmd = util::to_upper(util::trim(pieces[0]));
    if (is_key_command(data.cmd)) {
        parse_key_input(pieces, data);
    } else {
        parse_nonkey_input(pieces, data);
    }

    return data;
}

void parse_key_input(std::vector<std::string> pieces, InputData &data) {
    int i = 1;
    while (i < pieces.size()) {
        std::string piece = pieces[i];
        if (i == 1) {
            data.key = util::trim(piece);
        } else {
            // process commands with KEY
            if (data.cmd == GET || data.cmd == SET || data.cmd == DEL) {
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
                                    return;
                                }
                                data.ttl = ttl_str;
                                // i need forward one step, because consumed 2 pieces
                                // piece 1: -ttl, piece 2: the ttl
                                i++;
                            } catch (const std::invalid_argument &e) {
                                data.error = "invalid ttl";
                                return;
                            }
                        } else {
                            data.error = "invalid ttl";
                            return;
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
                            auto inc_str = util::trim(pieces[i + 1]);
                            auto inc = std::stoll(inc_str);
                            if (inc == 0) {
                                data.error = "INC value cannot be 0";
                                return;
                            }
                            data.inc = inc_str;
                            // i need forward one step, because consumed 2 pieces
                            // piece 1: -inc, piece 2: the inc number
                            i++;
                        } catch (const std::invalid_argument &e) {
                            data.error = "INC value must be a number";
                            return;
                        }
                    } else {
                        data.error = "INC value must be a number";
                        return;
                    }
                } else if (piece == ARG_DEL || piece == ARG_EX || piece == ARG_NX) {
                    data.args.push_back(piece);
                } else if (data.value.size() == 0) {
                    data.value = piece;
                } else {
                    data.error = "invalid command format";
                }
            } else {
            }
        }
        i++;
    }
}

void parse_nonkey_input(std::vector<std::string> pieces, InputData &data) {
    int i = 1;
    while (i < pieces.size()) {
        if (data.cmd == SHARD) {
            // New shard
            if (util::to_upper(pieces[i]) == "-NEW") {
                data.key = "NEW";
                if (i + 1 < pieces.size()) {
                    // todo: validate ip and port
                    data.value = pieces[i + 1];
                } else {
                    data.error = std::format("Missing ip:port");
                }
                i++;
            }
            // Remove shard
            else if (util::to_upper(pieces[i]) == "-RM") {
                data.key = "RM";
                if (i + 1 < pieces.size()) {
                    // the shard id
                    data.value = pieces[i + 1];
                } else {
                    data.error = std::format("Missing shard id");
                }
                i++;
            }
            // Add worker to shard
            else if (util::to_upper(pieces[i]) == "-ADD-WRK") {
                data.key = "ADD-WRK";
                if (i + 1 < pieces.size()) {
                    // todo: validate ip and port
                    data.value = pieces[i + 1];
                } else {
                    data.error = std::format("Missing ip:port");
                }
                i++;
            }
            // shard -id shard_id -add-wrk ip:port
            else if (util::to_upper(pieces[i]) == "-ID") {
                // store id as value
                if (i + 1 < pieces.size()) {
                    data.id = pieces[i + 1];
                } else {
                    data.error = std::format("Missing id");
                }
                i++;
            } else {
                std::cout << pieces[i] << std::endl;
                data.error = std::format("Invalid argument");
            }
        }
        /*
        // RESHARD
        // Format: reshard -f xxxx.txt
        if (data.cmd == RESHARD) {
            if (i == 1) {
                if (pieces[i] != "-f") {
                    data.error = "invalid command format";
                    return;
                }
            } else if (i == 2) {
                auto filename = util::trim(pieces[i]);
                if (!std::filesystem::exists(filename)) {
                    data.error = std::format("file {} not found", filename);
                }
                std::ifstream file(filename);
                std::string line;
                while (getline(file, line)) {
                    // Trim and ignore comments
                    util::trim(line);
                    if (line.starts_with("#")) {
                        continue;
                    }
                    data.value.append(line);
                }
                return;
            }
        }*/
        ++i;
    }
}


bool is_key_command(std::string cmd) {
    return cmd == GET || cmd == SET || cmd == DEL || cmd == KEY;
}
