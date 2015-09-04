#include <Arduino.h>

#ifndef _BitBangSerial_
#define _BitBangSerial_

class BitBangSerial {
  int txpin, rxpin;
  int start_bit_delay, bit_delay, outbit_delay;
  bool is_setup;
 public:
  BitBangSerial();
  BitBangSerial(int tx_pin, int rx_pin, int baud);
  int setup(int tx_pin, int rx_pin, int baud);
  int SetBaudRate(int baud);
  void SetPins(int tx, int rx);
  char readByte();
  char readByte(unsigned long timeout); // timeout in uS
  int readBytes(char *buf, int nbytes);
  int readBytes(char *buf, int nbytes, unsigned long timeout);
  int readBytesUntil(char *buf, char termination, unsigned long timeout);
  void writeByte(char b);
  void writeBytes(char *out);
};

#endif
