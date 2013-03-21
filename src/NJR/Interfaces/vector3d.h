/******************************************************************************
 * std::vector.h  header file for 3D std::vector Object
 * version   1.2
 *
 * NJRvector3d is a 3d [Mathemitics] std::vector which defines the attributes and
 * basic opertaion of std::vector.
 *
 * History:  Ver 1.0 Created 2001 Aug  by  C.T. Jitin Yang
 *           Ver 1.1 Modified 2003 Feb by  C.T. Jitin Yang
 *           For CAE summer traning course
 *           (C++ Programming Langurage)
 ******************************************************************************/

#ifndef _NJRVECTOR3d_H
#define _NJRVECTOR3d_H

#include <NJR/Interfaces/Constants.h>
#include <string>
#include <iostream>

namespace NJR
{

class NJRvector3d
{

public:

	NJRvector3d();

	NJRvector3d(const double& dx, const double& dy, const double& dz);

	NJRvector3d(const NJRvector3d& vInit);

	explicit NJRvector3d(const char* cInit);

	explicit NJRvector3d(const std::string& sInit);

	NJRvector3d(const NJRDXF::ParticularVector& pvInit);

	// Assigns dx, dy, dx to x, y, z
	const NJRvector3d& Set
		(const double& dx, const double& dy, const double& dz);

	const NJRvector3d& SetX(const double& dx);

	const NJRvector3d& SetY(const double& dy);

	const NJRvector3d& SetZ(const double& dz);

	const NJRvector3d& SetCylinder
		(const double& dRadius, const double& dThita, const double& dz);

	const NJRvector3d& SetSphere
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
	NJRvector3d direction() const;

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
	NJRvector3d RotateAround(const NJRvector3d& gv) const;

	/**************************************************************************
	 * Retrieves the new position of this std::vector
	 * after axial x has been replaced by nx
	 * and   axial z has been replaced by nz
	 *       axial y has been replaced by ny
	 **************************************************************************/
	NJRvector3d Trans
		(const NJRvector3d& nx, const NJRvector3d& ny, const NJRvector3d& nz)
		const;

	// Retrieves the projected std::vector on v of this std::vector
	NJRvector3d ProjectOn(const NJRvector3d& v) const;

	// Retrieves the dot product of this and input std::vector
	double Dot(const NJRvector3d& v) const;

	// Retrieves the cross product of this and input std::vector
	NJRvector3d Cross(const NJRvector3d& v) const;

	/**************************************************************************
	 * Assignment Operators of NJRvector3d
	 * NJRvector3d & operator = (char char *cAval)
	 * Assigns a input std::vector to this std::vector
	 * the std::vector's form is "(a,b,c)" where a,b,c are constants.
	 *
	 * NOTICE: It does not allow any char which exists between " and ( or ).
	 *
	 * For example: NJRvector3d v="(a,b,c)";
	 * the function will decompose the std::string "(a,b,c)" into 3 double variable
	 * and assigns them to this std::vector.
	 **************************************************************************/

	const NJRvector3d& operator = (const char* cAval);

	const NJRvector3d& operator = (const std::string& sAval);

	const NJRvector3d& operator = (const NJRDXF::ParticularVector& pvAval);

	const NJRvector3d& operator = (const NJRvector3d& vAval);

	const NJRvector3d& operator += (const NJRvector3d& v);

	const NJRvector3d& operator -= (const NJRvector3d& v);

	//	Arithmetic Operators of NJRvector3d
	NJRvector3d operator + (const NJRvector3d& v) const;

	NJRvector3d operator - (const NJRvector3d& v) const;

	//	Retrieves the dot product of left and right operands of %
	double operator % (const NJRvector3d& v) const;

	// Retrieves the cross product of left and right operands of * .
	NJRvector3d operator * (const NJRvector3d& v) const;

  	// Retrieves a std::vector which is a result of this std::vector
	// multiply by the given scalar ds
	NJRvector3d operator * (const double& ds) const;

	//	Retrieves the projected std::vector on v of this std::vector
	NJRvector3d operator >> (const NJRvector3d& v) const;

	// Retrieves true if all elements of this std::vector equals to vRight's
	bool operator == (NJRvector3d& vRight) const ;

	void print () const;

private:

	// NJRvector3d: A 3D std::vector that has three elements x, y ,z
	double _x;
	double _y;
	double _z;
};

};   // namespace NJR



/******************************************************************************
 * Retrieves a std::vector which is a result of this std::vector multiply by the given
 * scalar ds
 ******************************************************************************/

NJR::NJRvector3d operator - (const NJR::NJRvector3d& v);

NJR::NJRvector3d operator * (const double& ds, const NJR::NJRvector3d& v);

std::ostream& operator << (std::ostream& os, const NJR::NJRvector3d& v);

std::istream& operator >> (std::istream& is, NJR::NJRvector3d& v);

#endif // _NJRVECTOR3d_H
