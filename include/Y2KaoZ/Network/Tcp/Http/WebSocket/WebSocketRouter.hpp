#pragma once

#include "WebSocketSession.hpp"

namespace Y2KaoZ::Network::Tcp::Http::WebSocket {

class WebSocketRouter : public WebSocketSession::Handler {
public:
  using WebSocketSessionPtr = gsl::not_null<WebSocketSession*>;
  using HandlerFactory = std::function<std::shared_ptr<WebSocketSession::Handler>(const std::string& target)>;
  explicit WebSocketRouter(HandlerFactory handlerFactory);
  void onHandler(WebSocketSessionPtr session, Ptr oldHandler, Ptr newHandler) final;
  auto onError(WebSocketSessionPtr session, const std::string& where, boost::system::error_code ec) -> bool final;
  void onStart(WebSocketSessionPtr session, const WebSocketSession::HttpRequest& req) final;
  void onClose(WebSocketSessionPtr session) final;
  void onRead(WebSocketSessionPtr session, const std::string& /* message */) final;
  void onSend(WebSocketSessionPtr session, const std::string& /* message */) final;
  void onDestroy(WebSocketSessionPtr session) noexcept final;

private:
  HandlerFactory handlerFactory_;
};

} // namespace Y2KaoZ::Network::Tcp::Http::WebSocket