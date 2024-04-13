status_t Parcel::writeString16(const String16& str)
{
 return writeString16(str.string(), str.size());
}
