/// @filename MainApplication.h
/// @brief OpenMini standalone main application
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

#include "JuceHeader.h"
#include "modules/juce_audio_plugin_client/Standalone/juce_StandaloneFilterWindow.h"

/// @brief Main application class, holds everything.
/// It handles initialization and shutdown thanks to JUCEApplication.
class MainApplication : public juce::JUCEApplication {
 public:
  MainApplication();

  const juce::String getApplicationName();
  const juce::String getApplicationVersion();
  bool moreThanOneInstanceAllowed();

  void initialise(const juce::String& commandLine);

  void shutdown();

  void systemRequestedQuit();

  void anotherInstanceStarted(const juce::String& commandLine);

 private:
  StandaloneFilterWindow plugin_;
};

// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(MainApplication)
