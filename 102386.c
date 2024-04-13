SkISize SkIcoCodec::onGetScaledDimensions(float desiredScale) const {
 int origWidth = this->getInfo().width();
 int origHeight = this->getInfo().height();
 float desiredSize = desiredScale * origWidth * origHeight;
 float minError = ((float) (origWidth * origHeight)) - desiredSize + 1.0f;
 int32_t minIndex = -1;
 for (int32_t i = 0; i < fEmbeddedCodecs->count(); i++) {
 int width = fEmbeddedCodecs->operator[](i)->getInfo().width();
 int height = fEmbeddedCodecs->operator[](i)->getInfo().height();
 float error = SkTAbs(((float) (width * height)) - desiredSize);
 if (error < minError) {
            minError = error;
            minIndex = i;
 }
 }
 SkASSERT(minIndex >= 0);

 return fEmbeddedCodecs->operator[](minIndex)->getInfo().dimensions();
}
