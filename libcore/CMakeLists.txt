set(libcore_SRC
    cash_amount.cpp
    income_slice.cpp
    line_item.cpp
    tax_return.cpp
)

add_library(core ${libcore_SRC})

add_subdirectory(tests)