#ifndef _ED_FM_UTILITY_H_
#define _ED_FM_UTILITY_H_

#pragma once

#include <string>
#include <vector>

struct Vec3 
{
	Vec3(double x_ = 0, double y_ = 0, double z_ = 0) :x(x_), y(y_), z(z_) {};
	Vec3(const Vec3& rcopy) { x = rcopy.x; y = rcopy.y; z = rcopy.z;  };
	virtual ~Vec3() {};
	double x;
	double y;
	double z;
	inline Vec3& operator =(const Vec3& rLeft) { x = rLeft.x; y = rLeft.y; z = rLeft.z; return *this; }
	inline Vec3& operator +(const Vec3& rLeft) { x += rLeft.x; y += rLeft.y; z += rLeft.z; return *this; }
	inline Vec3& operator -(const Vec3& rLeft) { x -= rLeft.x; y -= rLeft.y; z -= rLeft.z; return *this; }
	inline Vec3& operator +=(const Vec3& rLeft) { x += rLeft.x; y += rLeft.y; z += rLeft.z; return *this; }
	inline Vec3& operator -=(const Vec3& rLeft) { x -= rLeft.x; y -= rLeft.y; z -= rLeft.z; return *this; }
	inline bool operator ==(const Vec3& rLeft) { if (x == rLeft.x && y == rLeft.y && z == rLeft.z) return true; return false; }
	inline bool operator !=(const Vec3& rLeft) { if (x != rLeft.x || y != rLeft.y || z != rLeft.z) return true; return false; }
	inline Vec3& operator *(const Vec3& b) // cross
	{
		x = y * b.z - z * b.y;   // (a2 b3 - a3 b2) i
		y = -x * b.z + z * b.x;  // - (a1 b3 - a3 b1 )j
		z =	x * b.y - y * b.x;   // (a1 b2 - a2 b1 ) k
	}
	// add multipication of constant 
	// 
	inline void clear() { setAxis(); }
	// 
	inline void setAxis(double x_ = 0, double y_ = 0, double z_ = 0)
	{
		x = x_;
		y = y_;
		z = z_;
	}
	inline void getAxis(double& _x, double& _y, double& _z)
	{
		_x = x;
		_y = y;
		_z = z;
	}
	/*/////////////////////////////////////////////////////////////////////////
function of force source in body axis
x,y,z			  - force components in body coordinate system
pos_x,pos_y,pos_z - position of force source in body coordinate system

body coordinate system system is always X - positive forward ,
										Y - positive up,
										Z - positive to right
*/
	inline void setEDAxis(double x_ = 0, double y_ = 0, double z_ = 0)
	{
		x = x_; 
		y = z_; 
		z = -y_; 
	}
	inline void getEDAxis(double& _x, double& _y, double& _z)
	{
		_x = x;
		_y = -z;
		_z = y;
	}
};

inline Vec3 cross(const Vec3 & a, const Vec3 & b)
{
	return Vec3 (a.y * b.z - a.z * b.y,   // (a2 b3 - a3 b2) i
				 -a.x * b.z + a.z * b.x,  // - (a1 b3 - a3 b1 )j
				 a.x * b.y - a.y * b.x);  // (a1 b2 - a2 b1 ) k
}

inline Vec3 EDcross(const Vec3 & a, const Vec3 & b)
{
	return Vec3 (a.y * b.z - a.z * b.y,
				 a.z * b.x - a.x * b.z,
				 a.x * b.y - a.y * b.x);
}

// only slightly faster than struct construct&copy when inlined
inline void clear_vec3(Vec3& dest)
{
	dest.x = 0;
	dest.y = 0;
	dest.z = 0;
}

// TODO: replace with operator+=() in Vec3
inline void sum_vec3(Vec3& dest, const Vec3& src)
{
	dest.x += src.x;
	dest.y += src.y;
	dest.z += src.z;
}

// TODO: replace with operator-=() in Vec3
inline void dec_vec3(Vec3& dest, const Vec3& src)
{
	dest.x -= src.x;
	dest.y -= src.y;
	dest.z -= src.z;
}


struct Matrix33
{
	Vec3 x;
	Vec3 y;
	Vec3 z;
};

struct Quaternion
{
	double x;
	double y;
	double z;
	double w;
};

class Matrix
{
	int m_row;
	int m_col;
	double* m_pData = NULL;
	//std::vector<std::vector<double>> data;
public:
	Matrix(size_t row, size_t col) : m_row(row), m_col(col) { m_pData = new double[row * col]; memset(m_pData, 0, row * col); };
	virtual ~Matrix() { delete[]m_pData; };
	int rows() { return m_row; };
	int columns() { return m_col; };
	bool set(size_t row, size_t col, double val) { 
		if (row > m_row || col > m_col) 
			return false;
		m_pData[col + (row * m_col)] = val;
		return true;
	}
	double get(size_t row, size_t col) { 
		if (row > m_row || col > m_col) 
			return 0.0;
		return m_pData[col + (row * m_col)];
	}
//	const std::vector<double>& operator[](int i) { return data[i]; };
	double operator[](int i) { return m_pData[i]; }
	void operator-(double val) {
		auto total = m_row * m_col;
		for (int i=0; i < total ; i++)
			m_pData[i] -= val;
	};
	void operator+(double val) { 
		auto total = m_row * m_col;
		for (int i=0; i < total; i++)
			m_pData[i] += val;
	};
	void operator*(double val ) {
		auto total = m_row * m_col;
		for (int i=0; i < total; i++)
			m_pData[i] *= val;
	};
	Matrix operator*(Matrix& B) {
		Matrix result(rows(), B.columns());
		int row = 0;
		int col = 0;
		double temp = 0.0;
		for (size_t Ri = 0; Ri < rows(); Ri++)
		{
			for (size_t Rj = 0; Rj < B.columns(); Rj++)
			{
				temp = 0.0;
				for (size_t j = 0; j < columns(); j++)
					temp += get(Ri,j) * B.get(Rj, j);
				//
				result.set(Ri, Rj, temp);
			}
		};
		return result;
	};
};

class _Matrix33
{
	std::vector<std::vector<double>> data;
public:
	static Matrix33 quaternion_to_matrix(const Quaternion& v)
	{
		Matrix33 mtrx;
		double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
		x2 = v.x + v.x;
		y2 = v.y + v.y;
		z2 = v.z + v.z;
		xx = v.x * x2;   xy = v.x * y2;   xz = v.x * z2;
		yy = v.y * y2;   yz = v.y * z2;   zz = v.z * z2;
		wx = v.w * x2;   wy = v.w * y2;   wz = v.w * z2;


		mtrx.x.x = 1.0 - (yy + zz);
		mtrx.x.y = xy + wz;
		mtrx.x.z = xz - wy;

		mtrx.y.x = xy - wz;
		mtrx.y.y = 1.0 - (xx + zz);
		mtrx.y.z = yz + wx;

		mtrx.z.x = xz + wy;
		mtrx.z.y = yz - wx;
		mtrx.z.z = 1.0 - (xx + yy);

		return mtrx;
	};
};


class Utils
{
public:
	static double lerp(double* x, double* f, unsigned sz, double t)
	{
		for (unsigned i = 0; i < sz; i++)
		{
			if (t <= x[i])
			{
				if (i > 0)
				{
					return ((f[i] - f[i - 1]) / (x[i] - x[i - 1]) * t +
						(x[i] * f[i - 1] - x[i - 1] * f[i]) / (x[i] - x[i - 1]));
				}
				return f[0];
			}
		}
		return f[sz - 1];
	};


	// Simple upper and lower limiter
	static double limit(double input, double lower_limit, double upper_limit)
	{
		if (input > upper_limit)
		{
			return upper_limit;
		}
		else if (input < lower_limit)
		{
			return lower_limit;
		}
		else
		{
			return input;
		};
	};
};
#endif