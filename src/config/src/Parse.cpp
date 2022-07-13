#include "Parse.h"

#include <doctest/doctest.h>
#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <string>
#include <type_traits>

namespace config {

auto parse(std::istream &input) -> Config {
  return YAML::Load(input).as<Config>();
}

} // namespace config

template <typename Test, template <typename...> class Ref>
struct is_specialization : std::false_type {};

template <template <typename...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

template <typename T>
auto operator>>(const YAML::Node &node, T &output) -> const YAML::Node & {
  if (!node.IsDefined()) {
    output = T{};
    return node;
  }

  if constexpr (is_specialization<T, std::vector>::value) {
    if (node.IsScalar()) {
      output.push_back(node.as<typename T::value_type>());
      return node;
    }
  }

  output = node.as<T>();
  return node;
}

namespace YAML {

template <> struct convert<std::filesystem::path> {
  static auto decode(const Node &node, std::filesystem::path &path) -> bool {
    std::string path_string;
    node >> path_string;
    path = path_string;
    return true;
  }
};

template <> struct convert<config::Conditions> {
  static auto decode(const Node &node, config::Conditions &conditions) -> bool {
    if (node.IsMap()) {
      node >> conditions.conditions;
      return true;
    }

    if (node.IsScalar()) {
      node >> conditions.condition;
      return true;
    }

    return false;
  }
};

template <> struct convert<config::Repository> {
  static auto decode(const Node &node, config::Repository &repository) -> bool {
    if (node.IsScalar()) {
      node >> repository.url;
      return true;
    }

    if (node.IsMap()) {
      node["url"] >> repository.url;
      node["branch"] >> repository.branch;
      node["subdirectory"] >> repository.subdirectory;
      node["patches"] >> repository.patches;
      return true;
    }

    return false;
  }
};

template <> struct convert<config::ProjectType> {
  static auto decode(const Node &node, config::ProjectType &project_type)
      -> bool {

    std::string project_type_string;
    node >> project_type_string;

    if (project_type_string == "executable") {
      project_type = config::ProjectType::Executable;
      return true;
    }

    if (project_type_string == "library") {
      project_type = config::ProjectType::Library;
      return true;
    }

    if (project_type_string == "interface") {
      project_type = config::ProjectType::Interface;
      return true;
    }

    return false;
  }
};

template <> struct convert<config::Option> {
  static auto decode(const Node &node, config::Option &option) -> bool {
    node["description"] >> option.description;
    node["default"] >> option.default_;
    node["definition"] >> option.definition;
    return true;
  }
};

template <> struct convert<config::Settings> {
  static auto decode(const Node &node, config::Settings &settings) -> bool {
    node["options"] >> settings.options;
    node["variables"] >> settings.variables;
    return true;
  }
};

template <typename T, config::Inheritance inheritance>
struct convert<config::InheritanceLists<T, inheritance>> {

  static auto decode(const Node &node,
                     config::InheritanceLists<T, inheritance> &lists) -> bool {

    if (node.IsScalar() || node.IsSequence()) {
      if constexpr (inheritance == config::Inheritance::Public) {
        node >> lists.public_;
      } else if constexpr (inheritance == config::Inheritance::Private) {
        node >> lists.private_;
      } else if constexpr (inheritance == config::Inheritance::Interface) {
        node >> lists.interface;
      }

      return true;
    }

    if (node.IsMap()) {
      node["public"] >> lists.public_;
      node["private"] >> lists.private_;
      node["interface"] >> lists.interface;
      return true;
    }

    return false;
  }
};

template <typename T, config::Inheritance inheritance>
struct convert<config::InheritanceMaps<T, inheritance>> {

  static auto decode(const Node &node,
                     config::InheritanceMaps<T, inheritance> &maps) -> bool {

    if (node.IsMap()) {
      if (node["public"] || node["private"] || node["interface"]) {
        node["public"] >> maps.public_;
        node["private"] >> maps.private_;
        node["interface"] >> maps.interface;
      } else {
        if constexpr (inheritance == config::Inheritance::Public) {
          node >> maps.public_;
        } else if constexpr (inheritance == config::Inheritance::Private) {
          node >> maps.private_;
        } else if constexpr (inheritance == config::Inheritance::Interface) {
          node >> maps.interface;
        }
      }

      return true;
    }

    return false;
  }
};

template <> struct convert<config::Project> {
  static auto decode(const Node &node, config::Project &project) -> bool {
    node >> project.settings;
    node["type"] >> project.type;
    node["sources"] >> project.sources;
    node["includes"] >> project.includes;
    node["pchs"] >> project.pchs;
    node["dependencies"] >> project.dependencies;
    node["definitions"] >> project.definitions;
    node["compile-options"] >> project.compile_options;
    return true;
  }
};

template <> struct convert<config::SwitchCase> {
  static auto decode(const Node &node, config::SwitchCase &switch_case)
      -> bool {

    node["case"] >> switch_case.case_;
    node >> switch_case.project;
    return true;
  }
};

template <> struct convert<config::Switch> {
  static auto decode(const Node &node, config::Switch &switch_) -> bool {
    node >> switch_.cases;
    return true;
  }
};

template <> struct convert<config::SwitchProject> {
  static auto decode(const Node &node, config::SwitchProject &switch_project)
      -> bool {

    node >> switch_project.project;
    node["switch"] >> switch_project.switch_;
    return true;
  }
};

template <> struct convert<config::Template> {
  static auto decode(const Node &node, config::Template &template_) -> bool {
    node["path"] >> template_.path;
    node["repository"] >> template_.repository;
    node["overrides"] >> template_.overrides;
    node["project"] >> template_.project;
    return true;
  }
};

template <> struct convert<config::Target> {
  static auto decode(const Node &node, config::Target &target) -> bool {
    node >> target.target;
    node["templates"] >> target.templates;
    return true;
  }
};

template <> struct convert<config::Config> {
  static auto decode(const Node &node, config::Config &config) -> bool {
    node["name"] >> config.name;
    node["include"] >> config.includes;
    node["conditions"] >> config.conditions;
    node["templates"] >> config.templates;
    node["targets"] >> config.targets;
    return true;
  }
};

} // namespace YAML

namespace tests {

TEST_CASE("test parsing name") {
  const auto node = YAML::Load(R"YAML(
    name: some name
  )YAML");

  config::Config config;
  node >> config;
  CHECK(config.name == "some name");
}

TEST_CASE("test parsing includes") {
  SUBCASE("includes as string") {
    const auto node = YAML::Load(R"YAML(
      include: file.yml
    )YAML");

    config::Config config;
    node >> config;
    CHECK(config.includes.size() == 1);
    CHECK(config.includes[0] == "file.yml");
  }

  SUBCASE("includes as list") {
    const auto node = YAML::Load(R"YAML(
      include: [file1.yml, file2.yml]
    )YAML");

    config::Config config;
    node >> config;
    CHECK(config.includes.size() == 2);
    CHECK(config.includes[0] == "file1.yml");
    CHECK(config.includes[1] == "file2.yml");
  }
}

TEST_CASE("test parsing conditions") {
  SUBCASE("conditions as string") {
    const auto node = YAML::Load(R"YAML(
      conditions:
        condition-key: condition-value
    )YAML");

    config::Config config;
    node >> config;
    CHECK(config.conditions.size() == 1);
    CHECK(config.conditions["condition-key"].conditions.empty());
    CHECK(config.conditions["condition-key"].condition == "condition-value");
  }

  SUBCASE("conditions as map") {
    const auto node = YAML::Load(R"YAML(
      conditions:
        condition1:
          key1: value1
          key2: value2
        condition2:
          key3: value3
    )YAML");

    config::Config config;
    node >> config;

    CHECK(config.conditions.size() == 2);
    CHECK(config.conditions["condition1"].condition.empty());
    CHECK(config.conditions["condition1"].conditions.size() == 2);
    CHECK(config.conditions["condition1"].conditions["key1"] == "value1");
    CHECK(config.conditions["condition1"].conditions["key2"] == "value2");
    CHECK(config.conditions["condition2"].conditions.size() == 1);
    CHECK(config.conditions["condition2"].conditions["key3"] == "value3");
  }
}

} // namespace tests
