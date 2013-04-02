#ifndef _DOMAP_H
#define _DOMAP_H

#include <Framework/Interfaces/DOWorld.h>

namespace vedo
{

class DOMap
{

public:

	explicit DOMap
		(const unsigned long& id,
		const DOStatus* cpdos,
		const DOModel* cpdoml,
		const double& SafeLength )
	{
		_id         = id;
		_cpdos      = cpdos;
		_cpdoml     = cpdoml;
		_SafeLength = SafeLength;
	};

	explicit DOMap()
	{
		_cpdos      = 0;
		_cpdoml     = 0;
		_SafeLength = 0.0;
	};

	DOMap(const DOMap& m)
	{
		_id         = m._id;
		_cpdos      = m._cpdos;
		_cpdoml     = m._cpdoml;
		_SafeLength = m._SafeLength;
	};

	DOMap& operator = (const DOMap& m)
	{
		_id         = m._id;
		_cpdos      = m._cpdos;
		_cpdoml     = m._cpdoml;
		_SafeLength = m._SafeLength;
		return *this;
	};

	inline const DOStatus* cpdos() const
	{
		return _cpdos;
	};

	inline const DOModel* cpdoml() const
	{
		return _cpdoml;
	};

	inline unsigned long id() const
	{
		return _id;
	};

	inline double SafeLength() const
	{
		return _SafeLength;
	};

	inline bool operator == (const DOMap& y)
	{
		return _id == y._id;
	};

	static bool ComX(DOMap x, DOMap y)
	{
		return ((x._cpdos->GetPosition().x()) < (y._cpdos->GetPosition().x()));
	};

	static bool ComID(DOMap x, DOMap y)
	{
		return ((x._id) < (y._id));
	};

	static bool ComY(DOMap x, DOMap y)
	{
		return ((x._cpdos->GetPosition().y()) < (y._cpdos->GetPosition().y()));
	};

	static bool ComZ(DOMap x, DOMap y)
	{
		return ((x._cpdos->GetPosition().z()) < (y._cpdos->GetPosition().z()));
	};

	static bool ComV(DOMap x, DOMap y)
	{
		return
			( (x._cpdos->GetVelocity().length())
			< (y._cpdos->GetVelocity().length()) );
	};

	static bool ComR(DOMap x, DOMap y)
	{
		return (x._cpdoml->GetRange() < y._cpdoml->GetRange());
//		return ((x._cpdoml->GetShapeAttributes().sphere.radius)
//			<   (y._cpdoml->GetShapeAttributes().sphere.radius) );
	};

	static bool ComS(DOMap x, DOMap y)
	{
		return (x._SafeLength < y._SafeLength);
	};

	static bool ISConstrained(DOMap m)
	{
		return ((m._cpdoml->GetBehavior()) == "constrained");
	};

	static bool ISFixed(DOMap m)
	{
		return ((m._cpdoml->GetBehavior()) == "fixed");
	};

	static bool ISMobile(DOMap m)
	{
		return ((m._cpdoml->GetBehavior()) == "mobile");
	};

	static bool ISColloid(DOMap m)
	{
		return ((m._cpdoml->GetBehavior()) == "Colloid");
	};

	static bool ComBehavior(DOMap x, DOMap y)
	{
		return ((x._cpdoml->GetBehavior()) == (y._cpdoml->GetBehavior()));
	};

	static const DOStatus* DOMap2DOStatus(DOMap x)
	{
		return x._cpdos;
	};

	static std::vector<DOMap> GetDOMap(const DOWorld*);

	static void ExtremeValue
		(std::vector<DOMap>,
		 double& maxX, double& minX,
		 double& maxY, double& minY,
		 double& maxZ, double& minZ,
		 double& maxR, double& maxV,
		 double& maxS               );

	static double CalSafeDistance(DOMap m, njr::Vector3d vFF, double dt);

//	static double CalDistance(DOMap m1, DOMap m2);

	static double CalDistance(DOMap m1, DOMap m2, const Boundary* pbc=0);

private:

	unsigned long   _id;
	const DOStatus* _cpdos;
	const DOModel*  _cpdoml;
	double          _SafeLength;
};



class OutOfArea
{

public:

	explicit OutOfArea
		(double _Xn, double _Xp, double _Yn, double _Yp, double _Zn, double _Zp)
	: Xn(_Xn), Xp(_Xp), Yn(_Yn), Yp(_Yp), Zn(_Zn), Zp(_Zp)
	{
	};

	bool operator () (DOMap m)
	{
		bool out = false;

		out |=	(Xp >= Xn) ?
				(m.cpdos()->GetPosition().x() < Xn) || (m.cpdos()->GetPosition().x() > Xp) :
				(m.cpdos()->GetPosition().x() > Xp) && (m.cpdos()->GetPosition().x() < Xn);

		out |=	(Yp >= Yn) ?
				(m.cpdos()->GetPosition().y() < Yn) || (m.cpdos()->GetPosition().y() > Yp) :
				(m.cpdos()->GetPosition().y() > Yp) && (m.cpdos()->GetPosition().y() < Yn);

		out |=	(Zp >= Zn) ?
				(m.cpdos()->GetPosition().z() < Zn) || (m.cpdos()->GetPosition().z() > Zp) :
				(m.cpdos()->GetPosition().z() > Zp) && (m.cpdos()->GetPosition().z() < Zn);

		return out;
/*
		if (   (m.cpdos()->GetPosition().x() < Xn)
				|| (m.cpdos()->GetPosition().x() > Xp)
				|| (m.cpdos()->GetPosition().y() < Yn)
				|| (m.cpdos()->GetPosition().y() > Yp)
				|| (m.cpdos()->GetPosition().z() < Zn)
				|| (m.cpdos()->GetPosition().z() > Zp) )
		{
			return true;
		}
		return false ;
*/
	};

private:

	const double Xp;
	const double Xn;
	const double Yp;
	const double Yn;
	const double Zp;
	const double Zn;
};

};   // namespace vedo

#endif // _DOMAP_H
