# catch-main re-used from libcore

# ruleset

set(ruleset_test_SRC
    ruleset_test.cpp
)

add_executable(ruleset_test ${ruleset_test_SRC})
target_include_directories(ruleset_test
    PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/librules
    ${CMAKE_SOURCE_DIR}/vendor
)
target_link_libraries(ruleset_test
    PRIVATE
    catch_main
    core
    rules
)