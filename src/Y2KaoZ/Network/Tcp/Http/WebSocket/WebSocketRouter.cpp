#include "Y2KaoZ/Network/Tcp/Http/WebSocket/WebSocketRouter.hpp"
#include <boost/log/trivial.hpp>
#include <filesystem>

namespace Y2KaoZ::Network::Tcp::Http::WebSocket {

WebSocketRouter::WebSocketRouter(HandlerFactory handlerFactory) : handlerFactory_{std::move(handlerFactory)} {
}

void WebSocketRouter::onHandler(WebSocketSessionPtr session, Ptr oldHandler, Ptr newHandler) {
  if (oldHandler.get() == this) {
    BOOST_LOG_TRIVIAL(trace) << "WebSocket session '" << session << "' switched handler from router.";
  }
  if (newHandler.get() == this) {
    BOOST_LOG_TRIVIAL(error) << "WebSocket session  '" << session << "' invalid operation.";
    session->close(boost::beast::websocket::close_reason(boost::beast::websocket::close_code::policy_error));
  }
}

auto WebSocketRouter::onError(WebSocketSessionPtr session, const std::string& where, boost::system::error_code ec)
    -> bool {
  if (ec != boost::asio::error::operation_aborted && ec != boost::beast::websocket::error::closed) {
    BOOST_LOG_TRIVIAL(error) << "WebSocket session '" << session << "' " << where << ": '" << ec.what() << "'";
  }
  return true;
}

void WebSocketRouter::onStart(WebSocketSessionPtr session, const WebSocketSession::HttpRequest& req) {
  auto target = std::filesystem::weakly_canonical(req.target()).relative_path();
  BOOST_LOG_TRIVIAL(trace) << "Routing session  '" << session << "' to " << target << "'s handler.";

  auto handler = handlerFactory_(target);

  if (session->isOpen() && handler) {
    session->handler(handler);
    handler->onStart(session, req);
  } else {
    BOOST_LOG_TRIVIAL(warning) << "The router has no handler for : " << target;
    session->close(boost::beast::websocket::close_reason(boost::beast::websocket::close_code::policy_error));
  }
}

void WebSocketRouter::onClose(WebSocketSessionPtr session) {
  BOOST_LOG_TRIVIAL(trace) << "WebSocket session  '" << session << "' closed using router handler.";
}

void WebSocketRouter::onRead(WebSocketSessionPtr session, const std::string& /* message */) {
  BOOST_LOG_TRIVIAL(error) << "WebSocket session  '" << session << "' invalid operation.";
  session->close(boost::beast::websocket::close_reason(boost::beast::websocket::close_code::policy_error));
}

void WebSocketRouter::onSend(WebSocketSessionPtr session, const std::string& /* message */) {
  BOOST_LOG_TRIVIAL(error) << "WebSocket session  '" << session << "' invalid operation.";
  session->close(boost::beast::websocket::close_reason(boost::beast::websocket::close_code::policy_error));
}

void WebSocketRouter::onDestroy(WebSocketSessionPtr session) noexcept {
  BOOST_LOG_TRIVIAL(trace) << "WebSocket session  '" << session << "' destroyed using router handler.";
}

} // namespace Y2KaoZ::Network::Tcp::Http::WebSocket