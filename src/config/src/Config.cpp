#include <config/Config.h>

#include <yaml-cpp/yaml.h>

namespace config {

void operator>>(const YAML::Node & /*node*/, Config & /*config*/) {}

auto parse(const std::istream & /*input*/) -> Config {
  Config config;

  return config;
}

} // namespace config
