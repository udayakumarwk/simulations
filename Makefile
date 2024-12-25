compile:
	cc -g -Wall -Wextra -o water_sim water_sim.c `sdl2-config --cflags --libs `

run: 
	cc -g -Wall -Wextra -o water_sim water_sim.c `sdl2-config --cflags --libs` && ./water_sim
