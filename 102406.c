 static bool HasParameterMapArg(FixedArray* parameter_map, uint32_t index) {
 uint32_t length = parameter_map->length() - 2;
 if (index >= length) return false;
 return !parameter_map->get(index + 2)->IsTheHole(
        parameter_map->GetIsolate());
 }
