#include "BitBangSerial.h"

BitBangSerial::BitBangSerial()
{
  // Sets default baudrate to 4800
  SetBaudRate(4800);
}

BitBangSerial::BitBangSerial(int tx_pin, int rx_pin, int baud)
{
  SetPins(tx_pin, rx_pin);
  SetBaudRate(baud);
}

int BitBangSerial::setup(int tx_pin, int rx_pin, int baud)
{
  SetPins(tx_pin, rx_pin);
  return SetBaudRate(baud);
}

void BitBangSerial::SetPins(int tx, int rx)
{
  txpin = tx; rxpin = rx;
  pinMode(txpin,OUTPUT);
  pinMode(rxpin,INPUT);
  digitalWrite(txpin,HIGH);
}

void BitBangSerial::writeBytes(char *out)
{
  char *ptr = out;
  // Send each byte in the buffer until '\0'
  while (*ptr != '\0') {
    writeByte(*ptr);
    ptr++;
  }
}

void BitBangSerial::writeByte(char b)
{
  // Drop pin to send start bit
  digitalWrite(txpin, LOW);
  delayMicroseconds(outbit_delay);
  // Send data bits
  for (int i = 0; i <= 7; i++)
    {
      (b & (1 << i)) ? digitalWrite(txpin,HIGH) : digitalWrite(txpin,LOW);
      delayMicroseconds(outbit_delay);
    }
  // Send stopbit
  digitalWrite(txpin, HIGH);
  delayMicroseconds(outbit_delay);
}

int BitBangSerial::readBytes(char *buf, int nbytes)
{ return readBytes(buf, nbytes, 200000UL); }

int BitBangSerial::readBytes(char *buf, int nbytes, unsigned long timeout)
{
  int i = 0;
  do {
    buf[i] = readByte(timeout);
    i++;
  } while (buf[i-1] && (i < nbytes));
  buf[i] = '\0';
  return (buf[0] == 0) ? 0 : i;
}

int BitBangSerial::readBytesUntil(char *buf, char termination, unsigned long timeout)
{
  int i = 0;
  do {
    buf[i] = readByte(timeout);
    i++;
  } while (buf[i-1] && (buf[i-1] != termination));
  buf[i] = '\0';
  return (buf[0] == 0) ? 0 : i;
}

char BitBangSerial::readByte()
{ return readByte(200000UL); }

char BitBangSerial::readByte(unsigned long timeout)
{
  if (!is_setup) return 0;
  
  unsigned long calltime = micros();
  char b = 0;
  // Detect startbit
  while (digitalRead(rxpin) != LOW) { if (micros() - calltime > timeout) return 0;}
  delayMicroseconds(start_bit_delay);
  if (digitalRead(rxpin) == HIGH) b |= 0b00000001;
  delayMicroseconds(bit_delay);
  if (digitalRead(rxpin) == HIGH) b |= 0b00000010;
  delayMicroseconds(bit_delay);
  if (digitalRead(rxpin) == HIGH) b |= 0b00000100;
  delayMicroseconds(bit_delay);
  if (digitalRead(rxpin) == HIGH) b |= 0b00001000;
  delayMicroseconds(bit_delay);
  if (digitalRead(rxpin) == HIGH) b |= 0b00010000;
  delayMicroseconds(bit_delay);
  if (digitalRead(rxpin) == HIGH) b |= 0b00100000;
  delayMicroseconds(bit_delay);
  if (digitalRead(rxpin) == HIGH) b |= 0b01000000;
  delayMicroseconds(bit_delay);
  if (digitalRead(rxpin) == HIGH) b |= 0b10000000;
  delayMicroseconds(bit_delay);

  return b;
}

int BitBangSerial::SetBaudRate(int baud)
{
  switch (baud)
    {
    default:
      is_setup = false; // If the requested baudrate doesn't match any of the available rates
    case 4800:
      start_bit_delay = 300;
      bit_delay = 200;
      outbit_delay = 200;
      is_setup = true;
      break;
    case 9600:
      start_bit_delay = 200;
      bit_delay = 100;
      outbit_delay = 100;
      is_setup = true;
      break;
    case 38400:
      start_bit_delay = 37;
      bit_delay = 23;
      outbit_delay = 23;
      is_setup = true;
      break;
    }
  return (is_setup ? 1 : 0);
}
