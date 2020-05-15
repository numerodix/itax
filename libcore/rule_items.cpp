#include "rule_items.h"

#include "algos.h"

namespace core {

RuleItems::RuleItems(RuleId rule_id, std::vector<LineItem> items)
    : m_rule_id{rule_id}, m_items{items} {}

RuleItems::~RuleItems() = default;

RuleId RuleItems::rule_id() const { return m_rule_id; }

const std::vector<LineItem> &RuleItems::items() const { return m_items; }

LineItem RuleItems::net() const {
    return sum(m_items.begin(), m_items.end(), LineItem{});
}

} // namespace core