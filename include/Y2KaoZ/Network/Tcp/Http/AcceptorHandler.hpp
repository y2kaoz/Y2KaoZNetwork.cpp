#pragma once

#include "Y2KaoZ/Network/Tcp/Acceptor.hpp"
#include "Y2KaoZ/Network/Tcp/Http/HttpSession.hpp"
#include "Y2KaoZ/Network/Visibility.hpp"

namespace Y2KaoZ::Network::Tcp::Http {

Y2KAOZNETWORK_EXPORT class AcceptorHandler : public Y2KaoZ::Network::Tcp::Acceptor::Handler {
public:
  AcceptorHandler(const std::filesystem::path& docRoot, HttpSession::Handler::Ptr handler);
  void onHandler(gsl::not_null<Acceptor*> acceptor, Handler::Ptr oldHandler, Handler::Ptr newHandler) final;
  auto onError(gsl::not_null<Acceptor*> acceptor, const std::string& where, const boost::system::error_code& ec)
      -> bool final;
  void onAccept(gsl::not_null<Acceptor*> acceptor, boost::asio::ip::tcp::socket&& socket) final;

private:
  std::shared_ptr<std::filesystem::path> docRoot_;
  HttpSession::Handler::Ptr handler_;
};

} // namespace Y2KaoZ::Network::Tcp::Http