#ifndef _DOWORLD_WRITE_VTK__H
#define _DOWORLD_WRITE_VTK__H

#include <vedo/framework/DOWorld.h>
#include <vedo/framework/IactRecordTab.h>

#include <cstdio>
#include <cstdlib>
#include <sstream>

namespace vedo
{

struct NullExternalFieldVTKWriter
{
	static void writeExternalFields(std::ofstream* ofpVTK)
	{
	}
};



class DataFieldVTKWriter
{

public:

	const _uint_t getNumOfArrays()
	{
		return numOfComp.size();
	}

	const _uint_t getNumOfComponentsOfArray(const _uint_t arrayID)
	{
		return numOfComp[arrayID];
	}

	std::vector<_float_t>& getArray(const _uint_t arrayID)
	{
		return fieldArr[arrayID];
	}

	const std::string getArrayName(const _uint_t arrayID)
	{
		return arrName[arrayID];
	}

	void addArray(const std::string& name, const _uint_t numOfComponent, std::vector<_float_t>& data)
	{
		numOfComp.push_back(numOfComponent);
		fieldArr.push_back(data);
		arrName.push_back(name);
	}

	void clearAll()
	{
		numOfComp.clear();
		fieldArr.clear();
		arrName.clear();
	}

	static DataFieldVTKWriter* Instance()
	{
		if (_instance == 0)
		{
			_instance = new DataFieldVTKWriter;
		}
		return _instance;
	}

	template<typename T>
	inline static void writeVTKDataArray
		(std::ofstream* ofpVTK, const std::string& type, const std::string& name, std::vector<T>& data, const _uint_t numOfComponent, bool bWriteNumberOfComponents)
	{
		if (data.size() == 0)
		{
			return;
		}

		*ofpVTK << "\t\t\t\t<DataArray type=\"" << type << "\"";
		if (name != "")
		{
			*ofpVTK << " Name=\"" << name << "\"";
		}
		if (bWriteNumberOfComponents)
		{
			if (numOfComponent != 1)
			{
				*ofpVTK << " NumberOfComponents=\"" << numOfComponent << "\"";
			}
		}
		*ofpVTK << " format=\"ascii\">" << std::endl;
		for (_uint_t i=0; i<(_uint_t)data.size()/numOfComponent; i++)
		{
		    *ofpVTK << "\t\t\t\t\t";
            for (_uint_t i2=0; i2<numOfComponent; i2++)
            {
                *ofpVTK << data[numOfComponent*i+i2] << "\t";
            }
		    *ofpVTK << std::endl;
		}
		*ofpVTK << "\t\t\t\t</DataArray>" << std::endl;
	}

	inline static void writeExternalFields(std::ofstream* ofpVTK)
	{
		DataFieldVTKWriter* wrapArr = DataFieldVTKWriter::Instance();
		for (_uint_t i=0; i<(_uint_t)wrapArr->getNumOfArrays(); i++)
		{
			writeVTKDataArray<_float_t>(ofpVTK, "Float64", wrapArr->getArrayName(i), wrapArr->getArray(i), wrapArr->getNumOfComponentsOfArray(i), true);
		}
	}

private:

	std::vector<_uint_t> numOfComp;

	std::vector<std::vector<_float_t> > fieldArr;

	std::vector<std::string> arrName;

	static DataFieldVTKWriter* _instance;

};



#ifdef _STD_CPP_11
	template<typename ExtFieldWriterT>
	void DOWorld::WriteVTK(const std::string filename) const
	{
		std::ofstream ofVTK(filename.c_str(), std::ios::out);
		if (!ofVTK)
		{
			std::cerr
				<< "Error!! Code: DOWorld::WriteVTK(const std::string, const DOWorld*)" << std::endl
				<< "        Note: DOWorld cannot write vtk file"                        << std::endl;
			exit(-1);
		}

		/// raw data arrays
		std::vector<_float_t> point_Position, point_Velocity, point_AngularVelocity;
		std::vector<_float_t> point_Radius, point_VelocityScale, point_AngularVelocityScale, point_GranularTempV, point_GranularTempAV;

		/// read data to arrays
		std::vector<DOStatus *>::const_iterator idos;
		_float_t Radius;
		Vector3df vTemp;
		for (idos=cDOStatus.begin(); idos!=cDOStatus.end(); ++idos)
		{
			switch (GetDOModel((*idos)->GetDOName())->GetShapeType())
			{
				case Sphere:
					Radius = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().sphere.radius;
					point_Radius.push_back(Radius);
					point_Position.push_back((*idos)->GetPosition().x());
					point_Position.push_back((*idos)->GetPosition().y());
					point_Position.push_back((*idos)->GetPosition().z());
					vTemp = (*idos)->GetVelocity();
					point_Velocity.push_back(vTemp.x());
					point_Velocity.push_back(vTemp.y());
					point_Velocity.push_back(vTemp.z());
					point_VelocityScale.push_back(vTemp.length());
					vTemp = (*idos)->GetAngularVelocity();
					point_AngularVelocity.push_back(vTemp.x());
					point_AngularVelocity.push_back(vTemp.y());
					point_AngularVelocity.push_back(vTemp.z());
					point_AngularVelocityScale.push_back(vTemp.length());
					point_GranularTempV.push_back((*idos)->GetGranularTemperatureV());
					point_GranularTempAV.push_back((*idos)->GetGranularTemperatureAV());
					break;
				//case Polyhedra:
					// TODO:
					//break;
				//case QuasiPlate:
					// TODO:
					//break;
				//case QuasiPlateWithCircularHole:
					// TODO:
					//break;
				//case QuasiCylinder:
					// TODO:
					//break;
				default:
					point_Radius.push_back(0.0);
					point_Position.push_back((*idos)->GetPosition().x());
					point_Position.push_back((*idos)->GetPosition().y());
					point_Position.push_back((*idos)->GetPosition().z());
					vTemp = (*idos)->GetVelocity();
					point_Velocity.push_back(vTemp.x());
					point_Velocity.push_back(vTemp.y());
					point_Velocity.push_back(vTemp.z());
					point_VelocityScale.push_back(vTemp.length());
					vTemp = (*idos)->GetAngularVelocity();
					point_AngularVelocity.push_back(vTemp.x());
					point_AngularVelocity.push_back(vTemp.y());
					point_AngularVelocity.push_back(vTemp.z());
					point_AngularVelocityScale.push_back(vTemp.length());
					point_GranularTempV.push_back((*idos)->GetGranularTemperatureV());
					point_GranularTempAV.push_back((*idos)->GetGranularTemperatureAV());
			}
		}

		// write VTK file
		ofVTK
			<< "<?xml version=\"1.0\"?>"                                                           << std::endl
			<< "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">"   << std::endl
			<< "\t<UnstructuredGrid>"                                                              << std::endl
			<< "\t\t<Piece NumberOfPoints=\"" << point_Radius.size() << "\" NumberOfCells=\"0\">" << std::endl
			<< "\t\t\t<PointData Scalars=\"Radius\">"                                              << std::endl;
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "Position"             , point_Position            , 3, true );
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "Velocity"             , point_Velocity            , 3, true );
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "VelocityScale"        , point_VelocityScale       , 1, false);
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "AngularVelocity"      , point_AngularVelocity     , 3, true );
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "AngularVelocityScale" , point_AngularVelocityScale, 1, false);
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "Radius"               , point_Radius              , 1, false);
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "GranularTemperatureV" , point_GranularTempV       , 1, false);
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "GranularTemperatureAV", point_GranularTempAV      , 1, false);
		ExtFieldWriterT::writeExternalFields(&ofVTK);

		ofVTK
			<< "\t\t\t</PointData>"                                                                << std::endl
			<< "\t\t\t<Points>"                                                                    << std::endl
			<< "\t\t\t\t<DataArray type=\"Float64\" NumberOfComponents=\"3\" format=\"ascii\">"    << std::endl;
		for (_uint_t u=0; u<(_uint_t)point_Position.size(); u++)
		{
			ofVTK << point_Position[u] << " ";
		}
		ofVTK
			<< std::endl
			<< "\t\t\t\t</DataArray>"                                                              << std::endl
			<< "\t\t\t</Points>"                                                                   << std::endl
			<< "\t\t\t<Cells>"                                                                     << std::endl
			<< "\t\t\t\t<DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">"                << std::endl
			<< "\t\t\t\t</DataArray>"                                                              << std::endl
			<< "\t\t\t</Cells>"                                                                    << std::endl
			<< "\t\t</Piece>"                                                                      << std::endl
			<< "\t</UnstructuredGrid>"                                                             << std::endl
			<< "</VTKFile>"                                                                        << std::endl;

		ofVTK.close();
	}

	template<typename ExtFieldWriterT>
	void DOWorld::WriteVTK(const std::string filename, const IactRecordTab* irtp) const
	{
		std::ofstream ofVTK(filename.c_str(), std::ios::out);
		if (!ofVTK)
		{
			std::cerr
				<< "Error!! Code: DOWorld::WriteVTK(const std::string, const DOWorld*)" << std::endl
				<< "        Note: DOWorld cannot write vtk file"                        << std::endl;
			exit(-1);
		}

		// read "DOStatus" data to arrays
		std::vector<_float_t>
			point_Position, point_Velocity, point_AngularVelocity,
			point_Radius, point_VelocityScale, point_AngularVelocityScale, point_GranularTempV, point_GranularTempAV;

		std::vector<DOStatus *>::const_iterator idos;
		_float_t Radius;
		Vector3df vTemp;
		for (idos=cDOStatus.begin(); idos!=cDOStatus.end(); ++idos)
		{
			switch (GetDOModel((*idos)->GetDOName())->GetShapeType())
			{
				case Sphere:
					Radius = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().sphere.radius;
					point_Radius.push_back(Radius);
					point_Position.push_back((*idos)->GetPosition().x());
					point_Position.push_back((*idos)->GetPosition().y());
					point_Position.push_back((*idos)->GetPosition().z());
					vTemp = (*idos)->GetVelocity();
					point_Velocity.push_back(vTemp.x());
					point_Velocity.push_back(vTemp.y());
					point_Velocity.push_back(vTemp.z());
					point_VelocityScale.push_back(vTemp.length());
					vTemp = (*idos)->GetAngularVelocity();
					point_AngularVelocity.push_back(vTemp.x());
					point_AngularVelocity.push_back(vTemp.y());
					point_AngularVelocity.push_back(vTemp.z());
					point_AngularVelocityScale.push_back(vTemp.length());
					point_GranularTempV.push_back((*idos)->GetGranularTemperatureV());
					point_GranularTempAV.push_back((*idos)->GetGranularTemperatureAV());
					break;
				//case Polyhedra:
					// TODO:
					//break;
				//case QuasiPlate:
					// TODO:
					//break;
				//case QuasiPlateWithCircularHole:
					// TODO:
					//break;
				//case QuasiCylinder:
					// TODO:
					//break;
				default:
					point_Radius.push_back(0.0);
					point_Position.push_back((*idos)->GetPosition().x());
					point_Position.push_back((*idos)->GetPosition().y());
					point_Position.push_back((*idos)->GetPosition().z());
					vTemp = (*idos)->GetVelocity();
					point_Velocity.push_back(vTemp.x());
					point_Velocity.push_back(vTemp.y());
					point_Velocity.push_back(vTemp.z());
					point_VelocityScale.push_back(vTemp.length());
					vTemp = (*idos)->GetAngularVelocity();
					point_AngularVelocity.push_back(vTemp.x());
					point_AngularVelocity.push_back(vTemp.y());
					point_AngularVelocity.push_back(vTemp.z());
					point_AngularVelocityScale.push_back(vTemp.length());
					point_GranularTempV.push_back((*idos)->GetGranularTemperatureV());
					point_GranularTempAV.push_back((*idos)->GetGranularTemperatureAV());
			}
		}

		// read "ImpactStatus" data to arrays
		std::vector<_float_t>
			interaction_NormalStiffness, interaction_InitialVelocity, interaction_Overlap,
			interaction_ShearForce, interaction_ShearForceScale, interaction_ImpactDirection,
			interaction_ImpactToMaster, interaction_ImpactToMasterScale,
			interaction_AngularImpactToMaster, interaction_AngularImpactToMasterScale;
        std::vector<_float_t> interaction_ImpactPoint;
		std::vector<_uint_t> interaction_SerialNumber, interaction_Contact, interaction_Bond;
		const std::map< std::pair<_uint_t, _uint_t>, ImpactStatus >* mapImStatusP = &(irtp->GetData());
		if (mapImStatusP->size() != 0)
		{
			std::map<std::pair<_uint_t, _uint_t>, ImpactStatus>::const_iterator mmapImStatus;
			const ImpactStatus* isp;
			Vector3df vShearForce, vImpactPoint, vImpactDirection, vImpactToMaster, vAngularImpactToMaster;
			const _float_t* dpUDV;
			for (mmapImStatus=(mapImStatusP->begin()); mmapImStatus!=(mapImStatusP->end()); mmapImStatus++)
			{
				interaction_SerialNumber              .push_back(mmapImStatus->first.first);
				interaction_SerialNumber              .push_back(mmapImStatus->first.second);
				isp = &(mmapImStatus->second);
				if (isp->Contact())
				{
					interaction_Contact               .push_back(1);
				}
				else
				{
					interaction_Contact               .push_back(0);
				}
				if (isp->Bond())
				{
					interaction_Bond                  .push_back(1);
				}
				else
				{
					interaction_Bond                  .push_back(0);
				}
				interaction_NormalStiffness           .push_back(isp->Kn());
				interaction_InitialVelocity           .push_back(isp->InitialVelocity());
				interaction_Overlap                   .push_back(isp->Overlap());
				vTemp = isp->ShearForce();
				interaction_ShearForce                .push_back(vTemp.x());
				interaction_ShearForce                .push_back(vTemp.y());
				interaction_ShearForce                .push_back(vTemp.z());
				interaction_ShearForceScale           .push_back(vTemp.length());
				vTemp = isp->ImpactPoint();
				interaction_ImpactPoint               .push_back(vTemp.x());
				interaction_ImpactPoint               .push_back(vTemp.y());
				interaction_ImpactPoint               .push_back(vTemp.z());
				vTemp = isp->ImpactDirection();
				interaction_ImpactDirection           .push_back(vTemp.x());
				interaction_ImpactDirection           .push_back(vTemp.y());
				interaction_ImpactDirection           .push_back(vTemp.z());
				vTemp = isp->ImpactToMaster();
				interaction_ImpactToMaster            .push_back(vTemp.x());
				interaction_ImpactToMaster            .push_back(vTemp.y());
				interaction_ImpactToMaster            .push_back(vTemp.z());
				interaction_ImpactToMasterScale       .push_back(vTemp.length());
				vTemp = isp->AngularImpactToMaster();
				interaction_AngularImpactToMaster     .push_back(vTemp.x());
				interaction_AngularImpactToMaster     .push_back(vTemp.y());
				interaction_AngularImpactToMaster     .push_back(vTemp.z());
				interaction_AngularImpactToMasterScale.push_back(vTemp.length());
			}
		}

		// write VTK file
		ofVTK
			<< "<?xml version=\"1.0\"?>"                                                           << std::endl
			<< "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">"   << std::endl
			<< "\t<UnstructuredGrid>"                                                              << std::endl
			<< "\t\t<Piece NumberOfPoints=\"" << point_Radius.size() << "\" NumberOfCells=\"" << interaction_Overlap.size() << "\">" << std::endl
			<< "\t\t\t<PointData Scalars=\"Radius\">"                                              << std::endl;
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "Position"             , point_Position            , 3, true );
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "Velocity"             , point_Velocity            , 3, true );
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "VelocityScale"        , point_VelocityScale       , 1, false);
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "AngularVelocity"      , point_AngularVelocity     , 3, true );
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "AngularVelocityScale" , point_AngularVelocityScale, 1, false);
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "Radius"               , point_Radius              , 1, false);
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "GranularTemperatureV" , point_GranularTempV       , 1, false);
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "GranularTemperatureAV", point_GranularTempAV      , 1, false);
		ExtFieldWriterT::writeExternalFields(&ofVTK);
		ofVTK << "\t\t\t</PointData>" << std::endl;

		ofVTK << "\t\t\t<CellData Scalars=\"InteractionImpactToMaster\">" << std::endl;
		DataFieldVTKWriter::writeVTKDataArray<_uint_t >(&ofVTK, "Int32"  , "InteractionContact"                   , interaction_Contact                   , 1, false);
		DataFieldVTKWriter::writeVTKDataArray<_uint_t >(&ofVTK, "Int32"  , "InteractionBond"                      , interaction_Bond                      , 1, false);
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "InteractionNormalStiffness"           , interaction_NormalStiffness           , 1, false);
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "InteractionInitialVelocity"           , interaction_InitialVelocity           , 1, false);
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "InteractionOverlap"                   , interaction_Overlap                   , 1, false);
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "InteractionImpactPoint"               , interaction_ImpactPoint               , 3, true );
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "InteractionShearForce"                , interaction_ShearForce                , 3, true );
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "InteractionShearForceScale"           , interaction_ShearForceScale           , 1, false);
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "InteractionImpactDirection"           , interaction_ImpactDirection           , 3, true );
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "InteractionImpactToMaster"            , interaction_ImpactToMaster            , 3, true );
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "InteractionImpactToMasterScale"       , interaction_ImpactToMasterScale       , 1, false);
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "InteractionAngularImpactToMaster"     , interaction_AngularImpactToMaster     , 3, true );
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "InteractionAngularImpactToMasterScale", interaction_AngularImpactToMasterScale, 1, false);

		ofVTK
            << "\t\t\t</CellData>" << std::endl;

		ofVTK
			<< "\t\t\t<Points>"                                                                    << std::endl;
		DataFieldVTKWriter::writeVTKDataArray<_float_t>(&ofVTK, "Float64", "", point_Position, 3, true );
		ofVTK
			<< "\t\t\t</Points>"                                                                   << std::endl;

		ofVTK << "\t\t\t<Cells>" << std::endl;
		DataFieldVTKWriter::writeVTKDataArray<_uint_t >(&ofVTK, "Int32"  , "connectivity", interaction_SerialNumber, 2, false);
		ofVTK << "\t\t\t\t<DataArray type=\"Int32\" Name=\"offsets\" Format=\"ascii\">" << std::endl;
		for (_uint_t u=0, uCounter=2; u<interaction_Overlap.size(); u++, uCounter+=2)
		{
			ofVTK << "\t\t\t\t\t" << uCounter << std::endl;
		}
		ofVTK << "\t\t\t\t</DataArray>" << std::endl;
		ofVTK << "\t\t\t\t<DataArray type=\"Int32\" Name=\"types\" Format=\"ascii\">" << std::endl;
		for (_uint_t u=0; u<interaction_Overlap.size(); u++)
		{
			ofVTK << "\t\t\t\t\t3" << std::endl;
		}
		ofVTK << "\t\t\t\t</DataArray>" << std::endl;
		ofVTK
			<< "\t\t\t</Cells>"                                                                    << std::endl
			<< "\t\t</Piece>"                                                                      << std::endl
			<< "\t</UnstructuredGrid>"                                                             << std::endl
			<< "</VTKFile>"                                                                        << std::endl;

		ofVTK.close();
	}
#endif   // _STD_CPP_11

}   // namespace vedo

#endif // _DOWORLD_WRITE_VTK__H
