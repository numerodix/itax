# catch-main

add_library(catch_main STATIC catch_main.cpp)
target_include_directories(catch_main
    PUBLIC
    ${CMAKE_SOURCE_DIR}/vendor
)

# cash-amount

set(cash_amount_test_SRC
    cash_amount_test.cpp
)

add_executable(cash_amount_test ${cash_amount_test_SRC})
target_include_directories(cash_amount_test
    PUBLIC
    ${CMAKE_SOURCE_DIR}/libcore
    ${CMAKE_SOURCE_DIR}/vendor
)
target_link_libraries(cash_amount_test
    PRIVATE
    catch_main
    core
)

# income-slice

set(income_slice_test_SRC
    income_slice_test.cpp
)

add_executable(income_slice_test ${income_slice_test_SRC})
target_include_directories(income_slice_test
    PUBLIC
    ${CMAKE_SOURCE_DIR}/libcore
    ${CMAKE_SOURCE_DIR}/vendor
)
target_link_libraries(income_slice_test
    PRIVATE
    catch_main
    core
)
