#ifndef GB_LINK_H
#define GB_LINK_H
#include <tonc.h>

void log(std::string text);
void wait(u32 verticalLines);
inline void VBLANK() {}
void init();
int start_link();

#endif
