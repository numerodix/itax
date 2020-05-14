set(libcore_SRC
    cash_amount.cpp
    income_slice.cpp
    tax_return.cpp
)

add_library(core ${libcore_SRC})

add_subdirectory(tests)