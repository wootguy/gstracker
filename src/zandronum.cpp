#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <thread>
#include <chrono>
#include "main.h"
#include "util.h"
#include "net_util.h"
#include "mstream.h"
#include "zandronum.h"


#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>
#include <bitset>


// ============================
// Huffman decode table (from Python)
// ============================
static std::unordered_map<std::string, uint8_t> huffman_decode_table = {
    {"0000", 128}, {"00010000", 38}, {"00010001", 34}, {"000100100", 80},
    {"0001001010", 110}, {"0001001011", 144}, {"00010011", 67}, {"000101000", 74},
    {"0001010010", 243}, {"0001010011", 142}, {"00010101", 37}, {"000101100", 124},
    {"000101101", 58}, {"00010111", 182}, {"00011000", 36}, {"0001100100", 221},
    {"0001100101", 131}, {"0001100110", 245}, {"0001100111", 163}, {"00011010", 35},
    {"000110110", 113}, {"000110111", 85}, {"00011100", 41}, {"000111010", 77},
    {"0001110110", 199}, {"0001110111", 130}, {"000111100", 206}, {"0001111010", 185},
    {"0001111011", 153}, {"000111110", 70}, {"000111111", 118}, {"00100", 3},
    {"00101", 5}, {"0011000", 24}, {"0011001000", 198}, {"0011001001", 190},
    {"001100101", 63}, {"0011001100", 139}, {"0011001101", 186}, {"001100111", 75},
    {"00110100", 44}, {"0011010100", 240}, {"0011010101", 218}, {"001101011", 56},
    {"00110110", 40}, {"00110111", 39}, {"0011100000", 244}, {"0011100001", 247},
    {"001110001", 81}, {"00111001", 65}, {"001110100", 9}, {"001110101", 125},
    {"001110110", 68}, {"001110111", 60}, {"001111000", 25}, {"0011110010", 191},
    {"0011110011", 138}, {"001111010", 86}, {"001111011", 17}, {"001111100", 23},
    {"0011111010", 220}, {"0011111011", 178}, {"0011111100", 165}, {"0011111101", 194},
    {"001111111", 14}, {"010", 0}, {"011000000", 208}, {"0110000010", 150},
    {"0110000011", 157}, {"01100001", 181}, {"01100010", 222}, {"0110001100", 216},
    {"0110001101", 230}, {"011000111", 211}, {"0110010000", 252}, {"0110010001", 141},
    {"011001001", 10}, {"01100101", 42}, {"0110011000", 134}, {"0110011001", 135},
    {"011001101", 104}, {"011001110", 103}, {"0110011110", 187}, {"0110011111", 225},
    {"01101", 95}, {"0111", 32}, {"10000000", 57}, {"100000010", 61},
    {"1000000110", 183}, {"1000000111", 237}, {"1000001000", 233}, {"1000001001", 234},
    {"1000001010", 246}, {"1000001011", 203}, {"1000001100", 250}, {"1000001101", 147},
    {"100000111", 79}, {"1000010", 129}, {"100001100", 7}, {"1000011010", 143},
    {"1000011011", 136}, {"100001110", 20}, {"1000011110", 179}, {"1000011111", 148},
    {"100010000", 28}, {"100010001", 106}, {"100010010", 101}, {"100010011", 87},
    {"10001010", 66}, {"1000101100", 180}, {"1000101101", 219}, {"1000101110", 227},
    {"1000101111", 241}, {"10001100", 26}, {"100011010", 251}, {"1000110110", 229},
    {"1000110111", 214}, {"10001110", 54}, {"10001111", 69}, {"1001000000", 231},
    {"1001000001", 212}, {"1001000010", 156}, {"1001000011", 176}, {"100100010", 93},
    {"100100011", 83}, {"100100100", 96}, {"100100101", 253}, {"100100110", 30},
    {"100100111", 13}, {"1001010000", 175}, {"1001010001", 254}, {"100101001", 94},
    {"100101010", 159}, {"100101011", 27}, {"100101100", 8}, {"1001011010", 204},
    {"1001011011", 226}, {"10010111", 78}, {"100110000", 107}, {"100110001", 88},
    {"100110010", 31}, {"1001100110", 137}, {"1001100111", 169}, {"1001101000", 215},
    {"1001101001", 145}, {"100110101", 6}, {"10011011", 4}, {"1001110", 127},
    {"100111100", 99}, {"1001111010", 209}, {"1001111011", 217}, {"1001111100", 213},
    {"1001111101", 238}, {"1001111110", 177}, {"1001111111", 170}, {"1010", 132},
    {"101100000", 22}, {"101100001", 12}, {"10110001", 114}, {"1011001000", 158},
    {"1011001001", 197}, {"101100101", 97}, {"10110011", 45}, {"10110100", 46},
    {"101101010", 112}, {"1011010110", 174}, {"1011010111", 249}, {"101101100", 224},
    {"101101101", 102}, {"1011011100", 171}, {"1011011101", 151}, {"101101111", 193},
    {"101110000", 15}, {"101110001", 16}, {"101110010", 2}, {"101110011", 168},
    {"10111010", 49}, {"101110110", 91}, {"101110111", 146}, {"10111100", 48},
    {"101111010", 173}, {"101111011", 29}, {"101111100", 19}, {"101111101", 126},
    {"101111110", 92}, {"101111111", 242}, {"110000000", 205}, {"110000001", 192},
    {"1100000100", 235}, {"1100000101", 149}, {"110000011", 255}, {"110000100", 223},
    {"110000101", 184}, {"11000011", 248}, {"110001000", 108}, {"110001001", 236},
    {"110001010", 111}, {"110001011", 90}, {"110001100", 117}, {"110001101", 115},
    {"11000111", 71}, {"11001000", 11}, {"11001001", 50}, {"110010100", 188},
    {"110010101", 119}, {"110010110", 122}, {"1100101110", 167}, {"1100101111", 162},
    {"1100110", 160}, {"11001110", 133}, {"110011110", 123}, {"110011111", 21},
    {"11010000", 59}, {"1101000100", 155}, {"1101000101", 154}, {"110100011", 98},
    {"1101001", 43}, {"11010100", 76}, {"11010101", 51}, {"110101100", 201},
    {"110101101", 116}, {"11010111", 72}, {"110110000", 109}, {"110110001", 100},
    {"11011001", 121}, {"110110100", 195}, {"110110101", 232}, {"11011011", 18},
    {"110111", 1}, {"1110000", 164}, {"111000100", 120}, {"111000101", 189},
    {"11100011", 73}, {"11100100", 196}, {"111001010", 239}, {"111001011", 210},
    {"11100110", 64}, {"11100111", 62}, {"11101", 89}, {"1111000", 33},
    {"111100100", 228}, {"111100101", 161}, {"11110011", 55}, {"11110100", 84},
    {"11110101", 152}, {"1111011", 47}, {"111110000", 207}, {"111110001", 172},
    {"11111001", 140}, {"11111010", 82}, {"11111011", 166}, {"11111100", 53},
    {"11111101", 105}, {"11111110", 52}, {"111111110", 202}, {"111111111", 200}
};


// ============================
// Build encode table
// ============================
static std::unordered_map<uint8_t, std::string> huffman_encode_table;

void init_encode_table() {
    for (auto& p : huffman_decode_table) {
        huffman_encode_table[p.second] = p.first;
    }
}

// ============================
// Bit helpers
// ============================
std::string byte_to_bitstring(uint8_t v) {
    std::string r;
    for (int i = 0; i < 8; i++) {
        r += ((v & (1 << i)) ? '1' : '0');
    }
    return r;
}

uint8_t bitstring_to_byte(const std::string& s) {
    if (s.size() != 8)
        throw std::runtime_error("bitstring must be 8 bits");

    uint8_t r = 0;
    for (int i = 0; i < 8; i++) {
        if (s[i] == '1')
            r |= (1 << i);
    }
    return r;
}

// ============================
// Decode helper
// ============================
bool find_match(const std::string& bits, size_t idx, size_t& out_len, uint8_t& out_val) {
    for (size_t len = 3; len <= 18; len++) {
        if (idx + len > bits.size())
            continue;

        auto sub = bits.substr(idx, len);
        auto it = huffman_decode_table.find(sub);
        if (it != huffman_decode_table.end()) {
            out_len = len;
            out_val = it->second;
            return true;
        }
    }
    return false;
}

// ============================
// Huffman decode (Zandronum style)
// ============================
std::vector<uint8_t> huffdecode(uint8_t* buffer, int len) {
    if (len == 0)
        return {};

    uint8_t padding = buffer[0];
    if (padding == 255) {
        std::vector<uint8_t> out;
        for (int i = 1; i < len; i++) {
            out.push_back(buffer[i]);
        }
        return out;
    }

    std::string bits;
    for (size_t i = 1; i < len; i++) {
        bits += byte_to_bitstring(buffer[i]);
    }

    size_t n_bits = bits.size() - padding;

    std::vector<uint8_t> out;
    size_t idx = 0;

    while (idx < n_bits) {
        size_t len;
        uint8_t val;

        if (!find_match(bits, idx, len, val))
            throw std::runtime_error("Huffman decode failed");

        out.push_back(val);
        idx += len;
    }

    return out;
}

// ============================
// Huffman encode
// ============================
std::vector<uint8_t> huffencode(uint8_t* input, int inputLen) {
    std::string bits;

    for (int i = 0; i < inputLen; i++) {
        auto it = huffman_encode_table.find(input[i]);
        if (it == huffman_encode_table.end())
            throw std::runtime_error("Missing encode entry");

        bits += it->second;
    }

    int padding = 8 - (bits.size() % 8);
    if (padding == 8) padding = 0;

    bits.append(padding, '0');

    std::vector<uint8_t> out;
    out.push_back((uint8_t)padding);

    for (size_t i = 0; i < bits.size(); i += 8) {
        int b = bitstring_to_byte(bits.substr(i, 8));
        out.push_back(b);
    }

    return out;
}


using namespace std::chrono;

#define MSC_ENDSERVERLIST 2
#define MSC_IPISBANNED 3
#define MSC_REQUESTIGNORED 4
#define MSC_WRONGVERSION 5
#define MSC_BEGINSERVERLISTPART 6
#define MSC_ENDSERVERLISTPART 7
#define MSC_SERVERBLOCK 8

#define SQF_NAME 0x00000001
#define SQF_URL 0x00000002
#define SQF_EMAIL 0x00000004
#define SQF_MAPNAME 0x00000008
#define SQF_MAXCLIENTS 0x00000010
#define SQF_MAXPLAYERS 0x00000020
#define SQF_PWADS 0x00000040
#define SQF_GAMETYPE 0x00000080
#define SQF_GAMENAME 0x00000100
#define SQF_IWAD 0x00000200
#define SQF_FORCEPASSWORD 0x00000400
#define SQF_FORCEJOINPASSWORD 0x00000800
#define SQF_GAMESKILL 0x00001000
#define SQF_BOTSKILL 0x00002000
#define SQF_DMFLAGS 0x00004000
#define SQF_LIMITS 0x00010000
#define SQF_TEAMDAMAGE 0x00020000
#define SQF_TEAMSCORES 0x00040000
#define SQF_NUMPLAYERS 0x00080000
#define SQF_PLAYERDATA 0x00100000
#define SQF_TEAMINFO_NUMBER 0x00200000
#define SQF_TEAMINFO_NAME 0x00400000
#define SQF_TEAMINFO_COLOR 0x00800000
#define SQF_TEAMINFO_SCORE 0x01000000
#define SQF_TESTING_SERVER 0x02000000
#define SQF_DATA_MD5SUM 0x04000000
#define SQF_ALL_DMFLAGS 0x08000000
#define SQF_SECURITY_SETTINGS 0x10000000
#define SQF_OPTIONAL_WADS 0x20000000
#define SQF_DEH 0x40000000
#define SQF_EXTENDED_INFO 0x80000000

#define SERVER_LAUNCHER_CHALLENGE 5660023
#define SERVER_LAUNCHER_IGNORING 5660024
#define SERVER_LAUNCHER_BANNED 5660025
#define SERVER_LAUNCHER_CHALLENGE_SEGMENTED 5660032

#define TIMEOUT 3000

char g_buf[4096];
mstream g_stream;

int g_master_socket;

std::unordered_map<std::string, ZanSvResp> zandronum_master_query() {
    uint64_t ip = ipstring_to_uint64("217.154.79.34_15300");
    sockaddr_in masterAddr = uint64_to_netaddr(ip);

    // send the query
    {
        uint32_t challenge = 5660028;
        uint16_t version = 2;
        g_stream.write(&challenge, 4);
        g_stream.write(&version, 2);

        sendPacket(g_master_socket, masterAddr, huffencode((uint8_t*)g_stream.getBuffer(), g_stream.tell()));
        //printf("Sent master query to %s\n", netaddr_to_ipstring(masterAddr).c_str());
    }
    
    uint64_t startQuery = getEpochMillis();

    std::unordered_map<std::string, ZanSvResp> allServers;

    while (1) {
        sockaddr_in from;
        socklen_t fromLen = sizeof(sockaddr_in);

        int ret = recvfrom(g_master_socket, (char*)g_buf, sizeof(g_buf), 0, (sockaddr*)&from, &fromLen);
        if (getEpochMillis() - startQuery > TIMEOUT && ret <= 0) {
            printf("Timed out waiting for master server response\n");
            break; // no more queued packets
        }

        uint64_t ipint = netaddr_to_uint64(from);
        string fromAddrStr = netaddr_to_ipstring(from);

        if (ret > 0) {
            //printf("Recv %d bytes from %s\n", ret, fromAddrStr.c_str());

            vector<uint8_t> bytes = huffdecode((uint8_t*)g_buf, ret);
            memcpy(g_buf, &bytes[0], bytes.size());
            g_stream = mstream(g_buf, bytes.size());

            uint32_t code;
            g_stream.read(&code, 4);

            bool valid = false;

            switch (code) {
            case MSC_IPISBANNED:
                printf("Master Server: Your IP is banned.\n");
                break;
            case MSC_REQUESTIGNORED:
                printf("Master Server: IP made a request in the past 3 seconds. Request Ignored.\n");
                break;
            case MSC_WRONGVERSION:
                printf("Master Server: Wrong version.\n");
                break;
            case MSC_BEGINSERVERLISTPART:
                valid = true;
                break;
            default:
                printf("Master Server: Unexpected master server response code %d.\n", code);
                break;
            }

            if (!valid)
                break;

            uint8_t num;
            g_stream.read(&num, 1);

            //printf("Read packet num %d\n", (int)num);

            uint8_t blockCode;
            g_stream.read(&blockCode, 1);

            if (blockCode != MSC_SERVERBLOCK) {
                printf("Unexpected block code %d\n", blockCode);
                continue;
            }

            while (!g_stream.eom()) {
                uint8_t numServers;
                g_stream.read(&numServers, 1);

                //printf("read %d servers\n", (int)numServers);
                if (numServers == 0) {
                    break;
                }

                uint8_t svip[4];
                g_stream.read(&svip, 4);

                for (int i = 0; i < numServers; i++) {
                    uint16_t port;
                    g_stream.read(&port, 2);
                    //printf("Server: %d.%d.%d.%d:%d\n", (int)svip[0], (int)svip[1], (int)svip[2], (int)svip[3], (int)port);
                
                    string svStr = to_string(svip[0]) + "." + to_string(svip[1]) + "."
                        + to_string(svip[2]) + "." + to_string(svip[3]) + "_" + to_string(port);
                    allServers[svStr] = ZanSvResp();
                }
            }

            uint8_t end;
            g_stream.read(&end, 1);

            if (end == MSC_ENDSERVERLIST) {
               // printf("Master Server: You got the full list.\n");
                break;
            }
            else if (end == MSC_ENDSERVERLISTPART) {
                //printf("Master Server: end of current part of list.\n");
            }
        }

        std::this_thread::sleep_for(milliseconds(1));
    }

    return allServers;
}

const char* read_string() {
    static char stemp[1024];

    for (int i = 0; i < sizeof(stemp); i++) {
        g_stream.read(stemp + i, 1);
        if (stemp[i] == 0)
            break;
    }
    stemp[sizeof(stemp) - 1] = 0;
    return stemp;
}

uint8_t read_u8() {
    uint8_t out;
    g_stream.read(&out, 1);
    return out;
}

uint16_t read_u16() {
    uint16_t out;
    g_stream.read(&out, 2);
    return out;
}

int32_t read_i32() {
    int32_t out;
    g_stream.read(&out, 4);
    return out;
}

float read_float() {
    float out;
    g_stream.read(&out, 4);
    return out;
}

bool is_team_game(int mode) {
    static unordered_set<int> team_modes = {
        ZAN_GAMEMODE_TEAMPLAY,
        ZAN_GAMEMODE_TEAMLMS,
        ZAN_GAMEMODE_TEAMPOSSESSION,
        ZAN_GAMEMODE_TEAMGAME,
        ZAN_GAMEMODE_CTF,
        ZAN_GAMEMODE_ONEFLAGCTF,
        ZAN_GAMEMODE_SKULLTAG,
        ZAN_GAMEMODE_DOMINATION
    };
    return team_modes.count(mode);
}

void parse_sv_response(ZanSvResp& resp, uint32_t flags) {
    resp.valid = true;

    if (flags & SQF_NAME) {
        resp.name = read_string();
    }
    if (flags & SQF_URL) {
        resp.url = read_string();
    }
    if (flags & SQF_EMAIL) {
        resp.email = read_string();
    }
    if (flags & SQF_MAPNAME) {
        resp.mapname = read_string();
    }
    if (flags & SQF_MAXCLIENTS) {
        resp.maxclients = read_u8();
    }
    if (flags & SQF_MAXPLAYERS) {
        resp.maxplayers = read_u8();
    }
    if (flags & SQF_PWADS) {
        resp.numPwads = read_u8();
        resp.pwads.clear();
        resp.pwads.reserve(resp.numPwads);

        for (uint8_t i = 0; i < resp.numPwads; i++) {
            resp.pwads.push_back(read_string());
        }
    }
    if (flags & SQF_GAMETYPE) {
        resp.gametype = read_u8();
        resp.instagib = read_u8();
        resp.buckshot = read_u8();
    }
    if (flags & SQF_GAMENAME) {
        resp.gamename = read_string();
    }
    if (flags & SQF_IWAD) {
        resp.iwad = read_string();
    }
    if (flags & SQF_FORCEPASSWORD) {
        resp.forcepw = read_u8();
    }
    if (flags & SQF_FORCEJOINPASSWORD) {
        resp.forcejoinpw = read_u8();
    }
    if (flags & SQF_GAMESKILL) {
        resp.gameskill = read_u8();
    }
    if (flags & SQF_BOTSKILL) {
        resp.botskill = read_u8();
    }
    if (flags & SQF_DMFLAGS) {
        resp.dmflags1 = read_i32();
        resp.dmflags2 = read_i32();
        resp.compatflags = read_i32();
    }
    if (flags & SQF_LIMITS) {
        resp.fraglimit = read_u16();
        resp.timelimit = read_u16();

        if (resp.timelimit > 0) {
            resp.timeleft = read_u16();
        }

        resp.duellimit = read_u16();
        resp.pointlimit = read_u16();
        resp.winlimit = read_u16();
    }
    if (flags & SQF_TEAMDAMAGE) {
        resp.teamdamage = read_float();
    }

    if (flags & SQF_TEAMSCORES) {
        resp.teamscores_blue = read_u16();
        resp.teamscores_red = read_u16();
    }

    uint8_t numplayers = 0;
    if (flags & SQF_NUMPLAYERS) {
        numplayers = read_u8();
    }

    if (flags & SQF_PLAYERDATA) {
        resp.players.clear();
        resp.players.reserve(numplayers);

        for (uint8_t i = 0; i < numplayers; i++) {
            ZanPlayerDat player;
            player.name = read_string();
            player.points = read_u16();
            player.ping = read_u16();
            player.spectate = read_u8();
            player.bot = read_u8();

            if (is_team_game(resp.gametype)) {
                player.team = read_u8();
            }
            else {
                player.team = 255;
            }

            player.time = read_u8();
            resp.players.push_back(std::move(player));
        }
    }
    if (flags & SQF_TEAMINFO_NUMBER) {
        uint8_t teamCount = read_u8();
        resp.teams.resize(teamCount);

        for (uint8_t i = 0; i < teamCount; i++) {
            resp.teams[i].num = i;
        }
    }
    if (flags & SQF_TEAMINFO_NAME) {
        for (uint8_t i = 0; i < resp.teams.size(); i++) {
            resp.teams[i].name = read_string();
        }
    }
    if (flags & SQF_TEAMINFO_COLOR) {
        for (uint8_t i = 0; i < resp.teams.size(); i++) {
            resp.teams[i].color = read_i32();
        }
    }
    if (flags & SQF_TEAMINFO_SCORE) {
        for (uint8_t i = 0; i < resp.teams.size(); i++) {
            resp.teams[i].score = read_u16();
        }
    }
    if (flags & SQF_TESTING_SERVER) {
        resp.test = read_u8();
        resp.test_name = read_string();
    }
    if (flags & SQF_DATA_MD5SUM) {
        resp.md5 = read_string();
    }
    if (flags & SQF_ALL_DMFLAGS) {
        uint8_t numDmFlags = read_u8();

        resp.dmflags.clear();
        resp.dmflags.reserve(numDmFlags);

        for (uint8_t i = 0; i < numDmFlags; i++) {
            resp.dmflags.push_back(read_i32());
        }
    }
    if (flags & SQF_SECURITY_SETTINGS) {
        resp.security = read_u8();
    }
    if (flags & SQF_OPTIONAL_WADS) {
        uint8_t numOptional = read_u8();

        resp.optional_pwads.clear();
        resp.optional_pwads.reserve(numOptional);

        for (uint8_t i = 0; i < numOptional; i++) {
            resp.optional_pwads.push_back(read_u8());
        }
    }
    if (flags & SQF_DEH) {
        uint8_t numDeh = read_u8();

        resp.deh_patches.clear();
        resp.deh_patches.reserve(numDeh);

        for (uint8_t i = 0; i < numDeh; i++) {
            resp.deh_patches.push_back(read_string());
        }
    }
    if (flags & SQF_EXTENDED_INFO) {
        resp.sqf2_flags = read_i32();
    }
}

std::unordered_map<std::string, ZanSvResp> g_zandronum_servers;

void update_zandronum_servers() {
    static bool didInit = false;
    if (!didInit) {
        didInit = true;
        init_encode_table();
        g_master_socket = create_udp_socket();
    }

    g_stream = mstream(g_buf, sizeof(g_buf));

    g_zandronum_servers = zandronum_master_query();
    
    // discard any packets that we didn't wait long enough for
    while (1) {
        sockaddr_in from;
        socklen_t fromLen = sizeof(sockaddr_in);
        int ret = recvfrom(g_udp_socket, (char*)g_buf, sizeof(g_buf), 0, (sockaddr*)&from, &fromLen);
        if (ret < 0)
            break;
    }

    //unordered_map<string, ZanSvResp> servers;
    //servers.clear();
    //servers["143.47.120.79_10666"] = ZanSvResp();

    for (auto item : g_zandronum_servers) {
        g_stream = mstream(g_buf, sizeof(g_buf));
        g_stream.seek(0);

        uint32_t challenge = 199;
        uint32_t flags = SQF_NAME | SQF_MAPNAME | SQF_MAXPLAYERS | SQF_NUMPLAYERS | SQF_PLAYERDATA;
        //uint32_t time = getEpochMillis() & 0xffffffffull;
        uint32_t time = 0; // docs say this is for calculating ping... but I can save this myself?

        g_stream.write(&challenge, 4);
        g_stream.write(&flags, 4);
        g_stream.write(&time, 4);

        uint64_t ip = ipstring_to_uint64(item.first);
        sockaddr_in svAddr = uint64_to_netaddr(ip);

        sendPacket(g_udp_socket, svAddr, huffencode((uint8_t*)g_stream.getBuffer(), g_stream.tell()));
        //printf("Sent server query to %s\n", netaddr_to_ipstring(svAddr).c_str());
    }

    uint64_t startQuery = getEpochMillis();

    while (1) {
        sockaddr_in from;
        socklen_t fromLen = sizeof(sockaddr_in);
        memset(g_buf, 0, sizeof(g_buf));

        int ret = recvfrom(g_udp_socket, (char*)g_buf, sizeof(g_buf), 0, (sockaddr*)&from, &fromLen);

        if (ret <= 0) {
            if (getEpochMillis() - startQuery > TIMEOUT) {
                printf("Timed out waiting for server response\n");
                break; // no more queued packets
            }

            std::this_thread::sleep_for(milliseconds(1));
            continue;
        }

        uint64_t ipint = netaddr_to_uint64(from);
        string fromAddrStr = netaddr_to_ipstring(from);

        vector<uint8_t> bytes = huffdecode((uint8_t*)g_buf, ret);
        memcpy(g_buf, &bytes[0], bytes.size());
        g_stream = mstream(g_buf, bytes.size());

        uint32_t code;
        g_stream.read(&code, 4);

        bool valid = false;
        bool segmented = false;

        switch (code) {
        case SERVER_LAUNCHER_CHALLENGE:
            valid = true;
            //printf("%s: Accepted.\n", fromAddrStr.c_str());
            break;
        case SERVER_LAUNCHER_IGNORING:
            printf("%s: Ignoring too fast query.\n", fromAddrStr.c_str());
            break;
        case SERVER_LAUNCHER_BANNED:
            printf("%s: Your IP is banned.\n", fromAddrStr.c_str());
            break;
        case SERVER_LAUNCHER_CHALLENGE_SEGMENTED:
            segmented = true;
            printf("%s: Segmented info follows.\n", fromAddrStr.c_str());
            break;
        default:
            printf("%s: Unexpected server response code %d.\n", fromAddrStr.c_str(), code);
            break;
        }

        if (!valid)
            break;

        uint32_t time;
        uint32_t flags;

        g_stream.read(&time, 4);
        const char* version = read_string();
        g_stream.read(&flags, 4);

        ZanSvResp& resp = g_zandronum_servers[fromAddrStr];
        parse_sv_response(resp, flags);
        //printf("(%d/%d) %s - %s\n", (int)resp.players.size(), resp.maxplayers, resp.mapname.c_str(), resp.name.c_str());
    }
}

void populate_zandronum_player_lists() {
    int numFailed = 0;

    for (auto& item : g_zandronum_servers) {
        ZanSvResp& resp = item.second;
        auto serv = g_servers.find(item.first);

        if (serv == g_servers.end()) {
            numFailed++;
            continue;
        }

        ServerState& state = serv->second;

        state.a2s_players.clear();
        state.a2s_success = resp.valid;

        for (int i = 0; i < resp.players.size(); i++) {
            ZanPlayerDat& src = resp.players[i];
            
            Player plr;
            plr.score = src.points;
            plr.name = src.name;
            plr.duration = src.time * 60;

            state.a2s_players.push_back(plr);
        }
    }

    printf("%d servers failed to respond\n", numFailed);
}