#ifndef _LINEAR_PROGRAMMING_H
#define _LINEAR_PROGRAMMING_H

#include <NJR/Interfaces/matrix.h>
#include <NJR/Interfaces/polyhedra.h>
#include <NJR/Interfaces/polygon.h>

class NJRLinearProgramming
{

public:

	NJRLinearProgramming();

	NJRLinearProgramming(const NJRLinearProgramming&);

	virtual ~NJRLinearProgramming();

	const NJRLinearProgramming& operator = (const NJRLinearProgramming&);

	void Set(const NJRpolyhedra &);

	bool Check();

	void Clear();

	NJRpolygon GetPolygon();

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
	NJRmatrix obj;
	NJRmatrix A;
	NJRmatrix rhs;
	NJRmatrix BI;
    void Allocation();
	double GetObjValue();
	bool PurgeArtificialVariable();

};

#endif // _LINEAR_PROGRAMMING_H
