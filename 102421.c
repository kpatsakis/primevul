status_t Parcel::readUniqueFileDescriptorVector(std::unique_ptr<std::vector<ScopedFd>>* val) const {
 return readNullableTypedVector(val, &Parcel::readUniqueFileDescriptor);
}
