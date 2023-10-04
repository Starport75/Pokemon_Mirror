#include "script_array.h"
#include "main_menu.h"
#include "text_engine.h"
#include "script.h"

#include <tonc.h>

int last_error;
Pokemon_Party party_data;

script_obj script[17] =
    {
        script_obj("", 2),                                                                             // 0
        script_obj("Game not detected. Please turn off your system and upload the program again.", 2), // 1
        script_obj("Welcome to Pokemon Mirror!", 3),                                                   // 2
        script_obj("Please connect your GBA to a GB in the trading room, and press A", 4, START_LINK), // 3
        script_obj("", 6, ERROR_COM_ENDED, 5),                                                         // 4
        script_obj("Error: Communication ended.", 3),                                                  // 5
        script_obj("", 8, ERROR_DISCONNECT, 7),                                                        // 6
        script_obj("Error: Cable disconnected.", 3),                                                   // 7
        script_obj("", 10, ERROR_TIMEOUT_ONE, 9),                                                      // 8
        script_obj("Error: Timed out (1).", 3),                                                        // 9
        script_obj("", 12, ERROR_TIMEOUT_TWO, 11),                                                     // 10
        script_obj("Error: Timed out (2)", 3),                                                         // 11
        script_obj("", 14, ERROR_COLOSSEUM, 13),                                                       // 12
        script_obj("Error: Went into Colosseum", 3),                                                   // 13
        script_obj("Connection successful! Transfering in Pokemon now", 15),                           // 14
        script_obj("", 16, IMPORT_POKEMON),                                                            // 15
        script_obj("Bye!", 0, BACK_TO_MENU),                                                          // 16
};

void add_script_party_var(Pokemon_Party var)
{
    party_data = var;
}

bool run_conditional(int index)
{
    // Here is most of the logic that drives what lines show up where. It's probably not the best way to code it, but it works
    switch (index)
    {

    case START_LINK:
        party_data.start_link();
        return true;

    case ERROR_COM_ENDED:
        return party_data.get_last_error() != ERROR_COM_ENDED;

    case ERROR_DISCONNECT:
        return party_data.get_last_error() != ERROR_DISCONNECT;

    case ERROR_TIMEOUT_ONE:
        return party_data.get_last_error() != ERROR_TIMEOUT_ONE;

    case ERROR_TIMEOUT_TWO:
        return party_data.get_last_error() != ERROR_TIMEOUT_TWO;

    case ERROR_COLOSSEUM:
        return party_data.get_last_error() != ERROR_COLOSSEUM;

    case IMPORT_POKEMON:
        party_data.load_pokemon();
        //party_data.inject_pokemon();
        inject_mystery(party_data.get_full_pokemon_array());
        return true;

    case BACK_TO_MENU:
        text_disable();
        main_menu_exit();
        return true;


    default:
        tte_set_pos(0, 0);
        tte_write("ERROR! No conditional found.");
        return false;
    }
}
