#include "Y2KaoZ/Network/Tcp/Http/AcceptorHandler.hpp"
#include <boost/log/trivial.hpp>

namespace Y2KaoZ::Network::Tcp::Http {

AcceptorHandler::AcceptorHandler(const std::filesystem::path& docRoot, HttpSession::Handler::Ptr handler)
  : docRoot_(std::make_shared<std::filesystem::path>(std::filesystem::canonical(docRoot)))
  , handler_(std::move(handler)) {
}

void AcceptorHandler::onHandler(gsl::not_null<Acceptor*> acceptor, Handler::Ptr oldHandler, Handler::Ptr newHandler) {
  BOOST_LOG_TRIVIAL(trace) << "Tcp Acceptor '" << acceptor << "' switched handler from '" << oldHandler << "' to '"
                           << newHandler << "'";
}

auto AcceptorHandler::onError(
  gsl::not_null<Acceptor*> acceptor,
  const std::string& where,
  const boost::system::error_code& ec) -> bool {
  BOOST_LOG_TRIVIAL(error) << "Tcp Acceptor '" << acceptor << "' error on " << where << ": '" << ec.message() << "'";
  return true;
}

void AcceptorHandler::onAccept(gsl::not_null<Acceptor*> acceptor, boost::asio::ip::tcp::socket&& socket) {
  BOOST_LOG_TRIVIAL(trace) << "Tcp Acceptor '" << acceptor << "' accepted a new connection from: '"
                           << socket.remote_endpoint() << "'";
  std::make_shared<HttpSession>(std::move(socket), docRoot_, handler_)->start();
}

} // namespace Y2KaoZ::Network::Tcp::Http