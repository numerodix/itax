set(librules_SRC
    ruleset.cpp
    rules_registry.cpp
    tax_calc.cpp
)

add_library(rules ${librules_SRC})
target_include_directories(rules
    PUBLIC
    ${CMAKE_SOURCE_DIR}
)
target_link_libraries(rules
    PRIVATE
    core
)

add_subdirectory(tests)