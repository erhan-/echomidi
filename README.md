# EchoMIDI

A gesture-controlled MIDI controller using ultrasonic distance sensing. Control your DAW parameters, synthesizers, and effects in Ableton Live (or any MIDI-compatible software) by simply moving your hand in the air.

## 🎛️ Overview

EchoMIDI transforms a Digispark ATtiny85 board and an HC-SR04 ultrasonic sensor into a USB MIDI controller. By measuring the distance of your hand (or any object) from the sensor, it sends MIDI Control Change (CC) messages that can be mapped to any parameter in your music production software.

Perfect for:
- Live performance control
- Hands-free parameter automation
- Creative sound design
- Accessible music production interfaces

## 🔧 Hardware Requirements

- **Digispark ATtiny85** development board (USB-based)
- **HC-SR04** ultrasonic distance sensor
- Jumper wires
- USB cable (for power and MIDI communication)

## 📐 Wiring Diagram

Connect the HC-SR04 sensor to your Digispark as follows:

| HC-SR04 Pin | Digispark Pin | Description |
|-------------|---------------|-------------|
| VCC         | 5V            | Power supply |
| GND         | GND           | Ground |
| TRIG        | P0            | Trigger pin |
| ECHO        | P2            | Echo pin |

The onboard LED on **P1** provides visual feedback when an object is detected in range.

## 🚀 Getting Started

### Prerequisites

1. **PlatformIO** - Install PlatformIO IDE or PlatformIO Core
   - [PlatformIO IDE](https://platformio.org/platformio-ide) (VS Code extension)
   - Or [PlatformIO Core](https://docs.platformio.org/en/latest/core/installation.html) (CLI)

2. **Digispark Drivers** - Install the Digispark USB drivers for your operating system
   - [Windows/Mac/Linux drivers](https://github.com/digistump/DigistumpArduino/releases)

### Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/echomidi.git
   cd echomidi
   ```

2. The project already includes the required DigisparkMIDI library in the `lib/` directory (forked from [erhan-/DigisparkMIDI](https://github.com/erhan-/DigisparkMIDI))

3. Build and upload to your Digispark:
   ```bash
   pio run --target upload
   ```

4. **Important**: When prompted, **plug in your Digispark board**. The bootloader has a 5-second window for uploading.

## 🎹 Usage

### Basic Operation

1. Connect the EchoMIDI device to your computer via USB
2. The device will appear as a USB MIDI controller
3. Move your hand between **2-30 cm** from the sensor
4. The onboard LED lights up when an object is detected in range
5. Distance is mapped to MIDI CC values (closer = higher value, 127 to 0)

### DAW Configuration

#### Ableton Live

1. Go to **Preferences → Link/Tempo/MIDI**
2. Enable **Track** and **Remote** for the "Digispark" MIDI device
3. Enter **MIDI Map Mode** (Cmd/Ctrl + M)
4. Click on any parameter you want to control
5. Move your hand over the sensor to map it
6. Exit MIDI Map Mode

#### Other DAWs

The device sends standard MIDI CC messages on **Channel 1**, **CC Number 24** by default. Consult your DAW's documentation for MIDI mapping instructions.

## ⚙️ Customization

### Changing the CC Number

Edit [`src/main.cpp`](src/main.cpp:18) and modify the `CC_NUMBER` constant:

```cpp
const int CC_NUMBER = 24;  // Change to any value 0-127
```

Common CC numbers:
- CC 1: Modulation Wheel
- CC 7: Volume
- CC 10: Pan
- CC 74: Filter Cutoff
- CC 71: Filter Resonance

### Adjusting Distance Range

Modify the distance range in [`src/main.cpp`](src/main.cpp:56-63):

```cpp
if (distance > 2 && distance < 30) {  // Valid range
    int ccValue = map(distance, 2, 50, 127, 0);  // Mapping range
    // ...
}
```

### Response Sensitivity

Adjust the change threshold to make the controller more or less sensitive:

```cpp
const int MIN_CHANGE_THRESHOLD = 1;  // Increase for less sensitivity
```

## 📊 Technical Details

- **Microcontroller**: ATtiny85 @ 16.5 MHz
- **USB Protocol**: USB MIDI Class Compliant
- **MIDI Channel**: 1 (default)
- **Control Change**: CC 24 (configurable)
- **Update Rate**: ~100 Hz (10ms loop delay)
- **Distance Range**: 2-30 cm (configurable)
- **Distance Resolution**: ~1 cm
- **CC Value Range**: 0-127 (MIDI standard)

## 🛠️ Troubleshooting

### Device Not Recognized

- Ensure Digispark USB drivers are installed
- Try a different USB port
- On Linux, you may need to add udev rules for the Digispark

### Erratic MIDI Values

- Check sensor wiring connections
- Ensure the sensor has a clear line of sight
- Increase `MIN_CHANGE_THRESHOLD` for smoother control
- Avoid reflective or sound-absorbing surfaces near the sensor

### Upload Issues

- The Digispark bootloader runs for only 5 seconds after plugging in
- Start the upload command first, then plug in the board when prompted
- If upload fails, unplug and try again

## 📚 Dependencies

This project uses the [DigisparkMIDI](https://github.com/erhan-/DigisparkMIDI) library by erhan-, a fork that provides MIDI functionality for Digispark boards. The library is included in the `lib/` directory.

## 🤝 Contributing

Contributions are welcome! Feel free to:
- Report bugs
- Suggest new features
- Submit pull requests
- Share your creative uses

## 📄 License

This project is open source. Please refer to the LICENSE file for details.

The DigisparkMIDI library has its own license - see [`lib/DigisparkMIDI/License.txt`](lib/DigisparkMIDI/License.txt) for details.

## 🎵 Inspiration & Use Cases

- **Live Performance**: Control filter sweeps, reverb, or delay parameters
- **Sound Design**: Modulate oscillator pitch or wavetable position
- **Mixing**: Adjust volume or pan with hand gestures
- **Accessibility**: Hands-free control for musicians with limited mobility
- **Installation Art**: Create interactive sound installations

## 🔗 Resources

- [DigisparkMIDI Library](https://github.com/erhan-/DigisparkMIDI)
- [Digispark Official Wiki](http://digistump.com/wiki/digispark)
- [PlatformIO Documentation](https://docs.platformio.org/)
- [MIDI CC List](https://www.midi.org/specifications-old/item/table-3-control-change-messages-data-bytes-2)

## ✨ Credits

- **DigisparkMIDI Library**: [erhan-](https://github.com/erhan-/DigisparkMIDI)
- **V-USB**: Low-speed USB implementation for AVR microcontrollers
- **Digistump**: Digispark hardware platform

---

**Made with ❤️ from the Once Upon a Beat community**
