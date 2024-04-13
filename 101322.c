gfx::RectF AXTree::RelativeToTreeBounds(const AXNode* node,
                                        gfx::RectF bounds,
                                        bool* offscreen,
                                        bool clip_bounds) const {
  if (bounds.width() == 0 && bounds.height() == 0) {
    bounds = node->data().relative_bounds.bounds;

    if (bounds.IsEmpty()) {
      for (size_t i = 0; i < node->children().size(); i++) {
        ui::AXNode* child = node->children()[i];
        bounds.Union(GetTreeBounds(child));
      }
      if (bounds.width() > 0 && bounds.height() > 0) {
        return bounds;
      }
    }
  } else {
    bounds.Offset(node->data().relative_bounds.bounds.x(),
                  node->data().relative_bounds.bounds.y());
  }

  while (node != nullptr) {
    if (node->data().relative_bounds.transform)
      node->data().relative_bounds.transform->TransformRect(&bounds);
    const AXNode* container;

    if (bounds.width() == 0 && bounds.height() == 0)
      container = node->parent();
    else
      container = GetFromId(node->data().relative_bounds.offset_container_id);
    if (!container && container != root())
      container = root();
    if (!container || container == node)
      break;

    gfx::RectF container_bounds = container->data().relative_bounds.bounds;
    bounds.Offset(container_bounds.x(), container_bounds.y());

    if (bounds.width() == 0 && bounds.height() == 0) {
      bounds.set_size(container_bounds.size());
      if (offscreen != nullptr)
        *offscreen |= true;
    }

    int scroll_x = 0;
    int scroll_y = 0;
    if (container->data().GetIntAttribute(ax::mojom::IntAttribute::kScrollX,
                                          &scroll_x) &&
        container->data().GetIntAttribute(ax::mojom::IntAttribute::kScrollY,
                                          &scroll_y)) {
      bounds.Offset(-scroll_x, -scroll_y);
    }

    gfx::RectF intersection = bounds;
    intersection.Intersect(container_bounds);

    gfx::RectF clipped = bounds;
    if (container->data().GetBoolAttribute(
            ax::mojom::BoolAttribute::kClipsChildren)) {
      if (!intersection.IsEmpty()) {
        clipped = intersection;
      } else {
        if (clipped.x() >= container_bounds.width()) {
          clipped.set_x(container_bounds.right() - 1);
          clipped.set_width(1);
        } else if (clipped.x() + clipped.width() <= 0) {
          clipped.set_x(container_bounds.x());
          clipped.set_width(1);
        }
        if (clipped.y() >= container_bounds.height()) {
          clipped.set_y(container_bounds.bottom() - 1);
          clipped.set_height(1);
        } else if (clipped.y() + clipped.height() <= 0) {
          clipped.set_y(container_bounds.y());
          clipped.set_height(1);
        }
      }
    }

    if (clip_bounds)
      bounds = clipped;

    if (container->data().GetBoolAttribute(
            ax::mojom::BoolAttribute::kClipsChildren) &&
        intersection.IsEmpty() && !clipped.IsEmpty()) {
      if (offscreen != nullptr)
        *offscreen |= true;
    }

    node = container;
  }

  return bounds;
}
