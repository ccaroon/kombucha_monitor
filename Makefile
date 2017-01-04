kombucha_monitor.bin: main.ino lib/*.h lib/*.cpp
	particle compile photon . --saveTo kombucha_monitor.bin

flash: kombucha_monitor.bin
	particle flash Greebo kombucha_monitor.bin

clean:
	rm -f kombucha_monitor.bin
