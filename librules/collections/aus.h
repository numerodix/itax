#include "libcore/bracket.h"
#include "libcore/cash_amount.h"
#include "libcore/format.h"
#include "libcore/rule.h"
#include "libcore/shortcuts.h"

namespace rules {
namespace collections {

using core::Rule;
using core::RuleId;

// REV_FY19: first active in the financial year 2018/19

const RuleId AUS_REV_FY19_BRACKET1 = 19001001;
const RuleId AUS_REV_FY19_BRACKET2 = 19001002;
const RuleId AUS_REV_FY19_BRACKET3 = 19001003;
const RuleId AUS_REV_FY19_BRACKET4 = 19001004;
const RuleId AUS_REV_FY19_BRACKET5 = 19001005;
const RuleId AUS_REV_FY19_MEDICARE_LEVY = 19009001;
const RuleId AUS_REV_FY19_MEDICARE_LEVY_SURCHARGE = 19009002;

Rule get_aus_rev_fy19_bracket1();
Rule get_aus_rev_fy19_bracket2();
Rule get_aus_rev_fy19_bracket3();
Rule get_aus_rev_fy19_bracket4();
Rule get_aus_rev_fy19_bracket5();
Rule get_aus_rev_fy19_medicare_levy();
Rule get_aus_rev_fy19_medicare_levy_surcharge();

// factory helpers

using RuleFactoryFn = std::function<Rule()>;

const std::vector<RuleFactoryFn> RULE_FACTORIES{
    // FY19
    get_aus_rev_fy19_bracket1,
    get_aus_rev_fy19_bracket2,
    get_aus_rev_fy19_bracket3,
    get_aus_rev_fy19_bracket4,
    get_aus_rev_fy19_bracket5,
    get_aus_rev_fy19_medicare_levy,
    get_aus_rev_fy19_medicare_levy_surcharge,
};

} // namespace collections
} // namespace rules