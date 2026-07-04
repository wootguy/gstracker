#pragma once
#include <vector>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
typedef int socklen_t;
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#endif

extern int g_udp_socket;

void sendPacket(int socket, const sockaddr_in& addr, const std::vector<uint8_t>& packet);

void sendPacket(int socket, const sockaddr_in& addr, void* data, int len);

int create_udp_socket();

uint64_t netaddr_to_uint64(sockaddr_in& addr);

sockaddr_in uint64_to_netaddr(uint64_t addrint);

uint64_t ipstring_to_uint64(const std::string& ipstring);

std::string netaddr_to_ipstring(const sockaddr_in& addr);

