AudioFramesPerBuffer AsAudioFramesPerBuffer(int frames_per_buffer) {
  switch (frames_per_buffer) {
    case 160: return k160;
    case 320: return k320;
    case 440: return k440;
    case 480: return k480;
    case 640: return k640;
    case 880: return k880;
    case 960: return k960;
    case 1440: return k1440;
    case 1920: return k1920;
  }
  return kUnexpectedAudioBufferSize;
}
