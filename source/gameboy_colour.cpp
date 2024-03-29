// Loosely based on code created by StevenChaulk
// Source: https://github.com/stevenchaulk/arduino-poke-gen2

#include <tonc.h>
#include <string>
#include "gameboy_colour.h"
#include "pokemon_trade.h"
#include "output.h"
#include "script_array.h"
#include "debug_mode.h"
#include "payload.h"
#include "interrupt.h"
#include "text_engine.h"
#include "global_frame_controller.h"

#define TIMEOUT 2
#define TIMEOUT_ONE_LENGTH 1000000 // Maybe keep a 10:1 ratio between ONE and TWO?
#define TIMEOUT_TWO_LENGTH 100000

#define hs 0
#define ack 1
#define menu 2
#define trade 3
#define party_preamble 4
#define colosseum 5
#define cancel 6
#define trade_data 7
#define box_preamble 8
#define box_data 9
#define end 10

const int MODE = 1; // mode=0 will transfer pokemon data from pokemon.h
                    // mode=1 will copy pokemon party data being received

LinkSPI *linkSPI = new LinkSPI();

uint8_t in_data;
uint8_t out_data;
uint frame;

connection_state_t connection_state;

int counter;
int data_counter = 0;
int gen_1_room_counter = 0;
int gen;
int trade_pokemon;

int FF_count;
int zero_count;

int state;
int mosi_delay = 3; // inital delay, speeds up once sending PKMN

std::string out_array[10];

void print(std::string str)
{
  for (int i = 10; i > 0; i--)
  {
    out_array[i] = out_array[i - 1];
  }
  out_array[0] = str + "\n";

  tte_erase_rect(LEFT, TOP, RIGHT, BOTTOM);
  tte_set_pos(LEFT, 0);
  for (int j = 0; j < 10; j++)
  {
    tte_write("#{cx:0xE000}");
    tte_write(out_array[j].c_str());
  }
}

void setup()
{
  interrupt_init();
  interrupt_set_handler(INTR_SERIAL, LINK_SPI_ISR_SERIAL);
  interrupt_enable(INTR_SERIAL);

  linkSPI->activate(LinkSPI::Mode::MASTER_256KBPS);
  linkSPI->setWaitModeActive(false);

  state = hs;

  in_data = 0;
  out_data = 0;
  frame = 0;

  connection_state = NOT_CONNECTED;
  counter = 0;

  gen = 0;

  trade_pokemon = -1;

  FF_count = 0;
  zero_count = 0;

  if (DEBUG_MODE)
  {
    tte_erase_rect(LEFT, TOP, RIGHT, BOTTOM);
    tte_set_pos(0, 0);
    tte_write("FEED ME POKEMON, I HUNGER!\n");
  }
}

byte handleIncomingByte(byte in, byte *box_data_storage)
{
  if (state == hs)
  {
    if (in == 0x00)
    {
      state = ack;
      return 0x01;
    }
  }

  else if (state == ack)
  {
    if (in == 0x00)
    {
      state = menu;
      return 0x00;
    }
  }

  else if (state == menu)
  {
    if (in == 0xd4)
    {
      state = trade;
      return in;
    }
    else
    {
      return in;
    }
  }

  else if (state == trade)
  {
    if (in == 0xfd)
    {
      state = party_preamble;
      mosi_delay = 0;
    }
    return in;
  }

  else if (state == party_preamble)
  {
    if (in != 0xfd)
    {
      state = trade_data;
      return exchange_parties(in);
    }
    return in;
  }

  else if (state == trade_data)
  {
    if (data_counter >= PAYLOAD_SIZE && in == 0xFD)
    {
      state = box_preamble;
    }
    return exchange_parties(in);
  }

  else if (state == box_preamble)
  {
    if (in != 0xFD)
    {
      state = box_data;
      data_counter = 0;
      mosi_delay = 1;
      return exchange_boxes(in, box_data_storage);
    }
    return in;
  }

  else if (state == box_data)
  {
    if (data_counter >= BOX_DATA_ARRAY_SIZE)
    {
      state = end;
    }
    return exchange_boxes(in, box_data_storage);
  }

  return in;
}

int loop(byte *box_data_storage)
{
  int counter = 0;
  while (true)
  {
    // TODO: Restore Errors
    in_data = linkSPI->transfer(out_data);

    if (DEBUG_MODE)
    {
      print(
          std::to_string(counter) + ": [" +
          std::to_string(data_counter) + "][" +
          std::to_string(state) + "][" +
          std::to_string(in_data) + "][" +
          std::to_string(out_data) + "]\n");
    }
    out_data = handleIncomingByte(in_data, box_data_storage);

    if (FF_count > 25)
    {
      // return COND_ERROR_DISCONNECT;
    }
    if (zero_count > 25)
    {
      // return COND_ERROR_COM_ENDED;
    }
    if (connection_state == COLOSSEUM)
    {
      return COND_ERROR_COLOSSEUM;
    }

    if (state == end)
    {
      return 0;
    }

    if (DEBUG_MODE && key_hit(KEY_SELECT)){
      return COND_ERROR_DISCONNECT;
    }

    counter++;
    for (int i = 0; i < mosi_delay; i++)
    {
      global_next_frame();
    }

    if (counter > (60 * 10))
    {
      // return COND_ERROR_TIMEOUT_ONE;
    }
  }
};

byte exchange_parties(byte curr_in)
{
  int ret = gen1_eng_payload[data_counter];
  data_counter += 1;
  return ret;
};

byte exchange_boxes(byte curr_in, byte *box_data_storage)
{
  box_data_storage[data_counter] = curr_in;
  data_counter += 1;
  return curr_in;
};
