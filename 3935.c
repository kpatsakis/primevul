String WddxPacket::packet_end() {
  if (!m_packetClosed) {
    if (m_manualPacketCreation) {
      m_packetString.append("</struct>");
    }
    m_packetString.append("</data></wddxPacket>");
  }
  m_packetClosed = true;
  return m_packetString.detach();
}