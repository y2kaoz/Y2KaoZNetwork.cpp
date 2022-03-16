#include "Y2KaoZ/Network/Tcp/Acceptor.hpp"
#include <boost/asio/strand.hpp>

namespace Y2KaoZ::Network::Tcp {

Acceptor::Acceptor(
  boost::asio::io_context& ioContext,
  const boost::asio::ip::tcp::endpoint& endpoint,
  Handler::Ptr handler)
  : ioContext_(ioContext)
  , acceptor_{ioContext_, endpoint, true}
  , handler_{std::move(handler)} {
  if (handler_ == nullptr) {
    throw std::runtime_error("Invalid null handler.");
  }
}

void Acceptor::handler(const Handler::Ptr& handler) {
  if (handler == nullptr) {
    throw std::runtime_error("Invalid null handler.");
  }
  boost::asio::post(ioContext_.get_executor(), [self = shared_from_this(), newHandler = handler]() {
    auto oldHandler = self->handler_;
    oldHandler->onHandler(self.get(), oldHandler, newHandler);
    newHandler->onHandler(self.get(), oldHandler, newHandler);
    self->handler_ = newHandler;
  });
}

auto Acceptor::handler() const noexcept -> const Handler::Ptr& {
  return handler_;
}

void Acceptor::accept() {
  acceptor_.async_accept(
    boost::asio::make_strand(ioContext_), /// <-- The new connection gets its own strand
    [self = shared_from_this()](const boost::system::error_code& ec, boost::asio::ip::tcp::socket&& socket) {
      if (ec && self->handler_->onError(self.get(), "accept", ec)) {
        return;
      }
      self->handler_->onAccept(self.get(), std::move(socket));
      self->accept();
    });
}

} // namespace Y2KaoZ::Network::Tcp