/******************************************************************************
 *                       ____    _    _____                                   *
 *                      / ___|  / \  |  ___|    C++                           *
 *                     | |     / _ \ | |_       Actor                         *
 *                     | |___ / ___ \|  _|      Framework                     *
 *                      \____/_/   \_|_|                                      *
 *                                                                            *
 * Copyright (C) 2011 - 2014                                                  *
 * Dominik Charousset <dominik.charousset (at) haw-hamburg.de>                *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License or  *
 * (at your option) under the terms and conditions of the Boost Software      *
 * License 1.0. See accompanying files LICENSE and LICENSE_ALTERNATIVE.       *
 *                                                                            *
 * If you did not receive a copy of the license files, see                    *
 * http://opensource.org/licenses/BSD-3-Clause and                            *
 * http://www.boost.org/LICENSE_1_0.txt.                                      *
 ******************************************************************************/

#ifndef CAF_DETAIL_SCOPE_GUARD_HPP
#define CAF_DETAIL_SCOPE_GUARD_HPP

#include <utility>

namespace caf {
namespace detail {

/**
 * A lightweight scope guard implementation.
 */
template <class Fun>
class scope_guard {

  scope_guard() = delete;
  scope_guard(const scope_guard&) = delete;
  scope_guard& operator=(const scope_guard&) = delete;

 public:

  scope_guard(Fun f) : m_fun(std::move(f)), m_enabled(true) {}

  scope_guard(scope_guard&& other)
      : m_fun(std::move(other.m_fun)), m_enabled(other.m_enabled) {
    other.m_enabled = false;
  }

  ~scope_guard() {
    if (m_enabled) m_fun();
  }

  /**
   * Disables this guard, i.e., the guard does not
   * run its cleanup code as it goes out of scope.
   */
  inline void disable() { m_enabled = false; }

 private:

  Fun m_fun;
  bool m_enabled;

};

/**
 * Creates a guard that executes `f` as soon as it goes out of scope.
 * @relates scope_guard
 */
template <class Fun>
scope_guard<Fun> make_scope_guard(Fun f) {
  return {std::move(f)};
}

} // namespace detail
} // namespace caf

#endif // CAF_DETAIL_SCOPE_GUARD_HPP
