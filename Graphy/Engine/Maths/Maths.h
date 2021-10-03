#pragma once

#define PI 3.14159265358979323846f
class vec2
{
	friend vec2 operator+(const vec2& left, const vec2& right);
	friend vec2 operator-(const vec2& left, const vec2& right);
	friend vec2 operator*(const vec2& left, const vec2& right);
	friend vec2 operator/(const vec2& left, const vec2& right);

	friend vec2 operator+(const vec2& left, float scalar);
	friend vec2 operator*(const vec2& left, float scalar);
	friend vec2 operator/(const vec2& left, float scalar);


public:
	vec2() = default;
	vec2(float x_, float y_) : x(x_), y(y_) { }

	float x;
	float y;

	vec2 normalize();
	float magnitude();

	bool operator==(const vec2& other) const;
};

class vec3
{
	friend vec3 operator+(const vec3& left, const vec3& right);
	friend vec3 operator-(const vec3& left, const vec3& right);
	friend vec3 operator*(const vec3& left, const vec3& right);
	friend vec3 operator/(const vec3& left, const vec3& right);

	friend vec3 operator*(const vec3& left, float scalar);
	friend vec3 operator/(const vec3& left, float scalar);

public:
	vec3() = default;
	vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) { }

	union { float x, r; };
	union { float y, g; };
	union { float z, b; };

	vec3 normalize();
	float magnitude();

	bool operator==(const vec3& other) const;
};

static float Clamp(float value, float min, float max)
{
	if (value > max) value = max;
	else if (value < min) value = min;
	return value;
}

static float degToRad(float deg)
{
	return deg * (double)(PI / 180.0f);
}

static float radToDeg(float rad)
{
	return rad * (double)(180 / PI);
}

static unsigned long createHex(int r, int g, int b)
{
	return ((r << 16) | (g << 8) | b);
}

// 0xrrggbb
static unsigned long createHex(const vec3& color)
{
	return (int(color.r * 255) << 16 | int(color.g * 255) << 8 | int(color.b * 255));
}

static vec3 createRGB(unsigned long hex)
{
	return vec3(float((hex >> 16) & 0xff), float((hex >> 8) & 0xff), float((hex) & 0xff)) / 255.0f;
}

static float distance(const vec2& left, const vec2& right)
{
	return sqrt((left.x - right.x) * (left.x - right.x) + (left.y - right.y) * (left.y - right.y));
}