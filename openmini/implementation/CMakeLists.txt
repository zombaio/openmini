# @brief OpenMini implementation root cmake file

# These have to be defined since JUCE needs it.
# Sometimes they're not passed to the compiler,
# so we're doing it here explicitly.
if (SYSTEM_IS_LINUX)
  add_definitions(-DLINUX)
elseif (SYSTEM_IS_WINDOWS)
  add_definitions(-D_WIN32)
endif ()

# Mac OS X being the default case for Juce, nothing more to declare

add_subdirectory(common)

if (OPENMINI_HAS_VST)
  add_subdirectory(plugin)
endif (OPENMINI_HAS_VST)

add_subdirectory(standalone)
