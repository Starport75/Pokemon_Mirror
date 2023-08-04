#ifndef TEXT_ENGINE_H
#define TEXT_ENGINE_H

#include <tonc.h>
#include <string>

#include "script_obj.h"

class text_engine
{
public:
    text_engine();
    void next_frame();

private:
    script_obj curr_line;
    int frame_count;
    Pokemon_Party *party_data;
    void set_next_line(std::string next);
};

#endif