//
//  Transform.cpp
//  OpenGLTesting
//
//  Created by Hakob on 11/7/14.
//  Copyright (c) 2014 Haksist. All rights reserved.
//

#include <iostream>
#include <math.h>
#include "Transform.h"

using namespace std;

Transform::~Transform()
{
	cout << "~Transform()" << endl;
}

Matrix4 Transform::getMatrix(bool calcScale)
{
	Vector3 _Right = Right();
	matrix[0] = _Right[0];
	matrix[1] = _Right[1];
	matrix[2] = _Right[2];
	matrix[3] = 0.0f;

	matrix[4] = _Up[0];
	matrix[5] = _Up[1];
	matrix[6] = _Up[2];
	matrix[7] = 0.0f;

	matrix[8] = _Forward[0];
	matrix[9] = _Forward[1];
	matrix[10] = _Forward[2];
	matrix[11] = 0.0f;

	matrix[12] = Location[0];
	matrix[13] = Location[1];
	matrix[14] = Location[2];
	matrix[15] = 1.0f;

	if (calcScale)
	{
		matrix[0] *= ScaleLocal[0];
		matrix[1] *= ScaleLocal[0];
		matrix[2] *= ScaleLocal[0];

		matrix[4] *= ScaleLocal[1];
		matrix[5] *= ScaleLocal[1];
		matrix[6] *= ScaleLocal[1];

		matrix[8] *= ScaleLocal[2];
		matrix[9] *= ScaleLocal[2];
		matrix[10] *= ScaleLocal[2];
	}

	return matrix;
}

void Transform::applyTransformation()
{
	getMatrix(true);
	glMultMatrixf(&matrix[0]);
}

void Transform::TranslateForward(GLfloat step)
{
	Location[0] += _Forward[0] * step;
	Location[1] += _Forward[1] * step;
	Location[2] += _Forward[2] * step;
}

void Transform::TranslateUp(GLfloat step)
{
	Location[0] += _Up[0] * step;
	Location[1] += _Up[1] * step;
	Location[2] += _Up[2] * step;
}

void Transform::TranslateRight(GLfloat step)
{
	Vector3 R = Right();
	Location[0] += R[0] * step;
	Location[1] += R[1] * step;
	Location[2] += R[2] * step;
}

void Transform::RotateY(GLfloat Angle)
{
	matrix.setRotation(Angle*DEG_TO_RAD, _Up[0], _Up[1], _Up[2]);
	_Forward.rotateVector(matrix);
}

void Transform::RotateZ(GLfloat Angle)
{
	matrix.setRotation(Angle*DEG_TO_RAD, _Forward[0], _Forward[1], _Forward[2]);
	_Up.rotateVector(matrix);
}

void Transform::RotateX(GLfloat Angle)
{
	Vector3 xAxis = Right();
	matrix.setRotation(Angle*DEG_TO_RAD, xAxis[0], xAxis[1], xAxis[2]);

	_Forward.rotateVector(matrix);
	_Up.rotateVector(matrix);
}

void Transform::Translate(GLfloat x, GLfloat y, GLfloat z)
{
	getMatrix();
	matrix.TranslateMatrix(x, y, z);

	memcpy(&Location[0], &matrix[12], sizeof(GLfloat)* 3);
}

void Transform::Rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	matrix.setRotation(angle*DEG_TO_RAD, x, y, z);

	_Up.rotateVector(matrix);
	_Forward.rotateVector(matrix);
}

void Transform::SetRotation(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	matrix.setRotation(angle*DEG_TO_RAD, x, y, z);

	memcpy(&_Up[0], &matrix[4], sizeof(GLfloat)* 3);
	memcpy(&_Forward[0], &matrix[8], sizeof(GLfloat)* 3);
}