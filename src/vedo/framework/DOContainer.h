#ifndef _DISCRETE_OBJECT_CONTAINER_H
#define _DISCRETE_OBJECT_CONTAINER_H

#include <vedo/framework/Boundary.h>
#include <vedo/framework/DiscreteObject.h>
#include <vedo/framework/DataType.h>
#include <vector>

namespace vedo
{

class DOContainer
{

public:

	DOContainer();

	~DOContainer();

	void AddFieldImpact(const aries::Vector3df& vFieldImpact);

	void AddImpact(const _uint_t& ul, const aries::Vector3df& vImpact, const aries::Vector3df& vAngularImpact);

	//void AddExternalImpact(const std::vector<std::pair<aries::Vector3df, aries::Vector3df> >& vvExternalImpact);

	void Response(const _float_t dt);

	void AddConstrainedImpact(const _float_t dt);

	void Clear();

	void Add(DiscreteObject* pdo);

	inline const DiscreteObject* GetDiscreteObject(_uint_t i)
	{
		return ( i<(lcDO.size()) ) ? lcDO[i] : 0;
	}

	inline DiscreteObject* operator[] (_uint_t i)
	{
		return ( i<(lcDO.size()) ) ? lcDO[i] : 0;
	}

	inline const std::vector<const DOStatus *>& GetDOStatus() const
	{
		return lcDOS;
	}

	inline const DOStatus* GetDOStatus(_uint_t ul) const
	{
		return lcDOS[ul];
	}

	inline _uint_t size() const
	{
		return (_uint_t)lcDO.size();
	}

	void Erase(const std::vector<_uint_t>&);

	void EnforcePeriodicBoundaryConditions(const Boundary& pbc);

private:

	std::vector<DiscreteObject*> lcDO;

	std::vector<const DOStatus*> lcDOS;
};

}   // namespace vedo

#endif // _DISCRETE_OBJECT_CONTAINER_H
