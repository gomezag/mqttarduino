#include "face.h"
#include <MicroView.h>

Face::Face()
{
  _len_img=384;
  _msg = "";
}

void Face::add_bitmaps(uint8_t*img){
  uint8_t buf[_len_img];
  memcpy_P(buf,img,_len_img);  
  for(int i=0;i<_len_img;i++){
    if((i%64)>45){
      if(buf[i]!=0x00){
        _screen_buffer[i]=buf[i];
      }
    }
  }
  uView.drawBitmap(_screen_buffer);
  viewPrint(_msg);
}

void Face::change_face(uint8_t*img){
  uint8_t buf[_len_img];
  memcpy_P(buf,img,_len_img);
  for(int i=0; i<_len_img-64;i++){
    if((i%64)<51){
        _screen_buffer[i]=buf[i];
    }
  }
  uView.drawBitmap(_screen_buffer);
  viewPrint(_msg);

}

void Face::viewPrint(char *message){
  uView.setCursor(0,40);
  uView.print("          ");
  _msg = message;
  strcpy(_msg, message);
  uView.setCursor(0,40);
  uView.print(_msg);
}


void Face::printSensor(char *message){
  uView.setCursor(47,24);
  uView.print(strcat(message,"%"));
} 

void Face::update_board(bool *states){
  byte mask=0x00;
  if(states[0]||states[2]){
    mask=mask+states[0]*64+states[2]*4;
    _screen_buffer[64+55]=_screen_buffer[64+55]^mask;
    mask=0x00;
  }
  uView.drawBitmap(_screen_buffer);
  viewPrint(_msg);

}

void Face::clear_logo(int row){
  for(int i=(row-1)*64;i<(row)*64;i++){
    if((i%64)>43){
        _screen_buffer[i]=0x00;
    }
  }
  uView.drawBitmap(_screen_buffer);
  viewPrint(_msg);
}

void Face::clear_lnk(){
    for(int i=(1)*64;i<(4)*64;i++){
    if((i%64)>43){
        _screen_buffer[i]=0x00;
    }
  }
  uView.drawBitmap(_screen_buffer);
  viewPrint(_msg);
}
