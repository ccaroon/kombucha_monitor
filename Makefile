kombucha_monitor.bin: project.properties lib/* src/*
	mkdir -p build/
	cp project.properties `find lib src -type f` build/
	particle compile photon build/ --saveTo kombucha_monitor.bin
	rm -rf build/

flash: kombucha_monitor.bin
	particle flash Greebo kombucha_monitor.bin

clean:
	rm -rf kombucha_monitor.bin build/
