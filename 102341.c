static bool findParam(uint32_t key, T *param,
 KeyedVector<uint32_t, uint64_t> &params) {
    CHECK(param);
 if (params.indexOfKey(key) < 0) {
 return false;
 }
 *param = (T) params[key];
 return true;
}
