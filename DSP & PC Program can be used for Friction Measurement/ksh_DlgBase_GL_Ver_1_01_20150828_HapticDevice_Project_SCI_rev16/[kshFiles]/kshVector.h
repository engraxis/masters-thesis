#ifndef __KSHVECTOR_H__
#define __KSHVECTOR_H__



class lwtVector {
public:

	lwtVector(void);
	lwtVector(double, double, double);
	lwtVector(const lwtVector&);
	
	~lwtVector(void);
	

	inline double x(void) {return vector[0];}
	inline double y(void) {return vector[1];}
	inline double z(void) {return vector[2];}
	lwtVector setx(double x);
	lwtVector sety(double y);
	lwtVector setz(double z);
	
	
	double norm(void) const;
	lwtVector& normalize(void) {return *this /= norm();}
	double dot(const lwtVector&) const;


	lwtVector operator*(const lwtVector&) const;
	lwtVector operator*(double) const;
	lwtVector operator/(double) const;
	lwtVector operator+(const lwtVector&) const;
	lwtVector operator-(const lwtVector&) const;
	lwtVector operator-(void) const;
	lwtVector& operator*=(const lwtVector&);
	lwtVector& operator*=(double);
	lwtVector& operator/=(double);
	lwtVector& operator+=(const lwtVector&);
	lwtVector& operator-=(const lwtVector&);
	lwtVector& operator=(const lwtVector&);
	int operator==(const lwtVector&) const;
	int operator!=(const lwtVector&) const;
	int operator<(const lwtVector& right) const;
	int operator>(const lwtVector& right) const;
	int operator<=(const lwtVector& right) const;
	int operator>=(const lwtVector& right) const;
	inline double& operator[](int i) {return vector[i];}	

protected:
	double vector[3];
};

lwtVector operator*(double, const lwtVector&);



#endif