#include <cmath>

#include "motion.hpp"

namespace eidos {

void advance_motion(Motion1D &motion, float delta_seconds,
                    float maximum_position) {
  const float speed{std::abs(motion.velocity)};

  if (maximum_position <= 0.0f) {
    motion.position = 0.0f;
    motion.velocity = speed;

    return;
  }

  motion.position += motion.velocity * delta_seconds;

  while (motion.position < 0.0f || motion.position > maximum_position) {
    if (motion.position > maximum_position) {
      const float overshoot{motion.position - maximum_position};

      motion.position = maximum_position - overshoot;
      motion.velocity = -speed;
    } else {
      motion.position = -motion.position;
      motion.velocity = speed;
    }
  }

  if (motion.position == 0.0f) {
    motion.velocity = speed;
  } else if (motion.position == maximum_position) {
    motion.velocity = -speed;
  }
}

} // namespace eidos
