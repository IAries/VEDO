#include <Iris/Interfaces/DConsultant.h>
#include <Iris/Interfaces/DOWorldSorter.h>
#include <NJRLib/Interfaces/Utility.h>
#include <Common/Interfaces/DOMap.h>
#include <algorithm>
#include <cmath>
#include <set>
#include <fstream>
#include <sstream>

#undef min

void DConsultant::SyncDOContainer(DOContainer & vDO) const
{
	if ( assistant.rank() == DConsultant::MASTER )
		MasterSyncDOContainer(vDO);
	else // it's not Master
		SlaveSyncDOContainer(vDO);
}

void DConsultant::MasterSyncDOContainer(DOContainer & vDO) const
{
	//ensure the buf needed is enough
	for(unsigned i=0; i<assistant.size(); i++) {
		if ( i== DConsultant::MASTER )
			continue;
		if (overlapTab[i].size() > sendBufVec[i].first) {
			sendBufVec[i].first = overlapTab[i].size();
			delete sendBufVec[i].second;
			sendBufVec[i].second=new double [ overlapTab[i].size()*6 ];
			recvBufVec[i].first = overlapTab[i].size();
			delete recvBufVec[i].second;
			recvBufVec[i].second = new double [ overlapTab[i].size()*6 ];
		}
	}

	// then recived the partial result made by Slaves

	for(unsigned i=0; i<assistant.size(); i++) {
		if( i == DConsultant::MASTER )
			continue;
		double* impactRecBuf = recvBufVec[i].second;
		assistant.transferImpactRecv(impactRecBuf,  6*overlapTab[i].size(), i);
	}

	assistant.wait();

	// calculate the global result
	for (unsigned i=0;i<assistant.size();++i) {
		if( i == DConsultant::MASTER )
			continue;
		double* impactRecBuf = recvBufVec[i].second;
		NJRvector3d im,aim;
		for(unsigned long j=0; j<overlapTab[i].size(); j++) {
			unsigned long idx_G= overlapTab[i][j];
			unsigned long tt= 6*j;
			im.Set(impactRecBuf[tt],impactRecBuf[tt+1],impactRecBuf[tt+2]);
			aim.Set(impactRecBuf[tt+3],impactRecBuf[tt+4],impactRecBuf[tt+5]);
			vDO[idx_G]->AddImpact(im , aim);
		}
	}

	//distribute the global result
	for(unsigned i=0; i< assistant.size() ; i++) {
		if( i== DConsultant::MASTER )
			continue;
		double* impactSedBuf=sendBufVec[i].second ;
		for(unsigned long j=0; j<overlapTab[i].size(); j++) {
			unsigned long tt = 6*j ;
			impactSedBuf[tt]=vDO[j]->GetImpact().x();
			impactSedBuf[tt+1]=vDO[j]->GetImpact().y();
			impactSedBuf[tt+2]=vDO[j]->GetImpact().z();
			impactSedBuf[tt+3]=vDO[j]->GetAngularImpact().x();
			impactSedBuf[tt+4]=vDO[j]->GetAngularImpact().y();
			impactSedBuf[tt+5]=vDO[j]->GetAngularImpact().z();

		}
		assistant.transferImpactSend(impactSedBuf, 6*overlapTab[i].size(), i);
	}

	assistant.wait();

}


void DConsultant::SlaveSyncDOContainer(DOContainer & vDO) const
{

	// ensure the buf needed is enough
	if (DConsultant::DOTab.size() > sendBufVec[0].first) {
		sendBufVec[0].first = DConsultant::DOTab.size();
		delete sendBufVec[0].second;
		sendBufVec[0].second=new double [ DConsultant::DOTab.size()*6 ];
		recvBufVec[0].first = DConsultant::DOTab.size();
		delete recvBufVec[0].second;
		recvBufVec[0].second = new double [ DConsultant::DOTab.size()*6 ];
	}

	// send the partial result to MASTER
	double* impactSedBuf=sendBufVec[0].second ;
	unsigned long tt;
	for(unsigned long j=0; j<vDO.size(); ++j) {
		tt=6*j;
		impactSedBuf[tt]=vDO[j]->GetImpact().x();
		impactSedBuf[tt+1]=vDO[j]->GetImpact().y();
		impactSedBuf[tt+2]=vDO[j]->GetImpact().z();
		impactSedBuf[tt+3]=vDO[j]->GetAngularImpact().x();
		impactSedBuf[tt+4]=vDO[j]->GetAngularImpact().y();
		impactSedBuf[tt+5]=vDO[j]->GetAngularImpact().z();
	}
	assistant.transferImpactSend(impactSedBuf, 6*vDO.size(), MASTER );

	// receive the global result from MASTER
	double* impactRecBuf = recvBufVec[0].second;
	assistant.transferImpactRecv(impactRecBuf,  6*vDO.size(), MASTER);
	assistant.wait();

	NJRvector3d im,aim;
	for(unsigned long j=0; j<vDO.size(); ++j) {
		unsigned long tt = 6*j;
		im.Set(impactRecBuf[tt],impactRecBuf[tt+1],impactRecBuf[tt+2]);
		aim.Set(impactRecBuf[tt+3],impactRecBuf[tt+4],impactRecBuf[tt+5]);
		vDO[j]->ClearImpact();
		vDO[j]->AddImpact( im , aim);
	}
}

