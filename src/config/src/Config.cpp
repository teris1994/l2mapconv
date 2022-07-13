#include <config/Config.h>

#include "Parse.h"

#include <yaml-cpp/yaml.h>

#include <fstream>

namespace config {

auto read(const std::filesystem::path &path) -> Config {
  std::ifstream file{path};
  return parse(file);
}

} // namespace config
