#include <vector>

#include "color.h"
#include "useful.h"

class Image {
public:
    Image(int w, int h);

    Image(int w, int h, Color c);

    int width() const;

    int height() const;

    int index(int i, int j) const;

    Color &at(int i, int j);

    Color at(int i, int j) const;

    Color &at(Point2D a);

    Color at(Point2D a) const;

    Color at(int i) const;

    int size() const;

    void clear();

    void vline(int x);

    void hline(int y);

    void drawCircle(Point2D p, int rayon, Color c);

    void drawDisque(Point2D p, int rayon, Color c);

    void drawPoint(Point2D p, int thickness, Color c);

    void lowLine(Point2D a, Point2D b, int thickness, const Color &c);

    void highLine(Point2D a, Point2D b, int thickness, const Color &c);

    void line(Point2D a, Point2D b, int thickness, const Color &c);

    friend std::ostream &operator<<(std::ostream &os, const Image &img);

private:
    int m_w;
    int m_h;
    std::vector <Color> m_body;
};
