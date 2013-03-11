#ifndef _DOWORLD_H
#define _DOWORLD_H

#include <FrameWork/Interfaces/DOModel.h>
#include <FrameWork/Interfaces/DOStatus.h>
#include <FrameWork/Interfaces/IactModel.h>
#include <FrameWork/Interfaces/SystemParameter.h>
#include <algorithm>
#include <list>
#include <string>
#include <vector>

struct NullExternalFieldVTKWriter;

// Data Type Class
class DOWorld
{

public:

	DOWorld();

	DOWorld(const DOWorld&);

	DOWorld
		(SystemParameter*,
		const std::list<DOModel*>&,
		const std::list<IactModel*>&,
		const std::vector<DOStatus*>& );

	~DOWorld();

	void Clear();

 	bool Check() const;

	void ClearSeparatedElements();

	bool IsEmpty() const;

	const DOWorld& operator = (const DOWorld& dow);

	const DOModel* GetDOModel(const std::string& DOName) const;

	const DOStatus* GetDOStatus(const unsigned long&) const;

	void SetDOStatus(const unsigned long&, const DOStatus&);

	void EraseDOStatus(const std::vector<unsigned long>&);

	inline void AddDOStatus (DOStatus* dosp)
	{
		cDOStatus.push_back(dosp);
		pSystemParameter->SetDONumber((pSystemParameter->GetDONumber())+1);
	}

	const std::vector<unsigned long> FindDO (std::string name) const;

	const IactModel* GetIactModel
		(const std::string& sMDOGroup, const std::string& sSDOGroup) const;

	inline SystemParameter* GetSystemParameter() const
	{
		return pSystemParameter;
	};

	inline const std::list<DOModel*>& GetDOModel() const
	{
		return cDOModel;
	};

	inline const std::list<IactModel*>& GetIactModel() const
	{
		return cIactModel;
	};

	inline const std::vector<DOStatus*>& GetDOStatus() const
	{
		return cDOStatus;
	};

	// Operations for altering simulation information
	inline void NextStep()
	{
		pSystemParameter->NextStep();
	}

	inline void SetSimulatedTime
		(const double dTimeInterval,
		 const double dTimeCurrent  )
	{
		pSystemParameter->SetTime(dTimeInterval, dTimeCurrent);
	}

	inline void SetSimulatedTime
		(const double dTimeStart,
		 const double dTimeStop,
		 const double dTimeInterval,
		 const double dTimeCurrent  )
	{
		pSystemParameter->SetTime(dTimeStart, dTimeStop, dTimeInterval, dTimeCurrent);
	}

	void SetFieldForce(const NJRvector3d&);

	bool UpdateDOStatus(const std::vector<const DOStatus *>& nDOStatus);

	bool DelDOModel (const std::string& DOName);

	/**************************************************************************
	 * Operation for writing down simulation information to files or
	 * visualization
	 **************************************************************************/
	void HighDraw(const char* filename) const;

	void Draw(const char* filename) const;

	bool ReadIDO(const char* idoFile);

	bool ReadIDO(const char* idoFile, unsigned int version);

	bool ReadIDO2011(const char* idoFile);

	bool ReadIDO2009(const char* idoFile);

	bool ReadIDO2008(const char* idoFile);

	void WriteIDO(const char* filename) const;

	bool ReadXML(const char* xmlFile);

	void WriteXML(const char* filename) const;

	// 2003.12.3 JF Lee Add for Visualization Postprocessor
	// 2003.12.5 Jitin Yang Modified
	void WriteVPF (const char *filename) const;

	void WriteVPF (const char *filename, const DOWorld* pw) const;

//	template<typename ExtFieldWriterT=NullExternalFieldVTKWriter>
//	void WriteVTK (const char *filename) const;

	inline void SetBoundary(const Boundary pBC)
	{
		pSystemParameter->SetBoundary(pBC);
	};

	void SetModels(const DOWorld* dow);

	void FreezeAllElements();

	void FreezeElements(std::string& sDOName);

	void SortingDOStatus();

	void TurnMonitor(bool);

	inline void SetMonitor(unsigned long& ul, bool m)
	{
		cDOStatus[ul]->SetMonitored(m);
	};

	inline void SetGranularTemperatureV(unsigned long& ul, double& gt)
	{
		cDOStatus[ul]->SetGranularTemperatureV(gt);
	};

	inline void SetGranularTemperatureAV(unsigned long& ul, double& gt)
	{
		cDOStatus[ul]->SetGranularTemperatureAV(gt);
	};

//	void CalculateSystemEnergy(const IactRecordTab*);
	void CalculateSystemEnergy();

	const std::pair<NJRvector3d, NJRvector3d>
		Distribution(double& dMeshLength) const;

	void Shift(const NJRvector3d& shift);

	void Shift(const NJRvector3d& shift, const std::string& DOName);

	void Rotate
		(const double& Angle2XAxis,
		 const double& Angle2YAxis,
		 const double& Angle2ZAxis );

	void Rotate
		(const NJRvector3d& eX,
		 const NJRvector3d& eZ,
		 const std::string& DOName);

	const DOStatus GetDOStatus
		(const std::string& DOName, const unsigned long ulSerialNumber) const;

	double Volume(double& dMeshLength) const;

	double ProjectedAreaX(double& dMeshLength) const;

	double ProjectedAreaY(double& dMeshLength) const;

	double ProjectedAreaZ(double& dMeshLength) const;

private:

	SystemParameter*       pSystemParameter;
	std::list<DOModel*>    cDOModel;
	std::list<IactModel*>  cIactModel;
	std::vector<DOStatus*> cDOStatus;

	// Friend class declaration in order to make docluster work
	friend class DOCluster;
};

#endif // _DOWORLD_H
