/******************************************************************************
 * vector.h  header file for 2D vector Object
 * version   1.0
 *
 * NJRvector2d is a 2d [Mathemitics] vector which defines the attributes and
 * basic opertaion of vector.
 *
 * History:  Ver 1.0 Created 2010 Oct  by  W.T. Aries Chang
 ******************************************************************************/

#ifndef _NJRVECTOR2d_H
#define _NJRVECTOR2d_H

#include <NJR/Interfaces/ParticularVector.h>
#include <string>
#include <iostream>

// Predefine Praticular Vectors

class NJRvector2d
{

public:

	NJRvector2d();

	NJRvector2d(const double& dx, const double& dy);

	NJRvector2d(const NJRvector2d& vInit);

	explicit NJRvector2d(const char* cInit);

	explicit NJRvector2d(const std::string& sInit);

	NJRvector2d(const ParticularVector& pvInit);

	// Assigns dx, dy to x, y
	const NJRvector2d& Set
		(const double& dx, const double& dy);

	const NJRvector2d& SetX(const double& dx);

	const NJRvector2d& SetY(const double& dy);

	const NJRvector2d& SetPolar
		(const double& dRadius, const double& dThita);

	// Retrieves element x
	inline double x() const
	{
		return _x;
	};

	// Retrieves element y
	inline double y() const
	{
		return _y;
	};

	// Retrieves the length (magnitude) of this vector
	double length() const;

	// Retrieves the direction of this vector
	NJRvector2d direction() const;

	// Outputs the version and Programmer
	inline void info() const
	{
		std::cout
			<< " NJRvector Ver 1.0 Modified 2010 Sep by W.T. Aries Chang"
			<< std::endl;
	};

	/**************************************************************************
	 * Retrieves the new position of this vector after this vector has been
	 * rotated around the given vector gv.
	 * ------------------------------------------------------------------------
	 * Let u be the direction of gv and L be the length of gv. Rotates this
	 * vector along the unit vector u with rotation angle L about u and then
	 * retrieves the new position of this vector.
	 * ------------------------------------------------------------------------
	 * EFERENCE: Computer Graphics 2th Donald. Hearn and M. Pauline Baker page
	 *           419-420
	 **************************************************************************/
//	NJRvector2d RotateAround(const NJRvector2d& gv) const;

	/**************************************************************************
	 * Retrieves the new position of this vector
	 * after axial x has been replaced by nx
	 * and   axial y has been replaced by ny
	 **************************************************************************/
	NJRvector2d Trans
		(const NJRvector2d& nx, const NJRvector2d& ny)
		const;

	// Retrieves the projected vector on v of this vector
	NJRvector2d ProjectOn(const NJRvector2d& v) const;

	// Retrieves the dot product of this and input vector
	double Dot(const NJRvector2d& v) const;

	/**************************************************************************
	 * Assignment Operators of NJRvector2d
	 * NJRvector2d & operator = (char char *cAval)
	 * Assigns a input vector to this vector
	 * the vector's form is "(a,b,c)" where a,b,c are constants.
	 *
	 * NOTICE: It does not allow any char which exists between " and ( or ).
	 *
	 * For example: NJRvector2d v="(a,b,c)";
	 * the function will decompose the string "(a,b,c)" into 3 double variable
	 * and assigns them to this vector.
	 **************************************************************************/

	const NJRvector2d& operator = (const char* cAval);

	const NJRvector2d& operator = (const std::string& sAval);

	const NJRvector2d& operator = (const ParticularVector& pvAval);

	const NJRvector2d& operator = (const NJRvector2d& vAval);

	const NJRvector2d& operator += (const NJRvector2d& v);

	const NJRvector2d& operator -= (const NJRvector2d& v);

	//	Arithmetic Operators of NJRvector2d
	NJRvector2d operator + (const NJRvector2d& v) const;

	NJRvector2d operator - (const NJRvector2d& v) const;

	//	Retrieves the dot product of left and right operands of %
	double operator % (const NJRvector2d& v) const;

  	// Retrieves a vector which is a result of this vector
	// multiply by the given scalar ds
	NJRvector2d operator * (const double& ds) const;

	//	Retrieves the projected vector on v of this vector
	NJRvector2d operator >> (const NJRvector2d& v) const;

	// Retrieves true if all elements of this vector equals to vRight's
	bool operator == (NJRvector2d& vRight) const ;

	void print () const;

private:

	// NJRvector2d: A 2d vector that has two elements x, y
	double _x;
	double _y;

};

/******************************************************************************
 * Retrieves a vector which is a result of this vector multiply by the given
 * scalar ds
 ******************************************************************************/

NJRvector2d operator * (const double& ds, const NJRvector2d& v);

NJRvector2d operator - (const NJRvector2d& v);

std::ostream& operator << (std::ostream& os, const NJRvector2d& v);

std::istream& operator >> (std::istream& is, NJRvector2d& v);

#endif // _NJRVECTOR2d_H
