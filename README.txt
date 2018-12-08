----------------------------------------------------------------------------------------
PROBLEMS
There are no problems in my code, everything works perfectly 

----------------------------------------------------------------------------------------
EXTRA CREDITS
1. recursive reflection 	NO
2. good anti-aliasing		YES
3. soft shadows			YES

extra credits can be activated by setting their boolean value to true in the main() function

----------------------------------------------------------------------------------------
SOURCES
- http://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/ray-tracing-practical-example
To get an idea of the general algorithm

- http://web.cse.ohio-state.edu/~shen.94/681/Site/Slides_files/basic_algo.pdf
To calculate ray direction and ray-sphere intersection 

----------------------------------------------------------------------------------------
PLAN
main.cpp 
	void loadScene(const char* path)
	bool intersection(const Vec3 &rayorig, const Vec3 &raydir, const Object* &object, float &tnear)
	Vec3 trace(const Vec3 &rayorig, const Vec3 &raydir)
	int main()

Objects.h
	class Camera
		pos, fov, f, a
		Camera()

	class Object 
		amb, dif, spe, shi
		Object() 
		virtual Vec3 getNormal(Vec3 phit) const

	class Plane : public Object
		pos, nor
		Plane()
		bool intersect(const Vec3 &rayorig, const Vec3 &raydir, float &t) const
		Vec3 getNormal(Vec3 phit) const

	class Sphere : public Object
		pos, rad2
		Sphere() 
		bool intersect(const Vec3 &rayorig, const Vec3 &raydir, float &t0, float &t1) const		
		Vec3 getNormal(Vec3 phit) const

	class Light 
		pos, col, positions
		Light()
		std::vector<Vec3> getLightPositions()
		
Vec3.h
	class Vec3 
		x, y, z
		operators overloading
		
