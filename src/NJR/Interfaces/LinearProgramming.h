#ifndef _NJR_LINEAR_PROGRAMMING_H
#define _NJR_LINEAR_PROGRAMMING_H

#include <NJR/Interfaces/Matrix.h>
#include <NJR/Interfaces/Polyhedra.h>
#include <NJR/Interfaces/Polygon.h>

namespace njr
{

class LinearProgramming
{

public:

	LinearProgramming();

	LinearProgramming(const LinearProgramming&);

	virtual ~LinearProgramming();

	const LinearProgramming& operator = (const LinearProgramming&);

	void Set(const njr::NJRpolyhedra &);

	bool Check();

	void Clear();

	njr::NJRpolygon GetPolygon();

	bool GetExtremeValue
		(double& maxX,
		double& minX,
		double& maxY,
		double& minY,
		double& maxZ,
		double& minZ);

	inline bool ISPolygon() const
	{
		return _polygon;
	};

	inline bool checked() const
	{
		return _checked;
	};

	inline bool feasiable() const
	{
		return _feasiable;
	};

	void print() const;

private:

	unsigned int cols;
	unsigned int rows;
	unsigned int num_m;
	unsigned int num_s;
	unsigned int nbas;
	bool _checked;
	bool _feasiable;
	bool _polygon;
	char *kid;
	unsigned int *basis;
	unsigned int *nbasis;
	Matrix obj;
	Matrix A;
	Matrix rhs;
	Matrix BI;
    void Allocation();
	double GetObjValue();
	bool PurgeArtificialVariable();

};

};   // namespace njr

#endif // _NJR_LINEAR_PROGRAMMING_H
