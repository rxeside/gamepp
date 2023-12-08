add_executable(game1.1 main.cpp)

target_include_directories(game1.1 PRIVATE ${SFML_INCLUDE_DIR})
target_compile_features(game1.1 PUBLIC cxx_std_17)
target_compile_definitions(game1.1 PRIVATE SFML_STATIC)

target_link_libraries(game1.1 ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})