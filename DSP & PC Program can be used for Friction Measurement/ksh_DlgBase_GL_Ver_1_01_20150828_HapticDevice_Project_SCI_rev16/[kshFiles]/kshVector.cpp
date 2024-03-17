#include <math.h>
#include "kshVector.h"


lwtVector::lwtVector(void) {
	vector[0] = 0.0; 
	vector[1] = 0.0; 
	vector[2] = 0.0;
}

lwtVector::lwtVector(double x, double y, double z) {
	vector[0] = x; vector[1] = y; vector[2] = z;
}

lwtVector::lwtVector(const lwtVector& v) {
	vector[0] = v.vector[0]; vector[1] = v.vector[1]; vector[2] = v.vector[2];
}

lwtVector::~lwtVector(void) {

}







lwtVector lwtVector::setx(double x) {
	vector[0] = x;
	return *this;
}

lwtVector lwtVector::sety(double y) {
	vector[1] = y;
	return *this;
}

lwtVector lwtVector::setz(double z) {
	vector[2] = z;
	return *this;
}









double lwtVector::norm(void) const {
	return sqrt((vector[0]*vector[0]) + (vector[1]*vector[1]) + (vector[2]*vector[2]));
}

double lwtVector::dot(const lwtVector& right) const {
	return vector[0] * right.vector[0] + vector[1] * right.vector[1] + vector[2] * right.vector[2];
}











lwtVector lwtVector::operator*(const lwtVector& right) const {
	// <x1, y1, z1> x <x2, y2, z2> = <y1*z2-y2*z1, z1*x2-z2*x1, x1*y2-x2*y1>
	return lwtVector(vector[1] * right.vector[2] - vector[2] * right.vector[1],
					 vector[2] * right.vector[0] - vector[0] * right.vector[2],
					 vector[0] * right.vector[1] - vector[1] * right.vector[0]);
}

lwtVector lwtVector::operator*(double right) const {
	return lwtVector(vector[0]*right, vector[1]*right, vector[2]*right);
}

lwtVector lwtVector::operator/(double right) const {
	return lwtVector(vector[0]/right, vector[1]/right, vector[2]/right);
}

lwtVector lwtVector::operator+(const lwtVector& right) const {
	return lwtVector(vector[0] + right.vector[0], vector[1] + right.vector[1], vector[2] + right.vector[2]);
}

lwtVector lwtVector::operator-(const lwtVector& right) const {
	return lwtVector(vector[0] - right.vector[0], vector[1] - right.vector[1], vector[2] - right.vector[2]);
}

lwtVector lwtVector::operator-(void) const {
	return lwtVector(-vector[0], -vector[1], -vector[2]);
}

lwtVector& lwtVector::operator*=(const lwtVector& right) {
	vector[0] = vector[1] * right.vector[2] - vector[2] * right.vector[1];
	vector[1] = vector[2] * right.vector[0] - vector[0] * right.vector[2];
	vector[2] = vector[0] * right.vector[1] - vector[1] * right.vector[0];
	return *this;
}

lwtVector& lwtVector::operator+=(const lwtVector& right) {
	vector[0] += right.vector[0]; 
	vector[1] += right.vector[1]; 
	vector[2] += right.vector[2];
	return *this;
}

lwtVector& lwtVector::operator-=(const lwtVector& right) {
	vector[0] -= right.vector[0]; 
	vector[1] -= right.vector[1]; 
	vector[2] -= right.vector[2];
	return *this;
}

lwtVector& lwtVector::operator*=(double right) {
	vector[0] *= right; 
	vector[1] *= right; 
	vector[2] *= right;
	return *this;
}

lwtVector& lwtVector::operator/=(double right) {
	vector[0] /= right; 
	vector[1] /= right; 
	vector[2] /= right;
	return *this;
}

lwtVector& lwtVector::operator=(const lwtVector& right) {
	vector[0] = right.vector[0]; 
	vector[1] = right.vector[1]; 
	vector[2] = right.vector[2];
	return *this;
}

int lwtVector::operator==(const lwtVector& right) const {
	return ((vector[0]==right.vector[0]) && (vector[1]==right.vector[1]) && (vector[2]==right.vector[2]));
}


int lwtVector::operator!=(const lwtVector& right) const {
	return !(*this == right);
}

int lwtVector::operator<(const lwtVector& right) const {
	return norm() < right.norm();
}

int lwtVector::operator>(const lwtVector& right) const {
	return norm() > right.norm();
}

int lwtVector::operator<=(const lwtVector& right) const {
	return norm() <= right.norm();
}

int lwtVector::operator>=(const lwtVector& right) const {
	return norm() >= right.norm();
}

lwtVector operator*(double left, const lwtVector& right) {
	return right * left;
}



