# Braille Text-to-Speech Project

This project consists of two main components:
1. A laptop program that converts speech to text and sends it to an Arduino.
2. An Arduino program that receives text and controls servo motors to display Braille characters.

## Laptop Setup

1. Install Python 3.x if not already installed.
2. Install the required packages:
   ```
   pip install -r requirements.txt
   ```
3. You may need to install additional system dependencies for PyAudio. On macOS, you can use:
   ```
   brew install portaudio
   pip install pyaudio
   ```

Note: If you see warnings about unresolved imports in VSCode for the laptop script, this is normal if you haven't installed the required packages yet. These warnings should disappear after installing the packages.

## Arduino Setup

1. Download and install the Arduino IDE from the [official Arduino website](https://www.arduino.cc/en/software).
2. Open the Arduino IDE.
3. Install the following libraries through the Library Manager (Tools > Manage Libraries...):
   - Adafruit PWM Servo Driver Library
   - Wire (should be included by default)
4. Connect your Arduino Mega 2560 to your computer.
5. Select the correct board and port in the Arduino IDE (Tools > Board and Tools > Port).
6. Open the `arduino_braille_controller.ino` file in the Arduino IDE.
7. Click the "Upload" button to compile and upload the sketch to your Arduino.

Note: If you see errors about missing header files in VSCode for the Arduino script, this is normal. VSCode doesn't have the Arduino libraries in its include path by default. The script will still compile and upload correctly in the Arduino IDE.

## Hardware Setup

1. Connect the PCA9685 servo driver to the Arduino Mega 2560 using the I2C pins:
   - Connect SDA on the PCA9685 to pin 20 (SDA) on the Arduino Mega
   - Connect SCL on the PCA9685 to pin 21 (SCL) on the Arduino Mega
   - Connect VCC on the PCA9685 to 5V on the Arduino Mega
   - Connect GND on the PCA9685 to GND on the Arduino Mega
2. Connect the 36 SG90 servo motors to the PCA9685 driver:
   - Connect each servo's signal wire to one of the PWM outputs on the PCA9685
   - Connect the servo power (red) and ground (brown) wires to an external 5V power supply
3. Ensure proper power supply for the servo motors (a dedicated 5V supply capable of providing enough current for all servos).

## Running the Project

1. Ensure the Arduino is connected and the sketch is uploaded.
2. Open a terminal/command prompt and navigate to the project directory.
3. Run the laptop program:
   ```
   python laptop_speech_to_braille.py
   ```
4. Speak clearly into the microphone when prompted. The recognized text will be sent to the Arduino and displayed in Braille.

Note: You may need to adjust the serial port in `laptop_speech_to_braille.py` to match your system's configuration. Look for the line:
```python
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
```
And change '/dev/ttyUSB0' to the appropriate port for your system. On Windows, this might be 'COM3' or similar. On macOS, it might be '/dev/tty.usbmodem1301' or similar.

## Troubleshooting

- If you encounter issues with speech recognition, ensure you have a working microphone and try adjusting your system's audio input settings.
- If the Arduino is not receiving data, double-check the serial port configuration and ensure the baud rates match (9600 in both the Python script and Arduino sketch).
- If servos are not moving correctly, verify all connections and ensure the PCA9685 is receiving adequate power.

For any other issues, please check the serial monitor in the Arduino IDE for debugging information from the Arduino, and the Python console for output from the laptop script.