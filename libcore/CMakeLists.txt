set(libcore_SRC
    cash_amount.cpp
)

add_library(core ${libcore_SRC})

add_subdirectory(tests)