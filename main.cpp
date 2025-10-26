//
//
//

#include <bitset>
#include <cstring>
#include <iostream>
#include <bit>
#include <iterator>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "include/argparse.hpp"
#include "kvtp/request.h"
#include "kvtp/response.h"
#include "parser/input_parser.h"
#include "util/byte_util.h"
#include "util/string_util.h"

#define PROMPT "yeekv> "

void printOS();

int main(int argc, char *argv[]) {
    printOS();

    argparse::ArgumentParser parser("yeekv");

    parser.add_argument("-h", "--host")
            .help("host ip");

    parser.add_argument("-p", "--port")
            .help("port").scan<'i', uint16_t>();

    parser.parse_args(argc, argv);

    std::string host = parser.get<std::string>("-h");
    uint16_t port = parser.get<uint16_t>("-p");

    int client_sock = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr);

    // sending connection request
    connect(client_sock, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr));

    // command line input
    std::string input;

    while (true) {
        std::cout << PROMPT;
        getline(std::cin, input);

        util::trim(input);
        if (input.empty()) {
            continue;
        }

        // parse input
        InputData data = parse_input(input);
        if (data.error.size() > 0) {
            std::cout << data.error << std::endl;
            continue;
        }

        // encode request
        auto request = kvtp::encode_request(data);

        // why request.size() -1? to remove the ending '\0'
        size_t n = send(client_sock, request.data(), request.size(), 0);
        if (n == 0) {
            std::cout << "server closed connection" << std::endl;
            break;
        }
        // response message length in terms of bytes
        // this first 4 bytes are the length for the response
        uint8_t len_bytes[4];
        n = read(client_sock, len_bytes, 4);
        if (n == 0) {
            std::cout << "server closed connection" << std::endl;
            break;
        } else {
            //std::cout << "read n:" << n << std::endl;
        }
        uint32_t len = util::bytes_to_uint32(len_bytes);

        //std::cout << "len:" << len << std::endl;

        // read response
        n = 0;
        size_t bufsize = 1024;
        uint8_t buffer[bufsize];
        size_t total_n = 0;
        std::vector<uint8_t> response;
        do {
            memset(buffer, 0, bufsize);
            n = read(client_sock, buffer, bufsize);
            if (n == 0) {
                std::cout << "server closed connection" << std::endl;
                break;
            }
            total_n += n;
            response.insert(response.end(), buffer, buffer + n);
        } while (total_n < len);

        // decode response
        kvtp::decode_response(response);
    }

    close(client_sock);
    return 0;
}

void printOS() {
    /*
    uint8_t bytes1[4], bytes2[4];
    util::int32_to_le_bytes(5, bytes1);
    util::int32_to_le_bytes(-5, bytes2);
    std::cout << "bytes:" << bytes1 << std::endl;
    std::cout << "bytes:" << bytes2 << std::endl;
    std::cout << std::hex << 2 << std::endl;
    std::cout << std::hex << -3 << std::endl;
    */

    /*
    int32_t i = -32;
    std::cout << i << " " <<std::hex << i << std::endl;
    int32_t j = std::byteswap(i);
    std::cout << std::dec << j << " " <<std::hex << j << std::endl;
    uint8_t bytes[4];
    util::int32_to_bytes(i, bytes);
    std::cout<< std::hex<<std::to_integer<int>((std::byte)bytes[0])
    <<std::to_integer<int>((std::byte)bytes[1])
    <<std::to_integer<int>((std::byte)bytes[2])
    <<std::to_integer<int>((std::byte)bytes[3]) <<std::endl;
    */
    /*
    if constexpr (std::endian::native == std::endian::little) {
        std::cout << "little endian" << std::endl;
        uint32_t i = 32;
        std::cout << std::hex << i << std::endl;
        uint32_t j = std::byteswap(i);
        std::cout << j << " " << std::hex << j << std::endl;

        uint8_t bytes[4];
        util::uint32_to_le_bytes(i, bytes);

        std::cout << "bytes:" << bytes << std::endl;

        //uint32_t ii = (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];
        uint32_t ii ;
        //std::memcpy(&ii, bytes, sizeof(ii));
        ii = util::convertBeBytesToUint32(bytes);
        std::cout << std::dec << ii << std::endl;
    } else if (std::endian::native == std::endian::big) {
        std::cout << "big endian" << std::endl;
    }
    */
}
