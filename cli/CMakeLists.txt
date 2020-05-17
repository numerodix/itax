set(cli_SRC
    main.cpp
)

add_executable(cli ${cli_SRC})
target_include_directories(cli
    PUBLIC
    ${CMAKE_SOURCE_DIR}
)
target_link_libraries(cli
    PRIVATE
    core
    rules
)