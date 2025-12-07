//
// author: Duan Hongxing
// Copyright(c) 2025 Duan Hongxing, hongxduan@gmail.com
//
// Input Parser implement
//

#include "input_parser.h"
#include "../util/string_util.h"
#include "../util/byte_util.h"

#include <format>
#include <fstream>
#include <iostream>
#include <list>
#include <ostream>
#include <vector>

using namespace std;

InputData parse_input(const std::string& input) {
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

    char pre;
    const uint len = input.size();
    for (uint i = 0; i < len; i++) {
        const char cur = input[i];
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
                if (!piece.empty()) {
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

void parse_key_input(const std::vector<std::string>& pieces, InputData& data) {
    if (data.cmd == GET) {
        parse_get(pieces, data);
    } else if (data.cmd == SET) {
        parse_set(pieces, data);
    } else if (data.cmd == LGET) {
        parse_lget(pieces, data);
    } else if (data.cmd == LSET) {
        parse_lset(pieces, data);
    } else if (data.cmd == HGET) {
        parse_hget(pieces, data);
    } else if (data.cmd == HSET) {
        parse_hset(pieces, data);
    } else if (data.cmd == SGET) {
        parse_sget(pieces, data);
    } else if (data.cmd == SSET) {
        parse_sset(pieces, data);
    } else if (data.cmd == OGET) {
        parse_oget(pieces, data);
    } else if (data.cmd == OSET) {
        parse_oset(pieces, data);
    }
}

void parse_get(const std::vector<std::string>& pieces, InputData& data) {
    int i = 1;
    while (i < pieces.size()) {
        const std::string& piece = pieces[i];
        if (i == 1) {
            data.key = util::trim(piece);
        } else {
            // process commands with KEY
            if (piece == ARG_DEL) {
                data.args.push_back(piece);
            } else if (piece == ARG_TTL) {
                // if GET key -ttl, then take ttl as args
                data.args.push_back(piece);
                // For get key -ttl, the -ttl don't have value, just set 0
                // -1: never expire
            } else {
                data.error = "invalid command format";
            }
        }
        i++;
    }
}

void parse_set(const std::vector<std::string>& pieces, InputData& data) {
    int i = 1;
    while (i < pieces.size()) {
        const std::string& piece = pieces[i];
        if (i == 1) {
            data.key = util::trim(piece);
        } else {
            // process commands with KEY
            if (piece == ARG_EX || piece == ARG_NX) {
                data.args.push_back(piece);
            } else if (piece == ARG_TTL) {
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
                    } catch (const std::invalid_argument& e) {
                        data.error = "invalid ttl";
                        return;
                    }
                } else {
                    data.error = "invalid ttl";
                    return;
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
                    } catch (const std::invalid_argument& e) {
                        data.error = "INC value must be a number";
                        return;
                    }
                } else {
                    data.error = "INC value must be a number";
                    return;
                }
            } else if (data.value.empty()) {
                // data.value = piece;
                for (auto c : piece) {
                    data.body.push_back(c);
                }
            } else {
                data.error = "invalid command format";
            }
        }
        i++;
    }
}

void parse_hget(const std::vector<std::string>& pieces, InputData& data) {
    int i = 1;
    std::string piece;
    auto has_arg = false;
    std::list<std::string> fields;
    while (i < pieces.size()) {
        piece = pieces[i];
        if (i == 1) {
            data.key = util::trim(piece);
        } else {
            if (piece == ARG_DEL || piece == ARG_EX || piece == ARG_F || piece == ARG_V) {
                has_arg = true;
                break;
            }
            fields.push_back(piece);
        }
        i++;
    }
    // parse args
    if (has_arg) {
        while (i < pieces.size()) {
            piece = pieces[i];
            if (piece == ARG_DEL || piece == ARG_EX || piece == ARG_F || piece == ARG_V) {
                data.args.push_back(util::trim(piece));
            } else {
                data.error = std::format("invalid arg {}", piece);
                return;
            }
            i++;
        }
        if (data.args.size() > 1) {
            data.error = "too many arguments";
            return;
        }
    }

    auto f_it = fields.begin();
    for (auto i = 0; i < fields.size(); i++) {
        // field
        const uint16_t f_len = f_it->length();
        u_char f_len_bytes[2];
        util::uint16_to_bytes(f_len, f_len_bytes);
        data.body.append_range(f_len_bytes);
        for (const auto c : *f_it) {
            data.body.push_back(c);
        }
        std::advance(f_it, 1);
    }
}

void parse_hset(const std::vector<std::string>& pieces, InputData& data) {
    int i = 1;
    std::string piece;
    auto has_args = false;
    std::list<std::string> fields;
    std::list<std::string> values;
    while (i < pieces.size()) {
        piece = pieces[i];
        if (i == 1) {
            data.key = util::trim(piece);
        } else {
            if (piece == ARG_NX) {
                has_args = true;
                break;
            }
            if (i % 2 == 0) {
                // field
                fields.push_back(piece);
            } else {
                // value
                values.push_back(piece);
            }
        }
        i++;
    }
    if (fields.size() != values.size()) {
        data.error = "invalid field or values count";
    }
    // parse args
    if (has_args) {
        while (i < pieces.size()) {
            piece = pieces[i];
            if (piece == ARG_NX) {
                data.args.push_back(util::trim(piece));
            } else {
                data.error = std::format("invalid arg {}", piece);
            }
            i++;
        }
    }
    //
    auto f_it = fields.begin();
    auto v_it = values.begin();
    for (auto i = 0; i < fields.size(); i++) {
        // field
        const uint16_t f_len = f_it->length();
        u_char f_len_bytes[2];
        util::uint16_to_bytes(f_len, f_len_bytes);
        data.body.append_range(f_len_bytes);
        for (const auto c : *f_it) {
            data.body.push_back(c);
        }
        // value
        const uint32_t v_len = v_it->length();
        u_char v_len_bytes[4];
        util::uint32_to_bytes(v_len, v_len_bytes);
        data.body.append_range(v_len_bytes);
        for (const auto c : *v_it) {
            data.body.push_back(c);
        }
        ++f_it;
        ++v_it;
    }
    std::string body(data.body.begin(), data.body.end());
}

void parse_lget(const std::vector<std::string>& pieces, InputData& data) {
    int i = 1;
    while (i < pieces.size()) {
        const std::string& piece = pieces[i];
        if (i == 1) {
            data.key = util::trim(piece);
        } else if (i == 2) {
            // Index
            data.id = util::trim(piece);
        } else if (piece == ARG_DEL) {
            data.args.push_back(piece);
        } else {
            data.error = "invalid command format";
        }

        i++;
    }
}

void parse_lset(const std::vector<std::string>& pieces, InputData& data) {
    int i = 1;
    while (i < pieces.size()) {
        const std::string& piece = pieces[i];
        if (i == 1) {
            data.key = util::trim(piece);
        } else if (i == 2) {
            // Index
            data.id = util::trim(piece);
        } else if (i == 3) {
            //data.value = util::trim(piece);
            for (auto c : piece) {
                data.body.push_back(c);
            }
        } else {
            if (piece == ARG_EX) {
                // Only when List exists already???
                data.args.push_back(piece);
            } else {
                data.error = "invalid command format";
            }
        }
        i++;
    }
}

void parse_oget(const std::vector<std::string>& pieces, InputData& data) {
}

void parse_oset(const std::vector<std::string>& pieces, InputData& data) {
}

void parse_sget(const std::vector<std::string>& pieces, InputData& data) {
}

void parse_sset(const std::vector<std::string>& pieces, InputData& data) {
}

void parse_del(const std::vector<std::string>& pieces, InputData& data) {
    int i = 1;
    while (i < pieces.size()) {
        const std::string& piece = pieces[i];
        if (i == 1) {
            data.key = util::trim(piece);
        }
        i++;
    }
}


void parse_nonkey_input(const std::vector<std::string>& pieces, InputData& data) {
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
        } else if (data.cmd == INFO) {
            if (util::to_upper(pieces[i]) == "-SHARD") {
                data.key = "SHARD";
                i++;
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


bool is_key_command(const std::string& cmd) {
    return cmd == GET
        || cmd == SET
        || cmd == LGET
        || cmd == LSET
        || cmd == HGET
        || cmd == HSET
        || cmd == SGET
        || cmd == SSET
        || cmd == OGET
        || cmd == OSET
        || cmd == DEL
        || cmd == KEY
        || cmd == TTL;
}
