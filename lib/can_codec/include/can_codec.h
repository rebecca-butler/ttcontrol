#ifndef _BRUPERVISOR_CANCODEC_H_
#define _BRUPERVISOR_CANCODEC_H_

#include "can_types.h"

#define MASK64(nbits) ((0xffffffffffffffff) >> (64 - nbits))

void CAN_store_signal(CAN_frame_t* frame, CAN_signal_t sig, double val);
double CAN_extract_signal(CAN_frame_t* frame, CAN_signal_t sig);

#endif // _BRUPERVISOR_CANCODEC_H_
