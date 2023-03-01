#include "p6/p6.h"
#include <cstdlib>
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

// My classes

class circle {
public:
  circle(p6::Context &ctx) {
    xPosition = p6::random::number();
    yPosition = p6::random::number();
    radius = 0.2f;

    leftLimit = ctx.current_canvas_width() + 2 * radius;
    rightLimit = ctx.current_canvas_width() - 2 * radius;
    topLimit = ctx.current_canvas_height() + 2 * radius;
    bottomLimit = ctx.current_canvas_height() - 2 * radius;
  }

  bool canvasBorders() {
    if (xPosition < static_cast<float>(leftLimit) &&
        xPosition > static_cast<float>(rightLimit) &&
        yPosition<static_cast<float>(topLimit) & yPosition> static_cast<float>(
            bottomLimit)) {
      return false;
    }
    return true;
  }

  float xPosition;
  float yPosition;
  float radius;

private:
  int leftLimit;
  int rightLimit;
  int topLimit;
  int bottomLimit;
};

/*-------------My functions ------------*/

void drawCircle(p6::Context &ctx, circle myCircle) {
  ctx.fill = {p6::NamedColor::Salmon};
  ctx.circle(p6::Center{myCircle.xPosition, myCircle.yPosition},
             p6::Radius{myCircle.radius});
};

// ------------------

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

  // Declare your infinite update loop.
  ctx.update = [&]() {
    ctx.background({p6::NamedColor::FloralWhite});
    // ctx.fill = {p6::NamedColor::Salmon};
    ctx.use_stroke = false;

    circle myCircle(ctx);
    myCircle.canvasBorders();
    drawCircle(ctx, myCircle);
  };

  // Should be done last. It starts the infinite loop.
  ctx.start();
}
