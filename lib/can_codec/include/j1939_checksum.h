#ifndef _BRUPERVISOR_J1939CHECKSUM_H_
#define _BRUPERVISOR_J1939CHECKSUM_H_

#include "can_types.h"

uint8_t j1939_checksum(CAN_frame_t* frame, const uint32_t id);

#endif // _BRUPERVISOR_J1939CHECKSUM_H_
