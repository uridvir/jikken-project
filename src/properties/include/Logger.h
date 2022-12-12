#pragma once

#include <iostream>

class Logger {
public:
    virtual void log(std::string text) = 0;
};