#include "square.h"

namespace cl {
Square::Square() {
  size = 0;
  xPos = 0;
  yPos = 0;
  color = sf::Color::Black;
  captured = false;
}

Square::Square(double size, double xPos, double yPos, sf::Color color) {
  this->size = size;
  this->xPos = xPos;
  this->yPos = yPos;
  this->color = color;
  this->captured = false;

  // set square properties
  square.setSize(sf::Vector2f(size, size));
  square.setPosition(xPos, yPos);
  square.setFillColor(color);
}

double Square::getSize() { return size; }

double Square::getX() { return xPos; }

double Square::getY() { return yPos; }

bool Square::getCaptured() { return captured; }

sf::Color Square::getColor() { return color; }

sf::RectangleShape Square::getShape() { return square; }

void Square::setSize(double size) {
  this->size = size;
  square.setSize(sf::Vector2f(size, size));
}

void Square::setX(double xPos) {
  this->xPos = xPos;
  square.setPosition(xPos, yPos);
}

void Square::setY(double yPos) {
  this->yPos = yPos;
  square.setPosition(xPos, yPos);
}

void Square::setCaptured(bool captured) { this->captured = captured; }

void Square::setColor(sf::Color color) {
  this->color = color;
  square.setFillColor(color);
}
}  // namespace cl