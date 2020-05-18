# catch-main re-used from libcore

# rules_registry

set(rules_registry_test_SRC
    rules_registry_test.cpp
)

add_executable(rules_registry_test ${rules_registry_test_SRC})
target_include_directories(rules_registry_test
    PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/librules
    ${CMAKE_SOURCE_DIR}/vendor
)
target_link_libraries(rules_registry_test
    PRIVATE
    catch_main
    core
    rules
)

# ruleset

set(ruleset_test_SRC
    ruleset_test.cpp
)

add_executable(ruleset_test ${ruleset_test_SRC})
target_include_directories(ruleset_test
    PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/librules
    ${CMAKE_SOURCE_DIR}/vendor
)
target_link_libraries(ruleset_test
    PRIVATE
    catch_main
    core
    rules
)

# rulesets_registry

set(rulesets_registry_test_SRC
    rulesets_registry_test.cpp
)

add_executable(rulesets_registry_test ${rulesets_registry_test_SRC})
target_include_directories(rulesets_registry_test
    PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/librules
    ${CMAKE_SOURCE_DIR}/vendor
)
target_link_libraries(rulesets_registry_test
    PRIVATE
    catch_main
    core
    rules
)