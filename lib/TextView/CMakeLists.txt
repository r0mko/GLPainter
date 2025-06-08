find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Qml)

# TextViewCore library
set(TEXTVIEW_CORE_HEADERS
    lib/FontLoader.h
    lib/TextFramebuffer.h
    lib/TextRenderer.h
    lib/TextAttributes.h
)

set(TEXTVIEW_CORE_SOURCES
    lib/FontLoader.cpp
    lib/TextFramebuffer.cpp
    lib/TextRenderer.cpp
    lib/TextAttributes.cpp
)

qt_add_library(TextViewCore
    SHARED
    ${TEXTVIEW_CORE_HEADERS}
    ${TEXTVIEW_CORE_SOURCES}
)

set(TEXTVIEW_SHADERS
    shaders/textview.vert
    shaders/textview.frag
)
set(TEXTVIEW_QSBS)
foreach(src ${TEXTVIEW_SHADERS})
    get_filename_component(fname ${src} NAME)
    set(out ${CMAKE_CURRENT_BINARY_DIR}/shaders/${fname}.qsb)
    add_custom_command(
        OUTPUT ${out}
        COMMAND /usr/lib/qt6/bin/qsb ${CMAKE_CURRENT_SOURCE_DIR}/${src} -o ${out} --glsl 440
        DEPENDS ${src}
    )
    list(APPEND TEXTVIEW_QSBS ${out})
endforeach()
add_custom_target(TextViewShaders ALL DEPENDS ${TEXTVIEW_QSBS})
add_dependencies(TextViewCore TextViewShaders)
foreach(qsb ${TEXTVIEW_QSBS})
    get_filename_component(alias ${qsb} NAME)
    set_source_files_properties(${qsb} PROPERTIES QT_RESOURCE_ALIAS ${alias})
endforeach()
qt_add_resources(TextViewCore textview_qsb PREFIX "/shaders" FILES ${TEXTVIEW_QSBS})

target_link_libraries(TextViewCore
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::GuiPrivate
)

# QML plugin using the library
qt_add_qml_module(TextViewPlugin
    URI UI.Editors
    VERSION 1.0
    CLASS_NAME UiEditorsPlugin
    SOURCES
        plugin/plugin.cpp
        plugin/plugin.h
        plugin/TextView.cpp
        plugin/TextView.h
        plugin/QuickTextViewRenderer.cpp
        plugin/QuickTextViewRenderer.h
        plugin/DFGlyph.cpp
        plugin/DFGlyph.h
        plugin/FontLoaderObject.cpp
        plugin/FontLoaderObject.h
        plugin/TextFramebufferObject.cpp
        plugin/TextFramebufferObject.h
        plugin/QmlTextAttributes.cpp
        plugin/QmlTextAttributes.h
        plugin/TextAttributesSingleton.h
)

target_link_libraries(TextViewPlugin
    PRIVATE
        TextViewCore
        Qt6::Quick
        Qt6::Qml
)

target_include_directories(TextViewPlugin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin
)
