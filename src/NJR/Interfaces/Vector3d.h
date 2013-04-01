/******************************************************************************
 * std::vector.h  header file for 3D std::vector Object
 * version   1.2
 *
 * Vector3d is a 3d [Mathemitics] std::vector which defines the attributes and
 * basic opertaion of std::vector.
 *
 * History:  Ver 1.0 Created 2001 Aug  by  C.T. Jitin Yang
 *           Ver 1.1 Modified 2003 Feb by  C.T. Jitin Yang
 *           For CAE summer traning course
 *           (C++ Programming Langurage)
 ******************************************************************************/

#ifndef _NJR_VECTOR_3D_H
#define _NJR_VECTOR_3D_H

#include <NJR/Interfaces/Constants.h>
#include <string>
#include <iostream>

namespace njr
{

class Vector3d
{

public:

	Vector3d();

	Vector3d(const double& dx, const double& dy, const double& dz);

	Vector3d(const Vector3d& vInit);

	explicit Vector3d(const char* cInit);

	explicit Vector3d(const std::string& sInit);

	Vector3d(const njrdxf::ParticularVector& pvInit);

	// Assigns dx, dy, dx to x, y, z
	const Vector3d& Set
		(const double& dx, const double& dy, const double& dz);

	const Vector3d& SetX(const double& dx);

	const Vector3d& SetY(const double& dy);

	const Vector3d& SetZ(const double& dz);

	const Vector3d& SetCylinder
		(const double& dRadius, const double& dThita, const double& dz);

	const Vector3d& SetSphere
		(const double& dRadius, const double& dAlpha, const double& dThita);

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

	// Retrieves element z
	inline double z() const
	{
		return _z;
	};

	// Retrieves the length (magnitude) of this std::vector
	double length() const;

	// Retrieves the direction of this std::vector
	Vector3d direction() const;

	// Outputs the version and Programmer
	inline void info() const
	{
		std::cout
			<< " NJRvector Ver 1.2 Modified 2003 July by C.T. Jitin Yang"
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
	Vector3d RotateAround(const Vector3d& gv) const;

	/**************************************************************************
	 * Retrieves the new position of this std::vector
	 * after axial x has been replaced by nx
	 * and   axial z has been replaced by nz
	 *       axial y has been replaced by ny
	 **************************************************************************/
	Vector3d Trans
		(const Vector3d& nx, const Vector3d& ny, const Vector3d& nz)
		const;

	// Retrieves the projected std::vector on v of this std::vector
	Vector3d ProjectOn(const Vector3d& v) const;

	// Retrieves the dot product of this and input std::vector
	double Dot(const Vector3d& v) const;

	// Retrieves the cross product of this and input std::vector
	Vector3d Cross(const Vector3d& v) const;

	/**************************************************************************
	 * Assignment Operators of Vector3d
	 * Vector3d & operator = (char char *cAval)
	 * Assigns a input std::vector to this std::vector
	 * the std::vector's form is "(a,b,c)" where a,b,c are constants.
	 *
	 * NOTICE: It does not allow any char which exists between " and ( or ).
	 *
	 * For example: Vector3d v="(a,b,c)";
	 * the function will decompose the std::string "(a,b,c)" into 3 double variable
	 * and assigns them to this std::vector.
	 **************************************************************************/

	const Vector3d& operator = (const char* cAval);

	const Vector3d& operator = (const std::string& sAval);

	const Vector3d& operator = (const njrdxf::ParticularVector& pvAval);

	const Vector3d& operator = (const Vector3d& vAval);

	const Vector3d& operator += (const Vector3d& v);

	const Vector3d& operator -= (const Vector3d& v);

	//	Arithmetic Operators of Vector3d
	Vector3d operator + (const Vector3d& v) const;

	Vector3d operator - (const Vector3d& v) const;

	//	Retrieves the dot product of left and right operands of %
	double operator % (const Vector3d& v) const;

	// Retrieves the cross product of left and right operands of * .
	Vector3d operator * (const Vector3d& v) const;

  	// Retrieves a std::vector which is a result of this std::vector
	// multiply by the given scalar ds
	Vector3d operator * (const double& ds) const;

	//	Retrieves the projected std::vector on v of this std::vector
	Vector3d operator >> (const Vector3d& v) const;

	// Retrieves true if all elements of this std::vector equals to vRight's
	bool operator == (Vector3d& vRight) const ;

	void print () const;

private:

	// Vector3d: A 3D std::vector that has three elements x, y ,z
	double _x;
	double _y;
	double _z;
};

};   // namespace njr



/******************************************************************************
 * Retrieves a std::vector which is a result of this std::vector multiply by the given
 * scalar ds
 ******************************************************************************/

njr::Vector3d operator - (const njr::Vector3d& v);

njr::Vector3d operator * (const double& ds, const njr::Vector3d& v);

std::ostream& operator << (std::ostream& os, const njr::Vector3d& v);

std::istream& operator >> (std::istream& is, njr::Vector3d& v);

#endif // _NJR_VECTOR_3D_H
