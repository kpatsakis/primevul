status_t Parcel::writeByte(int8_t val)
{
 return writeInt32(int32_t(val));
}
