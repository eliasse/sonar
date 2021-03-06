#include "Sonar.h"
#include <Arduino.h>

Sonar::Sonar(int id, char *name) 
{
  ID = id; 
  strcpy(NAME,name);
}

int Sonar::decode_buffer(char *in_buffer)
{
  char buf[100];  // 10 Rows (one for each found sentence)
  int r = 0, k = 0, l = 0, cs, CRCin, n_found = 0;

  while (in_buffer != '\0'){

    // Find sentence
    while (in_buffer[k++] != '$') { if (in_buffer[k] == '\0') return n_found; };
    do {
      buf[l++] = in_buffer[k];
    } while (in_buffer[++k] != '*');
    buf[l] = '\0';

    // Verify sentence
    char crc[3];
    crc[0] = in_buffer[++k];
    crc[1] = in_buffer[++k];
    crc[2] = '\0';
    cs = checksum(buf);
    CRCin = (int)strtol(crc,NULL,16);

    l = 0;
    
    // Decode sentence if checksums matches
    if (CRCin == cs) {
      // Count comma's 
      // Count double-occurrances and determine empty spots
      uint8_t i = 0;
      uint8_t n_fields = 0; // Number of commas found (thus the number of elements)
      bool is_empty[20];
      
      while (buf[i] != '\0'){
	if ((buf[i] == ',') || (buf[i] == '*')) {
	  n_fields++;
	  is_empty[n_fields] = false; // Just to make sure the bool is initialized
	  
	  if (buf[i-1] == ',') { 
	    is_empty[n_fields] = true;
	  }
	}
	i++;
      }
      n_fields++;
      
      char *tok;
      tok = strtok(buf,",*");
      
      if (!strcmp("SDDPT", tok)) { decode_DPT(n_fields, is_empty); n_found++; }
      else if (!strcmp("SDMTW", tok)) { decode_MTW(n_fields, is_empty); n_found++; }
      
      // Returns to the beginning of the while loop to check remaining buffer
  }
}

  return n_found; // Probably unnecessary 
}

void Sonar::decode_DPT(uint8_t n_fields, bool is_empty[])
{
  char * tmp;
  
  for (uint8_t element = 2; element <= n_fields; element++)
    {
      if (is_empty[element] == true) { continue; }
      
      switch (element)
	{
	default:
	  // Required if some elements are skipped
	  char * dump;
	  dump = strtok(NULL,",*");
	  (void)dump; 
	  break;

	case 2:
	  tmp = strtok(NULL,",*");
	  dbt = atof(tmp);
	  break;
	  
	case 3:
	  tmp = strtok(NULL,",*");
	  offset = atof(tmp);
	  break;
	
      	case 4:
	  tmp = strtok(NULL,",*");
	  if (!tmp) break;
	  max_range_scale = atof(tmp);
	  break;
	
	}
    }
}

void Sonar::decode_MTW(uint8_t n_fields, bool is_empty[])
{
  char * tmp;
  
  for (uint8_t element = 2; element <= n_fields; element++)
    {
      if (is_empty[element] == true) { continue; }
      
      switch (element)
	{
	default:
	  // Required if some elements are skipped
	  char * dump;
	  dump = strtok(NULL,",*");
	  (void)dump; 
	  break;

	case 2:
	  tmp = strtok(NULL,",*");
	  if (!tmp) break;
	  sea_water_temperature = atof(tmp);
	  break;
	}
    }
}

int Sonar::checksum(char * s) {
  int c = 0;
 
  while(*s)
        c ^= *s++;
 
  return c;
}

