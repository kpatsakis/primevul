HistogramIDBKeyPathType HistogramKeyPathType(const IndexedDBKeyPath& key_path) {
  switch (key_path.type()) {
    case blink::mojom::IDBKeyPathType::Null:
      return KEY_PATH_TYPE_NONE;
    case blink::mojom::IDBKeyPathType::String:
      return KEY_PATH_TYPE_STRING;
    case blink::mojom::IDBKeyPathType::Array:
      return KEY_PATH_TYPE_ARRAY;
  }
  NOTREACHED();
  return KEY_PATH_TYPE_NONE;
}
