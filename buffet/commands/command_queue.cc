// Copyright 2014 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "buffet/commands/command_queue.h"

namespace buffet {

std::string CommandQueue::Add(std::unique_ptr<const CommandInstance> instance) {
  std::string id = std::to_string(++next_id_);
  auto pair = map_.insert(std::make_pair(id, std::move(instance)));
  LOG_IF(FATAL, !pair.second) << "Command with ID '" << id
                              << "' is already in the queue";
  return id;
}

std::unique_ptr<const CommandInstance> CommandQueue::Remove(
    const std::string& id) {
  std::unique_ptr<const CommandInstance> instance;
  auto p = map_.find(id);
  if (p != map_.end()) {
    instance = std::move(p->second);
    map_.erase(p);
  }
  return instance;
}

const CommandInstance* CommandQueue::Find(const std::string& id) const {
  auto p = map_.find(id);
  return (p != map_.end()) ? p->second.get() : nullptr;
}

}  // namespace buffet
