add_library(gui ColorPickerWidget.h ColorPickerWidget.cpp MagnifierWidget.h
                MagnifierWidget.cpp)

target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(gui PUBLIC Qt6::Core Qt6::Gui Qt6::Widgets)

if(WIN32)
  target_link_libraries(gui PUBLIC user32)
endif()
