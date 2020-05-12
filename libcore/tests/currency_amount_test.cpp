#include <cassert>

#include "currency_amount.h"

int main() {
    core::CurrencyAmount amt{1234678900L};

    assert(amt.m_amount == 1234678900L);
}