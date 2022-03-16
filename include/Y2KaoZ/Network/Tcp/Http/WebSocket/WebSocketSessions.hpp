#pragma once

#include "Y2KaoZ/Network/Tcp/Http/WebSocket/WebSocketSession.hpp"
#include <unordered_set>

namespace Y2KaoZ::Network::Tcp::Http::WebSocket {

class WebSocketSessions {
public:
  auto insert(gsl::not_null<WebSocketSession*> session) -> bool;
  auto erase(gsl::not_null<WebSocketSession*> session) -> bool;
  auto sessions() -> std::vector<std::weak_ptr<WebSocketSession>>;

private:
  // This mutex synchronizes all access to sessions_
  std::mutex mutex_{};
  // Keep a list of all the connected clients
  std::unordered_set<WebSocketSession*> sessions_{};
};

} // namespace Y2KaoZ::Network::Tcp::Http::WebSocket