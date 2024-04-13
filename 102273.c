size_t IGraphicBufferProducer::QueueBufferInput::getFlattenedSize() const {
 return sizeof(timestamp)
 + sizeof(isAutoTimestamp)
 + sizeof(crop)
 + sizeof(scalingMode)
 + sizeof(transform)
 + sizeof(async)
 + fence->getFlattenedSize();
}
