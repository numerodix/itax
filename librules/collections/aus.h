#include <tuple>

#include "libcore/defs.h"
#include "libcore/rule.h"

namespace rules {
namespace collections {

using core::Rule;
using core::RuleId;

// REV_FY25: first active in the financial year 2024/25

const RuleId AUS_REV_FY25_BRACKET1 = 2025001001;
const RuleId AUS_REV_FY25_BRACKET2 = 2025001002;
const RuleId AUS_REV_FY25_BRACKET3 = 2025001003;
const RuleId AUS_REV_FY25_BRACKET4 = 2025001004;

Rule get_aus_rev_fy25_bracket1();
Rule get_aus_rev_fy25_bracket2();
Rule get_aus_rev_fy25_bracket3();
Rule get_aus_rev_fy25_bracket4();

// TODO: add updated lito/lmito for FY25 once it's official

// REV_FY21: first active in the financial year 2020/21

const RuleId AUS_REV_FY21_BRACKET1 = 2021001001;
const RuleId AUS_REV_FY21_BRACKET2 = 2021001002;
const RuleId AUS_REV_FY21_BRACKET3 = 2021001003;
const RuleId AUS_REV_FY21_BRACKET4 = 2021001004;
const RuleId AUS_REV_FY21_BRACKET5 = 2021001005;

Rule get_aus_rev_fy21_bracket1();
Rule get_aus_rev_fy21_bracket2();
Rule get_aus_rev_fy21_bracket3();
Rule get_aus_rev_fy21_bracket4();
Rule get_aus_rev_fy21_bracket5();

// TODO: add updated lito/lmito for FY21 once it's official

// REV_FY19: first active in the financial year 2018/19

const RuleId AUS_REV_FY19_BRACKET1 = 2019001001;
const RuleId AUS_REV_FY19_BRACKET2 = 2019001002;
const RuleId AUS_REV_FY19_BRACKET3 = 2019001003;
const RuleId AUS_REV_FY19_BRACKET4 = 2019001004;
const RuleId AUS_REV_FY19_BRACKET5 = 2019001005;
const RuleId AUS_REV_FY19_LMITO = 2019003002;
const RuleId AUS_REV_FY19_MEDICARE_LEVY = 2019009001;
const RuleId AUS_REV_FY19_MEDICARE_LEVY_SURCHARGE = 2019009002;

Rule get_aus_rev_fy19_bracket1();
Rule get_aus_rev_fy19_bracket2();
Rule get_aus_rev_fy19_bracket3();
Rule get_aus_rev_fy19_bracket4();
Rule get_aus_rev_fy19_bracket5();
Rule get_aus_rev_fy19_lmito();
Rule get_aus_rev_fy19_medicare_levy();
Rule get_aus_rev_fy19_medicare_levy_surcharge();

// REV_FY13: first active in the financial year 2012/13

const RuleId AUS_REV_FY13_LITO = 2013003001;

Rule get_aus_rev_fy13_lito();

// rule factory helpers

using RuleFactoryFn = std::function<Rule()>;

const std::vector<RuleFactoryFn> RULE_FACTORIES{
    // FY25
    get_aus_rev_fy25_bracket1,
    get_aus_rev_fy25_bracket2,
    get_aus_rev_fy25_bracket3,
    get_aus_rev_fy25_bracket4,
    // TODO add update lito/lmito

    // FY21
    get_aus_rev_fy21_bracket1,
    get_aus_rev_fy21_bracket2,
    get_aus_rev_fy21_bracket3,
    get_aus_rev_fy21_bracket4,
    get_aus_rev_fy21_bracket5,
    // TODO add update lito/lmito

    // FY19
    get_aus_rev_fy19_bracket1,
    get_aus_rev_fy19_bracket2,
    get_aus_rev_fy19_bracket3,
    get_aus_rev_fy19_bracket4,
    get_aus_rev_fy19_bracket5,
    get_aus_rev_fy19_lmito,
    get_aus_rev_fy19_medicare_levy,
    get_aus_rev_fy19_medicare_levy_surcharge,

    // FY13
    get_aus_rev_fy13_lito,
};

// ruleset factory helpers

using RulesetTuple = std::tuple<std::string, std::string, std::vector<RuleId>>;

const std::vector<RulesetTuple> RULESET_TUPLES{
    std::make_tuple("aus-2025", "Australian Income Tax FY 2025",
                    std::vector<RuleId>{
                        AUS_REV_FY25_BRACKET1, AUS_REV_FY25_BRACKET2,
                        AUS_REV_FY25_BRACKET3, AUS_REV_FY25_BRACKET4,
                        // TODO add missing lito/lmito
                        AUS_REV_FY19_MEDICARE_LEVY,
                        // AUS_REV_FY19_MEDICARE_LEVY_SURCHARGE,
                    }),
    std::make_tuple("aus-2021", "Australian Income Tax FY 2021",
                    std::vector<RuleId>{
                        AUS_REV_FY21_BRACKET1, AUS_REV_FY21_BRACKET2,
                        AUS_REV_FY21_BRACKET3, AUS_REV_FY21_BRACKET4,
                        AUS_REV_FY21_BRACKET5,
                        // TODO add missing lito/lmito
                        AUS_REV_FY19_MEDICARE_LEVY,
                        // AUS_REV_FY19_MEDICARE_LEVY_SURCHARGE,
                    }),
    std::make_tuple("aus-2020", "Australian Income Tax FY 2020",
                    std::vector<RuleId>{
                        AUS_REV_FY19_BRACKET1, AUS_REV_FY19_BRACKET2,
                        AUS_REV_FY19_BRACKET3, AUS_REV_FY19_BRACKET4,
                        AUS_REV_FY19_BRACKET5, AUS_REV_FY13_LITO,
                        AUS_REV_FY19_LMITO, AUS_REV_FY19_MEDICARE_LEVY,
                        // AUS_REV_FY19_MEDICARE_LEVY_SURCHARGE,
                    }),
};

} // namespace collections
} // namespace rules