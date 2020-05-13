#ifndef LIBCORE_SHORTCUTS_H
#define LIBCORE_SHORTCUTS_H

#include "cash_amount.h"

#define C(value) CashAmount::from("" #value)

#endif // LIBCORE_SHORTCUTS_H