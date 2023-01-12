
#ifndef _BRUPERVISOR_CANTYPES_H_
#define _BRUPERVISOR_CANTYPES_H_

#include <stdint.h>

typedef enum { CAN_UNSIGNED = 0, CAN_SIGNED = 1 } CAN_signedness_t;
typedef enum { CAN_BIG = 0, CAN_LITTLE = 1 } CAN_endianess_t;
typedef enum { CAN_STANDARD = 0, CAN_EXTENDED = 1} CAN_id_format_t;

// Store a can frame
typedef struct {
  uint8_t data[8];
  uint8_t length;
  CAN_id_format_t id_format;
  uint32_t id;
} CAN_frame_t;

// Stores a signal definition
typedef struct {
    uint8_t          bit_start;
    uint8_t          bit_len;
    CAN_endianess_t  endianess;
    CAN_signedness_t signedness;
    float            scale;
    float            offset;
    float            min;
    float            max;
} CAN_signal_t;

// Store meta data about a can message definition
// Could be stored in a CAN frame but that doesn't seem necessary and will consume more memory
typedef const struct{
  uint32_t id;
  uint8_t length;
  CAN_id_format_t format;
} CAN_msg_meta_t;


CAN_frame_t CAN_init_frame(CAN_msg_meta_t msg_meta);

#endif // _BRUPERVISOR_CANTYPES_H_
