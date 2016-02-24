#ifndef _DOMAP_H
#define _DOMAP_H

#include <vedo/framework/DOModel.h>
#include <vedo/framework/DOWorld.h>
#include <vedo/framework/DataType.h>

namespace vedo
{

class DOMap
{

public:

	explicit DOMap(const _uint_t& id, const DOStatus* cpdos, const DOModel* cpdoml, const _float_t& SafeLength)
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

	inline _uint_t id() const
	{
		return _id;
	}

	inline _float_t SafeLength() const
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
		 _float_t& maxX, _float_t& minX,
		 _float_t& maxY, _float_t& minY,
		 _float_t& maxZ, _float_t& minZ,
		 _float_t& maxR, _float_t& maxV, _float_t& maxS);

	static _float_t CalSafeDistance(DOMap m, Vector3df vFF, _float_t dt);

//	static _float_t CalDistance(DOMap m1, DOMap m2);

	static _float_t CalDistance(DOMap m1, DOMap m2, const Boundary* pbc=0);

private:

	_uint_t     _id;

	const DOStatus* _cpdos;

	const DOModel*  _cpdoml;

	_float_t    _SafeLength;
};



class OutOfArea
{

public:

	explicit OutOfArea
		(_float_t _Xn, _float_t _Xp, _float_t _Yn, _float_t _Yp, _float_t _Zn, _float_t _Zp):
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

	const _float_t Xp;

	const _float_t Xn;

	const _float_t Yp;

	const _float_t Yn;

	const _float_t Zp;

	const _float_t Zn;

	OutOfArea();

	~OutOfArea();

	OutOfArea(const OutOfArea&);

	const OutOfArea& operator = (const OutOfArea&);
};

}   // namespace vedo

#endif // _DOMAP_H
