#pragma once

#include "Y2KaoZ/Network/Tcp/Http/HttpSession.hpp"
#include "Y2KaoZ/Network/Tcp/Http/WebSocket/WebSocketSession.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"

namespace Y2KaoZ::Network::Tcp::Http::WebSocket {

Y2KAOZNETWORK_EXPORT class HttpSessionHandler : public Y2KaoZ::Network::Tcp::Http::HttpSession::Handler {
public:
  explicit HttpSessionHandler(WebSocketSession::Handler::Ptr handler);
  void onHandler(gsl::not_null<HttpSession*> session, Handler::Ptr oldHandler, Handler::Ptr newHandler) final;
  auto onError(gsl::not_null<HttpSession*> session, const std::string& where, const boost::system::error_code& ec)
      -> bool final;
  void onStart(gsl::not_null<HttpSession*> session) final;
  void onResponse(gsl::not_null<HttpSession*> session) final;
  void onShutdown(gsl::not_null<HttpSession*> session) final;
  void onUpgrade(gsl::not_null<HttpSession*> session, boost::asio::ip::tcp::socket&& socket, HttpSession::Request&& req)
      final;

private:
  WebSocketSession::Handler::Ptr handler_;
};

} // namespace Y2KaoZ::Network::Tcp::Http::WebSocket