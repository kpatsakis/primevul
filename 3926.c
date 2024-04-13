static String HHVM_FUNCTION(wddx_serialize_value, const Variant& var,
                            const Variant& comment) {
  auto wddxPacket = req::make<WddxPacket>(comment, false, false);
  wddxPacket->serialize_value(var);
  return wddxPacket->packet_end();
}