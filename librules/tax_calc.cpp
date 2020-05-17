#include "tax_calc.h"

namespace rules {

TaxCalculation::TaxCalculation(std::map<core::RuleId, core::Rule> rules)
    : m_rules{rules}, m_calc{} {}

TaxCalculation::~TaxCalculation() = default;

void TaxCalculation::add_slice(const core::IncomeSlice &slice,
                               const std::vector<core::RuleItems> &items) {
    m_calc[slice] = items;
}

std::size_t TaxCalculation::num_slices() const { return m_calc.size(); }

std::vector<core::IncomeSlice> TaxCalculation::slices() const {
    std::vector<core::IncomeSlice> slices{};

    for (auto pair : m_calc) {
        slices.push_back(pair.first);
    }

    return slices;
}

SliceIterator
TaxCalculation::slice_begin(const core::IncomeSlice &slice) const {
    auto rule_items = m_calc.at(slice);
    return rule_items.begin();
}

SliceIterator TaxCalculation::slice_end(const core::IncomeSlice &slice) const {
    auto rule_items = m_calc.at(slice);
    return rule_items.end();
}

const core::Rule &TaxCalculation::get_rule(core::RuleId rule_id) const {
    return m_rules.at(rule_id);
}

} // namespace rules