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

// Using directives for OpenMini parameters
using namespace openmini::synthesizer::Parameters;

OpenMiniAudioProcessorEditor::OpenMiniAudioProcessorEditor(
    OpenMiniAudioProcessor* owner)
    : AudioProcessorEditor(owner),
      owner_(owner),
      keyboard_(owner->keyboard_state_,
                juce::MidiKeyboardComponent::horizontalKeyboard),
      osc1_waveform_(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                     juce::Slider::TextEntryBoxPosition::NoTextBox),
      osc2_waveform_(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                     juce::Slider::TextEntryBoxPosition::NoTextBox),
      osc3_waveform_(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                     juce::Slider::TextEntryBoxPosition::NoTextBox),
      osc1_volume_(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                   juce::Slider::TextEntryBoxPosition::NoTextBox),
      osc2_volume_(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                   juce::Slider::TextEntryBoxPosition::NoTextBox),
      osc3_volume_(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                   juce::Slider::TextEntryBoxPosition::NoTextBox),
      filter_freq_(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                   juce::Slider::TextEntryBoxPosition::NoTextBox),
      filter_q_(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                juce::Slider::TextEntryBoxPosition::NoTextBox),
      debug_infos_() {
  // For some reason Juce keyboard is one octave higher...
  keyboard_.setAvailableRange(openmini::kMinKeyNote + 12,
                              openmini::kMaxKeyNote + 12);
  keyboard_.setKeyWidth(std::floor(static_cast<float>(kMainWindowSizeX)
                        / ((openmini::kMaxKeyNote - openmini::kMinKeyNote)
                        // remember, 5 keys out of 12 are black!
                        * 7.0f / 12.0f)));
  addAndMakeVisible(&keyboard_);
  addAndMakeVisible(&osc1_waveform_);
  addAndMakeVisible(&osc2_waveform_);
  addAndMakeVisible(&osc3_waveform_);
  addAndMakeVisible(&osc1_volume_);
  addAndMakeVisible(&osc2_volume_);
  addAndMakeVisible(&osc3_volume_);
  addAndMakeVisible(&filter_freq_);
  addAndMakeVisible(&filter_q_);
  osc1_waveform_.setRange(0.0, 1.0);
  osc2_waveform_.setRange(0.0, 1.0);
  osc3_waveform_.setRange(0.0, 1.0);
  osc1_volume_.setRange(0.0, 1.0);
  osc2_volume_.setRange(0.0, 1.0);
  osc3_volume_.setRange(0.0, 1.0);
  filter_freq_.setRange(0.0, 1.0);
  filter_q_.setRange(0.0, 1.0);
  osc1_waveform_.addListener(this);
  osc2_waveform_.addListener(this);
  osc3_waveform_.addListener(this);
  osc1_volume_.addListener(this);
  osc2_volume_.addListener(this);
  osc3_volume_.addListener(this);
  filter_freq_.addListener(this);
  filter_q_.addListener(this);

  // DEBUG
  addAndMakeVisible(&debug_infos_);
  this->startTimer(kTimerInterval);
  //  /DEBUG

  getProcessor()->addChangeListener(this);
  // This is where our plugin's editor size is set.
  setSize(kMainWindowSizeX, kMainWindowSizeY);
}

OpenMiniAudioProcessorEditor::~OpenMiniAudioProcessorEditor() {
  getProcessor()->removeChangeListener(this);
}

void OpenMiniAudioProcessorEditor::paint(juce::Graphics& g) {
  g.fillAll(Colours::white);
  const int kKeyboardHeight(this->getHeight() / 4);
  const int kKeyboardTop(this->getHeight() - kKeyboardHeight);
  keyboard_.setBounds(0, kKeyboardTop, this->getWidth(), kKeyboardHeight);
  osc1_waveform_.setBounds(0, 0, 60, 60);
  osc2_waveform_.setBounds(0, 70, 60, 60);
  osc3_waveform_.setBounds(0, 140, 60, 60);
  osc1_volume_.setBounds(200, 0, 60, 60);
  osc2_volume_.setBounds(200, 70, 60, 60);
  osc3_volume_.setBounds(200, 140, 60, 60);
  filter_freq_.setBounds(0, 210, 60, 60);
  filter_q_.setBounds(200, 210, 60, 60);

  // DEBUG
  debug_infos_.setBounds(300, 0, this->getWidth(), 100);
  //  /DEBUG
}

void OpenMiniAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
  const float value(static_cast<float>(slider->getValue()));
  if (slider == &osc1_waveform_) {
    getProcessor()->setParameterNotifyingHost(kOsc1Waveform, value);
  } else if (slider == &osc2_waveform_) {
    getProcessor()->setParameterNotifyingHost(kOsc2Waveform, value);
  } else if (slider == &osc3_waveform_) {
    getProcessor()->setParameterNotifyingHost(kOsc3Waveform, value);
  } else if (slider == &osc1_volume_) {
    getProcessor()->setParameterNotifyingHost(kOsc1Volume, value);
  } else if (slider == &osc2_volume_) {
    getProcessor()->setParameterNotifyingHost(kOsc2Volume, value);
  } else if (slider == &osc3_volume_) {
    getProcessor()->setParameterNotifyingHost(kOsc3Volume, value);
  } else if (slider == &filter_freq_) {
    getProcessor()->setParameterNotifyingHost(kFilterFreq, value);
  } else if (slider == &filter_q_) {
    getProcessor()->setParameterNotifyingHost(kFilterQ, value);
  } else {
    // Should never happen
    ASSERT(false);
  }
}
  }
}

void OpenMiniAudioProcessorEditor::changeListenerCallback(
    juce::ChangeBroadcaster *source) {
  OpenMiniAudioProcessor* proc(getProcessor());
  // No other change broacaster than the processor for now!
  ASSERT(source == proc);
  // Update UI components without sending back
  // notifications to the processor
  // TODO: Make this generic
  osc1_waveform_.setValue(proc->getParameter(kOsc1Waveform),
                          juce::dontSendNotification);
  osc2_waveform_.setValue(proc->getParameter(kOsc2Waveform),
                          juce::dontSendNotification);
  osc3_waveform_.setValue(proc->getParameter(kOsc3Waveform),
                          juce::dontSendNotification);
  osc1_volume_.setValue(proc->getParameter(kOsc1Volume),
                        juce::dontSendNotification);
  osc2_volume_.setValue(proc->getParameter(kOsc2Volume),
                        juce::dontSendNotification);
  osc3_volume_.setValue(proc->getParameter(kOsc3Volume),
                        juce::dontSendNotification);
  filter_freq_.setValue(proc->getParameter(kFilterFreq),
                        juce::dontSendNotification);
  filter_q_.setValue(proc->getParameter(kFilterQ),
                     juce::dontSendNotification);
}

void OpenMiniAudioProcessorEditor::timerCallback() {
  const double time(getProcessor()->GetLastProcessTime());
  debug_infos_.setText(juce::String(time));
}

OpenMiniAudioProcessor* OpenMiniAudioProcessorEditor::getProcessor() const {
  return static_cast<OpenMiniAudioProcessor*>(getAudioProcessor());
}
