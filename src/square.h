#ifndef SQUARE_H_
#define SQUARE_H_

namespace cl {
class Square {
 public:
  // default constructor
  Square();

  // fully-qualified constructor
  Square(double size, double xPos, double yPos, sf::Color color);

  // getters
  double getSize();
  double getX();
  double getY();
  bool getCaptured();
  sf::Color getColor();
  sf::RectangleShape getShape();

  // setters
  void setSize(double size);
  void setX(double xPos);
  void setY(double yPos);
  void setColor(sf::Color color);
  void setCaptured(bool captured);

 private:
  double size;
  double xPos;
  double yPos;
  bool captured;
  sf::Color color;
  sf::RectangleShape square;
};
}  // namespace cl

#endif  // SQUARE_H_
