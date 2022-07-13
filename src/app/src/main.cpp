#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <config/Config.h>

#include <iostream>

auto app(int /*argc*/, char ** /*argv*/) -> int;

auto main(int argc, char **argv) -> int {
#if !defined(DOCTEST_CONFIG_DISABLE)
  doctest::Context doctest_context;
  doctest_context.applyCommandLine(argc, argv);

  const auto doctest_result = doctest_context.run();

  return doctest_result;
#else
  return app(argc, argv);
#endif
}

auto app(int /*argc*/, char ** /*argv*/) -> int {
  std::cout << "Hello, World!" << std::endl;
  config::read(".vanity.yml");
  return EXIT_SUCCESS;
}
