inline void Splash::drawSpan(SplashPipe *pipe, int x0, int x1, int y,
			     GBool noClip) {
  int x;

  if (noClip) {
    pipeSetXY(pipe, x0, y);
    for (x = x0; x <= x1; ++x) {
      (this->*pipe->run)(pipe);
    }
    updateModX(x0);
    updateModX(x1);
    updateModY(y);
  } else {
    if (x0 < state->clip->getXMinI()) {
      x0 = state->clip->getXMinI();
    }
    if (x1 > state->clip->getXMaxI()) {
      x1 = state->clip->getXMaxI();
    }
    pipeSetXY(pipe, x0, y);
    for (x = x0; x <= x1; ++x) {
      if (state->clip->test(x, y)) {
	(this->*pipe->run)(pipe);
	updateModX(x);
	updateModY(y);
      } else {
	pipeIncX(pipe);
      }
    }
  }
}
