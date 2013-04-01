/******************************************************************************
 * std::vector.h  header file for 2D std::vector Object
 * version   1.0
 *
 * Vector2d is a 2d [Mathemitics] std::vector which defines the attributes and
 * basic opertaion of std::vector.
 *
 * History:  Ver 1.0 Created 2010 Oct  by  W.T. Aries Chang
 ******************************************************************************/

#ifndef _NJR_VECTOR_2D_H
#define _NJR_VECTOR_2D_H

#include <NJR/Interfaces/Constants.h>
#include <string>
#include <iostream>

namespace njr
{

// Predefine Praticular Vectors
class Vector2d
{

public:

	Vector2d();

	Vector2d(const double& dx, const double& dy);

	Vector2d(const Vector2d& vInit);

	explicit Vector2d(const char* cInit);

	explicit Vector2d(const std::string& sInit);

	Vector2d(const njrdxf::ParticularVector& pvInit);

	// Assigns dx, dy to x, y
	const Vector2d& Set
		(const double& dx, const double& dy);

	const Vector2d& SetX(const double& dx);

	const Vector2d& SetY(const double& dy);

	const Vector2d& SetPolar
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

	// Retrieves the length (magnitude) of this std::vector
	double length() const;

	// Retrieves the direction of this std::vector
	Vector2d direction() const;

	// Outputs the version and Programmer
	inline void info() const
	{
		std::cout
			<< " NJRvector Ver 1.0 Modified 2010 Sep by W.T. Aries Chang"
			<< std::endl;
	};

	/**************************************************************************
	 * Retrieves the new position of this std::vector after this std::vector has been
	 * rotated around the given std::vector gv.
	 * ------------------------------------------------------------------------
	 * Let u be the direction of gv and L be the length of gv. Rotates this
	 * std::vector along the unit std::vector u with rotation angle L about u and then
	 * retrieves the new position of this std::vector.
	 * ------------------------------------------------------------------------
	 * EFERENCE: Computer Graphics 2th Donald. Hearn and M. Pauline Baker page
	 *           419-420
	 **************************************************************************/
//	Vector2d RotateAround(const Vector2d& gv) const;

	/**************************************************************************
	 * Retrieves the new position of this std::vector
	 * after axial x has been replaced by nx
	 * and   axial y has been replaced by ny
	 **************************************************************************/
	Vector2d Trans
		(const Vector2d& nx, const Vector2d& ny)
		const;

	// Retrieves the projected std::vector on v of this std::vector
	Vector2d ProjectOn(const Vector2d& v) const;

	// Retrieves the dot product of this and input std::vector
	double Dot(const Vector2d& v) const;

	/**************************************************************************
	 * Assignment Operators of Vector2d
	 * Vector2d & operator = (char char *cAval)
	 * Assigns a input std::vector to this std::vector
	 * the std::vector's form is "(a,b,c)" where a,b,c are constants.
	 *
	 * NOTICE: It does not allow any char which exists between " and ( or ).
	 *
	 * For example: Vector2d v="(a,b,c)";
	 * the function will decompose the std::string "(a,b,c)" into 3 double variable
	 * and assigns them to this std::vector.
	 **************************************************************************/

	const Vector2d& operator = (const char* cAval);

	const Vector2d& operator = (const std::string& sAval);

	const Vector2d& operator = (const njrdxf::ParticularVector& pvAval);

	const Vector2d& operator = (const Vector2d& vAval);

	const Vector2d& operator += (const Vector2d& v);

	const Vector2d& operator -= (const Vector2d& v);

	//	Arithmetic Operators of Vector2d
	Vector2d operator + (const Vector2d& v) const;

	Vector2d operator - (const Vector2d& v) const;

	//	Retrieves the dot product of left and right operands of %
	double operator % (const Vector2d& v) const;

  	// Retrieves a std::vector which is a result of this std::vector
	// multiply by the given scalar ds
	Vector2d operator * (const double& ds) const;

	//	Retrieves the projected std::vector on v of this std::vector
	Vector2d operator >> (const Vector2d& v) const;

	// Retrieves true if all elements of this std::vector equals to vRight's
	bool operator == (Vector2d& vRight) const ;

	void print () const;

private:

	// Vector2d: A 2d std::vector that has two elements x, y
	double _x;
	double _y;

};

};   // namespace njr



/******************************************************************************
 * Retrieves a std::vector which is a result of this std::vector multiply by the given
 * scalar ds
 ******************************************************************************/

njr::Vector2d operator * (const double& ds, const njr::Vector2d& v);

njr::Vector2d operator - (const njr::Vector2d& v);

std::ostream& operator << (std::ostream& os, const njr::Vector2d& v);

std::istream& operator >> (std::istream& is, njr::Vector2d& v);

#endif // _NJR_VECTOR_2D_H
