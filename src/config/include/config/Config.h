#pragma once

#include <filesystem>
#include <istream>
#include <map>
#include <string>
#include <vector>

namespace config {

enum class Inheritance {
  Public,
  Private,
  Interface,
};

struct Conditions {
  std::map<std::string, std::string> conditions;
  std::string condition;
};

struct Repository {
  std::string url;
  std::string branch;
  std::filesystem::path subdirectory;
  std::vector<std::filesystem::path> patches;
};

enum class ProjectType {
  Executable,
  Library,
  Interface,
};

struct Option {
  std::string description;
  std::string default_;
  std::string definition;
};

struct Settings {
  std::map<std::string, Option> options;
  std::map<std::string, std::string> variables;
};

template <typename T, Inheritance> struct InheritanceMaps {
  std::map<std::string, T> public_;
  std::map<std::string, T> private_;
  std::map<std::string, T> interface;
};

template <typename T, Inheritance> struct InheritanceLists {
  std::vector<T> public_;
  std::vector<T> private_;
  std::vector<T> interface;
};

struct Project {
  ProjectType type = ProjectType::Executable;
  std::vector<std::filesystem::path> sources;
  InheritanceLists<std::filesystem::path, Inheritance::Public> includes;
  InheritanceLists<std::filesystem::path, Inheritance::Private> pchs;
  InheritanceLists<std::string, Inheritance::Private> dependencies;
  Settings settings;
  InheritanceLists<std::string, Inheritance::Private> definitions;
  InheritanceMaps<std::string, Inheritance::Private> compile_options;
};

struct SwitchCase {
  std::string case_;
  Project project;
};

struct Switch {
  std::vector<SwitchCase> cases;
};

struct SwitchProject {
  Project project;
  Switch switch_;
};

struct Template {
  std::filesystem::path path;
  Repository repository;
  std::map<std::string, std::string> overrides;
  SwitchProject project;
};

struct Target {
  Template target;
  std::vector<std::string> templates;
};

struct Config {
  std::string name;
  std::vector<std::filesystem::path> includes;
  std::map<std::string, Conditions> conditions;
  std::map<std::string, Template> templates;
  std::map<std::string, Target> targets;
  Settings settings;
  std::map<std::string, std::string> definitions;
  std::vector<std::string> compile_options;
};

auto read(const std::filesystem::path &path) -> Config;

} // namespace config
