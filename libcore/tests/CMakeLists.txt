# catch-main

add_library(catch_main STATIC catch_main.cpp)
target_include_directories(catch_main
    PUBLIC
    ${CMAKE_SOURCE_DIR}/vendor
)

# currency-amount-test

set(currency_amount_test_SRC
    currency_amount_test.cpp
)

add_executable(currency_amount_test ${currency_amount_test_SRC})
target_include_directories(currency_amount_test
    PUBLIC
    ${CMAKE_SOURCE_DIR}/libcore
    ${CMAKE_SOURCE_DIR}/vendor
)
target_link_libraries(currency_amount_test
    PRIVATE
    catch_main
    core
)