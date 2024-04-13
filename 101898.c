void DelegatedFrameHost::DidNotProduceFrame(const viz::BeginFrameAck& ack) {
  DCHECK(!ack.has_damage);
  support_->DidNotProduceFrame(ack);
}
