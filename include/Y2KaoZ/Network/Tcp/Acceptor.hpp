#pragma once

#include "Y2KaoZ/Network/Visibility.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <gsl/pointers>
#include <memory>

namespace Y2KaoZ::Network::Tcp {

Y2KAOZNETWORK_EXPORT class Acceptor : public std::enable_shared_from_this<Acceptor> {
public:
  class Handler {
  public:
    using Ptr = gsl::shared_ptr<Handler>;

    Handler() = default;
    Handler(const Handler&) = default;
    Handler(Handler&&) noexcept = default;
    auto operator=(const Handler&) -> Handler& = default;
    auto operator=(Handler&&) -> Handler& = default;
    virtual ~Handler() = default;

    virtual void onHandler(gsl::not_null<Acceptor*> acceptor, Ptr oldHandler, Ptr newHandler) = 0;
    virtual auto onError(
      gsl::not_null<Acceptor*> acceptor,
      const std::string& where,
      const boost::system::error_code& ec) -> bool = 0;
    virtual void onAccept(gsl::not_null<Acceptor*> acceptor, boost::asio::ip::tcp::socket&& socket) = 0;
  };

  Acceptor(boost::asio::io_context& ioContext, const boost::asio::ip::tcp::endpoint& endpoint, Handler::Ptr handler);
  void handler(const Handler::Ptr& handler);
  [[nodiscard]] auto handler() const noexcept -> const Handler::Ptr&;
  void accept();

private:
  boost::asio::io_context& ioContext_;
  boost::asio::ip::tcp::acceptor acceptor_;
  Handler::Ptr handler_;
};

} // namespace Y2KaoZ::Network::Tcp