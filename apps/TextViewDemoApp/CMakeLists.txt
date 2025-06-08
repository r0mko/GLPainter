qt_add_executable(TextViewDemoApp
    main.cpp
)

find_package(Qt6 REQUIRED COMPONENTS Quick Qml)

qt_add_qml_module(TextViewDemoApp
    URI TextViewDemoApp
    VERSION 1.0
    QML_FILES main.qml
)

target_link_libraries(TextViewDemoApp PRIVATE Qt6::Quick TextViewPlugin)

install(TARGETS TextViewDemoApp RUNTIME DESTINATION bin)
