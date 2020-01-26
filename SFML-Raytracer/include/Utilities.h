#pragma once
#include <cmath>

namespace AA
{
	class Vec
	{
	public:
		double _x, _y, _z;
		Vec() : _x(0), _y(0), _z(0) { }
		Vec(double x, double y, double z) : _x(x), _y(y), _z(z) { }
		Vec operator - (Vec rh) const { return Vec(_x - rh._x, _y - rh._y, _z - rh._z); }
		Vec operator + (Vec rh) const { return Vec(_x + rh._x, _y + rh._y, _z + rh._z); }
		Vec operator * (double rh) const { return Vec(_x * rh, _y * rh, _z * rh); }
		Vec operator / (double rh) const { return Vec(_x / rh, _y / rh, _z / rh); }
	};

	class Ray
	{
	public:
		Vec _startPos;
		Vec _dir;
		Ray(Vec startPos, Vec dir) : _startPos(startPos), _dir(dir) { }
	};

	class Sphere
	{
	public:
		Vec _origin;
		double _radius;
		sf::Color _col;
		Sphere(Vec o, double r, sf::Color col) : _origin(o), _radius(r), _col(col) { }
	};

	class ColourArray
	{
	public:
		ColourArray() = delete;
		ColourArray(int rows, int columns) : _rows(rows), _columns(columns), _colours(std::vector<sf::Color>(rows * columns, sf::Color(0,0,0,255))) { }

		sf::Color& GetColourAtPosition(int x, int y)
		{
			return _colours.at(y * _columns + x);
		}

		void ColourPixelAtPosition(int x, int y, sf::Color col)
		{
			_colours.at(y * _columns + x) = col;
		}

		void* GetDataBasePointer()
		{
			return reinterpret_cast<void*>(_colours.data());
		}

		size_t GetDataSize()
		{
			return _colours.size() * sizeof(sf::Color);
		}

	private:
		int _rows;
		int _columns;

		std::vector<sf::Color> _colours;
	};

	static double DotProduct(Vec a, Vec b)
	{
		return ( (a._x * b._x) + (a._y * b._y) + (a._z * b._z) );
	}

	static bool RayIntersectSphere(Ray ray, Sphere sphere, double& t)
	{
		Vec sphereToRay = ray._startPos - sphere._origin;
		Vec rayToSphere = ray._dir;
		double b = 2 * DotProduct(sphereToRay, rayToSphere);			
		double c = DotProduct(sphereToRay, sphereToRay) - (sphere._radius * sphere._radius);
		double delta = b * b - 4 * c;	//Quick test of the appropraite part of the Quad formula to see if it intersects at all
		if (delta < 0 ) { return false; }	//Ray didn't intersect with the sphere at all
		else
		{
			// Find the points that it intersects at
			delta = sqrt(delta);
			double t0 = -b - delta;
			double t1 = -b + delta;

			//Store the distance the of the first intersection in t
			//Use this later to scrub along the ray to sphere to work out where we need to calc normals.
			t = t0 < t1 ? t0 : t1;
			return true;
		}
	}
}
