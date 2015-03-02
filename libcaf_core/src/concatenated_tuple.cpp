/******************************************************************************
 *                       ____    _    _____                                   *
 *                      / ___|  / \  |  ___|    C++                           *
 *                     | |     / _ \ | |_       Actor                         *
 *                     | |___ / ___ \|  _|      Framework                     *
 *                      \____/_/   \_|_|                                      *
 *                                                                            *
 * Copyright (C) 2011 - 2015                                                  *
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

#include "caf/message.hpp"
#include "caf/detail/concatenated_tuple.hpp"

namespace caf {
namespace detail {

concatenated_tuple::concatenated_tuple(std::initializer_list<message*> msgs)
  : m_messages(msgs) {
  // nop
}

auto concatenated_tuple::make(std::initializer_list<message*> msgs) -> pointer {
  return pointer{new concatenated_tuple(msgs)};
}

void* concatenated_tuple::mutable_at(size_t pos) {
  CAF_REQUIRE(pos < size());
  size_t idx = 0;
  for (auto m : m_messages) {
    if (idx + m->size() < pos) {
      idx += m->size();
    } else {
      return m->mutable_at(pos - idx);
    }
  }
  return nullptr;
}

size_t concatenated_tuple::size() const {
  size_t size = 0;
  auto size_of = [&](message* m) {
    size += m->size();
  };
  std::for_each(m_messages.begin(), m_messages.end(), size_of);
  return size;
}

concatenated_tuple* concatenated_tuple::copy() const {
  return new concatenated_tuple(*this);
}

const void* concatenated_tuple::at(size_t pos) const {
  CAF_REQUIRE(pos < size());
  size_t idx = 0;
  for (auto& m : m_messages) {
    if (idx + m->size() < pos) {
      idx += m->size();
    } else {
      return m->at(pos - idx);
    }
  }
  return nullptr;
}

bool concatenated_tuple::match_element(size_t pos, uint16_t typenr,
                                       const std::type_info* rtti) const {
  for (auto m : m_messages) {
    if (m->match_element(pos, typenr, rtti)) {
      return true;
    }
  }
  return false;
}

uint32_t concatenated_tuple::type_token() const {
  return m_type_token;
}

const char* concatenated_tuple::uniform_name_at(size_t pos) const {
  size_t idx = 0;
  for (auto m : m_messages) {
    if (idx + m->size() < pos) {
      idx += m->size();
    } else {
      return m->uniform_name_at(pos - idx);
    }
  }
  return nullptr;
}

uint16_t concatenated_tuple::type_nr_at(size_t pos) const {
  size_t idx = 0;
  for (auto m : m_messages) {
    if (idx + m->size() < pos) {
      idx += m->size();
    } else {
      return m->vals()->type_nr_at(pos - idx);
    }
  }
  return 0;
}

} // namespace detail
} // namespace caf
