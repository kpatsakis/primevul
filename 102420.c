status_t Parcel::readFloatVector(std::vector<float>* val) const {
 return readTypedVector(val, &Parcel::readFloat);
}
