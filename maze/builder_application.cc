#include <maze/image.h>

#include <iostream>
#include <random>
#include <cstdint>
#include <vector>
#include <tuple>
#include <stdexcept>
#include <cassert>

enum class Action {
  UP, DOWN, RIGHT, LEFT
};

struct Wall {
  int x_, y_;
  Action action_;
  std::size_t distance_;
};

Action flip(const Action action) {
  switch (action) {
    case Action::UP:
      return Action::DOWN;
    case Action::DOWN:
      return Action::UP;
    case Action::RIGHT:
      return Action::LEFT;
    case Action::LEFT:
      return Action::RIGHT;
    default:
      throw std::runtime_error("flip: weird action");
  }
}

std::pair<int, int> increment(const Action direction, const
    int x, const int y) {
  switch (direction) {
    case Action::UP:
      return std::make_pair(x, y-1);
    case Action::DOWN:
      return std::make_pair(x, y+1);
    case Action::RIGHT:
      return std::make_pair(x+1, y);
    case Action::LEFT:
      return std::make_pair(x-1, y);
    default:
      throw std::runtime_error("increment: weird action");
  }
}

int main(int argc, const char* argv[]) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " <maze.png>" <<
      std::endl;
    return 1;
  }

  const uint8_t freeValue = 255;
  const uint8_t wallValue = 0;

  // get a randomness source
  std::random_device rd;
  std::mt19937 gen(rd());

  const std::size_t width = 101;
  const std::size_t height = 101;

  maze::image_type img(width, height);

  img.fill(wallValue);

  std::vector<Wall> walls;

  // set up the initial seed -- this will be our exit
  img.atXY(1, 0) = freeValue;
  img.atXY(1, 1) = freeValue;
  // add the first wall
  walls.push_back(Wall {1, 2, Action::DOWN, 1});
  walls.push_back(Wall {2, 1, Action::RIGHT, 1});

  std::vector<Action> actions = {Action::UP, Action::DOWN, Action::RIGHT,
    Action::LEFT};

  while (!walls.empty()) {
    std::uniform_int_distribution<> distribution(0, walls.size() - 1);
    const std::size_t index = distribution(gen);

    const Wall w = walls[index];

    assert(w.x_ >= 0 && w.x_ < width);
    assert(w.y_ >= 0 && w.y_ < height);

    uint8_t& mazeWall0 = img(w.x_, w.y_);
    const auto next = increment(w.action_, w.x_, w.y_);
    uint8_t& mazeWall1 = img(next.first, next.second);

    // count how many free fields are surrounding us
    std::size_t freeFields = 0;
    for (const Action& a : actions) {
      const auto next_ = increment(a, next.first, next.second);
      const uint8_t& nextMaze_ = img.atXY(next_.first, next_.second, 0, 0,
          freeValue);

      freeFields += nextMaze_ == freeValue;
    }

    // disallow junctions with more the two options
    const auto prev = increment(flip(w.action_), w.x_, w.y_);
    std::size_t junctionSize = 0;
    for (const Action& a : actions) {
      const auto prev_ = increment(a, prev.first, prev.second);
      const uint8_t& prevMaze = img(prev_.first, prev_.second);

      junctionSize += prevMaze == freeValue;
    }

    if (freeFields == 0 && junctionSize < 3) {
      // add to maze
      mazeWall0 = freeValue;
      mazeWall1 = freeValue;

      // add walls around it to the walls list
      walls.push_back(Wall {next.first - 1,
          next.second,
          Action::LEFT,
          w.distance_+1});
      walls.push_back(Wall {next.first + 1,
          next.second,
          Action::RIGHT,
          w.distance_+1});
      walls.push_back(Wall {next.first,
          next.second - 1,
          Action::UP,
          w.distance_+1});
      walls.push_back(Wall {next.first,
          next.second + 1,
          Action::DOWN,
          w.distance_+1});
    }

    // remove the currently selected wall
    walls[index] = walls.back();
    walls.pop_back();
  }

  img.resize(width * 5, height * 5);
  img.save(argv[1]);

  return 0;
}
