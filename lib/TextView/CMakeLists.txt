find_package(Qt6 REQUIRED COMPONENTS Quick Qml)

qt_add_library(TextViewLib STATIC
    FontLoader.h FontLoader.cpp
    TextAttributes.cpp TextAttributes.h)

target_sources(TextViewLib
    PRIVATE
        TextView.cpp
        TextFrameBuffer.cpp
    PUBLIC
        TextView.h
        TextFrameBuffer.h
)

target_link_libraries(TextViewLib
    PUBLIC
        Qt6::Quick
)

qt_add_qml_module(TextViewPlugin
    URI UI.Editors
    VERSION 1.0
    CLASS_NAME UiEditorsPlugin
    SOURCES
        plugin.cpp
        plugin.h
)

target_link_libraries(TextViewPlugin
    PRIVATE
        TextViewLib
        Qt6::Quick
        Qt6::Qml
)
