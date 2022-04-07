#pragma once

#include "Y2KaoZ/Network/Visibility.hpp"
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/websocket/stream.hpp>
#include <gsl/pointers>
#include <memory>
#include <span>

namespace Y2KaoZ::Network::Tcp::Http::WebSocket {

Y2KAOZNETWORK_EXPORT class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
public:
  using HttpRequest = boost::beast::http::request<boost::beast::http::string_body>;
  using CloseReason = boost::beast::websocket::close_reason;

  class Handler {
  public:
    using Ptr = std::shared_ptr<Handler>;
    Handler() = default;
    Handler(const Handler&) = default;
    Handler(Handler&&) noexcept = default;
    auto operator=(const Handler&) -> Handler& = default;
    auto operator=(Handler&&) -> Handler& = default;
    virtual ~Handler() = default;

    virtual void onHandler(gsl::not_null<WebSocketSession*> session, Ptr oldHandler, Ptr newHandler) = 0;
    virtual auto onError(
        gsl::not_null<WebSocketSession*> session,
        const std::string& where,
        boost::system::error_code ec) -> bool = 0;
    virtual void onStart(gsl::not_null<WebSocketSession*> session, const WebSocketSession::HttpRequest& req) = 0;
    virtual void onClose(gsl::not_null<WebSocketSession*> session) = 0;
    virtual void onRead(gsl::not_null<WebSocketSession*> session, const std::string& message) = 0;
    virtual void onSend(gsl::not_null<WebSocketSession*> session, const std::string& message) = 0;
    virtual void onDestroy(gsl::not_null<WebSocketSession*> session) noexcept = 0;
  };

  WebSocketSession(const WebSocketSession&) = delete;
  WebSocketSession(WebSocketSession&&) noexcept = default;
  auto operator=(const WebSocketSession&) -> WebSocketSession& = delete;
  auto operator=(WebSocketSession&&) noexcept -> WebSocketSession& = delete;

  WebSocketSession(boost::asio::ip::tcp::socket&& socket, Handler::Ptr handler);

  void handler(const Handler::Ptr& handler);
  [[nodiscard]] auto handler() const noexcept -> const Handler::Ptr&;
  void start(const HttpRequest& req);
  void close(const CloseReason& reason = CloseReason{boost::beast::websocket::close_code::normal});
  void send(std::shared_ptr<const std::string> message);
  void send(std::string message);
  void send(std::span<const std::byte> bytes);
  auto isOpen() const noexcept -> bool;

  ~WebSocketSession() noexcept;

private:
  boost::beast::websocket::stream<boost::beast::tcp_stream> stream_;
  Handler::Ptr handler_;
  boost::beast::flat_buffer buffer_{};
  std::vector<std::shared_ptr<const std::string>> queue_{};
  auto onError(gsl::not_null<WebSocketSession*> session, const std::string& where, boost::system::error_code ec) const
      -> bool;
  void write(const std::shared_ptr<const std::string>& message);
  void read();
};

} // namespace Y2KaoZ::Network::Tcp::Http::WebSocket