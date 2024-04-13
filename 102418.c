status_t Parcel::readDoubleVector(std::vector<double>* val) const {
 return readTypedVector(val, &Parcel::readDouble);
}
