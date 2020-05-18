#include <tuple>

#include "libcore/defs.h"
#include "libcore/rule.h"

namespace rules {
namespace collections {

using core::Rule;
using core::RuleId;

// REV_FY19: first active in the financial year 2018/19

const RuleId AUS_REV_FY19_BRACKET1 = 2019001001;
const RuleId AUS_REV_FY19_BRACKET2 = 2019001002;
const RuleId AUS_REV_FY19_BRACKET3 = 2019001003;
const RuleId AUS_REV_FY19_BRACKET4 = 2019001004;
const RuleId AUS_REV_FY19_BRACKET5 = 2019001005;
const RuleId AUS_REV_FY19_MEDICARE_LEVY = 2019009001;
const RuleId AUS_REV_FY19_MEDICARE_LEVY_SURCHARGE = 2019009002;

Rule get_aus_rev_fy19_bracket1();
Rule get_aus_rev_fy19_bracket2();
Rule get_aus_rev_fy19_bracket3();
Rule get_aus_rev_fy19_bracket4();
Rule get_aus_rev_fy19_bracket5();
Rule get_aus_rev_fy19_medicare_levy();
Rule get_aus_rev_fy19_medicare_levy_surcharge();

// rule factory helpers

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

// ruleset factory helpers

using RulesetTuple = std::tuple<std::string, std::string, std::vector<RuleId>>;

const std::vector<RulesetTuple> RULESET_TUPLES{
    std::make_tuple("aus-2020", "Australian Income Tax 2020",
                    std::vector<RuleId>{
                        AUS_REV_FY19_BRACKET1, AUS_REV_FY19_BRACKET2,
                        AUS_REV_FY19_BRACKET3, AUS_REV_FY19_BRACKET4,
                        AUS_REV_FY19_BRACKET5, AUS_REV_FY19_MEDICARE_LEVY,
                        // AUS_REV_FY19_MEDICARE_LEVY_SURCHARGE,
                    }),
};

} // namespace collections
} // namespace rules