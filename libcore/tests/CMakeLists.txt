include_directories(..)

set(currency_amount_test_SRC
    currency_amount_test.cpp
)

add_executable(currency_amount_test ${currency_amount_test_SRC})
target_link_libraries(currency_amount_test core)