#include <iomanip>
#include <iostream>

#include "cli/presenter.h"
#include "libcore/argparse.h"
#include "librules/rules_registry.h"
#include "librules/rulesets_registry.h"

int main(int argc, const char *argv[]) {
    auto rulesets_registry = rules::RulesetsRegistry::instance();
    std::string ruleset_id{"aus-2021"};

    // Temporary hack to parse --rules aus-2020 which has to be at the
    // beginning of the args, before any of the income slices.
    // This really ought to be absorbed by argparse.
    char **argv_cursor = const_cast<char **>(argv);
    int argc_remaining = argc;
    for (int i = 0; i < argc; ++i) {
        if ((std::string(argv[i]) == "--rules") && i + 1 < argc) {
            ruleset_id = std::string(argv[i + 1]);
            argc_remaining = argc - i - 1;
            argv_cursor = const_cast<char **>(argv) + i + 1;
        }
    }

    core::ArgParser parser{};
    auto taxrets =
        parser.parse(argc_remaining, const_cast<const char **>(argv_cursor));

    if (taxrets.size() < 1) {
        std::cout << "Usage: " << argv[0]
                  << " [--rules <ruleset_id>] <income_slice>+\n\n";
        std::cout << "Rulesets:\n";
        for (const std::string &id : rulesets_registry->get_all_ruleset_ids()) {
            std::cout << "- " << id << "\n";
        }
        return EXIT_FAILURE;
    }

    core::TaxReturn taxret = taxrets[0];

    auto rules_registry = rules::RulesRegistry::instance();

    // TODO: handle unknown ruleset_id
    const rules::Ruleset &ruleset = rulesets_registry->get_ruleset(ruleset_id);

    rules::TaxCalculation calc = ruleset.apply(taxret);

    cli::Presenter presenter{rules_registry, calc};

    std::cout << "Applying ruleset: " << ruleset.desc() << "\n";
    std::cout << "\n";

    auto block = presenter.present();
    std::cout << block;

    return 0;
}