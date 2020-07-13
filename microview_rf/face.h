#ifndef Face_h
#define Face_h

#include "Arduino.h"

class Face
{
  public:
    Face();
    void change_face(uint8_t*img);
    void viewPrint(char *message);
    void add_bitmaps(uint8_t*img);
    void printSensor(char *message);
    void update_board(bool *states);
    void clear_logo(int num);
    void clear_lnk();

   private:
     uint8_t _screen_buffer[384];
     int _len_img;
     char* _msg;

};

#endif
