#ifndef ROM_POINTERS_H
#define ROM_POINTERS_H

#include <tonc.h>

#define RUBY_ID 0x415856      // AXV
#define SAPPHIRE_ID 0x415850  // AXP
#define FIRERED_ID 0x425052   // BPR
#define LEAFGREEN_ID 0x425047 // BPG
#define EMERALD_ID 0x425045   // BPE

#define VERS_1_0 0
#define VERS_1_1 1
#define VERS_1_2 2

#define LANG_JPN 'J'
#define LANG_ENG 'E'
#define LANG_FRE 'F'
#define LANG_GER 'D'
#define LANG_ITA 'I'
#define LANG_SPA 'S'

class rom_data
{
public:
    rom_data();
    bool load_rom();
    bool is_hoenn();
    bool is_ruby_sapphire();
    void print_rom_info();

    int gamecode;
    int version;
    int language;

    int loc_sendMonToPC;
    int loc_gSpecialVar_0x8000;
    int loc_gSaveBlock1;    // Only used in R/S
    int loc_gSaveBlock1PTR; // Only used in FR/LG/E
    int loc_setPokedexFlag;
    int offset_ramscript; // Ramscript offset as found within the SaveBlock1 struct in global.h
    int offset_flags;     // Flag offset as found within the SaveBlock1 struct in global.h
    int offset_wondercard;
    int offset_script;
    int text_region;

    // PKHeX's list of flags is useful for making sure the detection is accurate: https://github.com/kwsch/PKHeX/blob/78a557c3cdaa6f48b42cc96df8ccb4d20b897937/PKHeX.Core/Resources/text/other/flags_rs.txt
    int e4_flag; // The flag that is set when you become champion. Often listed as "GAME_CLEAR"
    int mg_flag; // The flag that is set when you enable Mystery Gift. Known as "EXDATA_ENABLE" in RS

    u8 map_bank;
    u8 map_id;
    u8 npc_id;

    u8 def_map_bank;
    u8 def_map_id;
    u8 def_npc_id;
};

#endif