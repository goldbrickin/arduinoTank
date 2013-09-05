import serial, time

arduino = serial.Serial('/dev/ttyAMA0', 9600, timeout=1)

arduino.open()

try:
	arduino.write('f10')
	response = arduino.readline()
	print response
		
except KeyboardInterrupt:
	arduino.close()
