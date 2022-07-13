#pragma once

#include <filesystem>
#include <istream>
#include <map>
#include <string>
#include <vector>

namespace config {

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

enum class Inheritance {
  Public,
  Private,
  Interface,
};

struct Include {
  std::filesystem::path directory;
  Inheritance inheritance = Inheritance::Public;
};

struct PCH {
  std::filesystem::path path;
  Inheritance inheritance = Inheritance::Private;
};

struct Dependency {
  std::string name;
  Inheritance inheritance = Inheritance::Private;
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

struct ProjectDefinition {
  std::string value;
  Inheritance inheritance = Inheritance::Private;
};

struct CompileOption {
  std::string option;
  Inheritance inheritance = Inheritance::Private;
};

struct Project {
  ProjectType type;
  std::vector<std::filesystem::path> sources;
  std::vector<Include> includes;
  std::vector<PCH> pchs;
  std::vector<Dependency> dependencies;
  Settings settings;
  std::map<std::string, ProjectDefinition> definitions;
  std::vector<CompileOption> compile_options;
};

struct Case {
  std::string case_;
  Project project;
};

struct Switch {
  std::vector<Case> cases;
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
