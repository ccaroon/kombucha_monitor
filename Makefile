kombucha_monitor.bin: main.ino lib/*
	mkdir -p dist
	cp main.ino lib/* project.properties dist/
	particle compile photon dist/ --saveTo kombucha_monitor.bin
	rm -rf dist/

flash: kombucha_monitor.bin
	particle flash Greebo kombucha_monitor.bin

clean:
	rm -rf kombucha_monitor.bin dist/
