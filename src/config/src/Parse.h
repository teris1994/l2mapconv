#pragma once

#include <config/Config.h>

#include <istream>

namespace config {

auto parse(std::istream &input) -> Config;

} // namespace config
