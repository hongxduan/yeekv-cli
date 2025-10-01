#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "include/argparse.hpp"
#include "kvtp/request.h"
#include "parser/input_parser.h"

#define PROMPT "yeekv> "

int main(int argc, char *argv[]) {
    // todo: parse argv
    argparse::ArgumentParser parser("yeekv");

    parser.add_argument("-h", "--host")
            .help("host ip");

    parser.add_argument("-p", "--port")
            .help("port").scan<'i', uint16_t>();

    parser.parse_args(argc, argv);

    std::string host = parser.get<std::string>("-h");
    uint16_t port = parser.get<uint16_t>("-p");

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr);

    // sending connection request
    connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));


    std::string input;

    while (true) {
        std::cout << PROMPT;
        getline(std::cin, input);

        // parse input
        InputData data = parseInput(input);
        if (data.error.size() > 0) {
            std::cout << data.error << std::endl;
            continue;
        }

        // encode request
        auto request = kvtp::encodeRequest(data);
        std::cout << request.data() << std::endl;

        send(clientSocket, request.data(), request.size(), 0);
        //std::cout << input << std::endl;
    }

    close(clientSocket);
    return 0;
}
