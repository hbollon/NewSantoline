#ifndef __VECTORIELLE__
#define __VECTORIELLE__

#include "algorithm.h"
#include <algorithm>
#include <useful.h>

class Vectorielle : public Algorithm {
public:
  Vectorielle(Connector *connector, double duration, int cell_dim);
};

#endif //__VECTORIELLE__
