#ifndef _DISCRETE_OBJECT_CONTAINER_H
#define _DISCRETE_OBJECT_CONTAINER_H

#include <FrameWork/Interfaces/Boundary.h>
#include <FrameWork/Interfaces/DiscreteObject.h>
#include <vector>

namespace VEDO
{

class DOContainer
{

public:

	DOContainer();

	~DOContainer();

	void AddFieldImpact(const NJR::Vector3d& vFieldImpact);

	void AddImpact(const unsigned long& ul, const NJR::Vector3d& vImpact, const NJR::Vector3d& vAngularImpact);

	//void AddExternalImpact
	//	(const std::vector<std::pair<NJR::Vector3d, NJR::Vector3d> >&
	//	 vvExternalImpact                                        );

	void Response(const double dt);

	void AddConstrainedImpact(const double dt);

	void Clear();

	void Add(DiscreteObject* pdo);

	inline const DiscreteObject* GetDiscreteObject(unsigned int i)
	{
		return ( i<(lcDO.size()) ) ? lcDO[i] : 0;
	};

	inline DiscreteObject* operator[] (unsigned int i)
	{
		return ( i<(lcDO.size()) ) ? lcDO[i] : 0;
	};

	inline const std::vector<const DOStatus *>& GetDOStatus() const
	{
		return lcDOS;
	};

	inline const DOStatus* GetDOStatus(unsigned long ul) const
	{
		return lcDOS[ul];
	};

	inline unsigned long size() const
	{
		return (unsigned long)lcDO.size();
	};

	void Erase(const std::vector<unsigned long>&);

	void EnforcePeriodicBoundaryConditions(const Boundary& pbc);

private:

	std::vector<DiscreteObject*> lcDO;
	std::vector<const DOStatus*> lcDOS;
};

};   // namespace VEDO

#endif // _DISCRETE_OBJECT_CONTAINER_H
