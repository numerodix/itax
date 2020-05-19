#ifndef LIBCORE_FORMAT_H
#define LIBCORE_FORMAT_H

#include "cash_amount.h"
#include "defs.h"

namespace core {

std::string format_plain(const CashAmount &amount);
std::string format_with_commas(const CashAmount &amount);

std::string format_percent(double perc);

std::string format_sign(CreditDebit cd);
std::string format_with_sign(CreditDebit cd, const CashAmount &amount);

} // namespace core

#endif // LIBCORE_FORMAT_H
