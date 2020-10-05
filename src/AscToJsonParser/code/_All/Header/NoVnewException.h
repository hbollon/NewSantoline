#ifndef PROJECT_NOVNEWEXCEPTION_H
#define PROJECT_NOVNEWEXCEPTION_H

#include <exception>

class NoVnewException : public std::exception {
  virtual const char *what() const throw() { return "Pas de Vnew trouvée ..."; }
};
#endif // PROJECT_NOVNEWEXCEPTION_H
