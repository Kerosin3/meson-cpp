#pragma once

#include <cstddef>
#include <iostream>
#include <ranges>

#include <boost/program_options.hpp>

#include "finder.hpp"

namespace po = boost::program_options;
using namespace finder;

using std::cerr;
using std::cout;

bool
setupFinder(int ac, char** av);