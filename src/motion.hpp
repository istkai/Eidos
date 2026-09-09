#ifndef EIDOS_MOTION_HPP
#define EIDOS_MOTION_HPP

namespace eidos {

struct Motion1D {
  float position{};
  float velocity{};
};

/// Advances motion between 0 and maximum_position, reflecting at boundaries.
/// If maximum_position <= 0, parks at 0 with non-negative velocity.
/// Requires finite inputs, non-negative delta-seconds, and ordinary
/// animation-sized values.
void advance_motion(Motion1D &motion, float delta_seconds,
                    float maximum_position);

} // namespace eidos

#endif
