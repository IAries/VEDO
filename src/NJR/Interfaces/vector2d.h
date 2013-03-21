/******************************************************************************
 * std::vector.h  header file for 2D std::vector Object
 * version   1.0
 *
 * NJRvector2d is a 2d [Mathemitics] std::vector which defines the attributes and
 * basic opertaion of std::vector.
 *
 * History:  Ver 1.0 Created 2010 Oct  by  W.T. Aries Chang
 ******************************************************************************/

#ifndef _NJRVECTOR2d_H
#define _NJRVECTOR2d_H

#include <NJR/Interfaces/Constants.h>
#include <string>
#include <iostream>

namespace NJR
{

// Predefine Praticular Vectors
class NJRvector2d
{

public:

	NJRvector2d();

	NJRvector2d(const double& dx, const double& dy);

	NJRvector2d(const NJRvector2d& vInit);

	explicit NJRvector2d(const char* cInit);

	explicit NJRvector2d(const std::string& sInit);

	NJRvector2d(const NJRDXF::ParticularVector& pvInit);

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

	// Retrieves the length (magnitude) of this std::vector
	double length() const;

	// Retrieves the direction of this std::vector
	NJRvector2d direction() const;

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
//	NJRvector2d RotateAround(const NJRvector2d& gv) const;

	/**************************************************************************
	 * Retrieves the new position of this std::vector
	 * after axial x has been replaced by nx
	 * and   axial y has been replaced by ny
	 **************************************************************************/
	NJRvector2d Trans
		(const NJRvector2d& nx, const NJRvector2d& ny)
		const;

	// Retrieves the projected std::vector on v of this std::vector
	NJRvector2d ProjectOn(const NJRvector2d& v) const;

	// Retrieves the dot product of this and input std::vector
	double Dot(const NJRvector2d& v) const;

	/**************************************************************************
	 * Assignment Operators of NJRvector2d
	 * NJRvector2d & operator = (char char *cAval)
	 * Assigns a input std::vector to this std::vector
	 * the std::vector's form is "(a,b,c)" where a,b,c are constants.
	 *
	 * NOTICE: It does not allow any char which exists between " and ( or ).
	 *
	 * For example: NJRvector2d v="(a,b,c)";
	 * the function will decompose the std::string "(a,b,c)" into 3 double variable
	 * and assigns them to this std::vector.
	 **************************************************************************/

	const NJRvector2d& operator = (const char* cAval);

	const NJRvector2d& operator = (const std::string& sAval);

	const NJRvector2d& operator = (const NJRDXF::ParticularVector& pvAval);

	const NJRvector2d& operator = (const NJRvector2d& vAval);

	const NJRvector2d& operator += (const NJRvector2d& v);

	const NJRvector2d& operator -= (const NJRvector2d& v);

	//	Arithmetic Operators of NJRvector2d
	NJRvector2d operator + (const NJRvector2d& v) const;

	NJRvector2d operator - (const NJRvector2d& v) const;

	//	Retrieves the dot product of left and right operands of %
	double operator % (const NJRvector2d& v) const;

  	// Retrieves a std::vector which is a result of this std::vector
	// multiply by the given scalar ds
	NJRvector2d operator * (const double& ds) const;

	//	Retrieves the projected std::vector on v of this std::vector
	NJRvector2d operator >> (const NJRvector2d& v) const;

	// Retrieves true if all elements of this std::vector equals to vRight's
	bool operator == (NJRvector2d& vRight) const ;

	void print () const;

private:

	// NJRvector2d: A 2d std::vector that has two elements x, y
	double _x;
	double _y;

};

};   // namespace NJR



/******************************************************************************
 * Retrieves a std::vector which is a result of this std::vector multiply by the given
 * scalar ds
 ******************************************************************************/

NJR::NJRvector2d operator * (const double& ds, const NJR::NJRvector2d& v);

NJR::NJRvector2d operator - (const NJR::NJRvector2d& v);

std::ostream& operator << (std::ostream& os, const NJR::NJRvector2d& v);

std::istream& operator >> (std::istream& is, NJR::NJRvector2d& v);

#endif // _NJRVECTOR2d_H
