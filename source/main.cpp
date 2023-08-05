#include <tonc.h>
#include <string>

#include "debug.h"
#include "flash_mem.h"
#include "gba_flash.h"
#include "interrupt.h"
#include "gb_link.h"
#include "gameboy_colour.h"
#include "LinkGPIO.h"
#include "pokemon.h"
#include "random.h"
#include "text_engine.h"
#include "background_engine.h"
#include "pokemon_party.h"
#include "script_array.h"

/*TODO:
--------
ACCURACY:
- See if shiny Pokemon from gen 2 can be square shinies or if they're all normal

LINK CABLE:
- Prevent trading through time capsule
- Prevent going to the battle area
- Implement gen 1 lol

CONVERSION:
- Fix fateful encounter so Mew and Celebi work
- Fix unown sometimes not being right (J becomes N)
- Add in check for glitch Pokemon
- Add in gen 1
	- Gen 1 character array
- Restrict illegal moves
- Set full default PP points for ALL moves (and PP ups)
- Check for duplicate moves?
- What happens with Smeargle?

INJECTION:
- Redo injection to be picked up like a Mystery Gift
- Double check that returns false when PC is full BEFORE injecting
- Fix coruption and Pokemon injection
- Add Pokemon to Pokedex
- Randomize base seed
- Enable ribbon viewing

TESTING:
- Test all the aspects of a Pokemon (Shiny, Pokerus, etc.)
- Test invalid moves
- Test move bubbling
--------
*/

// This file is autogenerated from the file in the graphics folder
#include "metr.h"

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer = (OBJ_AFFINE *)obj_buffer;

// byte party_data[441] = {0x8A, 0x91, 0x88, 0x92, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x6, 0x9B, 0x9B, 0x9B, 0x9B, 0x9B, 0x9B, 0xFF, 0x25, 0xDA, 155, 0x0, 0x21, 0x2B, 0x0, 0x0, 0x25, 0xDA, 0x0, 0x0, 0x87, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x72, 0x21, 0x23, 0x1E, 0x0, 0x0, 0x46, 0x0, 0x85, 0x81, 0x5, 0x0, 0x0, 0x0, 0x13, 0x0, 0x13, 0x0, 0x0A, 0x0, 0x9, 0x0, 0x0B, 0x0, 0x0B, 0x0, 0x0A, 0x9B, 0x0, 0x21, 0x2B, 0x0, 0x0, 0x25, 0xDA, 0x0, 0x0, 0x87, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x72, 0x21, 0x23, 0x1E, 0x0, 0x0, 0x46, 0x0, 0x85, 0x81, 0x5, 0x0, 0x0, 0x0, 0x13, 0x0, 0x13, 0x0, 0x0A, 0x0, 0x9, 0x0, 0x0B, 0x0, 0x0B, 0x0, 0x0A, 0x9B, 0x0, 0x21, 0x2B, 0x0, 0x0, 0x25, 0xDA, 0x0, 0x0, 0x87, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x72, 0x21, 0x23, 0x1E, 0x0, 0x0, 0x46, 0x0, 0x85, 0x81, 0x5, 0x0, 0x0, 0x0, 0x13, 0x0, 0x13, 0x0, 0x0A, 0x0, 0x9, 0x0, 0x0B, 0x0, 0x0B, 0x0, 0x0A, 0x9B, 0x0, 0x21, 0x2B, 0x0, 0x0, 0x25, 0xDA, 0x0, 0x0, 0x87, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x72, 0x21, 0x23, 0x1E, 0x0, 0x0, 0x46, 0x0, 0x85, 0x81, 0x5, 0x0, 0x0, 0x0, 0x13, 0x0, 0x13, 0x0, 0x0A, 0x0, 0x9, 0x0, 0x0B, 0x0, 0x0B, 0x0, 0x0A, 0x9B, 0x0, 0x21, 0x2B, 0x0, 0x0, 0x25, 0xDA, 0x0, 0x0, 0x87, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x72, 0x21, 0x23, 0x1E, 0x0, 0x0, 0x46, 0x0, 0x85, 0x81, 0x5, 0x0, 0x0, 0x0, 0x13, 0x0, 0x13, 0x0, 0x0A, 0x0, 0x9, 0x0, 0x0B, 0x0, 0x0B, 0x0, 0x0A, 0x9B, 0x0, 0x21, 0x2B, 0x0, 0x0, 0x25, 0xDA, 0x0, 0x0, 0x87, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x72, 0x21, 0x23, 0x1E, 0x0, 0x0, 0x46, 0x0, 0x85, 0x81, 0x5, 0x0, 0x0, 0x0, 0x13, 0x0, 0x13, 0x0, 0x0A, 0x0, 0x9, 0x0, 0x0B, 0x0, 0x0B, 0x0, 0x0A, 0x8A, 0x91, 0x88, 0x92, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x8A, 0x91, 0x88, 0x92, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x8A, 0x91, 0x88, 0x92, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x8A, 0x91, 0x88, 0x92, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x8A, 0x91, 0x88, 0x92, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x8A, 0x91, 0x88, 0x92, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x82, 0x98, 0x8D, 0x83, 0x80, 0x90, 0x94, 0x88, 0x8B, 0x50, 0x50, 0x82, 0x98, 0x8D, 0x83, 0x80, 0x90, 0x94, 0x88, 0x8B, 0x50, 0x50, 0x82, 0x98, 0x8D, 0x83, 0x80, 0x90, 0x94, 0x88, 0x8B, 0x50, 0x50, 0x82, 0x98, 0x8D, 0x83, 0x80, 0x90, 0x94, 0x88, 0x8B, 0x50, 0x50, 0x82, 0x98, 0x8D, 0x83, 0x80, 0x90, 0x94, 0x88, 0x8B, 0x50, 0x50, 0x82, 0x98, 0x8D, 0x83, 0x80, 0x90, 0x94, 0x88, 0x8B, 0x50, 0x50};

/*byte party_data[444] = {
	0x8A, 0x91, 0x88, 0x92, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x06, 0x9B, 0x13, 0xA1, 0xA1, 0xA1, 0xA1, 0xFF, 0x25, 0xDA,

	0x8B, 0x00, 0x37, 0x35, 0x21, 0x2B, 0x25, 0xDA, 0x00, 0x00, 0xA7, 0x00, 0x46, 0x00, 0x5C, 0x00, 0x44, 0x00, 0x28, 0x00, 0x46, 0xFA, 0xAA, 0x1F, 0x1E, 0x00, 0x00, 0x45, 0x00, 0x85, 0x81, 0x05, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x13, 0x00, 0x0A, 0x00, 0x09, 0x00, 0x0B, 0x00, 0x0B, 0x00, 0x0A,
	0xC9, 0x00, 0x21, 0x27, 237, 0x00, 0x25, 0xDA, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2D, 0x11, 0x23, 0x1E, 0x00, 0x00, 0x46, 0x00, 0x82, 0x82, 0x02, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x0D, 0x00, 0x07, 0x00, 0x06, 0x00, 0x07, 0x00, 0x06, 0x00, 0x06,
	0xA1, 0x00, 0x21, 0x00, 0x00, 0x00, 0x25, 0xDA, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x8B, 0x23, 0x00, 0x00, 0x00, 0x46, 0x00, 0x83, 0x82, 0x03, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x08, 0x00, 0x07, 0x00, 0x06, 0x00, 0x07, 0x00, 0x08,
	0x97, 0x00, 0x21, 0x00, 0x00, 0x00, 0x25, 0xDA, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDB, 0x87, 0x21, 0x00, 0x00, 0x00, 0x46, 0x00, 0x82, 0x82, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x0D, 0x00, 0x07, 0x00, 0x06, 0x00, 0x06, 0x00, 0x06, 0x00, 0x07,
	0xFB, 0x00, 0x21, 0x00, 0x00, 0x00, 0x25, 0xDA, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x34, 0x21, 0x00, 0x00, 0x00, 0x46, 0x00, 0x83, 0x82, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0F, 0x00, 0x07, 0x00, 0x07, 0x00, 0x06, 0x00, 0x07, 0x00, 0x07,
	0xA1, 0x00, 0x21, 0x00, 0x00, 0x00, 0x25, 0xDA, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x23, 0x00, 0x00, 0x00, 0x46, 0x00, 0x82, 0x82, 0x02, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x0D, 0x00, 0x07, 0x00, 0x06, 0x00, 0x05, 0x00, 0x06, 0x00, 0x06,

	0x8A, 0x91, 0x88, 0x92, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50,
	0x8A, 0x91, 0x88, 0x92, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50,
	0x8A, 0x91, 0x88, 0x92, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50,
	0x8A, 0x91, 0x88, 0x92, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50,
	0x8A, 0x91, 0x88, 0x92, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50,
	0x8A, 0x91, 0x88, 0x92, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50,

	0x82, 0x98, 0x8D, 0x83, 0x80, 0x90, 0x94, 0x88, 0x8B, 0x50, 0x50,
	0x91, 0x80, 0x93, 0x93, 0x80, 0x93, 0x80, 0x50, 0x50, 0x50, 0x50,
	0xF1, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xE1, 0xE2, 0x50, 0x50,
	0x92, 0x84, 0x8D, 0x93, 0x91, 0x84, 0x93, 0x50, 0x50, 0x50, 0x50,
	0x92, 0x84, 0x8D, 0x93, 0x91, 0x84, 0x93, 0x50, 0x50, 0x50, 0x50,
	0xE3, 0xE6, 0xE7, 0xF3, 0xF2, 0xF4, 0x7F, 0x50, 0x50, 0x50, 0x50,

	0x00, 0x00, 0x00};
*/
Pokemon_Party party = Pokemon_Party();
text_engine main_text = text_engine();

//Pokemon party[6] = {};

vu8 pkmn[480] = {
	0x5C, 0xF2, 0x9B, 0x70, 0x9B, 0x2A, 0x81, 0xE4, 0xBB, 0xC6, 0xBC, 0xBF, 0xCC, 0xCE, 0xFF, 0x00,
	0x00, 0x00, 0x02, 0x02, 0xBB, 0xCF, 0xCD, 0xCE, 0xC3, 0xC8, 0xFF, 0x00, 0x8B, 0x07, 0x00, 0x00,
	0x0E, 0xD8, 0x1A, 0x94, 0xC7, 0xD8, 0x1A, 0x94, 0xC7, 0xD8, 0x1A, 0x94, 0xC7, 0x60, 0x1B, 0xB6,
	0xBD, 0xC2, 0x51, 0x85, 0xC7, 0xD8, 0x1A, 0x94, 0x2A, 0xD8, 0x1A, 0x94, 0xC7, 0xD8, 0x1A, 0x94,
	0xC8, 0xD8, 0x1A, 0x94, 0xC7, 0xD8, 0x1A, 0x94, 0xC7, 0xD8, 0x1A, 0x94, 0xC7, 0xD8, 0x1A, 0x94,

	0xD1, 0xA8, 0x64, 0x28, 0x9B, 0x2A, 0x81, 0xE4, 0xBC, 0xBB, 0xCC, 0xCE, 0xFF, 0xCE, 0xFF, 0x00,
	0x00, 0x00, 0x02, 0x02, 0xBB, 0xCF, 0xCD, 0xCE, 0xC3, 0xC8, 0xFF, 0x00, 0x8B, 0x07, 0x00, 0x00,
	0x83, 0x82, 0xE5, 0xCC, 0x4A, 0x82, 0xE5, 0xCC, 0x4A, 0x82, 0xE5, 0xCC, 0xA7, 0x82, 0xE5, 0xCC,
	0x4A, 0x82, 0xE5, 0xCC, 0x45, 0x82, 0xE5, 0xCC, 0x4A, 0x3A, 0xE4, 0xEE, 0x30, 0x98, 0xAE, 0xDD,
	0x4A, 0x82, 0xE5, 0xCC, 0x4A, 0x82, 0xE5, 0xCC, 0x4A, 0x82, 0xE5, 0xCC, 0x4A, 0x82, 0xE5, 0xCC,

	0x4E, 0x5B, 0xB7, 0xF3, 0x9B, 0x2A, 0x81, 0xE4, 0xBD, 0xBB, 0xCC, 0xCE, 0xBF, 0xCC, 0xFF, 0x00,
	0x00, 0x00, 0x02, 0x02, 0xBB, 0xCF, 0xCD, 0xCE, 0xC3, 0xC8, 0xFF, 0x00, 0x8B, 0x07, 0x00, 0x00,
	0xD5, 0xC9, 0x37, 0x35, 0xAF, 0x6B, 0x7D, 0x06, 0xD5, 0x71, 0x36, 0x17, 0xD5, 0x71, 0x36, 0x17,
	0xD5, 0x71, 0x36, 0x17, 0xD5, 0x71, 0x36, 0x17, 0x1C, 0x71, 0x36, 0x17, 0xD5, 0x71, 0x36, 0x17,
	0xD5, 0x71, 0x36, 0x17, 0x38, 0x71, 0x36, 0x17, 0xD5, 0x71, 0x36, 0x17, 0xDA, 0x71, 0x36, 0x17,

	0x5F, 0x80, 0x16, 0xBB, 0x9B, 0x2A, 0x81, 0xE4, 0xBE, 0xBB, 0xD1, 0xC8, 0xFF, 0xCC, 0xFF, 0x00,
	0x00, 0x00, 0x02, 0x02, 0xBB, 0xCF, 0xCD, 0xCE, 0xC3, 0xC8, 0xFF, 0x00, 0x8B, 0x07, 0x00, 0x00,
	0xC4, 0xAA, 0x97, 0x5F, 0xC4, 0xAA, 0x97, 0x5F, 0xC4, 0xAA, 0x97, 0x5F, 0x29, 0xAA, 0x97, 0x5F,
	0xC4, 0xAA, 0x97, 0x5F, 0xCB, 0xAA, 0x97, 0x5F, 0xC4, 0x12, 0x96, 0x7D, 0xBE, 0xB0, 0xDC, 0x4E,
	0xC4, 0xAA, 0x97, 0x5F, 0x0D, 0xAA, 0x97, 0x5F, 0xC4, 0xAA, 0x97, 0x5F, 0xC4, 0xAA, 0x97, 0x5F,

	0x14, 0x43, 0x7A, 0xCE, 0x9B, 0x2A, 0x81, 0xE4, 0xBF, 0xCC, 0xC3, 0xC5, 0xFF, 0xCC, 0xFF, 0x00,
	0x00, 0x00, 0x02, 0x02, 0xBB, 0xCF, 0xCD, 0xCE, 0xC3, 0xC8, 0xFF, 0x00, 0x8B, 0x07, 0x00, 0x00,
	0x46, 0x69, 0xFB, 0x2A, 0x8F, 0x69, 0xFB, 0x2A, 0x8F, 0x69, 0xFB, 0x2A, 0x8F, 0xD1, 0xFA, 0x08,
	0xF5, 0x73, 0xB0, 0x3B, 0x8F, 0x69, 0xFB, 0x2A, 0x62, 0x69, 0xFB, 0x2A, 0x8F, 0x69, 0xFB, 0x2A,
	0x80, 0x69, 0xFB, 0x2A, 0x8F, 0x69, 0xFB, 0x2A, 0x8F, 0x69, 0xFB, 0x2A, 0x8F, 0x69, 0xFB, 0x2A,

	0xF5, 0x91, 0x98, 0x00, 0x9B, 0x2A, 0x81, 0xE4, 0xC0, 0xCC, 0xBB, 0xC8, 0xC5, 0xFF, 0xFF, 0x00,
	0x00, 0x00, 0x02, 0x02, 0xBB, 0xCF, 0xCD, 0xCE, 0xC3, 0xC8, 0xFF, 0x00, 0x8B, 0x07, 0x00, 0x00,
	0xA7, 0xBB, 0x19, 0xE4, 0x6E, 0xBB, 0x19, 0xE4, 0x6E, 0xBB, 0x19, 0xE4, 0x6E, 0x03, 0x18, 0xC6,
	0x14, 0xA1, 0x52, 0xF5, 0x6E, 0xBB, 0x19, 0xE4, 0x6E, 0xBB, 0x19, 0xE4, 0x6E, 0xBB, 0x19, 0xE4,
	0x6E, 0xBB, 0x19, 0xE4, 0x83, 0xBB, 0x19, 0xE4, 0x6E, 0xBB, 0x19, 0xE4, 0x61, 0xBB, 0x19, 0xE4};

void load_sprite(void)
{
	// Load tiles and palette of sprite into video and palete RAM
	memcpy32(&tile_mem[4][0], metrTiles, metrTilesLen / 4);
	memcpy32(pal_obj_mem, metrPal, metrPalLen / 4);

	oam_init(obj_buffer, 128);

	OBJ_ATTR *metr = &obj_buffer[0];
	obj_set_attr(metr,
				 ATTR0_SQUARE,			// Square, regular sprite
				 ATTR1_SIZE_64,			// 64x64 pixels,
				 ATTR2_PALBANK(0) | 0); // palette index 0, tile index 0

	// Set position
	obj_set_pos(metr, 0, 0);

	oam_copy(oam_mem, obj_buffer, 1); // Update first OAM object
}

/*void load_pokemon(byte *party_array, int gen)
{
	int party_size = party_array[11];
	for (int i = 0; i < party_size; i++)
	{
		party[i].convert_from_gen(2, i, &party_array[0]);
		poke_rand.next_frame();
		party[i].convert_to_gen_three(poke_rand.get_rand());
	}
}
*/

int main(void)
{

	// Initalizations
	linkGPIO->reset();

	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;

	irq_init(NULL);
	irq_enable(II_VBLANK);

	flash_init(FLASH_SIZE_128KB);
	initalize_memory_locations();
	rand_set_seed(0x12162001);

	back_main();
	add_script_party_var(party);
	// load_sprite();

	// MAIN LOOP
	while (1)
	{
		key_poll();
		rand_next_frame();
		back_frame();
		main_text.next_frame();
	}
}

/*
{
	bool print = false;
	char str[128] = {'\0'};
	int arr = 0;
	int frame = 0;
	while (1)
	{
		u16 keys = ~REG_KEYS & KEY_ANY;
		poke_rand.next_frame();

		vid_vsync();
		key_poll();
		if (key_released(KEY_UP))
		{
			arr++;
			if (arr == 14)
			{
				arr = 0;
			}
		}
		if (key_released(KEY_DOWN))
		{
			arr--;
			if (arr == -1)
			{
				arr = 13;
			}
		}
		if (key_released(KEY_A))
		{
			load_pokemon(&party_data[0], 2);
			insert_pokemon(&party[0], 6);
			print = true;
		}

		if (key_released(KEY_START))
		{
			setup();
			loop(&party_data[0]);
		}

		tte_erase_screen();

		tte_set_pos(0, 0);
		printValueAt(memory_section_array[arr] + 0xFF4);

		tte_set_pos(0, 10);
		sprintf(str, "[%c-%X] R:%X", mem_name, arr, poke_rand.get_rand());
		tte_write(str);

		tte_set_pos(0, 20);
		sprintf(str, "%X %X %X %X %X %X %X %X", party_data[0], party_data[1], party_data[2], party_data[3], party_data[4], party_data[5], party_data[6], party_data[7]);
		tte_write(str);
		if (print)
		{
			tte_set_pos(0, 30);
			tte_write("Pokemon Injected!");
			tte_set_pos(0, 40);
			for (int i = 0; i < 80; i++)
			{
				tte_write(intToHex(party[0].get_gen_3_data(i)).c_str());
				tte_write(" ");
			}
			tte_set_pos(0, 120);
			for (int i = 0; i < 49; i++)
			{
				tte_write(intToHex(party[0].get_unencrypted_data(i)).c_str());
				tte_write(" ");
			}
		}

		tte_set_pos(0, 30);

		frame++;
		VBlankIntrWait();
	}
}
}
*/
/*Credits:
https://github.com/laqieer/libsavgba
https://github.com/rodri042/gba-link-connection
arduino-boy

Notes:
- GBA and GBC link cables are different, that's why it didn't work initally on mGBA
*/
