# aus_fy13_lito

set(aus_fy13_lito_test_SRC
    aus_fy13_lito_test.cpp
)

add_executable(aus_fy13_lito_test ${aus_fy13_lito_test_SRC})
target_include_directories(aus_fy13_lito_test
    PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/librules
    ${CMAKE_SOURCE_DIR}/vendor
)
target_link_libraries(aus_fy13_lito_test
    PRIVATE
    catch_main
    core
    rules
)
