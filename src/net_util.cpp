#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <unordered_map>
#include <thread>
#include <chrono>
#include "main.h"
#include "util.h"
#include "net_util.h"

using namespace std::chrono;

#define MAX_REQ_ATTEMPTS 3 // give up A2S query after this many attempts
#define REQ_TIMEOUT 1000 // milliseconds to wait between A2S query attempts

int g_udp_socket;

void sendPacket(int socket, const sockaddr_in& addr, const std::vector<uint8_t>& packet) {
    sendto(socket, (const char*)packet.data(), packet.size(), 0,
        (const sockaddr*)&addr, sizeof(addr));
}

void sendPacket(int socket, const sockaddr_in& addr, void* data, int len) {
    sendto(socket, (const char*)data, len, 0,
        (const sockaddr*)&addr, sizeof(addr));
}


uint64_t netaddr_to_uint64(sockaddr_in& addr) {
    return (uint64_t)addr.sin_addr.s_addr + ((uint64_t)addr.sin_port << 32);
}

sockaddr_in uint64_to_netaddr(uint64_t addrint) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = (addrint >> 32) & 0xffff;
    addr.sin_addr.s_addr = addrint & 0xffffffff;
    return addr;
}

uint64_t ipstring_to_uint64(const std::string& ipstring) {
    int sep = ipstring.find("_");
    if (sep == -1) {
        return 0;
    }

    std::string ip = ipstring.substr(0, sep);
    std::string port = ipstring.substr(sep + 1);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port.c_str()));
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    return netaddr_to_uint64(addr);
}

std::string netaddr_to_ipstring(const sockaddr_in& addr)
{
    char ipstr[128] = { 0 };

#ifdef _WIN32
    inet_ntop(AF_INET, (void*)&addr.sin_addr, ipstr, sizeof(ipstr));
#else
    inet_ntop(AF_INET, &addr.sin_addr, ipstr, sizeof(ipstr));
#endif

    return std::string(ipstr) + "_" + std::to_string(ntohs(addr.sin_port));
}

int create_udp_socket() {
    int isock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (isock < 0) {
        printf("Failed to create UDP socket\n");
        return false;
    }

#ifdef _WIN32
    u_long mode = 1; // 1 = non-blocking, 0 = blocking
    if (ioctlsocket(isock, FIONBIO, &mode) != 0) {
        printf("ioctlsocket failed: %d\n", WSAGetLastError());
    }
#else
    int flags = fcntl(isock, F_GETFL, 0);
    if (flags == -1) {
        printf("fcntl F_GETFL");
    }

    if (fcntl(isock, F_SETFL, flags | O_NONBLOCK) == -1) {
        printf("fcntl F_SETFL");
    }
#endif

    return isock;
}

bool net_init() {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

    g_udp_socket = create_udp_socket();

    return true;
}

void net_cleanup() {
#ifdef _WIN32
    closesocket(g_udp_socket);
    WSACleanup();
#else
    close(g_udp_socket);
#endif
}
