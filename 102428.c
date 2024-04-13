Maybe<std::u16string> getFullyQualifiedClassName(const StringPiece16& package,
 const StringPiece16& className) {
 if (className.empty()) {
 return {};
 }

 if (util::isJavaClassName(className)) {
 return className.toString();
 }

 if (package.empty()) {
 return {};
 }

 if (className.data()[0] != u'.') {
 return {};
 }

    std::u16string result(package.data(), package.size());
    result.append(className.data(), className.size());
 if (!isJavaClassName(result)) {
 return {};
 }
 return result;
}
