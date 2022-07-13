#include "Parse.h"

#include <doctest/doctest.h>
#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <string>

template <typename T>
auto operator>>(const YAML::Node &node, T &output) -> const YAML::Node & {
  output = node.as<T>();
  return node;
}

template <typename T>
auto operator>=(const YAML::Node &node, T &output) -> const YAML::Node & {
  if (node.IsDefined()) {
    output = node.as<T>();
  }

  return node;
}

namespace YAML {

template <> struct convert<std::filesystem::path> {
  static auto decode(const Node &node, std::filesystem::path &path) -> bool {
    node >> path;
    return true;
  }
};

template <> struct convert<config::Conditions> {
  static auto decode(const Node &node, config::Conditions &conditions) -> bool {
    if (node.IsSequence()) {
      node >> conditions.conditions;
    } else {
      node >> conditions.condition;
    }

    return true;
  }
};

template <> struct convert<config::Config> {
  static auto decode(const Node &node, config::Config &config) -> bool {
    node["name"] >> config.name;
    node["include"] >= config.includes;
    node["conditions"] >= config.conditions;
    return true;
  }
};

} // namespace YAML

namespace config {

auto parse(std::istream &input) -> Config {
  return YAML::Load(input).as<Config>();
}

} // namespace config

TEST_CASE("TEST") {
  auto node = YAML::Load("x: 123");
  std::string str;
  node["x"] >> str;
  CHECK(str == "123");
}
