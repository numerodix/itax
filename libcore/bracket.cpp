#include "bracket.h"

#include <stdexcept>

namespace core {

Bracket::Bracket(CashAmount lower, CashAmount upper)
    : m_lower{lower}, m_upper{upper} {}

Bracket::~Bracket() = default;

Bracket::Bracket(const Bracket &other) = default;

Bracket &Bracket::operator=(const Bracket &other) = default;

Bracket::Bracket(Bracket &&other) = default;

Bracket &Bracket::operator=(Bracket &&other) = default;

CashAmount Bracket::lower() const { return m_lower; }

CashAmount Bracket::upper() const { return m_upper; }

CashAmount Bracket::in_bracket(const IncomeSlice &slice) const {
    if (slice.upper_bound() <= m_lower) {
        // slice is below the bracket
        return CashAmount{0L};

    } else if (m_upper <= slice.lower_bound()) {
        // slice is above the bracket
        return CashAmount{0L};

    } else if (slice.lower_bound() <= m_lower) {
        // slice starts before or at the start of the bracket

        if (slice.upper_bound() < m_upper) {
            // slice ends before the end of the bracket
            return slice.upper_bound() - m_lower;

        } else {
            // slice ends at or after the end of the bracket
            return m_upper - m_lower;
        }

    } else if (m_lower < slice.lower_bound()) {
        // slice starts after the start of the bracket

        if (slice.upper_bound() < m_upper) {
            // slice ends before the end of the bracket
            return slice.upper_bound() - slice.lower_bound();

        } else {
            // slice ends after the end of the bracket
            return m_upper - slice.lower_bound();
        }
    }

    throw std::runtime_error("should never reach this point");
}

} // namespace core