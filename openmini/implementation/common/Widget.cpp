/// @filename Widget.cpp
/// @brief OpenMini generic widget component
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


// This file is NOT generated by Juce (at least not after the first time),
// That's why we apply our coding style here

#include "openmini/implementation/common/Widget.h"

Widget::Widget(const ParameterMeta& param_meta)
    : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                   juce::Slider::TextEntryBoxPosition::NoTextBox),
      label_() {
  label_.setText(juce::String(param_meta.name()));
  addAndMakeVisible(&label_);
}

Widget::~Widget() {
  // Nothing to do here for now
}

void Widget::paint(juce::Graphics& g) {
  juce::Slider::paint(g);
  label_.setCentreRelative(0.5f, 0.95f);
}
