EvalJsResult GetOriginFromRenderer(FrameTreeNode* ftn) {
  return EvalJs(ftn, "self.origin;");
}
