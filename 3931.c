WddxPacket::WddxPacket(const Variant& comment, bool manualPacket, bool sVar) :
                       m_packetClosed(false),
                       m_manualPacketCreation(manualPacket) {
  m_packetString.append("<wddxPacket version='1.0'>");

  if (!comment.isNull() && !sVar) {
    m_packetString.append("<header><comment>");
    m_packetString.append(comment);
    m_packetString.append("</comment></header>");
  } else {
    m_packetString.append("<header/>");
  }

  m_packetString.append("<data>");

  if (m_manualPacketCreation) {
    m_packetString.append("<struct>");
  }
}