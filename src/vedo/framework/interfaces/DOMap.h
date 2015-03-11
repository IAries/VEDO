#ifndef _DOMAP_H
#define _DOMAP_H

#include <vedo/framework/interfaces/DOModel.h>
#include <vedo/framework/interfaces/DOWorld.h>

namespace vedo
{

class DOMap
{

public:

	explicit DOMap(const vedo_uint_t& id, const DOStatus* cpdos, const DOModel* cpdoml, const vedo_float_t& SafeLength)
	{
		_id         = id;
		_cpdos      = cpdos;
		_cpdoml     = cpdoml;
		_SafeLength = SafeLength;
	}

	explicit DOMap()
	{
		_cpdos      = 0;
		_cpdoml     = 0;
		_SafeLength = 0.0;
	}

	DOMap(const DOMap& m)
	{
		_id         = m._id;
		_cpdos      = m._cpdos;
		_cpdoml     = m._cpdoml;
		_SafeLength = m._SafeLength;
	}

	DOMap& operator = (const DOMap& m)
	{
		_id         = m._id;
		_cpdos      = m._cpdos;
		_cpdoml     = m._cpdoml;
		_SafeLength = m._SafeLength;
		return *this;
	}

	inline const DOStatus* cpdos() const
	{
		return _cpdos;
	}

	inline const DOModel* cpdoml() const
	{
		return _cpdoml;
	}

	inline vedo_uint_t id() const
	{
		return _id;
	}

	inline vedo_float_t SafeLength() const
	{
		return _SafeLength;
	}

	inline bool operator == (const DOMap& y)
	{
		return _id == y._id;
	}

	static bool ComX(DOMap x, DOMap y)
	{
		return ((x._cpdos->GetPosition().x()) < (y._cpdos->GetPosition().x()));
	}

	static bool ComID(DOMap x, DOMap y)
	{
		return ((x._id) < (y._id));
	}

	static bool ComY(DOMap x, DOMap y)
	{
		return ((x._cpdos->GetPosition().y()) < (y._cpdos->GetPosition().y()));
	}

	static bool ComZ(DOMap x, DOMap y)
	{
		return ((x._cpdos->GetPosition().z()) < (y._cpdos->GetPosition().z()));
	}

	static bool ComV(DOMap x, DOMap y)
	{
		return ((x._cpdos->GetVelocity().length()) < (y._cpdos->GetVelocity().length()));
	}

	static bool ComR(DOMap x, DOMap y)
	{
		return (x._cpdoml->GetRange() < y._cpdoml->GetRange());
//		return ((x._cpdoml->GetShapeAttributes().sphere.radius)
//			<   (y._cpdoml->GetShapeAttributes().sphere.radius) );
	}

	static bool ComS(DOMap x, DOMap y)
	{
		return (x._SafeLength < y._SafeLength);
	}

	static bool ISConstrained(DOMap m)
	{
		return ((m._cpdoml->GetBehavior()) == constrained);
		// return ((m._cpdoml->GetBehavior()) == DOBehaviorType::constrained);
	}

	static bool ISFixed(DOMap m)
	{
		return ((m._cpdoml->GetBehavior()) == fixed);
		//return ((m._cpdoml->GetBehavior()) == DOBehaviorType::fixed);
	}

	static bool ISMobile(DOMap m)
	{
		return ((m._cpdoml->GetBehavior()) == mobile);
		//return ((m._cpdoml->GetBehavior()) == DOBehaviorType::mobile);
	}

	static bool ISOrbital(DOMap m)
	{
		return ((m._cpdoml->GetBehavior()) == orbital);
		//return ((m._cpdoml->GetBehavior()) == DOBehaviorType::orbital);
	}

	static bool ComBehavior(DOMap x, DOMap y)
	{
		return ((x._cpdoml->GetBehavior()) == (y._cpdoml->GetBehavior()));
	}

	static const DOStatus* DOMap2DOStatus(DOMap x)
	{
		return x._cpdos;
	}

	static std::vector<DOMap> GetDOMap(const DOWorld*);

	static void ExtremeValue
		(std::vector<DOMap>,
		 vedo_float_t& maxX, vedo_float_t& minX,
		 vedo_float_t& maxY, vedo_float_t& minY,
		 vedo_float_t& maxZ, vedo_float_t& minZ,
		 vedo_float_t& maxR, vedo_float_t& maxV, vedo_float_t& maxS);

	static vedo_float_t CalSafeDistance(DOMap m, njr::Vector3d vFF, vedo_float_t dt);

//	static vedo_float_t CalDistance(DOMap m1, DOMap m2);

	static vedo_float_t CalDistance(DOMap m1, DOMap m2, const Boundary* pbc=0);

private:

	vedo_uint_t     _id;

	const DOStatus* _cpdos;

	const DOModel*  _cpdoml;

	vedo_float_t    _SafeLength;
};



class OutOfArea
{

public:

	explicit OutOfArea
		(vedo_float_t _Xn, vedo_float_t _Xp, vedo_float_t _Yn, vedo_float_t _Yp, vedo_float_t _Zn, vedo_float_t _Zp):
			Xn(_Xn), Xp(_Xp), Yn(_Yn), Yp(_Yp), Zn(_Zn), Zp(_Zp)
	{
	}

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
	}

private:

	const vedo_float_t Xp;

	const vedo_float_t Xn;

	const vedo_float_t Yp;

	const vedo_float_t Yn;

	const vedo_float_t Zp;

	const vedo_float_t Zn;

	OutOfArea();

	~OutOfArea();

	OutOfArea(const OutOfArea&);

	const OutOfArea& operator = (const OutOfArea&);
};

}   // namespace vedo

#endif // _DOMAP_H
