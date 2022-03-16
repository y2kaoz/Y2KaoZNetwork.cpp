#include "Y2KaoZ/Network/Tcp/Http/WebSocket/HttpSessionHandler.hpp"

#include <boost/log/trivial.hpp>

namespace Y2KaoZ::Network::Tcp::Http::WebSocket {

HttpSessionHandler::HttpSessionHandler(WebSocketSession::Handler::Ptr handler) : handler_(std::move(handler)) {
}

void HttpSessionHandler::onHandler(
  gsl::not_null<HttpSession*> session,
  Handler::Ptr oldHandler,
  Handler::Ptr newHandler) {
  BOOST_LOG_TRIVIAL(trace) << "Http session '" << session << "' switched handler from '" << oldHandler << "' to '"
                           << newHandler << "'";
}

auto HttpSessionHandler::onError(
  gsl::not_null<HttpSession*> session,
  const std::string& where,
  const boost::system::error_code& ec) -> bool {
  if (ec == boost::beast::http::error::end_of_stream) {
    session->shutdown();
    return true;
  }

  if (ec == boost::asio::error::operation_aborted || ec == boost::asio::error::timed_out) {
    return true;
  }

  BOOST_LOG_TRIVIAL(error) << "Http session '" << session << "' error on " << where << ": '" << ec.message() << "'";
  return true;
}

void HttpSessionHandler::onStart(gsl::not_null<HttpSession*> session) {
  BOOST_LOG_TRIVIAL(trace) << "Http session '" << session << "' has started.";
}

void HttpSessionHandler::onResponse(gsl::not_null<HttpSession*> session) {
  BOOST_LOG_TRIVIAL(trace) << "Http session '" << session << "' sent response.";
}

void HttpSessionHandler::onShutdown(gsl::not_null<HttpSession*> session) {
  BOOST_LOG_TRIVIAL(trace) << "Http session '" << session << "' has shutdown.";
}

void HttpSessionHandler::onUpgrade(
  gsl::not_null<HttpSession*> session,
  boost::asio::ip::tcp::socket&& socket,
  HttpSession::Request&& req) {

  auto wsSession = std::make_shared<WebSocketSession>(std::move(socket), handler_);
  BOOST_LOG_TRIVIAL(trace) << "Http session '" << session << "' is upgrading to WebSocket session '" << wsSession
                           << "'";
  wsSession->start(req);
}

} // namespace Y2KaoZ::Network::Tcp::Http::WebSocket