led:led.c
	gcc -Wall led.c -I oled.h -I oledfont.h -o led -lbcm2835
clean:led
	rm led led.o
