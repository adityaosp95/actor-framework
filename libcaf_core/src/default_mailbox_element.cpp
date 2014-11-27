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

#include "caf/detail/default_mailbox_element.hpp"

namespace caf {
namespace detail {

default_mailbox_element::default_mailbox_element(actor_addr&& sender,
                                                 message_id id,
                                                 message&& content)
    : mailbox_element(std::move(sender), id),
      m_msg(std::move(content)) {
  // nop
}


void default_mailbox_element::dispose_mailbox_element() {
  delete this;
}

message& default_mailbox_element::msg() {
  return m_msg;
}

} // namespace detail
} // namespace caf
