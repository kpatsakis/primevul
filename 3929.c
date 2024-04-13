static Resource HHVM_FUNCTION(wddx_packet_start, const Variant& comment) {
  return Resource(req::make<WddxPacket>(comment, true, false));
}