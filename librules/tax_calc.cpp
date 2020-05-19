#include "tax_calc.h"

namespace rules {

TaxCalculation::TaxCalculation(std::vector<core::Rule> rules)
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

const std::vector<core::RuleItems> &
TaxCalculation::get_ruleitems(const core::IncomeSlice &slice) const {
    return m_calc.at(slice);
}

core::LineItem
TaxCalculation::slice_total(const core::IncomeSlice &slice) const {
    core::LineItem sum{};

    auto rule_items_vec = m_calc.at(slice);
    for (const auto &rule_items : rule_items_vec) {
        sum = sum + rule_items.net();
    }

    sum.set_taxable(slice.amount());

    return sum;
}

std::vector<core::RuleItems> TaxCalculation::net_slices() const {
    std::map<core::RuleId, core::RuleItems> rule_items_map{};

    // fill the map with an empty RuleItems for each RuleId
    for (const auto &pair : m_calc) {
        const auto &rule_items_vec = pair.second;
        for (const core::RuleItems &rule_items : rule_items_vec) {
            core::RuleId rule_id = rule_items.rule_id();
            core::RuleItems empty{rule_id, {}};
            rule_items_map[rule_id] = empty;
        }
    }

    // fill each RuleItems in the map with LineItem from each slice
    for (const auto &pair : m_calc) {
        const auto &rule_items_vec = pair.second;
        for (const core::RuleItems &rule_items : rule_items_vec) {
            core::RuleId rule_id = rule_items.rule_id();
            core::LineItem netted = rule_items.net();
            rule_items_map[rule_id].append_item(netted);
        }
    }

    // create a vector to return
    std::vector<core::RuleItems> rule_items_vec{};
    for (const auto &rule : m_rules) {
        core::RuleId rule_id = rule.rule_id();
        const core::RuleItems &rule_items = rule_items_map[rule_id];
        rule_items_vec.push_back(rule_items);
    }

    return rule_items_vec;
}

core::LineItem TaxCalculation::return_total() const {
    core::LineItem sum{};

    for (const auto &pair : m_calc) {
        const auto &slice = pair.first;
        sum = sum + slice_total(slice);
    }

    return sum;
}

} // namespace rules