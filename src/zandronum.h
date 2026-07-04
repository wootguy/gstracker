#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

enum GameModes {
    ZAN_GAMEMODE_COOPERATIVE = 0,
    ZAN_GAMEMODE_SURVIVAL = 1,
    ZAN_GAMEMODE_INVASION = 2,
    ZAN_GAMEMODE_DEATHMATCH = 3,
    ZAN_GAMEMODE_TEAMPLAY = 4,
    ZAN_GAMEMODE_DUEL = 5,
    ZAN_GAMEMODE_TERMINATOR = 6,
    ZAN_GAMEMODE_LASTMANSTANDING = 7,
    ZAN_GAMEMODE_TEAMLMS = 8,
    ZAN_GAMEMODE_POSSESSION = 9,
    ZAN_GAMEMODE_TEAMPOSSESSION = 10,
    ZAN_GAMEMODE_TEAMGAME = 11,
    ZAN_GAMEMODE_CTF = 12,
    ZAN_GAMEMODE_ONEFLAGCTF = 13,
    ZAN_GAMEMODE_SKULLTAG = 14,
    ZAN_GAMEMODE_DOMINATION = 15,
};

struct ZanPlayerDat {
    string name;		    // SQF_PLAYERDATA 	Player's name
    uint16_t points = 0;	// SQF_PLAYERDATA 	Player's pointcount/fragcount/killcount
    uint16_t ping = 0;		// SQF_PLAYERDATA 	Player's ping
    uint8_t	spectate = 0;	// SQF_PLAYERDATA 	Player is spectating - true (1) / false (0)
    uint8_t	bot = 0;		// SQF_PLAYERDATA 	Player is a bot - true (1) / false (0)
    uint8_t	team = 0;		// SQF_PLAYERDATA 	Player's team (returned on team games, 255 is no team)
    uint8_t	time = 0;		// SQF_PLAYERDATA 	Player's time on the server, in minutes. Note:SQF_PLAYERDATA information is sent once for each player on the server.
};

struct ZanTeamInfo {
    uint8_t num = 0;	// SQF_TEAMINFO_NUMBER 	The number of teams used.
    string name;		// SQF_TEAMINFO_NAME 	The team's name. (Sent for each team.)
    int32_t color = 0;	// SQF_TEAMINFO_COLOR 	The team's color. (Sent for each team.)
    uint16_t score = 0;	// SQF_TEAMINFO_SCORE 	The team's score. (Sent for each team.)
};

struct ZanSvResp {
    bool valid = false;             // true if any data was received
    string name;	                // SQF_NAME 	The server's name (sv_hostname)
    string url;		                // SQF_URL 	The server's WAD URL (sv_website)
    string email;	                // SQF_EMAIL 	The server host's e-mail (sv_hostemail)
    string mapname;	                // SQF_MAPNAME 	The current map's name
    uint8_t maxclients = 0;	        // SQF_MAXCLIENTS 	The max number of clients (sv_maxclients)
    uint8_t	maxplayers = 0;         // SQF_MAXPLAYERS 	The max number of players (sv_maxplayers)
    uint8_t	numPwads = 0;	        // SQF_PWADS 	The number of PWADs loaded
    vector<string> pwads;		    // SQF_PWADS 	The PWAD's name (Sent for each PWAD)
    uint8_t gametype = 0;			// SQF_GAMETYPE 	The current game mode. See below.
    uint8_t	instagib = 0;		    // SQF_GAMETYPE 	Instagib - true (1) / false (0)
    uint8_t	buckshot = 0;           // SQF_GAMETYPE 	Buckshot - true (1) / false (0)
    string gamename;		        // SQF_GAMENAME 	The base game's name ("DOOM", "DOOM II", "HERETIC", "HEXEN", "ERROR!")
    string iwad;			        // SQF_IWAD 	The IWAD's name
    uint8_t forcepw = 0;		    // SQF_FORCEPASSWORD 	Whether a password is required to join the server (sv_forcepassword)
    uint8_t	forcejoinpw = 0;		// SQF_FORCEJOINPASSWORD 	Whether a password is required to join the game (sv_forcejoinpassword)
    uint8_t	gameskill = 0;		    // SQF_GAMESKILL 	The game's difficulty (skill)
    uint8_t	botskill = 0;		    // SQF_BOTSKILL 	The bot difficulty (botskill)
    int32_t	dmflags1 = 0;		    // SQF_DMFLAGS 	[Deprecated] Value of dmflags
    int32_t	dmflags2 = 0;	    	// SQF_DMFLAGS 	[Deprecated] Value of dmflags2
    int32_t	compatflags = 0;		// SQF_DMFLAGS 	[Deprecated] Value of compatflags
    uint16_t fraglimit = 0;			// SQF_LIMITS 	Value of fraglimit
    uint16_t timelimit = 0;			// SQF_LIMITS 	Value of timelimit
    uint16_t timeleft = 0;			// SQF_LIMITS 	time left in minutes (only sent if timelimit > 0)
    uint16_t duellimit = 0;			// SQF_LIMITS 	duellimit
    uint16_t pointlimit = 0;		// SQF_LIMITS 	pointlimit
    uint16_t winlimit = 0;			// SQF_LIMITS 	winlimit
    float teamdamage = 0;		    // SQF_TEAMDAMAGE 	The team damage scalar (teamdamage)
    uint16_t teamscores_blue = 0;   // SQF_TEAMSCORES 	[Deprecated] Blue team's fragcount/wincount/score
    uint16_t teamscores_red = 0;	// SQF_TEAMSCORES 	[Deprecated] Red team's fragcount/wincount/score
    vector<ZanPlayerDat> players;   // SQF_PLAYERDATA
    vector<ZanTeamInfo> teams;
    uint8_t test = 0;	 		    // SQF_TESTING_SERVER 	Whether this server is running a testing binary - true (1) / false (0)
    string test_name;		        // SQF_TESTING_SERVER 	An empty string in case the server is running a stable binary, otherwise name of the testing binary archive found in http://www.skulltag.com/testing/files/
    string md5;			            // SQF_DATA_MD5SUM 	[Deprecated] Returns an empty string.
    vector<int32_t> dmflags;	    // SQF_ALL_DMFLAGS 	The value of the flags (Sent for each flag in the order dmflags, dmflags2, zadmflags, compatflags, zacompatflags, compatflags2)
    uint8_t security = 0;		    // SQF_SECURITY_SETTINGS 	Whether the server is enforcing the master ban list - true (1) / false (0) The other bits of this uint8_t may be used to transfer other security related settings in the future.
    vector<uint8_t> optional_pwads;	// SQF_OPTIONAL_WADS 	Index number into the list sent with SQF_PWADS - this wad is optional (sent for each optional Wad)
    vector<string> deh_patches;	    // SQF_DEH 	Deh patch name (one string for each deh patch)
    int32_t sqf2_flags = 0;         // SQF_EXTENDED_INFO 	The flags specifying the SQF2_data that immediately follows. 
};

extern std::unordered_map<std::string, ZanSvResp> g_zandronum_servers;

void update_zandronum_servers();

void populate_zandronum_player_lists();