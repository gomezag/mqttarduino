#ifndef RFhandler_h
#define RFhandler_h

#include "Arduino.h"

class RFhandler
{
  public:
    RFhandler(unsigned long *dic, unsigned int n_states);
    int parse_state(unsigned long msg);
    int state();
    void set_idle();

  private:
    int _state;
    unsigned long _dictionary[10] = {0};
    int _nstates;
};

#endif