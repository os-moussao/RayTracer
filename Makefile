all: raytracer
	./raytracer

raytracer: RayTracer.cpp
	clang++ $(CFLAGS) RayTracer.cpp -o $@

clean:
	rm -f raytracer
	rm -f pixel_map.ppm

re: clean all

CFLAGS = -Wall -Werror -Wextra -Wshadow