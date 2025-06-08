find_package(Qt6 REQUIRED COMPONENTS Quick Qml)

qt_add_qml_module(TextViewPlugin
    URI UI.Editors
    VERSION 1.0
    CLASS_NAME UiEditorsPlugin
    SOURCES
        plugin.cpp
        plugin.h
        TextView.cpp
        TextView.h
        QuickTextViewRenderer.cpp
        QuickTextViewRenderer.h
        DFGlyph.cpp
        DFGlyph.h
        FontLoaderObject.cpp
        FontLoaderObject.h
        TextFramebufferObject.cpp
        TextFramebufferObject.h
)

target_link_libraries(TextViewPlugin
    PRIVATE
        TextViewCore
        Qt6::Quick
        Qt6::Qml
)

target_include_directories(TextViewPlugin PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/TextViewCore
)
