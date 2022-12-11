#pragma once

#include<iostream>

class CameraQuerier {
public:
    virtual std::string getCameraProperty(std::string prop) = 0;
};