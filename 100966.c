bool ParamTraits<Message>::Read(const Message* m, PickleIterator* iter,
                                Message* r) {
  uint32 routing_id, type, flags;
  if (!m->ReadUInt32(iter, &routing_id) ||
      !m->ReadUInt32(iter, &type) ||
      !m->ReadUInt32(iter, &flags))
    return false;

  int payload_size;
  const char* payload;
  if (!m->ReadData(iter, &payload, &payload_size))
    return false;

  r->SetHeaderValues(static_cast<int32>(routing_id), type, flags);
  return r->WriteBytes(payload, payload_size);
}
