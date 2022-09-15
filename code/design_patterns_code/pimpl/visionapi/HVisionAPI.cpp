#include "HVisionAPI.h"

#include <utility>

#include "HVisionEvent.h"

HVisionAPI::HVisionAPI() {
  std::unique_ptr<HVisionEvent> ptr(new HVisionEvent());
  d_ = std::move(ptr);
}

HVisionAPI::~HVisionAPI() { d_.reset(); }

void HVisionAPI::SetPath(const std::string& path) { d_->SetPath(path); }

void HVisionAPI::ShowPath() const { d_->ShowPath(); }
