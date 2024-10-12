import speech_recognition as sr
import serial
import time

def speech_to_text():
    recognizer = sr.Recognizer()
    with sr.Microphone() as source:
        print("Listening...")
        audio = recognizer.listen(source)
    
    try:
        text = recognizer.recognize_google(audio)
        print(f"Recognized: {text}")
        return text
    except sr.UnknownValueError:
        print("Could not understand audio")
        return None
    except sr.RequestError as e:
        print(f"Could not request results; {e}")
        return None

def send_to_arduino(ser, text):
    # Send only the first 6 characters to match the 6 Braille cells
    text_to_send = text[:6].lower()
    ser.write(text_to_send.encode())
    print(f"Sent to Arduino: {text_to_send}")

def main():
    # Set up serial connection to Arduino
    # Note: You may need to change the port name depending on your system
    ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
    time.sleep(2)  # Wait for the connection to establish

    print("Speech to Braille converter")
    print("Speak clearly into the microphone")
    
    while True:
        text = speech_to_text()
        if text:
            send_to_arduino(ser, text)

if __name__ == "__main__":
    main()