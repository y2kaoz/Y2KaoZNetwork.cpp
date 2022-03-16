#include "Y2KaoZ/Network/Tcp/Http/HttpSession.hpp"
#include <boost/beast/http/read.hpp>
#include <boost/beast/websocket/rfc6455.hpp>

namespace {

template <typename KeyT, typename ValueT, std::size_t Size>
struct ArrayMap {
  std::array<std::pair<KeyT, ValueT>, Size> data; // NOLINT
  [[nodiscard]] constexpr auto at(const KeyT& key) const -> ValueT {
    const auto i = std::find_if(std::begin(data), std::end(data), [&key](const auto& v) { return v.first == key; });
    if (i != std::end(data)) {
      return i->second;
    }
    throw std::range_error("Not found");
  }
};

auto mimeLookup(const std::filesystem::path& path) -> std::string_view {
  using namespace std::string_view_literals;
  constexpr ArrayMap<std::string_view, std::string_view, 22> arrayMap{.data{{
    {".bmp"sv, "image/bmp"sv},
    {".css"sv, "text/css"sv},
    {".flv"sv, "application/x-flv"sv},
    {".gif"sv, "image/gif"sv},
    {".htm"sv, "text/html"sv},
    {".html"sv, "text/html"sv},
    {".ico"sv, "image/vnd.microsoft.icon"sv},
    {".jpe"sv, "image/jpeg"sv},
    {".jpeg"sv, "image/jpeg"sv},
    {".jpg"sv, "image/jpeg"sv},
    {".js"sv, "application/javascript"sv},
    {".json"sv, "application/json"sv},
    {".mjs"sv, "application/javascript"sv},
    {".php"sv, "text/html"sv},
    {".png"sv, "image/png"sv},
    {".svg"sv, "image/svg+xml"sv},
    {".svgz"sv, "image/svg+xml"sv},
    {".swf"sv, "application/x-shockwave-flash"sv},
    {".tif"sv, "image/tiff"sv},
    {".tiff"sv, "image/tiff"sv},
    {".txt"sv, "text/plain"sv},
    {".xml"sv, "application/xml"sv},
  }}};
  try {
    return arrayMap.at(path.extension().string());
  } catch (const std::range_error&) {
    return "application/text"sv;
  }
}

using HttpSession = Y2KaoZ::Network::Tcp::Http::HttpSession;

class BadRequestResponse : public HttpSession::StrResponse {
public:
  BadRequestResponse(const HttpSession::Request& req, const std::string_view& why)
    : HttpSession::StrResponse{boost::beast::http::status::bad_request, req.version()} {
    set(boost::beast::http::field::server, "Boost.Beast");
    set(boost::beast::http::field::content_type, "text/html");
    keep_alive(req.keep_alive());
    body() = std::string(why);
    prepare_payload();
  }
};

class NotFoundResponse : public HttpSession::StrResponse {
public:
  NotFoundResponse(const HttpSession::Request& req, const std::string_view& target)
    : HttpSession::StrResponse{boost::beast::http::status::not_found, req.version()} {
    set(boost::beast::http::field::server, "Boost.Beast");
    set(boost::beast::http::field::content_type, "text/html");
    keep_alive(req.keep_alive());
    body() = "The resource '" + std::string(target) + "' was not found.";
    prepare_payload();
  }
};

class ServerErrorResponse : public HttpSession::StrResponse {
public:
  ServerErrorResponse(const HttpSession::Request& req, const std::string_view& what)
    : HttpSession::StrResponse{boost::beast::http::status::internal_server_error, req.version()} {
    set(boost::beast::http::field::server, "Boost.Beast");
    set(boost::beast::http::field::content_type, "text/html");
    keep_alive(req.keep_alive());
    body() = "An error occurred: '" + std::string(what) + "'";
    prepare_payload();
  }
};

class HeadResponse : public HttpSession::EmptyResponse {
public:
  HeadResponse(const HttpSession::Request& req, const std::filesystem::path& path)
    : HttpSession::EmptyResponse{boost::beast::http::status::ok, req.version()} {
    set(boost::beast::http::field::server, "Boost.Beast");
    set(boost::beast::http::field::content_type, mimeLookup(path));
    content_length(std::filesystem::file_size(path));
    keep_alive(req.keep_alive());
  }
};

class GetResponse : public HttpSession::FileResponse {
  auto openBody(const std::filesystem::path& path) -> boost::beast::http::file_body::value_type {
    boost::system::error_code ec;
    boost::beast::http::file_body::value_type body;
    body.open(path.c_str(), boost::beast::file_mode::scan, ec);
    if (ec) {
      throw std::runtime_error(ec.message());
    }
    return body;
  }

public:
  GetResponse(const HttpSession::Request& req, const std::filesystem::path& path)
    : HttpSession::FileResponse{
        std::piecewise_construct,
        std::make_tuple(openBody(path)),
        std::make_tuple(boost::beast::http::status::ok, req.version())} {
    set(boost::beast::http::field::server, "Boost.Beast");
    set(boost::beast::http::field::content_type, mimeLookup(path));
    content_length(std::filesystem::file_size(path));
    keep_alive(req.keep_alive());
  }
};

} // namespace

namespace Y2KaoZ::Network::Tcp::Http {

HttpSession::HttpSession(
  boost::asio::ip::tcp::socket&& socket,
  std::shared_ptr<std::filesystem::path> docRoot,
  Handler::Ptr handler,
  std::size_t bodyLimit,
  std::chrono::seconds expiresAfter)
  : stream_{std::move(socket)}
  , handler_{std::move(handler)}
  , docRoot_{std::move(docRoot)}
  , bodyLimit_{bodyLimit}
  , expiresAfter_{expiresAfter} {
  if (handler_ == nullptr) {
    throw std::runtime_error("Invalid null handler.");
  }
}

void HttpSession::handler(const Handler::Ptr& handler) {
  if (handler == nullptr) {
    throw std::runtime_error("Invalid null handler.");
  }
  boost::asio::post(stream_.get_executor(), [self = shared_from_this(), newHandler = handler]() {
    auto oldHandler = self->handler_;
    oldHandler->onHandler(self.get(), oldHandler, newHandler);
    newHandler->onHandler(self.get(), oldHandler, newHandler);
    self->handler_ = newHandler;
  });
}

auto HttpSession::handler() const noexcept -> const Handler::Ptr& {
  return handler_;
}

void HttpSession::start() {
  handler_->onStart(this);
  read();
}

void HttpSession::shutdown() {
  boost::asio::post(stream_.get_executor(), [self = shared_from_this()]() {
    boost::system::error_code ec;
    self->stream_.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
    if (ec && self->handler_->onError(self.get(), "shutdown", ec)) {
      return;
    }
    self->handler_->onShutdown(self.get());
  });
}

void HttpSession::read() {
  parser_.emplace();
  parser_->body_limit(bodyLimit_);
  stream_.expires_after(expiresAfter_);
  boost::beast::http::async_read(
    stream_,
    buffer_,
    parser_->get(),
    [self = shared_from_this()](boost::system::error_code ec, std::size_t /*bytes*/) {
      if (ec && self->handler_->onError(self.get(), "read", ec)) {
        return;
      }
      self->onRead(self->parser_->release());
    });
}

void HttpSession::onRead(Request request) {
  if (boost::beast::websocket::is_upgrade(request)) {
    return handler_->onUpgrade(this, stream_.release_socket(), std::move(request));
  }
  // Send the response
  auto target = std::filesystem::weakly_canonical(request.target()).relative_path();
  std::filesystem::path path = *docRoot_ / target;
  if (std::filesystem::is_directory(path)) {
    path /= "index.html";
  }

  auto send = [this](auto&& response) {
    boost::beast::http::async_write(
      stream_,
      *response,
      [self = shared_from_this(), response](boost::system::error_code ec, std::size_t /*bytes*/) {
        if (ec && self->handler_->onError(self.get(), "send", ec)) {
          return;
        }
        if (response->need_eof()) {
          return self->shutdown();
        }
        self->handler_->onResponse(self.get());
        self->read();
      });
  };

  if (!std::filesystem::exists(path)) {
    return send(std::make_shared<NotFoundResponse>(request, "/" + target.string()));
  }
  try {
    switch (request.method()) {
      case boost::beast::http::verb::get:
        return send(std::make_shared<GetResponse>(request, path));
      case boost::beast::http::verb::head:
        return send(std::make_shared<HeadResponse>(request, path));
      default:
        return send(std::make_shared<BadRequestResponse>(request, "Unknown HTTP-method"));
    };
  } catch (const std::exception& e) {
    send(std::make_shared<ServerErrorResponse>(request, e.what()));
  }
}

} // namespace Y2KaoZ::Network::Tcp::Http