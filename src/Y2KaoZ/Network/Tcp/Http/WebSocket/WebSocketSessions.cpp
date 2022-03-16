#include "Y2KaoZ/Network/Tcp/Http/WebSocket/WebSocketSessions.hpp"

namespace Y2KaoZ::Network::Tcp::Http::WebSocket {

auto WebSocketSessions::insert(gsl::not_null<WebSocketSession*> session) -> bool {
  std::lock_guard<std::mutex> lock(mutex_);
  return sessions_.insert(session).second;
}

auto WebSocketSessions::erase(gsl::not_null<WebSocketSession*> session) -> bool {
  std::lock_guard<std::mutex> lock(mutex_);
  return sessions_.erase(session) != 0;
}

auto WebSocketSessions::sessions() -> std::vector<std::weak_ptr<WebSocketSession>> {
  std::vector<std::weak_ptr<WebSocketSession>> v;
  std::lock_guard<std::mutex> lock(mutex_);
  v.reserve(sessions_.size());
  for (auto* p : sessions_) {
    v.emplace_back(p->weak_from_this());
  }
  return v;
}

} // namespace Y2KaoZ::Network::Tcp::Http::WebSocket