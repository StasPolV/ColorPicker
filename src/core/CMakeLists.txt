add_library(core ScreenGrabber.h ScreenGrabber.cpp ColorGrabber.h ColorGrabber.cpp)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(core PUBLIC Qt6::Core Qt6::Gui)
