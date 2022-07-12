#pragma once

#include <filesystem>
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
  Inheritance inheritance;
};

struct PCH {
  std::filesystem::path path;
  Inheritance inheritance;
};

struct Dependency {
  Inheritance inheritance;
};

struct Option {
  std::string description;
  std::string default_;
  std::string definition;
};

struct Settings {
  std::map<std::string, Option> options;
  std::map<std::string, std::string> sets;
};

struct ProjectDefinition {
  std::string value;
  Inheritance inheritance;
};

struct Project {
  ProjectType type;
  std::vector<std::filesystem::path> sources;
  std::vector<Include> includes;
  std::vector<PCH> pchs;
  std::map<std::string, Dependency> dependencies;
  Settings settings;
  std::map<std::string, ProjectDefinition> definitions;
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
  std::vector<std::filesystem::path> patches;
  std::map<std::string, std::string> overrides;
  SwitchProject project;
};

struct Target {
  Template target;
  std::vector<std::string> templates;
};

struct Config {
  std::string name;
  std::vector<std::string> include;
  std::map<std::string, Conditions> conditions;
  std::map<std::string, Template> templates;
  std::map<std::string, Target> targets;
  Settings settings;
  std::map<std::string, std::string> definitions;
};

} // namespace config
