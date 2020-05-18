#ifndef LIBRULES_TAX_CALC_H
#define LIBRULES_TAX_CALC_H

#include <map>
#include <vector>

#include "libcore/income_slice.h"
#include "libcore/line_item.h"
#include "libcore/rule.h"
#include "libcore/rule_items.h"

namespace rules {

// Represents the tax calculation for a given tax year.

class TaxCalculation {
  public:
    TaxCalculation();
    ~TaxCalculation();

    // Producer

    void add_slice(const core::IncomeSlice &slice,
                   const std::vector<core::RuleItems> &items);

    // Consumer

    std::size_t num_slices() const;
    std::vector<core::IncomeSlice> slices() const;

    const std::vector<core::RuleItems> &
    get_ruleitems(const core::IncomeSlice &slice) const;

    core::LineItem slice_total(const core::IncomeSlice &slice) const;

    std::vector<core::RuleItems> net_slices() const;
    core::LineItem return_total() const;

  private:
    std::map<core::IncomeSlice, std::vector<core::RuleItems>> m_calc;
};

} // namespace rules

#endif // LIBRULES_TAX_CALC_H
