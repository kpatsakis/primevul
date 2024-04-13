status_t Parcel::readBoolVector(std::vector<bool>* val) const {
 int32_t size;
 status_t status = readInt32(&size);

 if (status != OK) {
 return status;
 }

 if (size < 0) {
 return UNEXPECTED_NULL;
 }

    val->resize(size);

 /* C++ bool handling means a vector of bools isn't necessarily addressable
     * (we might use individual bits)
     */
 bool data;
 for (int32_t i = 0; i < size; ++i) {
        status = readBool(&data);
 (*val)[i] = data;

 if (status != OK) {
 return status;
 }
 }

 return OK;
}
