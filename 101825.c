  ClientSharedBitmap(
      scoped_refptr<mojom::ThreadSafeSharedBitmapAllocationNotifierPtr>
          shared_bitmap_allocation_notifier,
      base::SharedMemory* shared_memory,
      const SharedBitmapId& id,
      uint32_t sequence_number)
      : SharedBitmap(static_cast<uint8_t*>(shared_memory->memory()),
                     id,
                     sequence_number),
        shared_bitmap_allocation_notifier_(
            std::move(shared_bitmap_allocation_notifier)) {}
