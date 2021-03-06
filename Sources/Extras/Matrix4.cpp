
#include "Matrix4.h"

Matrix4 Matrix4::setOrthoFrustum(float l, float r, float b, float t, float n, float f)
{
	Matrix4 mat;
	mat[0] = 2 / (r - l);
	mat[5] = 2 / (t - b);
	mat[10] = -2 / (f - n);
	mat[12] = -(r + l) / (r - l);
	mat[13] = -(t + b) / (t - b);
	mat[14] = -(f + n) / (f - n);
	return mat;
}

Matrix4 Matrix4::setFrustum(float l, float r, float b, float t, float n, float f)
{
	Matrix4 mat;
	mat[0] = 2 * n / (r - l);
	mat[5] = 2 * n / (t - b);
	mat[8] = (r + l) / (r - l);
	mat[9] = (t + b) / (t - b);
	mat[10] = -(f + n) / (f - n);
	mat[11] = -1;
	mat[14] = -(2 * f * n) / (f - n);
	mat[15] = 0;
	return mat;
}

Matrix4 Matrix4::setFrustum(float fovY, float aspect, float n, float f)
{
	float tangent = tanf(fovY / 2 * DEG_TO_RAD);
	float height = f * tangent;
	float width = height * aspect;

	return setFrustum(-width, width, -height, height, f, n);
}

Matrix4& Matrix4::setScale(GLfloat x, GLfloat y, GLfloat z)
{
	printf("SCALING DONT WORK");
	/*m[0] = Sign(m[0])*x;
	m[1] = Sign(m[1])*x;
	m[2] = Sign(m[2])*x;

	m[4] = Sign(m[4])*y;
	m[5] = Sign(m[5])*y;
	m[6] = Sign(m[6])*y;

	m[8] = Sign(m[8])*z;
	m[9] = Sign(m[9])*z;
	m[10] = Sign(m[10])*z;*/

	return *this;
}

Matrix4& Matrix4::setRotation(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	float vecLength, sinSave, cosSave, oneMinusCos;
	float xx, yy, zz, xy, yz, zx, xs, ys, zs;

	// If NULL vector passed in, this will blow up...
	if (x == 0.0f && y == 0.0f && z == 0.0f)
	{
		makeIdentity();
		return *this;
	}

	// Scale vector
	vecLength = (float)sqrt(x*x + y*y + z*z);

	// Rotation matrix is normalized
	x /= vecLength;
	y /= vecLength;
	z /= vecLength;

	sinSave = (float)sin(angle);
	cosSave = (float)cos(angle);
	oneMinusCos = 1.0f - cosSave;

	xx = x * x;
	yy = y * y;
	zz = z * z;
	xy = x * y;
	yz = y * z;
	zx = z * x;
	xs = x * sinSave;
	ys = y * sinSave;
	zs = z * sinSave;

	m[0] = (oneMinusCos * xx) + cosSave;
	m[4] = (oneMinusCos * xy) - zs;
	m[8] = (oneMinusCos * zx) + ys;
	m[12] = 0.0f;

	m[1] = (oneMinusCos * xy) + zs;
	m[5] = (oneMinusCos * yy) + cosSave;
	m[9] = (oneMinusCos * yz) - xs;
	m[13] = 0.0f;

	m[2] = (oneMinusCos * zx) - ys;
	m[6] = (oneMinusCos * yz) + xs;
	m[10] = (oneMinusCos * zz) + cosSave;
	m[14] = 0.0f;

	m[3] = 0.0f;
	m[7] = 0.0f;
	m[11] = 0.0f;
	m[15] = 1.0f;

	return *this;
}

Matrix4& Matrix4::TranslateMatrix(GLfloat x, GLfloat y, GLfloat z)
{
	Matrix4 matrix;
	matrix.setTranslation(x, y, z);

	*this = matrix * (*this);
	return *this;
}

Matrix4& Matrix4::RotateMatrix(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	Matrix4 matrix;
	matrix.setRotation(angle, x, y, z);

	*this = matrix * (*this);
	return *this;
}

Matrix4& Matrix4:: operator*=(const Matrix4& n)
{
	*this = *this*n;
	return *this;
}

Matrix4 Matrix4:: operator*(const Matrix4& n) const
{
	Matrix4 matrix;
	matrix[0] = m[0] * n[0] + m[4] * n[1] + m[8] * n[2] + m[12] * n[3];
	matrix[4] = m[0] * n[4] + m[4] * n[5] + m[8] * n[6] + m[12] * n[7];
	matrix[8] = m[0] * n[8] + m[4] * n[9] + m[8] * n[10] + m[12] * n[11];
	matrix[12] = m[0] * n[12] + m[4] * n[13] + m[8] * n[14] + m[12] * n[15];

	matrix[1] = m[1] * n[0] + m[5] * n[1] + m[9] * n[2] + m[13] * n[3];
	matrix[5] = m[1] * n[4] + m[5] * n[5] + m[9] * n[6] + m[13] * n[7];
	matrix[9] = m[1] * n[8] + m[5] * n[9] + m[9] * n[10] + m[13] * n[11];
	matrix[13] = m[1] * n[12] + m[5] * n[13] + m[9] * n[14] + m[13] * n[15];

	matrix[2] = m[2] * n[0] + m[6] * n[1] + m[10] * n[2] + m[14] * n[3];
	matrix[6] = m[2] * n[4] + m[6] * n[5] + m[10] * n[6] + m[14] * n[7];
	matrix[10] = m[2] * n[8] + m[6] * n[9] + m[10] * n[10] + m[14] * n[11];
	matrix[14] = m[2] * n[12] + m[6] * n[13] + m[10] * n[14] + m[14] * n[15];

	matrix[3] = m[3] * n[0] + m[7] * n[1] + m[11] * n[2] + m[15] * n[3];
	matrix[7] = m[3] * n[4] + m[7] * n[5] + m[11] * n[6] + m[15] * n[7];
	matrix[11] = m[3] * n[8] + m[7] * n[9] + m[11] * n[10] + m[15] * n[11];
	matrix[15] = m[3] * n[12] + m[7] * n[13] + m[11] * n[14] + m[15] * n[15];

	return matrix;
}

Matrix4& Matrix4::MultiplyMatrices(Matrix4& m, Matrix4& n, Matrix4& result)
{
	result[0] =  m[0] * n[0] + m[4] *  n[1] + m[8] *  n[2] +  m[12] * n[3];
	result[4] =  m[0] * n[4] + m[4] *  n[5] + m[8] *  n[6] +  m[12] * n[7];
	result[8] =  m[0] * n[8] + m[4] *  n[9] + m[8] *  n[10] + m[12] * n[11];
	result[12] = m[0] * n[12] + m[4] * n[13] + m[8] * n[14] + m[12] * n[15];

	result[1] =  m[1] * n[0] +  m[5] * n[1] +  m[9] * n[2] +  m[13] * n[3];
	result[5] =  m[1] * n[4] +  m[5] * n[5] +  m[9] * n[6] +  m[13] * n[7];
	result[9] =  m[1] * n[8] +  m[5] * n[9] +  m[9] * n[10] + m[13] * n[11];
	result[13] = m[1] * n[12] + m[5] * n[13] + m[9] * n[14] + m[13] * n[15];

	result[2] =  m[2] * n[0] +  m[6] * n[1] +  m[10] * n[2] +  m[14] * n[3];
	result[6] =  m[2] * n[4] +  m[6] * n[5] +  m[10] * n[6] +  m[14] * n[7];
	result[10] = m[2] * n[8] +  m[6] * n[9] +  m[10] * n[10] + m[14] * n[11];
	result[14] = m[2] * n[12] + m[6] * n[13] + m[10] * n[14] + m[14] * n[15];

	result[3] =  m[3] * n[0] +  m[7] * n[1] +  m[11] * n[2] +  m[15] * n[3];
	result[7] =  m[3] * n[4] +  m[7] * n[5] +  m[11] * n[6] +  m[15] * n[7];
	result[11] = m[3] * n[8] +  m[7] * n[9] +  m[11] * n[10] + m[15] * n[11];
	result[15] = m[3] * n[12] + m[7] * n[13] + m[11] * n[14] + m[15] * n[15];

	return result;
}