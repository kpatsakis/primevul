size_t Parcel::dataAvail() const
{
 return dataSize() - dataPosition();
}
