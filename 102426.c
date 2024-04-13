status_t Parcel::writeUtf8VectorAsUtf16Vector(const std::vector<std::string>& val) {
 return writeTypedVector(val, &Parcel::writeUtf8AsUtf16);
}
