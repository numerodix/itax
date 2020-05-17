#ifndef LIBRULES_TAX_CALC_H
#define LIBRULES_TAX_CALC_H

#include <map>
#include <vector>

#include "libcore/income_slice.h"
#include "libcore/rule.h"
#include "libcore/rule_items.h"

namespace rules {

using SliceIterator = std::vector<core::RuleItems>::const_iterator;

// Represents the tax calculation for a given tax year.

class TaxCalculation {
  public:
    TaxCalculation(std::map<core::RuleId, core::Rule> rules);
    ~TaxCalculation();

    // Producer

    void add_slice(const core::IncomeSlice &slice,
                   const std::vector<core::RuleItems> &items);

    // Consumer

    std::size_t num_slices() const;
    std::vector<core::IncomeSlice> slices() const;

    SliceIterator slice_begin(const core::IncomeSlice &slice) const;
    SliceIterator slice_end(const core::IncomeSlice &slice) const;

    const core::Rule &get_rule(core::RuleId rule_id) const;

  private:
    std::map<core::RuleId, core::Rule> m_rules;
    std::map<core::IncomeSlice, std::vector<core::RuleItems>> m_calc;
};

} // namespace rules

#endif // LIBRULES_TAX_CALC_H
