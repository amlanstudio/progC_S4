#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "imgui.h"
#include "p6/p6.h"
#include <cmath>
#include <cstdlib>
#include <vector>
#define DOCTEST_CONFIG_IMPLEMENT
#include "Boid.hpp"
#include "doctest/doctest.h"

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
  std::vector<Boid> myBoids;
  for (int i = 0; i < 20; i++) {
    myBoids.emplace_back(Boid(ctx));
  }

  // Declare your infinite update loop.
  ctx.update = [&]() {
    ctx.background({p6::NamedColor::FloralWhite});
    ctx.use_stroke = false;

    for (Boid &boid : myBoids) {
      for (Boid &otherBoid : myBoids) {
        if (&boid != &otherBoid) {
          boid.follow(otherBoid);
          boid.avoidBoids(boid, otherBoid, 0.9f);
        }
      }
      boid.updatePosition();

      if (boid.canvasBorders(ctx)) {
        boid.drawBoid(ctx);
      } else {
        boid.turnBack();
      }
    }
  };

  // Should be done last. It starts the infinite loop.
  ctx.start();
}
