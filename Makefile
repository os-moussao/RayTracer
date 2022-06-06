all: raytracer
	./raytracer
	open pixel_map.ppm

raytracer: RayTracer.cpp
	clang++ RayTracer.cpp -o $@

clean:
	rm -f raytracer
	rm -f pixel_map.ppm