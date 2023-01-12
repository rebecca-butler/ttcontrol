#include "can_codec.h"
#include "can_types.h"
#include <stdint.h>
#include <math.h>


// TODO test signedness for encoding(this seems to be handled by casting)
// NOTE We don't clamp the during signal extraction
// NOTE only supports max int32/uint32


/**
* Store a signal into a CAN Frame
* Supports endianess, signedness(+-),offset, scale, min,max
* Does not support Float and double signedness/type
*/
void CAN_store_signal(CAN_frame_t* frame, CAN_signal_t sig, double val){

  // Clamp the value if min or max is set
  if(sig.min != 0 | sig.max != 0){
    val = fmax(fmin(val,sig.max),sig.min);
  }

  // Convert from physical value(signedness handled by cast)
  uint64_t raw = (uint64_t)((val - sig.offset) / sig.scale);

  // Mask the value
  raw &= MASK64(sig.bit_len);

  /// Motorola (big endian)
  if (sig.endianess == CAN_BIG)
  {

    uint8_t MSB_index = sig.bit_start / 8;
    uint8_t MSB_len = fmin((sig.bit_start % 8) +1,sig.bit_len);

    uint8_t full_bytes = (sig.bit_len - MSB_len)/8;

    uint8_t LSB_index = MSB_index + ceil((sig.bit_len - MSB_len)/8.0);
    uint8_t LSB_len = (sig.bit_len-MSB_len-1)%8+1;
    uint8_t LSB_bit_offset = 8-LSB_len;

    // Signal spans a signal byte
    if(MSB_index == LSB_index){
      // Create a mask of ones the length of the signal and shift to proper location
      uint8_t shift = (sig.bit_start%8) + 1 - sig.bit_len;
      uint8_t mask = ((0x01<<sig.bit_len) - 1) << shift;

      // clear existing data
      frame->data[MSB_index] &= ~mask;

      // Load in signal
      frame->data[MSB_index] |= (uint8_t) (raw << shift) & mask;
    }

    // Signal spans multiple bytes
    else{

      // Load LSB first
      uint8_t lsb_mask = 0xFF << LSB_bit_offset; // Generate mask for active bits in this byte
      frame->data[LSB_index] &= ~lsb_mask; // Clear any existing data from this region
      frame->data[LSB_index] |= (uint8_t) (raw << LSB_bit_offset) & lsb_mask;

      // Load the remaining bits (LSB+1 to MSB)
      uint8_t loaded_bytes = 1;
      for(int8_t index = LSB_index - 1; index >= MSB_index; index--){

        // Create mask for each byte in the frame
        uint8_t mask = 0xFF; // single byte

        // Check if this is our last byte and only have to apply to part of the byte
        int8_t remaining_bits = sig.bit_len - ((loaded_bytes + 1) * 8) + LSB_bit_offset;
        if(remaining_bits < 0){
          mask = 0xFF >> (remaining_bits * -1);
        }

        // Clear any existing data from this region(byte)
        frame->data[index] &= ~mask;

        // Load shifted signal into proper byte
        uint8_t right_shift = 8 * (loaded_bytes-1) + LSB_len;
        frame->data[index] |= (uint8_t) (raw >> right_shift) & mask;

        loaded_bytes++;
      }
    }

  }
  // Intel (little endian)
  else{

    uint8_t LSB_index = sig.bit_start/8;
    uint8_t LSB_bit_offset = sig.bit_start %8;
    uint8_t mask = ((0x01<<(uint8_t)fmin(8-LSB_bit_offset,sig.bit_len))-1) << LSB_bit_offset;

    // Clear data in LSB byte
    frame->data[LSB_index] &= ~mask;

    // Write bits of startbyte
    frame->data[LSB_index] |= (uint8_t) (raw << LSB_bit_offset) & mask;

    uint8_t MSB_index = (sig.bit_start + sig.bit_len - 1) / 8;
    uint8_t loaded_bytes = 1;
    for (int16_t index = LSB_index + 1; index <= MSB_index; index++)
    {
      int8_t remaining_bits = sig.bit_len - (8-LSB_bit_offset) - loaded_bytes*8;
      // Create mask for signal
      if(remaining_bits < 0){
        mask = 0xFF >> (remaining_bits * -1);
      }else{
        mask = 0xFF;
      }
      // Clear data in signal
      frame->data[index] &= ~mask;

      // Shift in signal
      uint8_t right_shift = loaded_bytes * 8 - LSB_bit_offset;//(loaded_bytes-1)*8 + (8 - LSB_bit_offset);
      frame->data[index] |= (uint8_t) (raw >> right_shift) & mask;
      loaded_bytes++;
    }
  }
}

/**
* Extracts a signal from a CAN Frame
* Supports endianess, signedness(+-),offset, scale, [min,max](disabled)
* Does not support Float and double signedness/type
*/
double CAN_extract_signal(CAN_frame_t* frame, CAN_signal_t sig){

  uint64_t target = 0;

  // Motorola (big endian)
  if (sig.endianess == CAN_BIG){
    uint8_t MSB_index = sig.bit_start / 8;
    uint8_t MSB_len = fmin((sig.bit_start % 8) +1,sig.bit_len);

    uint8_t full_bytes = (sig.bit_len - MSB_len)/8;

    uint8_t LSB_index = MSB_index + ceil((sig.bit_len - MSB_len)/8.0);
    uint8_t LSB_len = (sig.bit_len-MSB_len-1)%8+1;
    uint8_t LSB_bit_offset = 8-LSB_len;

    if(MSB_index == LSB_index){
      // Extract LSB
      target |= frame->data[LSB_index] >> ((sig.bit_start%8) - sig.bit_len + 1);
    }else{
      // Extract LSB
      target |= frame->data[LSB_index] >> LSB_bit_offset;
      // Extract remaining bytes
      uint8_t loaded_bytes = 1;
      for(uint8_t index = MSB_index; index < LSB_index; index++){
        int8_t shift = sig.bit_len - MSB_len - ((loaded_bytes - 1) * 8);
        target |= (frame->data[index] << shift);
        loaded_bytes++;
      }
    }


  }
  // Intel (little endian)
  else{
    uint8_t LSB_index = sig.bit_start/8;
    uint8_t LSB_bit_offset = sig.bit_start %8;
    uint8_t MSB_index = (sig.bit_start + sig.bit_len - 1) / 8;

    // Extract bits of LSB
    target |= (frame->data[LSB_index] >> LSB_bit_offset);

    // Extract the rest of the payload
    uint8_t loaded_bytes = 1;
    for (int16_t index = LSB_index+1; index <= MSB_index; index++){
      uint8_t shift = (loaded_bytes * 8) - LSB_bit_offset;
      target |= (uint64_t)(frame->data[index]) << shift; // MUST cast the byte to uint64_t otherwise sign extension is performed
      loaded_bytes++;
    }
  }

  // Mask value
  target &= MASK64(sig.bit_len);

  // perform sign extension
  if (sig.signedness == CAN_SIGNED){
    int64_t msb_sign_mask = 1ULL << (sig.bit_len - 1);
    target = ((int64_t)target ^ msb_sign_mask) - msb_sign_mask;
  }

  // Convert to physical value
  double val;
  if (sig.signedness == CAN_SIGNED){
    val = ((int64_t)target) * sig.scale + sig.offset;
  }
  else{
    val = target * sig.scale + sig.offset;
  }

  /*
  // Clamp the value (if we dont trust that the sender did)
  val = fmax(fmin(val,sig.max),sig.min);
  */
  return val;
}
