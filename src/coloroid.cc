#include "coloroid.h"

#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>

#include "platform/platform.h"

namespace cl {
// get a random color
sf::Color Coloroid::getRandomColor() {
  // #247ba0, #70c1b3, #f3ffbd, #ff1654, #5c7457, #8b5d33
  std::vector<sf::Color> colors = {sf::Color(36, 123, 160),  sf::Color(112, 193, 179),
                                   sf::Color(243, 255, 189), sf::Color(255, 22, 84),
                                   sf::Color(92, 116, 87),   sf::Color(139, 93, 51)};

  return colors.at(rand() % colors.size());
}

// check if the entire board has the same color
bool Coloroid::isEntireBoardSameColor() {
  for (size_t i = 0; i < squares.size(); i++) {
    if (squares.at(i).getColor() != squares.at(0).getColor()) {
      return false;
    }
  }
  return true;
}

// check if neighbor to the current square is valid
bool Coloroid::isNeighborValid(int current, int neighbor) {
  // current square on top row and its neighbor is above
  if (current < level && neighbor == current - level) {
    return false;
  }
  // current square on botom row and its neighbor is below
  if (current >= ((level - 1) * level) && neighbor == current + level) {
    return false;
  }
  // current square on left border and its neightbor is to the left
  if (current % level == 0 && neighbor == current - 1) {
    return false;
  }
  // current square on right border and neighbor is to the right
  if ((current + 1) % level == 0 && neighbor == current + 1) {
    return false;
  }
  return true;
}

// update the squares to be captured
int Coloroid::updateCaptured(int selected, sf::Color selectedColor) {
  int changed = 0;
  int directions[4] = {-1, -level, 1, level};  // left, up, right, down

  for (int i = 0; i < 4; i++) {
    int neighbor = selected + directions[i];
    if (isNeighborValid(selected, neighbor)) {
      if (!squares.at(neighbor).getCaptured()) {
        if (squares.at(neighbor).getColor() == selectedColor) {
          squares.at(neighbor).setCaptured(true);
          changed++;
        }
      }
    }
  }
  return changed;
}

// update the board
void Coloroid::updateBoard(int selected) {
  sf::Color selectedColor = squares.at(selected).getColor();

  // set all captured squares to the selected color
  for (size_t i = 0; i < squares.size(); i++) {
    if (squares.at(i).getCaptured()) {
      squares.at(i).setColor(selectedColor);
    }
  }

  while (true) {
    int changed = 0;
    for (size_t i = 0; i < squares.size(); i++) {
      if (squares.at(i).getCaptured()) {
        changed += updateCaptured(i, selectedColor);
      }
    }
    if (changed == 0) {
      break;
    }
  }
}

// main event loop
void Coloroid::startGame() {
  const int windowXSize = 600;
  const int windowYSize = windowXSize + 72;
  const int maxLevel = 10;
  int numMoves = 0;
  int squareSize = 0;
  bool setupBoard = true;
  bool isResized = false;

  level = 4;  // default level

  srand(static_cast<unsigned int>(time(nullptr)));  // seed random

  util::Platform platform;  // initialize platform depedencies

  // initialize the font
  sf::Font font;
  if (!font.loadFromFile("res/arial.ttf")) {
    std::cout << "Unable to load resource: \"arial.ttf\"" << std::endl;
    exit(EXIT_FAILURE);
  }

  // initialize the icon
  sf::Image icon;
  if (!icon.loadFromFile("res/icon.png")) {
    std::cout << "Unable to load resource: \"icon.png\"" << std::endl;
    exit(EXIT_FAILURE);
  }

  // initialize the main window
  sf::RenderWindow window;
  float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
  window.create(sf::VideoMode(windowXSize * screenScalingFactor, windowYSize * screenScalingFactor),
                "Coloroid");
  platform.setIcon(window.getSystemHandle());
  window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
  window.setFramerateLimit(60);  // max 60 fps to lower cpu usage

  // initialize the bottom text
  sf::Text bottomText = sf::Text("", font, 36);
  bottomText.setPosition(0, windowYSize - bottomText.getCharacterSize() - 24);

  // running
  while (window.isOpen()) {
    if (setupBoard) {
      // initialize the board of random colored squares
      squareSize = static_cast<float>(windowXSize) / level;
      squares.resize(pow(level, 2));  // resize the vector based on the level

      for (int i = 0; i < level; i++) {
        for (int j = 0; j < level; j++) {
          squares.at(j + i * level) =
              Square(squareSize, squareSize * j, squareSize * i, getRandomColor());
        }
      }

      // start game: mark the first square captured
      // also its adjacent squares if they have the same color
      squares.at(0).setCaptured(true);
      updateBoard(0);

      setupBoard = false;
    }

    // easter egg when you reach the maximum level
    if (level == maxLevel) {
      window.setFramerateLimit(5);  // decrease the fps
      setupBoard = true;
    }

    // check all the window's events that were triggered since the last
    // iteration of the loop
    // all events must be inside window.pollEvent(event) loop
    sf::Event event;

    while (window.pollEvent(event)) {
      // exit if close event requested or escape pressed
      if ((event.type == sf::Event::Closed) ||
          ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) {
        window.close();
        break;
      }

      // resize event
      if (event.type == sf::Event::Resized) {
        isResized = true;
      }

      // press R to restart the level
      if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::R)) {
        numMoves = 0;
        setupBoard = true;
      }

      // left click event
      if ((event.type == sf::Event::MouseButtonPressed) &&
          (event.mouseButton.button == sf::Mouse::Left)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // if the window was resized, map mouse coordinates to world coordinates
        if (isResized) {
          sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
          mousePos.x = worldPos.x;
          mousePos.y = worldPos.y;
        }

        // clamp the mouse position to prevent out-of-bound access to vector
        mousePos.x = std::min(static_cast<int>(window.getSize().x), std::max(0, mousePos.x));
        mousePos.y = std::min(static_cast<int>(window.getSize().y), std::max(0, mousePos.y));

        // calcuate the chosen square position based on mouse position
        sf::Vector2f selectedSquarePos;
        selectedSquarePos.x = mousePos.x / squareSize;
        selectedSquarePos.y = mousePos.y / squareSize;
        int selectedSquareIndex = floor(selectedSquarePos.x) + floor(selectedSquarePos.y) * level;

        numMoves++;  // increase the number of moves

        // find the next captured area based on the selected square
        updateBoard(selectedSquareIndex);
      }
    }

    // clear the window, erasing what is displayed
    window.clear();

    // draw the board of squares
    for (int i = 0; i < level; i++) {
      for (int j = 0; j < level; j++) {
        window.draw(squares.at(j + i * level).getShape());
      }
    }

    // draw the bottom notification
    std::string s = (level < maxLevel)
                        ? "(Par " + std::to_string(level * 2) + ") Move " + std::to_string(numMoves)
                        : "You won! Game over!";
    bottomText.setString(s);
    window.draw(bottomText);

    // end game: entire board has the same color
    if (isEntireBoardSameColor()) {
      if (numMoves <= level * 2) {
        level++;  // level passed
      }
      numMoves = 0;       // reset the number of moves
      setupBoard = true;  // reset the board
    }

    // display the background frame buffer, replacing the previous RenderWindow frame contents
    window.display();
  }
}
}  // namespace cl