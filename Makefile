kombucha_monitor.bin: project.properties main.ino lib/*
	particle compile photon . --saveTo kombucha_monitor.bin

flash: kombucha_monitor.bin
	particle flash Greebo kombucha_monitor.bin

clean:
	rm -f kombucha_monitor.bin
