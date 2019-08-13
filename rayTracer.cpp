#define _USE_MATH_DEFINES

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <vector>

class Vector3D
{
public:
	Vector3D() {};
	Vector3D(float x, float y, float z) : x(x), y(y), z(z) {};

	Vector3D operator-(const Vector3D &v) const
	{
		return Vector3D(x - v.x, y - v.y, z - v.z);
	}

	Vector3D &operator-=(const Vector3D &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3D operator+(const Vector3D &v) const
	{
		return Vector3D(x + v.x, y + v.y, z + v.z);
	}

	Vector3D &operator+=(const Vector3D &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector3D operator*(const float f) const
	{
		return Vector3D(x * f, y * f, z * f);
	}

	Vector3D &operator*(const Vector3D &v) const
	{
		return Vector3D(x * v.x, y * v.y, z * v.z);
	}

	Vector3D &operator*=(const Vector3D &v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	Vector3D &operator*=(const float f)
	{
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	bool operator==(const Vector3D &v) const
	{
		if (x == v.x && y == v.y && z == v.z)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	float magnitude()
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vector3D &normalize()
	{
		float m = magnitude();
		x /= m;
		y /= m;
		z /= m;
		return *this;
	}

	float dotProduct(const Vector3D &v) const
	{
		return x * v.x + y * v.y;
	}

	Vector3D cross(const Vector3D &v) const
	{
		float tempX = y * v.z - z * v.y;
		float tempY = z * v.x - x * v.z;
		float tempZ = x * v.y - y * v.x;

		return Vector3D(tempX, tempY, tempZ);
	}

	void print()
	{
		if (!isnan(x) && !isnan(y) && !isnan(z))
		{
			printf("X - %f, Y - %f, Z - %f\n", x, y, z);
		}
	}
public:
	float x;
	float y;
	float z;
};

class Line
{
public:
	Line() {};
	Line(const Vector3D &p1, const Vector3D &p2) : m_p1(p1), m_p2(p2), m_direction(m_p2 - m_p1) {};

	Vector3D intersect(const Line &l)
	{
		if (isIntersect(l))
		{
			Vector3D a1 = (l.m_p1 - m_p1).cross(l.m_direction);
			Vector3D a2 = (l.m_p1 - m_p1).cross(m_direction);
			Vector3D b = m_direction.cross(l.m_direction);

			float t = 0;
			float u = 0;
			if (b.x != 0)
			{
				t = a1.x / b.x;
				u = a2.x / b.x;
			}
			else if (b.y != 0)
			{
				t = a1.y / b.y;
				u = a2.y / b.y;
			}
			else if (b.z != 0)
			{
				t = a1.z / b.z;
				u = a2.z / b.z;
			}

			if ((t > 1.0f || t < 0.0f) || (u > 1.0f || u < 0.0f))
			{
				return Vector3D(NAN, NAN, NAN);
			}

			Vector3D v = m_p1 + (m_direction * t);

			return v;
		}
		else
		{
			return Vector3D(NAN, NAN, NAN);
		}
	}
	float distanceToPoint(const Vector3D &point)
	{
		Vector3D c = point - m_p1;
		Vector3D v = (m_p2 - m_p1).normalize();
		float d = (m_p2 - m_p1).magnitude();

		float t = v.dotProduct(c);

		if (t < 0)
		{
			return (point - m_p1).magnitude();
		}
		if (t > d)
		{
			return (point - m_p2).magnitude();
		}

		v *= t;

		return (m_p1 + v - point).magnitude();
	}
	void print()
	{
		printf("<<");
		m_p1.print();
		printf("><");
		m_p2.print();
		printf(">>\n");
	}
public:
	Vector3D m_p1;
	Vector3D m_p2;
	Vector3D m_direction;
private:
	bool isIntersect(const Line &l)
	{
		Vector3D v = m_direction.cross(l.m_direction);

		if (v.x == 0 && v.y == 0 && v.z == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
};

class Circle
{
public:
	Circle() {};
	Circle(const Vector3D &pos, float r) : pos(pos), r(r) {};

	void placePoints(int amount)
	{
		float step = 2.0f * M_PI / (float)amount;
		for (float i = 0; i < M_PI * 2.0f; i += step)
		{
			Vector3D point = placePoint2D(i);
			Vector3D normal = normal2D(point);

			Line l(point, normal);
			circleLines.push_back(l);
		}
	}

	void intersectPoints(std::vector<Line> &lines)
	{
		for (int i = 0; i < lines.size(); i++)
		{
			Line &l = lines[i];

			float minDistanceA = (l.m_p2 - l.m_p1).magnitude();
			float minDistanceB = (l.m_p1 - l.m_p2).magnitude();
			for (int j = 0; j < circleLines.size(); j++)
			{
				Line &circleLine = circleLines[j];

				Vector3D intersectPoint = circleLine.intersect(l);

				if (!isnan(intersectPoint.x) && !isnan(intersectPoint.y) && !isnan(intersectPoint.z))
				{
					circleLine.m_p2 = intersectPoint;

					float vectorMagnitudeA = (intersectPoint - l.m_p1).magnitude();
					if (vectorMagnitudeA < minDistanceA)
					{
						l.m_p1 = intersectPoint;

						minDistanceA = vectorMagnitudeA;
					}

					float vectorMagnitudeB = (intersectPoint - l.m_p2).magnitude();
					if (vectorMagnitudeB < minDistanceB)
					{
						l.m_p2 = intersectPoint;

						minDistanceB = vectorMagnitudeB;
					}
				}
			}
		}

		for (int i = 0; i < lines.size(); i++)
		{
			Line l = lines[i];
			l.print();
		}
	}
public:
	Vector3D pos;
	float r;
	std::vector<Line> circleLines;
private:
	Vector3D placePoint2D(float theta) const // 0 -> 2pi
	{
		float tempX = pos.x + r * cos(theta);
		float tempY = pos.y + r * sin(theta);

		return Vector3D(tempX, tempY, 0.0f);
	}

	Vector3D normal2D(const Vector3D &point) const // don't need to normalize
	{
		float tempX = 2 * (point.x - pos.x);
		float tempY = 2 * (point.y - pos.y);

		return Vector3D(tempX, tempY, 0.0f);
	}
};


void test()
{
	Circle c(Vector3D(0.0f, 0.0f, 0.0f), 5.0f);
	c.placePoints(10);

	Line l1(Vector3D(10.0f, -7.0f, 0.0f), Vector3D(10.0f, 7.0f, 0.0f));
	Line l2(Vector3D(10.0f, -7.0f, 0.0f), Vector3D(-10.0f, -7.0f, 0.0f));
	//Line l3(Vector3D(10.0f, -7.0f, 0.0f), Vector3D(10.0f, 7.0f, 0.0f));
	//Line l4(Vector3D(-10.0f, -7.0f, 0.0f), Vector3D(-10.0f, 7.0f, 0.0f));
	std::vector<Line> lines;
	lines.push_back(l1);
	lines.push_back(l2);
	//lines.push_back(l3);
	//lines.push_back(l4);

	// printf("Before:\n");
	// for (int i = 0; i < c.circleLines.size(); i++)
	// {
	// 	Line l = c.circleLines[i];
	// 	l.print();
	// }

	c.intersectPoints(lines);

	// printf("After:\n");
	// for (int i = 0; i < c.circleLines.size(); i++)
	// {
	// 	Line l = c.circleLines[i];
	// 	l.print();
	// }
}

int main()
{
	test();
	// testDistance();
	// testDistanceOnLine();
}