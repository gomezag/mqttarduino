#include "rfhandler.h"

RFhandler::RFhandler(unsigned long *dic, unsigned int n_states)
{
  if(n_states>10){ n_states = 10; }

  for(int i=0; i<n_states; i++){
    _dictionary[i] = dic[i];
  }
  _state = 0;
  _nstates = n_states;
}

int RFhandler::parse_state(unsigned long msg)
{
    for(int i=0; i<_nstates; i++){
        if(msg == _dictionary[i]){
            _state = i;
            return _state;
        }
    }
    return _state;
}

int RFhandler::state()
{
 return _state;
}

void RFhandler::set_idle()
{
    _state = 0;
    return ;
}