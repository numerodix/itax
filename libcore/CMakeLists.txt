set(libcore_SRC
    argparse.cpp
    bracket.cpp
    cash_amount.cpp
    income_slice.cpp
    line_item.cpp
    rule_items.cpp
    tax_return.cpp
)

add_library(core ${libcore_SRC})

add_subdirectory(tests)