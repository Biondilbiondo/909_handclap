PK2CMD = /home/mattia/Downloads/pk2cmd-x86_64.AppImage

all: 909_noise.hex
909_noise.o: 909_noise.asm
	gpasm -c -p p12f675 909_noise.asm
909_noise.hex: 909_noise.o
	gplink -m -c 909_noise.o -o 909_noise.hex
clean:
	rm -f 909_noise.cod  909_noise.cof  909_noise.hex  909_noise.lst  909_noise.map  909_noise.o
flash: 909_noise.hex
	$(PK2CMD) -P -F 909_noise.hex -M -T -R 

