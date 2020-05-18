#include "libcore/bracket.h"
#include "libcore/cash_amount.h"
#include "libcore/format.h"
#include "libcore/rule.h"
#include "libcore/shortcuts.h"

namespace rules {
namespace db {

using core::Bracket;
using core::CashAmount;
using core::CreditDebit;
using core::FnCalc;
using core::IncomeSlice;
using core::LineItem;
using core::Rule;
using core::RuleId;
using core::TaxReturn;

const RuleId AUS_BRACKET_1 = 1001;

Rule get_bracket_1(RuleId rule_id) {
    std::string slug{"Bracket 0 - 18.2k"};
    std::string desc{"0% on income 0k - 18.2k"};

    Bracket bracket{C(0), C(18200)};

    FnCalc fn = FN_CALC_SIGNATURE {
        CashAmount taxable = bracket.in_bracket(slice);
        CashAmount payable = taxable * 0.0;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule rule{rule_id, slug, desc, fn};
    return rule;
}

} // namespace db
} // namespace rules