#ifndef CIPR_NATIVE_NET_H
#define CIPR_NATIVE_NET_H

#include "Interpreter/Callable.h"
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <netinet/in.h>

struct NativeConnect final : Callable {
    int arity() override {
        return 2;
    }

    Literal call(Interpreter&, const std::vector<Literal> args) override {
        if (!std::holds_alternative<std::string>(args[0]) || !std::holds_alternative<double>(args[1]))
            return -1.0;

        const auto host = std::get<std::string>(args[0]);
        const std::string port = std::to_string(static_cast<int>(std::get<double>(args[1])));

        addrinfo hints{}, *res;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        if (getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0)
            return -1.0;

        const int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        if (fd == -1) { freeaddrinfo(res);
            return -1.0; }

        if (connect(fd, res->ai_addr, res->ai_addrlen) == -1) { close(fd); freeaddrinfo(res);
            return -1.0; }
        freeaddrinfo(res);

        return (double)fd;
    }

    std::string toString() override {
        return "<native fn connect>";
    }
};

struct NativeSend final : Callable {
    int arity() override {
        return 2;
    }

    Literal call(Interpreter&, const std::vector<Literal> args) override {
        if (!std::holds_alternative<double>(args[0]) || !std::holds_alternative<std::string>(args[1]))
            return -1.0;
        const int fd = static_cast<int>(std::get<double>(args[0]));
        const auto d = std::get<std::string>(args[1]);
        return static_cast<double>(send(fd, d.c_str(), d.length(), 0));
    }

    std::string toString() override {
        return "<native fn send>";
    }
};

struct NativeRecv final : Callable {
    int arity() override {
        return 2;
    }

    Literal call(Interpreter&, const std::vector<Literal> args) override {
        if (!std::holds_alternative<double>(args[0]) || !std::holds_alternative<double>(args[1]))
            return std::monostate{};
        const int fd = static_cast<int>(std::get<double>(args[0]));
        const int sz = static_cast<int>(std::get<double>(args[1]));
        std::vector<char> buf(sz);
        if (const ssize_t n = recv(fd, buf.data(), sz, 0); n > 0)
            return std::string(buf.data(), n);
        return std::monostate{};
    }

    std::string toString() override {
        return "<native fn recv>";
    }
};

struct NativeClose final : Callable {
    int arity() override {
        return 1;
    }

    Literal call(Interpreter&, const std::vector<Literal> args) override {
        if (!std::holds_alternative<double>(args[0]))
            return false;
        close(static_cast<int>(std::get<double>(args[0])));
        return true;
    }

    std::string toString() override {
        return "<native fn close>";
    }
};

struct NativeHttpGet final : Callable {
    int arity() override {
        return 1;
    }

    Literal call(Interpreter&, const std::vector<Literal> args) override {
        if (!std::holds_alternative<std::string>(args[0]))
            return std::monostate{};
        auto url = std::get<std::string>(args[0]);

        if (url.substr(0, 7) == "http://")
            url = url.substr(7);

        const size_t slash = url.find('/');
        const std::string host = (slash == std::string::npos) ? url : url.substr(0, slash);
        const std::string path = (slash == std::string::npos) ? "/" : url.substr(slash);

        addrinfo hints{}, *res;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        if (getaddrinfo(host.c_str(), "80", &hints, &res) != 0)
            return std::monostate{};

        const int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (fd == -1) {
            freeaddrinfo(res); return std::monostate{};
        }
        
        if (connect(fd, res->ai_addr, res->ai_addrlen) == -1) {
            close(fd); freeaddrinfo(res); return std::monostate{};
        }
        freeaddrinfo(res);

        const std::string req = "GET " + path + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";
        send(fd, req.c_str(), req.length(), 0);

        std::string raw;
        char buf[4096];
        ssize_t n;
        while ((n = recv(fd, buf, sizeof(buf), 0)) > 0) raw.append(buf, n);
        close(fd);

        size_t headers_end = raw.find("\r\n\r\n");
        if (headers_end != std::string::npos) return raw.substr(headers_end + 4);
        return raw;
    }
    std::string toString() override { return "<native fn http_get>"; }
};

struct NativeHttpPost final : Callable {
    int arity() override {
        return 2;
    }

    Literal call(Interpreter&, const std::vector<Literal> args) override {
        if (!std::holds_alternative<std::string>(args[0]) || !std::holds_alternative<std::string>(args[1])) 
            return std::monostate{};
        auto url = std::get<std::string>(args[0]);
        const auto body = std::get<std::string>(args[1]);

        if (url.substr(0, 7) == "http://") url = url.substr(7);
        const size_t slash = url.find('/');
        const std::string host = (slash == std::string::npos) ? url : url.substr(0, slash);
        const std::string path = (slash == std::string::npos) ? "/" : url.substr(slash);

        addrinfo hints{}, *res;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        if (getaddrinfo(host.c_str(), "80", &hints, &res) != 0)
            return std::monostate{};

        const int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (fd == -1) { freeaddrinfo(res);
            return std::monostate{}; }
        
        if (connect(fd, res->ai_addr, res->ai_addrlen) == -1) {
            close(fd); freeaddrinfo(res);
            return std::monostate{};
        }
        freeaddrinfo(res);

        const std::string req = "POST " + path + " HTTP/1.1\r\n" +
                          "Host: " + host + "\r\n" +
                          "Content-Length: " + std::to_string(body.length()) + "\r\n" +
                          "Content-Type: text/plain\r\n" +
                          "Connection: close\r\n\r\n" + body;
        send(fd, req.c_str(), req.length(), 0);

        std::string raw;
        char buf[4096];
        ssize_t n;
        while ((n = recv(fd, buf, sizeof(buf), 0)) > 0) raw.append(buf, n);
        close(fd);

        size_t headers_end = raw.find("\r\n\r\n");
        if (headers_end != std::string::npos) return raw.substr(headers_end + 4);
        return raw;
    }
        std::string toString() override {
          return "<native fn http_post>";
        }
    };
    
struct NativeListen final : Callable {
    int arity() override {
      return 1;
    }

    Literal call(Interpreter&, const std::vector<Literal> args) override {
        if (!std::holds_alternative<double>(args[0]))
          return -1.0;

        const auto port = static_cast<int>(std::get<double>(args[0]));
        const auto fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd == -1) return -1.0;

        constexpr int opt = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        if (bind(fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) == -1) {
            close(fd);
            return -1.0;
        }

        if (listen(fd, 10) == -1) {
            close(fd);
            return -1.0;
        }

        return static_cast<double>(fd);
    }

    std::string toString() override {
      return "<native fn listen>";
    }
};

struct NativeAccept final : Callable {
    int arity() override {
      return 1;
    }

    Literal call(Interpreter&, const std::vector<Literal> args) override {
        if (!std::holds_alternative<double>(args[0]))
          return -1.0;

        const auto server_fd = static_cast<int>(std::get<double>(args[0]));
        const auto client_fd = accept(server_fd, nullptr, nullptr);

        return static_cast<double>(client_fd);
    }

    std::string toString() override {
      return "<native fn accept>";
    }
};
    
#endif