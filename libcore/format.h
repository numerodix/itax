#ifndef LIBCORE_FORMAT_H
#define LIBCORE_FORMAT_H

#include "cash_amount.h"

namespace core {

std::string format_plain(const CashAmount& amount);
std::string format_with_commas(const CashAmount& amount);

std::string format_percent(double perc);

} // namespace core

#endif // LIBCORE_FORMAT_H
