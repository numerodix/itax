# catch-main

add_library(catch_main STATIC catch_main.cpp)
target_include_directories(catch_main
    PUBLIC
    ${CMAKE_SOURCE_DIR}/vendor
)

# argparse

set(argparse_test_SRC
    argparse_test.cpp
)

add_executable(argparse_test ${argparse_test_SRC})
target_include_directories(argparse_test
    PUBLIC
    ${CMAKE_SOURCE_DIR}/libcore
    ${CMAKE_SOURCE_DIR}/vendor
)
target_link_libraries(argparse_test
    PRIVATE
    catch_main
    core
)

# bracket

set(bracket_test_SRC
    bracket_test.cpp
)

add_executable(bracket_test ${bracket_test_SRC})
target_include_directories(bracket_test
    PUBLIC
    ${CMAKE_SOURCE_DIR}/libcore
    ${CMAKE_SOURCE_DIR}/vendor
)
target_link_libraries(bracket_test
    PRIVATE
    catch_main
    core
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

# line-item

set(line_item_test_SRC
    line_item_test.cpp
)

add_executable(line_item_test ${line_item_test_SRC})
target_include_directories(line_item_test
    PUBLIC
    ${CMAKE_SOURCE_DIR}/libcore
    ${CMAKE_SOURCE_DIR}/vendor
)
target_link_libraries(line_item_test
    PRIVATE
    catch_main
    core
)

# rule-items

set(rule_items_test_SRC
    rule_items_test.cpp
)

add_executable(rule_items_test ${rule_items_test_SRC})
target_include_directories(rule_items_test
    PUBLIC
    ${CMAKE_SOURCE_DIR}/libcore
    ${CMAKE_SOURCE_DIR}/vendor
)
target_link_libraries(rule_items_test
    PRIVATE
    catch_main
    core
)

# tax-return

set(tax_return_test_SRC
    tax_return_test.cpp
)

add_executable(tax_return_test ${tax_return_test_SRC})
target_include_directories(tax_return_test
    PUBLIC
    ${CMAKE_SOURCE_DIR}/libcore
    ${CMAKE_SOURCE_DIR}/vendor
)
target_link_libraries(tax_return_test
    PRIVATE
    catch_main
    core
)
