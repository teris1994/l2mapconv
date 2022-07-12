#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace config {

struct Name {
  std::string name;
};

struct URL {
  std::string url;
};

struct Conditions {
  std::map<Name, std::string> conditions;
  std::string condition;
};

struct Repository {
  URL url;
  Name branch;
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
  Inheritance inheritance = Inheritance::Public;
};

struct PCH {
  std::filesystem::path path;
  Inheritance inheritance = Inheritance::Private;
};

struct Dependency {
  Name name;
  Inheritance inheritance = Inheritance::Private;
};

struct Option {
  std::string description;
  std::string default_;
  Name definition;
};

struct Settings {
  std::map<Name, Option> options;
  std::map<Name, std::string> variables;
};

struct ProjectDefinition {
  std::string value;
  Inheritance inheritance = Inheritance::Private;
};

struct Project {
  ProjectType type;
  std::vector<std::filesystem::path> sources;
  std::vector<Include> includes;
  std::vector<PCH> pchs;
  std::vector<Dependency> dependencies;
  Settings settings;
  std::map<Name, ProjectDefinition> definitions;
};

struct Case {
  Name case_;
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
  std::map<Name, std::string> overrides;
  SwitchProject project;
};

struct Target {
  Template target;
  std::vector<Name> templates;
};

struct Config {
  Name name;
  std::vector<std::filesystem::path> include;
  std::map<Name, Conditions> conditions;
  std::map<Name, Template> templates;
  std::map<Name, Target> targets;
  Settings settings;
  std::map<Name, std::string> definitions;
};

} // namespace config
