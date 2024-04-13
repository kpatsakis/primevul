  explicit MidiManagerUsbForTesting(
      scoped_ptr<UsbMidiDevice::Factory> device_factory)
      : MidiManagerUsb(device_factory.Pass()) {}
