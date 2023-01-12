#include "can_types.h"
#include "j1939_checksum.h"

uint8_t j1939_checksum(CAN_frame_t* frame, const uint32_t id)
{
    uint32_t checksum = 0;
    for (int i = 0; i < 7; i++) checksum += frame->data[i];
    checksum += frame->data[7] & 0xF;
    for (int i = 0; i < 4; i++) checksum += (id >> (i * 8)) & 0xFF;
    return ((checksum >> 4) + checksum) & 0xF;
}
