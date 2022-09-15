#include "HVisionEvent.h"

#include <iostream>


HVisionEvent::~HVisionEvent() {}

void HVisionEvent::SetPath(const std::string& path) { modelPath_ = path; }

void HVisionEvent::ShowPath() const { std::cout << modelPath_ << std::endl; }
