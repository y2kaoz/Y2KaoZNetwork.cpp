#include "Y2KaoZ/Network/Tcp/Http/WebSocket/WebSocketSession.hpp"
#include <boost/beast/core/buffers_to_string.hpp>

namespace Y2KaoZ::Network::Tcp::Http::WebSocket {

WebSocketSession::WebSocketSession(boost::asio::ip::tcp::socket&& socket, Handler::Ptr handler)
    : stream_(std::move(socket))
    , handler_(std::move(handler)) {
  if (handler_ == nullptr) {
    throw std::runtime_error("Invalid null handler.");
  }
}

void WebSocketSession::handler(const Handler::Ptr& handler) {
  if (handler == nullptr) {
    throw std::runtime_error("Invalid null handler.");
  }
  auto oldHandler = handler_;
  oldHandler->onHandler(this, oldHandler, handler);
  handler->onHandler(this, oldHandler, handler);
  handler_ = handler;
}

auto WebSocketSession::handler() const noexcept -> const Handler::Ptr& {
  return handler_;
}

void WebSocketSession::start(const HttpRequest& req) {
  if (isOpen()) {
    return;
  }
  // Set suggested timeout settings for the websocket
  stream_.set_option(boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::server));

  // Set a decorator to change the Server of the handshake
  stream_.set_option(boost::beast::websocket::stream_base::decorator([](boost::beast::websocket::response_type& res) {
    res.set(boost::beast::http::field::server, "websocket-server");
  }));

  // Accept the websocket handshake
  stream_.async_accept(req, [self = shared_from_this(), req](boost::system::error_code ec) {
    if (ec) {
      if (self->onError(self.get(), "start", ec)) {
        return;
      }
    }
    self->handler_->onStart(self.get(), req);
    self->read();
  });
}

void WebSocketSession::close(const CloseReason& reason) {
  if (!isOpen()) {
    return;
  }
  stream_.async_close(reason, [self = shared_from_this()](boost::system::error_code ec) {
    if (ec) {
      if (self->onError(self.get(), "close", ec)) {
        return;
      }
    }
    self->handler_->onClose(self.get());
  });
}

void WebSocketSession::send(std::shared_ptr<const std::string> message) {
  if (!isOpen()) {
    return;
  }
  // Post our work to the strand, this ensures that the members of `this` will not be accessed concurrently.
  boost::asio::post(stream_.get_executor(), [self = shared_from_this(), message = std::move(message)]() {
    // Always add to queue
    self->queue_.push_back(message);
    // Are we already writing?
    if (self->queue_.size() > 1) {
      return;
    }
    // We are not currently writing, so send this immediately
    self->write(self->queue_.front());
  });
}

void WebSocketSession::send(std::string message) {
  send(std::make_shared<const std::string>(std::move(message)));
}

void WebSocketSession::send(std::span<const std::byte> bytes) {
  std::string message(bytes.size(), '\0');
  std::memcpy(message.data(), bytes.data(), message.size());
  send(std::make_shared<const std::string>(std::move(message)));
}

auto WebSocketSession::isOpen() const noexcept -> bool {
  return stream_.is_open();
}

WebSocketSession::~WebSocketSession() noexcept {
  handler_->onDestroy(this);
}

auto WebSocketSession::onError(
    gsl::not_null<WebSocketSession*> session,
    const std::string& where,
    boost::system::error_code ec) const -> bool {
  if (ec == boost::asio::error::operation_aborted || ec == boost::beast::websocket::error::closed) {
    return true;
  }
  return handler_->onError(session, where, ec);
}

void WebSocketSession::write(const std::shared_ptr<const std::string>& message) {
  if (!isOpen()) {
    return;
  }
  stream_.async_write(
      boost::asio::buffer(*message),
      [self = shared_from_this(), message](boost::system::error_code ec, std::size_t /*bytes*/) {
        if (ec) {
          if (self->onError(self.get(), "send", ec)) {
            return;
          }
        }
        self->handler_->onSend(self.get(), *self->queue_.front());

        // Remove the string from the queue
        self->queue_.erase(self->queue_.begin());

        // Send the next message if any
        if (!self->queue_.empty()) {
          self->write(self->queue_.front());
        }
      });
}

void WebSocketSession::read() {
  if (!isOpen()) {
    return;
  }
  stream_.async_read(buffer_, [self = shared_from_this()](boost::system::error_code ec, std::size_t /*bytes*/) {
    if (ec) {
      if (self->onError(self.get(), "read", ec)) {
        return;
      }
    }
    self->handler_->onRead(self.get(), boost::beast::buffers_to_string(self->buffer_.data()));
    self->buffer_.consume(self->buffer_.size());
    self->read();
  });
}

} // namespace Y2KaoZ::Network::Tcp::Http::WebSocket