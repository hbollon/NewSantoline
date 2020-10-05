#include <gtkmm.h>

#include "connector.h"
#include "algorithm.h"

class Debuger : public Gtk::Window
{

public:
  Debuger();
  Debuger(Algorithm* algorithm, Connector* connector);
  ~Debuger();

protected:
  
private:

  Algorithm* m_algorithm;
  Connector* m_connector;
  
};
