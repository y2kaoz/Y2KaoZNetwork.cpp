#pragma once

#include "Y2KaoZ/Network/Visibility.hpp"
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/http/empty_body.hpp>
#include <boost/beast/http/file_body.hpp>
#include <boost/beast/http/parser.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/write.hpp>
#include <chrono>
#include <filesystem>
#include <gsl/pointers>
#include <memory>

namespace Y2KaoZ::Network::Tcp::Http {

Y2KAOZNETWORK_EXPORT class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
  static const std::size_t ONE_KILOBYTE = 1024U;
  using Request = boost::beast::http::request<boost::beast::http::string_body>;

  using StrResponse = boost::beast::http::response<boost::beast::http::string_body>;
  using FileResponse = boost::beast::http::response<boost::beast::http::file_body>;
  using EmptyResponse = boost::beast::http::response<boost::beast::http::empty_body>;

  class Handler {
  public:
    using Ptr = std::shared_ptr<Handler>;

    Handler() = default;
    Handler(const Handler&) = default;
    Handler(Handler&&) noexcept = default;
    auto operator=(const Handler&) -> Handler& = default;
    auto operator=(Handler&&) -> Handler& = default;
    virtual ~Handler() = default;

    virtual void onHandler(gsl::not_null<HttpSession*> session, Ptr oldHandler, Ptr newHandler) = 0;
    virtual auto onError(
        gsl::not_null<HttpSession*> session,
        const std::string& where,
        const boost::system::error_code& ec) -> bool = 0;
    virtual void onStart(gsl::not_null<HttpSession*> session) = 0;
    virtual void onResponse(gsl::not_null<HttpSession*> session) = 0;
    virtual void onShutdown(gsl::not_null<HttpSession*> session) = 0;
    virtual void onUpgrade(
        gsl::not_null<HttpSession*> session,
        boost::asio::ip::tcp::socket&& socket,
        Request&& req) = 0;
  };

  HttpSession(
      boost::asio::ip::tcp::socket&& socket,
      std::shared_ptr<std::filesystem::path> docRoot,
      Handler::Ptr handler,
      std::size_t bodyLimit = ONE_KILOBYTE,
      std::chrono::seconds expiresAfter = std::chrono::minutes{1});

  void handler(const Handler::Ptr& handler);
  [[nodiscard]] auto handler() const noexcept -> const Handler::Ptr&;
  void start();
  void shutdown();

private:
  boost::beast::tcp_stream stream_;
  boost::beast::flat_buffer buffer_;
  Handler::Ptr handler_;
  std::shared_ptr<std::filesystem::path> docRoot_;
  boost::optional<boost::beast::http::request_parser<boost::beast::http::string_body>> parser_;
  std::size_t bodyLimit_;
  std::chrono::seconds expiresAfter_;

  void onRead(Request request);
  void read();
};

} // namespace Y2KaoZ::Network::Tcp::Http
