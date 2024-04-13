status_t Parcel::writeByteVector(const std::vector<int8_t>& val) {
 return writeByteVectorInternal(this, val);
}
