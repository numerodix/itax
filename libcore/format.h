#ifndef LIBCORE_FORMAT_H
#define LIBCORE_FORMAT_H

#include "cash_amount.h"
#include "defs.h"

namespace core {

std::string format_plain(const CashAmount &amount);
std::string format_with_commas(const CashAmount &amount);

std::string format_percent(double perc);

std::string format_prefix(CreditDebit cd);

} // namespace core

#endif // LIBCORE_FORMAT_H
