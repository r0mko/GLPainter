find_package(Qt6 REQUIRED COMPONENTS Quick Qml)

qt_add_library(TextViewLib
    SHARED
    FontLoader.h
    TextView.h
    TextFramebuffer.h
    TextViewRenderer.h
    DFCache.h
    DFGlyph.h
    FontLoader.cpp
    TextView.cpp
    TextFramebuffer.cpp
    TextViewRenderer.cpp
    DFCache.cpp
    DFGlyph.cpp
    TextAttributes.cpp
    TextAttributes.h
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
add_dependencies(TextViewLib TextViewShaders)
foreach(qsb ${TEXTVIEW_QSBS})
    get_filename_component(alias ${qsb} NAME)
    set_source_files_properties(${qsb} PROPERTIES QT_RESOURCE_ALIAS ${alias})
endforeach()
qt_add_resources(TextViewLib textview_qsb PREFIX "/shaders" FILES ${TEXTVIEW_QSBS})

target_link_libraries(TextViewLib
    PUBLIC
        Qt6::Quick
        Qt6::QuickPrivate
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
