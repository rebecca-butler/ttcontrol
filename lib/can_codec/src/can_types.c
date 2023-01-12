#include "can_types.h"

CAN_frame_t CAN_init_frame(CAN_msg_meta_t meta){
  CAN_frame_t frame;
  for(int i =0; i<8; i++){
    frame.data[i] = 0;
  }
  frame.id = meta.id;
  frame.id_format = meta.format;
  frame.length = meta.length;
  return frame;
}
