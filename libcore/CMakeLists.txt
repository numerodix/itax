set(libcore_SRC
    currency_amount.cpp
)

add_library(core ${libcore_SRC})

add_subdirectory(tests)