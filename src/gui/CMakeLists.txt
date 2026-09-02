add_library(gui ColorPickerWidget.h ColorPickerWidget.cpp)

target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(gui PUBLIC Qt6::Core Qt6::Gui Qt6::Widgets)
