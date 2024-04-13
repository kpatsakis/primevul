void OutOfProcessInstance::StopFind() {
  engine_->StopFind();
  tickmarks_.clear();
  SetTickmarks(tickmarks_);
}
