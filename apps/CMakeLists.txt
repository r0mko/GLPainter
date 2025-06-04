add_executable(GLPainterApp main.cpp)

target_link_libraries(GLPainterApp PRIVATE Qt6::Core)

install(TARGETS GLPainterApp RUNTIME DESTINATION bin)
