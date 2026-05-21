#pragma once

namespace test_constants {

// 1 meter = 3.28084 feet (factor stored as feet-per-meter hub factor)
inline constexpr double kMeterToFeetFactor = 3.28084;
// 1 meter = 1.09361 yard
inline constexpr double kMeterToYardFactor = 1.09361;
inline constexpr double kEpsilonStrict = 1e-9;
inline constexpr double kEpsilonRelaxed = 1e-5;

}  // namespace test_constants
