MagickExport size_t GetQuantumExtent(const Image *image,
  const QuantumInfo *quantum_info,const QuantumType quantum_type)
{
  size_t
    packet_size;

  assert(quantum_info != (QuantumInfo *) NULL);
  assert(quantum_info->signature == MagickSignature);
  packet_size=1;
  switch (quantum_type)
  {
    case GrayAlphaQuantum: packet_size=2; break;
    case IndexAlphaQuantum: packet_size=2; break;
    case RGBQuantum: packet_size=3; break;
    case BGRQuantum: packet_size=3; break;
    case RGBAQuantum: packet_size=4; break;
    case RGBOQuantum: packet_size=4; break;
    case BGRAQuantum: packet_size=4; break;
    case CMYKQuantum: packet_size=4; break;
    case CMYKAQuantum: packet_size=5; break;
    default: break;
  }
  if (quantum_info->pack == MagickFalse)
    return((size_t) (packet_size*image->columns*((quantum_info->depth+7)/8)));
  return((size_t) ((packet_size*image->columns*quantum_info->depth+7)/8));
}
