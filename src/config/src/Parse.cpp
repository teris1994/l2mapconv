#include "Parse.h"

#include <yaml-cpp/yaml.h>

namespace config {

void operator>>(const YAML::Node &node, std::string &string) {
  string = node.as<std::string>();
}

auto parse(std::istream &input) -> Config {
  Config config;

  auto node = YAML::Load(input);

  return config;
}

} // namespace config
