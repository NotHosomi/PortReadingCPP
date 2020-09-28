#include <iostream>
#include <windows.h>
#include "Serial.hpp"
#include <chrono>

const int POLL_TIME = 100;
const int PRINT_TIME = 500;


void truncateBuffer(char *buffer, unsigned int content_length)
{
  int end_index = -1;
  int start_index = -1;
  for(int i = content_length-1; i >= 0; --i)
  {
    if(buffer[i] == '!')
    {
      if(end_index == -1)
      {
        end_index = i;
      }
      else
      {
        start_index = i + 1;
        break;
      }
    }
  }
  if(end_index == -1 || start_index == -1)
  {
    printf("Bad truncate buffer: ");
    return;
  }
  int output_length = end_index - start_index;
  char temp[256] = "";
  for(int i = 0; i < output_length; ++i)
  {
    temp[i] = buffer[start_index + i];
  }
  for(int i = 0; i < output_length; ++i)
  {
    buffer[i] = temp[i];
  }
  buffer[output_length] = '\0';
}

int main() {
  std::cout << "Hello, World!" << std::endl;
  
  Serial* port = new Serial("\\\\.\\COM5");
  
  if(port->IsConnected())
    printf("Connected successfully\n");
  
  char buffer[256] = "";
  unsigned int buffer_size = 255;
  unsigned int length = 0;
  auto tmr_start = std::chrono::high_resolution_clock::now();
  auto tmr_tick = std::chrono::high_resolution_clock::now();
  
  while(port->IsConnected())
  {
    length = port->ReadData(buffer, buffer_size);
    //printf("Bytes read: %i\n", length);
    buffer[length] = 0;
    
    tmr_tick = std::chrono::high_resolution_clock::now();
    if(std::chrono::duration_cast<std::chrono::milliseconds>(tmr_tick - tmr_start).count() > PRINT_TIME)
    {
      truncateBuffer(buffer, length);
      printf("%s\n", buffer);
      tmr_start = std::chrono::high_resolution_clock::now();
    }
    Sleep(POLL_TIME);
  }
  
  delete port;
  return 0;
}