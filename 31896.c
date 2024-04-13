void doMergeNameTree(PDFDoc *doc, XRef *srcXRef, XRef *countRef, int oldRefNum, int newRefNum, Dict *srcNameTree, Dict *mergeNameTree, int numOffset) {
  Object mergeNameArray;
  Object srcNameArray;
  mergeNameTree->lookup("Names", &mergeNameArray);
  srcNameTree->lookup("Names", &srcNameArray);
  if (mergeNameArray.isArray() && srcNameArray.isArray()) {
    Object *newNameArray = new Object();
    newNameArray->initArray(srcXRef);
    int j = 0;
    for (int i = 0; i < srcNameArray.arrayGetLength() - 1; i += 2) {
      Object key;
      Object value;
      srcNameArray.arrayGetNF(i, &key);
      srcNameArray.arrayGetNF(i + 1, &value);
      if (key.isString() && value.isRef()) {
        while (j < mergeNameArray.arrayGetLength() - 1) {
          Object mkey;
          Object mvalue;
          mergeNameArray.arrayGetNF(j, &mkey);
          mergeNameArray.arrayGetNF(j + 1, &mvalue);
          if (mkey.isString() && mvalue.isRef()) {
            if (mkey.getString()->cmp(key.getString()) < 0) {
              Object *newKey = new Object();
	      newKey->initString(new GooString(mkey.getString()->getCString()));
              newNameArray->arrayAdd(newKey);
              Object *newValue = new Object();
              newValue->initRef(mvalue.getRef().num + numOffset, mvalue.getRef().gen);
              newNameArray->arrayAdd(newValue);
              delete newKey;
              delete newValue;
              j += 2;
            } else if (mkey.getString()->cmp(key.getString()) == 0) {
              j += 2;
            } else {
              mkey.free();
              mvalue.free();
              break;
            }
          } else {
            j += 2;
          }
          mkey.free();
          mvalue.free();
        }
        Object *newKey = new Object();
        newKey->initString(new GooString(key.getString()->getCString()));
        newNameArray->arrayAdd(newKey);
        Object *newValue = new Object();
        newValue->initRef(value.getRef().num, value.getRef().gen);
        newNameArray->arrayAdd(newValue);
        delete newKey;
        delete newValue;
      }
      key.free();
      value.free();
    }
    while (j < mergeNameArray.arrayGetLength() - 1) {
      Object mkey;
      Object mvalue;
      mergeNameArray.arrayGetNF(j, &mkey);
      mergeNameArray.arrayGetNF(j + 1, &mvalue);
      if (mkey.isString() && mvalue.isRef()) {
        Object *newKey = new Object();
        newKey->initString(new GooString(mkey.getString()->getCString()));
        newNameArray->arrayAdd(newKey);
        Object *newValue = new Object();
        newValue->initRef(mvalue.getRef().num + numOffset, mvalue.getRef().gen);
        newNameArray->arrayAdd(newValue);
        delete newKey;
        delete newValue;
      }
      j += 2;
      mkey.free();
      mvalue.free();
    }
    srcNameTree->set("Names", newNameArray);
    doc->markPageObjects(mergeNameTree, srcXRef, countRef, numOffset, oldRefNum, newRefNum);
    delete newNameArray;
  } else if (srcNameArray.isNull() && mergeNameArray.isArray()) {
    Object *newNameArray = new Object();
    newNameArray->initArray(srcXRef);
    for (int i = 0; i < mergeNameArray.arrayGetLength() - 1; i += 2) {
      Object key;
      Object value;
      mergeNameArray.arrayGetNF(i, &key);
      mergeNameArray.arrayGetNF(i + 1, &value);
      if (key.isString() && value.isRef()) {
        Object *newKey = new Object();
	newKey->initString(new GooString(key.getString()->getCString()));
        newNameArray->arrayAdd(newKey);
        Object *newValue = new Object();
        newValue->initRef(value.getRef().num + numOffset, value.getRef().gen);
        newNameArray->arrayAdd(newValue);
        delete newKey;
        delete newValue;
      }
      key.free();
      value.free();
    }
    srcNameTree->add(copyString("Names"), newNameArray);
    doc->markPageObjects(mergeNameTree, srcXRef, countRef, numOffset, oldRefNum, newRefNum);
  }
  mergeNameArray.free();
  srcNameArray.free();
}
