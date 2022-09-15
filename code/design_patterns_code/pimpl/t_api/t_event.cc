#include "t_event.h"
#include <iostream>

void TEvent::SetPath(const std::string& path) { model_path_ = path; }

void TEvent::ShowPath() const { std::cout << model_path_ << std::endl; }
