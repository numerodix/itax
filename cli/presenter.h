#ifndef CLI_PRESENTER_H
#define CLI_PRESENTER_H

#include <string>

#include "librules/rules_registry.h"
#include "librules/tax_calc.h"

namespace cli {

class Presenter {
  public:
    Presenter(const rules::RulesRegistry *rules_registry,
              const rules::TaxCalculation &calc);

    std::string present() const;
    std::string format_slice_header(int sliceno,
                                    const core::IncomeSlice &slice) const;

  private:
    const rules::RulesRegistry *m_rules_registry{nullptr};
    const rules::TaxCalculation &m_calc;
};

} // namespace cli

#endif // CLI_PRESENTER_H
