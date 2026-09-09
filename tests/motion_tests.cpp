#include <cassert>
#include <cmath>
#include <iostream>

#include "motion.hpp"

void check_motion(eidos::Motion1D motion, float delta_seconds,
                  float maximum_position, eidos::Motion1D expected) {
  eidos::advance_motion(motion, delta_seconds, maximum_position);

  assert(std::abs(motion.position - expected.position) < 0.001f);
  assert(std::abs(motion.velocity - expected.velocity) < 0.001f);
}

int main() {
  // Movement without reaching a boundary.
  check_motion({5.0f, 2.0f}, 0.5f, 10.0f, {6.0f, 2.0f});

  // Preserve overshoot at each boundary.
  check_motion({715.0f, 1000.0f}, 0.01f, 720.0f, {715.0f, -1000.0f});
  check_motion({5.0f, -1000.0f}, 0.01f, 720.0f, {5.0f, 1000.0f});

  // Multiple crossings within one update.
  check_motion({8.0f, 30.0f}, 1.0f, 10.0f, {2.0f, -30.0f});
  check_motion({2.0f, -30.0f}, 1.0f, 10.0f, {8.0f, 30.0f});

  // Landing exactly on a boundary points velocity inward.
  check_motion({8.0f, 2.0f}, 1.0f, 10.0f, {10.0f, -2.0f});
  check_motion({2.0f, -2.0f}, 1.0f, 10.0f, {0.0f, 2.0f});

  // Zero elapsed time and zero velocity.
  check_motion({4.0f, -3.0f}, 0.0f, 10.0f, {4.0f, -3.0f});
  check_motion({4.0f, 0.0f}, 1.0f, 10.0f, {4.0f, 0.0f});

  // No room to move.
  check_motion({4.0f, -3.0f}, 1.0f, 0.0f, {0.0f, 3.0f});
  check_motion({4.0f, -3.0f}, 1.0f, -5.0f, {0.0f, 3.0f});

  // Splitting the same elapsed time should preserve the outcome
  eidos::Motion1D single_step{8.0f, 30.0f};
  eidos::Motion1D split_steps{single_step};

  eidos::advance_motion(single_step, 1.0f, 10.0f);

  for (int step{0}; step < 4; ++step) {
    eidos::advance_motion(split_steps, 0.25f, 10.0f);
  }

  assert(std::abs(single_step.position - split_steps.position) < 0.001f);
  assert(std::abs(single_step.velocity - split_steps.velocity) < 0.001f);

  std::cout << "All motion tests passed.\n";
}
