#include <cstdlib>

#include "framework/unit.hpp"

#include "caf/all.hpp"

int main(int argc, char* argv[]) {
  using namespace caf;

  bool colorize = true;
  std::string log_file;
  int verbosity_console = 3;
  int verbosity_file = 3;
  int max_runtime = 10;
  std::regex suites{".*"};
  std::regex not_suites;
  std::regex tests{".*"};
  std::regex not_tests;

  message_handler parser = (
    on("-n") >> [&] {
      colorize = false;
    },
    on("-l", arg_match) >> [&](std::string const& file_name) {
      log_file = file_name;
    },
    on("-v", arg_match) >> [&](std::string const& n) {
      verbosity_console = std::strtol(n.c_str(), nullptr, 10);
    },
    on("-V", arg_match) >> [&](std::string const& n) {
      verbosity_file = std::strtol(n.c_str(), nullptr, 10);
    },
    on("-r", arg_match) >> [&](std::string const& n) {
      max_runtime = std::strtol(n.c_str(), nullptr, 10);
    },
    on("-s", arg_match) >> [&](std::string const& str) {
      suites = str;
    },
    on("-S", arg_match) >> [&](std::string const& str) {
      not_suites = str;
    },
    on("-t", arg_match) >> [&](std::string const& str) {
      tests = str;
    },
    on("-T", arg_match) >> [&](std::string const& str) {
      not_tests = str;
    },
    others() >> [&] {
      std::cerr << "invalid command line argument" << std::endl;
      std::exit(1);
    }
  );
  auto flags = "nlvVrsStT";
  for (int i = 1; i < argc; ++i) {
    message_builder builder;
    std::string arg{argv[i]};
    if (arg.empty() || arg[0] != '-') {
      std::cerr << "invalid option: " << arg << std::endl;
      return 1;
    }
    builder.append(std::move(arg));
    if (i + 1 < argc) {
      std::string next{argv[i + 1]};
      if (next.empty() || next[0] != '-' || next.find_first_of(flags) != 1) {
        builder.append(std::move(next));
        ++i;
      }
    }
    builder.apply(parser);
  }

  auto result = unit::engine::run(colorize, log_file, verbosity_console,
                                  verbosity_file, max_runtime, suites,
                                  not_suites, tests, not_tests);
  return result ? 0 : 1;
}
