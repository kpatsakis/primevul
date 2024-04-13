status_t Parcel::writeString16Vector(
 const std::unique_ptr<std::vector<std::unique_ptr<String16>>>& val)
{
 return writeNullableTypedVector(val, &Parcel::writeString16);
}
