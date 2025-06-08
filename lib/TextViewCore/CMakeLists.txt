find_package(Qt6 REQUIRED COMPONENTS Core Gui)

qt_add_library(TextViewCore
    SHARED
    FontLoader.h
    TextFramebuffer.h
    TextRenderer.h
    DFCache.h
    TextAttributes.h
    FontLoader.cpp
    TextFramebuffer.cpp
    TextRenderer.cpp
    DFCache.cpp
    TextAttributes.cpp
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
