#include <exception>

class NoIntersectionException : public std::exception {
  virtual const char *what() const throw() {
    return "Pas d'intersections trouvées...";
  }
};
