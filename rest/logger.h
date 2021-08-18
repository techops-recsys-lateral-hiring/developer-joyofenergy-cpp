#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_LOGGER_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_LOGGER_H

#include <iostream>

#include <boost/beast/core/error.hpp>

class logger {
public:
    virtual void fail(boost::beast::error_code ec, char const *what) = 0;
};

class logger_impl : public logger {
public:
    void fail(boost::beast::error_code ec, const char *what) override {
        std::cerr << what << ": " << ec.message() << "\n";
    }
};

static inline void fail(boost::beast::error_code ec, const char *what) { logger_impl().fail(ec, what); }

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_LOGGER_H
