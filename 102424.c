status_t Parcel::writeInt64Vector(const std::vector<int64_t>& val)
{
 return writeTypedVector(val, &Parcel::writeInt64);
}
