#pragma once

#include <utility>
#include <iostream>
#include <string>

using namespace std;

class NoConfigException {
public:
    explicit NoConfigException(string msg) : msg_(std::move(msg)){}
    ~NoConfigException() = default;

    string getMessage() const {return msg_;}
private:
    string msg_;
};