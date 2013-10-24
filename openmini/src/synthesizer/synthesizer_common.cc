/// @filename synthesizer_common.cc
/// @brief OpenMini common stuff for synthesizer classes - declarations
/// @author gm
/// @copyright gm 2013
///
/// This file is part of OpenMini
///
/// OpenMini is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// OpenMini is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with OpenMini.  If not, see <http://www.gnu.org/licenses/>.

// std::floor, std::pow
#include <cmath>

#include "openmini/src/synthesizer/synthesizer_common.h"

namespace openmini {
namespace synthesizer {

int ConvertFloorAsInteger(const float value) {
  return static_cast<int>(std::floor(value));
}

bool IsInteger(const float value) {
  return std::floor(value) == value;
}

float NoteToFrequency(const unsigned int key_number) {
  const float exponent((static_cast<float>(key_number) - 49.0f) / 12.0f);
  return std::pow(2.0f, exponent) * 440.0f;
}

}  // namespace synthesizer
}  // namespace openmini