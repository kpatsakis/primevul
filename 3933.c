static String HHVM_FUNCTION(wddx_packet_end, const Resource& packet_id) {
  return cast<WddxPacket>(packet_id)->packet_end();
}