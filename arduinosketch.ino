#include "BitBangSerial.h"
#include "Sonar.h"

unsigned long last_sent = 0;

char read_sentence[50];
int n_sent, n_received;

BitBangSerial bbs;
Sonar sonar1(1,"PORT");

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(38400);
  
  bbs.setup(23,22,4800); // (tx, rx, baud)
  
  Serial.print("Sonar name: "); Serial.println(sonar1.NAME);
  Serial.print("Sonar ID: "); Serial.println(sonar1.ID);
  
  Serial.println("Setting up sonar!");
  bbs.writeBytes(sonar1.disable_all_sentences);
  bbs.writeBytes(sonar1.enable_sentence_dpt);
  bbs.writeBytes(sonar1.enable_sentence_at_ping);
  bbs.writeBytes(sonar1.disable_interval_pinging);
}

void loop() {
  static unsigned long ping_time = 0, response_time = 0;
  static unsigned long timer = 0;
  
  if (millis() - timer > 500) {
    Serial.println("Trying to ping.");
    ping_time = micros();
    bbs.writeBytes(sonar1.ping);
    timer = millis();
  }  
    
   if (bbs.readBytesUntil(read_sentence,'\n',50000UL)) 
   {
     sonar1.decode_buffer(read_sentence);
     response_time = micros() - ping_time;
     Serial.println(read_sentence);
     Serial.print("Depth: "); Serial.println(sonar1.dbt);
     Serial.print("Response time: "); Serial.println(response_time);
   }
}
