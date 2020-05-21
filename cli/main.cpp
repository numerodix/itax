#include <iomanip>
#include <iostream>

#include "cli/presenter.h"
#include "libcore/argparse.h"
#include "librules/rules_registry.h"
#include "librules/rulesets_registry.h"

int main(int argc, const char *argv[]) {
    core::ArgParser parser{};
    auto taxrets = parser.parse(argc, argv);

    if (taxrets.size() < 1) {
        std::cout << "Usage: " << argv[0] << " <income_slice>+\n";
        return EXIT_FAILURE;
    }

    core::TaxReturn taxret = taxrets[0];

    auto rules_registry = rules::RulesRegistry::instance();
    auto rulesets_registry = rules::RulesetsRegistry::instance();

    const rules::Ruleset &ruleset = rulesets_registry->get_ruleset("aus-2020");

    rules::TaxCalculation calc = ruleset.apply(taxret);

    cli::Presenter presenter{rules_registry, calc};

    std::cout << "Applying ruleset: " << ruleset.desc() << "\n";
    std::cout << "\n";

    auto block = presenter.present();
    std::cout << block;

    return 0;
}