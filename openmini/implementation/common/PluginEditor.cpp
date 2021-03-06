/// @filename PluginEditor.cpp
/// @brief OpenMini plugin editor (UI & control)
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

// std::floor
#include <cmath>

#include "openmini/implementation/common/PluginEditor.h"

#include "openmini/src/common.h"
#include "openmini/src/synthesizer/parameters.h"

OpenMiniAudioProcessorEditor::OpenMiniAudioProcessorEditor(
    OpenMiniAudioProcessor* owner)
    : AudioProcessorEditor(owner),
      widgets_manager_(openmini::synthesizer::Parameters::kParametersMeta),
      resizer_(this, &resize_limits_),
      resize_limits_(),
      keyboard_(owner->keyboard_state_,
                juce::MidiKeyboardComponent::horizontalKeyboard),
      debug_infos_() {
  addAndMakeVisible(&widgets_manager_);
  // TODO(gm): this is not normal,
  // plus adding/removing change listeners should be done in the same class
  addChangeListener(&widgets_manager_);
  // For some reason Juce keyboard is one octave higher...
  keyboard_.setAvailableRange(openmini::kMinKeyNote + 12,
                              openmini::kMaxKeyNote + 12);
  addAndMakeVisible(&keyboard_);

  // DEBUG
  addAndMakeVisible(&debug_infos_);
  this->startTimer(kTimerInterval);
  //  /DEBUG

  addAndMakeVisible(resizer_);
  resize_limits_.setSizeLimits(150, 150, kMaxWindowWidth, kMaxWindowHeight);

  getProcessor()->addChangeListener(this);
  // This is where our plugin's editor size is set.
  setSize(owner->lastUIWidth, owner->lastUIHeight);
}

OpenMiniAudioProcessorEditor::~OpenMiniAudioProcessorEditor() {
  getProcessor()->removeChangeListener(this);
}

void OpenMiniAudioProcessorEditor::paint(juce::Graphics& g) {
  g.fillAll(Colours::white);
}

void OpenMiniAudioProcessorEditor::resized() {
  Rectangle<int> area(getLocalBounds());

  widgets_manager_.setBounds(area.removeFromTop(getHeight() * 1 / 2).reduced(8));

  // DEBUG
  debug_infos_.setBounds(area.removeFromTop(getHeight() * 1 / 8).reduced(8));
  //  /DEBUG

  keyboard_.setBounds(area.removeFromTop(getHeight() * 1 / 8).reduced(8));
  keyboard_.setKeyWidth(std::floor(static_cast<float>(getWidth())
                        / ((openmini::kMaxKeyNote - openmini::kMinKeyNote)
                        // remember, 5 keys out of 12 are black!
                        * 7.0f / 12.0f)));

  resizer_.setBounds(getWidth() - 16, getHeight() - 16, 16, 16);
  getProcessor()->lastUIWidth = getWidth();
  getProcessor()->lastUIHeight = getHeight();
}

void OpenMiniAudioProcessorEditor::changeListenerCallback(
    juce::ChangeBroadcaster *source) {
  OpenMiniAudioProcessor* proc(getProcessor());
  // No other change broacaster than the processor for now!
  OPENMINI_ASSERT(source == proc);
  // Inform all UI subcomponents of any change
  sendChangeMessage();
}

void OpenMiniAudioProcessorEditor::timerCallback() {
  const double time(getProcessor()->GetLastProcessTime());
  debug_infos_.setText(juce::String(time));
}

float OpenMiniAudioProcessorEditor::GetParamValue(const int param_id) {
  return getProcessor()->getParameter(param_id);
}

void OpenMiniAudioProcessorEditor::ParamValueChanged(const int param_id,
                                                     const float value) {
  getProcessor()->setParameterNotifyingHost(param_id, value);
}

void OpenMiniAudioProcessorEditor::ParamGestureBegan(const int param_id) {
  getProcessor()->beginParameterChangeGesture(param_id);
}

void OpenMiniAudioProcessorEditor::ParamGestureEnded(const int param_id) {
  getProcessor()->endParameterChangeGesture(param_id);
}

OpenMiniAudioProcessor* OpenMiniAudioProcessorEditor::getProcessor() const {
  return static_cast<OpenMiniAudioProcessor*>(getAudioProcessor());
}
