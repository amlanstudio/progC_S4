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
  {
    // Run the tests
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
  // auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
  auto ctx = p6::Context{{1280, 720, "Dear ImGui"}};
  auto boidsNumber = 20;
  auto followSpeed = 0.00001f;
  auto avoidDistance = 0.9f;
  auto turnBack = 0.5f;
  auto alignDistance = 0.5f;
  auto alignStrength = 0.5f;

  std::vector<Boid> myBoids;

  ctx.maximize_window();

  // My GUInterface
  ctx.imgui = [&]() {
    // My window GUI
    ImGui::Begin("Gestion de mes boids");
    if (ImGui::SliderInt("Boids Number", &boidsNumber, 0.f, 100.f)) {
      generateBoids(myBoids, boidsNumber, ctx);
    };
    ImGui::SliderFloat("Follow Speed", &followSpeed, 0.f, 0.5f);
    ImGui::SliderFloat("Turn Back Speed", &turnBack, 0.f, 0.5f);
    ImGui::SliderFloat("Avoid Distance", &avoidDistance, 0.f, 2.f);
    ImGui::End();

    // Show the official ImGui demo window
    ImGui::ShowDemoWindow();
  };
  generateBoids(myBoids, boidsNumber, ctx);

  // Declare your infinite update loop.
  ctx.update = [&]() {
    ctx.background({p6::NamedColor::FloralWhite});
    ctx.use_stroke = false;

    for (Boid &boid : myBoids) {
      for (Boid &otherBoid : myBoids) {
        if (boid.turnBackOutBorder(turnBack)) {
          boid.drawBoid(ctx);
          if (&boid != &otherBoid) {
            boid.follow(otherBoid, followSpeed);
            boid.separate(boid, otherBoid, avoidDistance);
            boid.align(otherBoid, alignDistance, alignStrength);
          }
        }
      }
      boid.updatePosition();
    }
  };

  // Should be done last. It starts the infinite loop.
  ctx.start();
}
