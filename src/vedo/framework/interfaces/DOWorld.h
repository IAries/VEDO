#ifndef _DOWORLD_H
#define _DOWORLD_H

#include <vedo/framework/interfaces/DOModel.h>
#include <vedo/framework/interfaces/DOStatus.h>
#include <vedo/framework/interfaces/IactModel.h>
//#include <vedo/framework/interfaces/IactRecordTab.h>
#include <vedo/framework/interfaces/SystemParameter.h>
#include <algorithm>
#include <list>
#include <string>
#include <vector>

namespace vedo
{

struct NullExternalFieldVTKWriter;

class IactRecordTab;

// Data Type Class
class DOWorld
{

public:

	DOWorld();

	DOWorld(const DOWorld&);

	DOWorld(SystemParameter*, const std::list<DOModel*>&, const std::list<IactModel*>&, const std::vector<DOStatus*>&);

	~DOWorld();

	void Clear();

 	bool Check() const;

	void ClearSeparatedElements();

	bool IsEmpty() const;

	const DOWorld& operator = (const DOWorld& dow);

	const DOModel* GetDOModel(const std::string& DOName) const;

	const DOStatus* GetDOStatus(const vedo_uint_t&) const;

	void SetDOStatus(const vedo_uint_t&, const DOStatus&);

	void SetDOStatusVelocityAndAngularVelocity
		(const vedo_uint_t& ulID, const njr::Vector3d& vVelocity, const njr::Vector3d& vAngularVelocity);

	void EraseDOStatus(const std::vector<vedo_uint_t>&);

	void EraseDOStatus(const vedo_uint_t&);

	inline void AddDOStatus (DOStatus* dosp)
	{
		cDOStatus.push_back(dosp);
		pSystemParameter->SetDONumber((pSystemParameter->GetDONumber())+1);
	}

	const std::vector<vedo_uint_t> FindDO (std::string name) const;

	const IactModel* GetIactModel(const std::string& sMDOGroup, const std::string& sSDOGroup) const;

	inline SystemParameter* GetSystemParameter() const
	{
		return pSystemParameter;
	}

	inline const std::list<DOModel*>& GetDOModel() const
	{
		return cDOModel;
	}

	inline const std::list<IactModel*>& GetIactModel() const
	{
		return cIactModel;
	}

	inline const std::vector<DOStatus*>& GetDOStatus() const
	{
		return cDOStatus;
	}

	std::string GetDOName(const vedo_uint_t&) const;

	// Operations for altering simulation information
	inline void NextStep()
	{
		pSystemParameter->NextStep();
	}

	inline void SetSimulatedTime(const vedo_float_t dTimeInterval, const vedo_float_t dTimeCurrent)
	{
		pSystemParameter->SetTime(dTimeInterval, dTimeCurrent);
	}

	inline void SetSimulatedTime
		(const vedo_float_t dTimeStart,
		 const vedo_float_t dTimeStop,
		 const vedo_float_t dTimeInterval,
		 const vedo_float_t dTimeCurrent  )
	{
		pSystemParameter->SetTime(dTimeStart, dTimeStop, dTimeInterval, dTimeCurrent);
	}

	void SetFieldAcceleration(const njr::Vector3d&);

	bool UpdateDOStatus(const std::vector<const DOStatus *>& nDOStatus);

	bool DelDOModel (const std::string& DOName);

	// Operation for writing down simulation information to files or visualization
	void HighDraw(const char* filename) const;

	void Draw(const char* filename) const;

	bool ReadIDO(const std::string filename);

	bool ReadIDO(const std::string filename, IactRecordTab*);

	void WriteIDO(const std::string filename) const;

	void WriteIDO(const std::string filename, const IactRecordTab*) const;

	bool ReadXML(const std::string filename, IactRecordTab*);

	bool ReadXML(const std::string filename);

	void WriteXMLPrefix(const std::string filename) const;

	void WriteXMLPostfix(const std::string filename) const;

	void WriteXMLIactRecordTab(const std::string filename, const IactRecordTab* irtp) const;

	void WriteXML(const std::string filename) const;

	void WriteXML(const std::string filename, const IactRecordTab*) const;

	// 2003.12.3 JF Lee Add for Visualization Postprocessor
	// 2003.12.5 Jitin Yang Modified
	void WriteVPF(const std::string filename) const;

	void WriteVPF(const std::string filename, const DOWorld* pw) const;

    #ifdef _STD_CPP_11
        template<typename ExtFieldWriterT=NullExternalFieldVTKWriter>
        void WriteVTK (const std::string filename) const;
    #endif   // _STD_CPP_11

	inline void SetBoundary(const Boundary pBC)
	{
		pSystemParameter->SetBoundary(pBC);
	}

	void SetModels(const DOWorld* dow);

	void FreezeAllElements();

	void FreezeElements(std::string& sDOName);

	void SortingDOStatus();

	void TurnMonitor(bool);

	inline void SetMonitor(vedo_uint_t& ul, bool m)
	{
		cDOStatus[ul]->SetMonitored(m);
	}

	inline void SetGranularTemperatureV(vedo_uint_t& ul, vedo_float_t& gt)
	{
		cDOStatus[ul]->SetGranularTemperatureV(gt);
	}

	inline void SetGranularTemperatureAV(vedo_uint_t& ul, vedo_float_t& gt)
	{
		cDOStatus[ul]->SetGranularTemperatureAV(gt);
	}

//	void CalculateSystemEnergy(const IactRecordTab*);
	void CalculateSystemEnergy();

	//const std::pair<njr::Vector3d, njr::Vector3d> Distribution(vedo_float_t& dMeshLength) const;

	void Shift(const njr::Vector3d& shift);

	void Shift(const njr::Vector3d& shift, const std::string& DOName);

	void Rotate(const vedo_float_t& Angle2XAxis, const vedo_float_t& Angle2YAxis, const vedo_float_t& Angle2ZAxis);

	void Rotate(const njr::Vector3d& eX, const njr::Vector3d& eZ, const std::string& DOName);

	const DOStatus GetDOStatus(const std::string& DOName, const vedo_uint_t ulSerialNumber) const;

	vedo_float_t Volume(vedo_float_t& dMeshLength) const;

	vedo_float_t ProjectedAreaX(vedo_float_t& dMeshLength) const;

	vedo_float_t ProjectedAreaY(vedo_float_t& dMeshLength) const;

	vedo_float_t ProjectedAreaZ(vedo_float_t& dMeshLength) const;

	void DumpDOStatus(const char* filename) const;

private:

	SystemParameter*       pSystemParameter;

	std::list<DOModel*>    cDOModel;

	std::list<IactModel*>  cIactModel;

	std::vector<DOStatus*> cDOStatus;

	// Friend class declaration in order to make docluster work
	friend class DOCluster;
};

}   // namespace vedo

#endif // _DOWORLD_H
