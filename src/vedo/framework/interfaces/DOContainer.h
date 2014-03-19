#ifndef _DISCRETE_OBJECT_CONTAINER_H
#define _DISCRETE_OBJECT_CONTAINER_H

#include <vedo/framework/interfaces/Boundary.h>
#include <vedo/framework/interfaces/DiscreteObject.h>
#include <vector>

namespace vedo
{

class DOContainer
{

public:

	DOContainer();

	~DOContainer();

	void AddFieldImpact(const njr::Vector3d& vFieldImpact);

	void AddImpact(const unsigned long& ul, const njr::Vector3d& vImpact, const njr::Vector3d& vAngularImpact);

	//void AddExternalImpact
	//	(const std::vector<std::pair<njr::Vector3d, njr::Vector3d> >&
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

};   // namespace vedo

#endif // _DISCRETE_OBJECT_CONTAINER_H
