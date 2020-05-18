#include "format.h"

#include <cmath>
#include <iomanip>
#include <sstream>

namespace core {

std::string format_plain(const CashAmount &amount) {
    int64_t value_part = amount.rounded_value_part();

    int64_t int_part = value_part / 100;
    int64_t dec_part = value_part - (int_part * 100);

    std::stringstream ss{};
    ss << int_part << '.' << std::setfill('0') << std::setw(2) << dec_part;
    return ss.str();
}

std::string format_with_commas(const CashAmount &amount) {
    std::string plain = format_plain(amount);

    std::stringstream ss{};
    int counter = 0;
    bool past_decimal_point = false;

    for (auto it = plain.rbegin(); it != plain.rend(); ++it) {
        if (past_decimal_point) {
            if ((counter > 0) && (counter % 3 == 0)) {
                ss << ',';
            }

            ss << *it;
            ++counter;

        } else {
            if (*it == '.') {
                past_decimal_point = true;
            }

            ss << *it;
        }
    }

    std::string backwards = ss.str();
    std::string forwards(backwards.rbegin(), backwards.rend());
    return forwards;
}

std::string format_percent(double perc) {
    perc *= 100.0;

    int int_part = static_cast<int>(perc);
    int dec_part = std::round((perc - int_part) * 100.0);

    std::stringstream ss{};
    ss << std::setw(2) << std::setfill('0') << int_part;
    ss << '.';
    ss << std::setw(2) << std::setfill('0') << dec_part;

    return ss.str();
}

std::string format_prefix(CreditDebit cd) {
    return cd == CreditDebit::CREDIT ? " " : "-";
}

} // namespace core