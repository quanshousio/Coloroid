#ifndef COLOROID_H_
#define COLOROID_H_

#include "square.h"

namespace cl {
class Coloroid {
 public:
  void startGame();

 private:
  sf::Color getRandomColor();
  bool isEntireBoardSameColor();
  bool isNeighborValid(int current, int neighbor);
  int updateCaptured(int selected, sf::Color selectedColor);
  void updateBoard(int selected);

  std::vector<Square> squares;
  int level;
};
}  // namespace cl

#endif  // COLOROID_H_
