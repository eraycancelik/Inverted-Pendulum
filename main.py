import serial

ser = serial.Serial('/dev/serial/by-id/usb-Teensyduino_USB_Serial_13406730-if00', 115200, timeout=2)

while True:
    line = ser.readline().decode('utf-8').strip()
    if line:
        print(line)

