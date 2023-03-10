#include "imgui.h"
#include "p6/p6.h"
#include <cmath>
#include <cstdlib>
#include <vector>
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

// My classes

class circle {
public:
  circle() {
    // position = glm::vec2(p6::random::number(-1, 1), p6::random::number(-1,
    // 1));
    position = glm::vec2(0, 0);
    speed = glm::vec2(p6::random::number(-0.5, 0.05),
                      p6::random::number(-0.5, 0.05)) *
            0.01f;
    // speed = glm::vec2(0.05, 0.05) * 0.001f;
    radius = 0.05f;
    leftLimit = -1.80 + radius; // changer pour adapter a la taille de la
                                // fenetre
    rightLimit = 1.80 - radius;
    topLimit = -1 + radius;
    bottomLimit = 1 - radius;
  }

  bool canvasBorders() {
    if (position.x < leftLimit || position.x > rightLimit ||
        position.y < topLimit || position.y > bottomLimit) {
      return false;
    }
    return true;
  }

  void drawCircle(p6::Context &ctx) {
    ctx.fill = {p6::NamedColor::Salmon};
    ctx.circle(p6::Center{position}, p6::Radius{radius});
  };

  void updatePosition() { position += speed; }
  void turnBack() { speed = -speed; }

private:
  glm::vec2 position;
  glm::vec2 speed;
  float radius;
  float leftLimit;
  float rightLimit;
  float topLimit;
  float bottomLimit;
};

int main(int argc, char *argv[]) {
  { // Run the tests
    if (doctest::Context{}.run() != 0)
      return EXIT_FAILURE;

    // The CI does not have a GPU so it cannot run the rest of the code.
    const bool no_gpu_available =
        argc >= 2 &&
        strcmp(argv[1], "-nogpu") ==
            0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    if (no_gpu_available) {
      return EXIT_SUCCESS;
    }
  }

  // Actual app
  auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
  ctx.maximize_window();
  std::vector<circle> myBoids;
  for (int i = 0; i < 20; i++) {
    myBoids.emplace_back(circle());
  }

  // Declare your infinite update loop.
  ctx.update = [&]() {
    ctx.background({p6::NamedColor::FloralWhite});
    ctx.use_stroke = false;

    for (int i = 0; i < 20; i++) {
      myBoids[i].updatePosition();

      if (myBoids[i].canvasBorders()) {
        myBoids[i].drawCircle(ctx);
      } else {
        myBoids[i].turnBack();
      }
    }
  };

  // Should be done last. It starts the infinite loop.
  ctx.start();
}
