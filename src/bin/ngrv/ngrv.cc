#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string>

#include <nghttp2/asio_http2_server.h>

using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::server;

int main(int argc, char *argv[]) {
  try {
    // Check command line arguments.
    if (argc < 3) {
      std::cerr << "Usage: ngrv <address> <port>"
                << "[<private-key-file> <cert-file>]\n";
      return 1;
    }

    boost::system::error_code ec;

    std::string addr = argv[1];
    std::string port = argv[2];
    std::string docroot = "/tmp";

    http2 server;

    server.handle("/", [&docroot](const request &req, const response &res) {
      std::cerr << "received request" << std::endl;
      std::cerr << " method: " << req.method() << std::endl;
      auto uri = req.uri();
      std::cerr << " uri: " << uri.scheme << "://" << uri.host << uri.path << std::endl;
      std::cerr << " #headers: " << req.header().size() << std::endl;
      for (auto h : req.header()) {
	  std::cerr << " (header): " << h.first
	            << " = " << h.second.value << std::endl;
      }
      auto path = percent_decode(req.uri().path);
      if (!check_path(path)) {
        res.write_head(404);
        res.end();
        return;
      }

      auto header = header_map();

      path = docroot + path;
      auto fd = open(path.c_str(), O_RDONLY);
      if (fd == -1) {
        res.write_head(404);
        res.end();
        return;
      }



      struct stat stbuf;
      if (stat(path.c_str(), &stbuf) == 0) {
        header.emplace("content-length",
                       header_value{std::to_string(stbuf.st_size)});
        header.emplace("last-modified",
                       header_value{http_date(stbuf.st_mtime)});
      }
      res.write_head(200, std::move(header));
      res.end(file_generator_from_fd(fd));
    });

    if (argc >= 5) {
      boost::asio::ssl::context tls(boost::asio::ssl::context::sslv23);
      tls.use_private_key_file(argv[3], boost::asio::ssl::context::pem);
      tls.use_certificate_chain_file(argv[4]);

      configure_tls_context_easy(ec, tls);

      if (server.listen_and_serve(ec, tls, addr, port)) {
        std::cerr << "error: " << ec.message() << std::endl;
      }
    } else {
      if (server.listen_and_serve(ec, addr, port)) {
        std::cerr << "error: " << ec.message() << std::endl;
      }
    }
  } catch (std::exception &e) {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}
