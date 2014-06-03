ARDUINO_MODEL=uno #Can be find with the command 'ino list-models'

all:
	ino build -m $(ARDUINO_MODEL) > /dev/null
	ino upload -m $(ARDUINO_MODEL)

clean:
	ino clean
