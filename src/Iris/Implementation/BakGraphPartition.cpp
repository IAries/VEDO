#include <Iris/Interfaces/NBSParallelConsultant.h>
#include <NJRLib/Interfaces/Utility.h>
#include <Common/Interfaces/NBSConsultant.h>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <limits>
#include <ctime>

#include <parmetis.h>

/*  the two undef prevents confliction min and max in std namespace
	conflicts with the macros in <mpi.h> */
#undef min
#undef max

using namespace std;

void NBSParallelConsultant::GraphPartitionIactPair()
{

	unsigned long totalParticle = this->GetDOWorld()->GetSystemParameter()->GetDONumber();
    unsigned long avgParticle = totalParticle / NP;

	vector<vector<unsigned long> > vecAdjncy ;
	if (rank != NP-1 )
        vecAdjncy.assign(avgParticle  ,vector<unsigned long>() );
	else
		vecAdjncy.assign(avgParticle+ totalParticle%NP ,vector<unsigned long>() );

	unsigned long mp ,sp ;
	for (int i=0;i<IactPairTab.size();++i) {
		mp = IactPairTab[i].first ;
		int mr = mp / avgParticle;
		if (mr == NP )
			mr=mr-1;
		sp = IactPairTab[i].second ;
		int sr = sp / avgParticle;
		if (sr == NP )
			sr=sr-1;
		if ( mr == rank )
			vecAdjncy[mp - rank*avgParticle].push_back(sp);
		if ( sr == rank )
			vecAdjncy[sp - rank*avgParticle].push_back(mp);

	}

	// finish the xyz array
	float* xyz = new float [3*vecAdjncy.size()];
	for (int i=0;i<vecAdjncy.size();++i) {
		int gid = i+rank*avgParticle;
		NJRvector3d p = pDOWorld->GetDOStatus(gid)->GetPosition();
		xyz[3*i] = p.x();
		xyz[3*i+1] = p.y();
		xyz[3*i+2] = p.z();
	}


	//finish the xadj and adjncy array;
	int totalSize = 0;
	for (int i=0; i< vecAdjncy.size();++i)
        totalSize += vecAdjncy[i].size();
	int* adjncy = new int[ totalSize ];
	int* adjwgt = new int[ totalSize ];
	fill(adjwgt,adjwgt+totalSize,1);
	int* xadj = new int[ vecAdjncy.size() +1];
	int* vwgt = new int[ vecAdjncy.size() ];
	xadj[ vecAdjncy.size() ] = totalSize;
	int offset =0 ;
	for (int i=0; i< vecAdjncy.size();++i)
	{
		copy(vecAdjncy[i].begin() , vecAdjncy[i].end(), adjncy+offset);
		xadj[i]= offset ;
		offset+= vecAdjncy[i].size();
		vwgt[i] = vecAdjncy[i].size();
	}
	//finish the vtxdist array
	int* vtxdist = new int[NP+1 ];
	for (int i=0;i<NP;++i)
		vtxdist[i] = i*avgParticle;
	vtxdist[NP] = totalParticle ;

	//
	int* part =new int [ vecAdjncy.size() ];

	// wgtflag
	// wgtflag=0 represents no any weight
	int wgtflag = 2;

	// numflag
	int numflag = 0;

	//
	int ncon =1 ;

	//
	int nparts =NP;

	//
	float *tpwgts = new float[ NP ];

	fill( tpwgts,tpwgts+NP, 1.0/(double)NP );

	//float ubvec
	float ubvec =1.00 ;

	//option
	int option =0;

	int edgecut;

	int ndims=3;

	MPI_Comm comm;
	MPI_Comm_dup(MPI_COMM_WORLD,&comm);

	//ParMETIS_V3_PartKway(vtxdist,xadj,adjncy,vwgt,adjwgt,&wgtflag,&numflag,&ncon,&nparts,
	//	tpwgts,&ubvec,&option,&edgecut,part,&comm);
	ParMETIS_V3_PartGeomKway(vtxdist,xadj,adjncy,vwgt,adjwgt,&wgtflag,&numflag,
		&ndims,xyz,&ncon,&nparts,
		tpwgts,&ubvec,&option,&edgecut,part,&comm);
	//ParMETIS_V3_RefineKway(vtxdist,xadj,adjncy,vwgt,adjwgt,&wgtflag,&numflag,&ncon,&nparts,
	//	tpwgts,&ubvec,&option,&edgecut,part,&comm);
	//ParMETIS_V3_RefineKway(vtxdist,xadj,adjncy,vwgt,adjwgt,&wgtflag,&numflag,&ncon,&nparts,
	//	tpwgts,&ubvec,&option,&edgecut,part,&comm);


	//to gather
	int *globalpart = new int[ totalParticle ];
	int *recvcounts = new int[NP];
	int *displs = new int[NP];
	for (int i=0;i<NP;++i) {
		if ( i==NP-1 )
			recvcounts[i] = avgParticle + totalParticle%NP;
		else
			recvcounts[i] = avgParticle;
		displs[i] = i*avgParticle;
	}
	MPI_Allgatherv(part,vecAdjncy.size(),MPI_INT,globalpart,recvcounts,displs,MPI_INT,MPI_COMM_WORLD);

	int* globalwgt = new int[totalParticle];
	MPI_Allgatherv(vwgt,vecAdjncy.size(),MPI_INT,globalwgt,recvcounts,displs,MPI_INT,MPI_COMM_WORLD);

	int localwgt=0;
	for (int i=0;i<totalParticle;++i) {
		if (globalpart[i] == rank) {
			localwgt+=globalwgt[i];
		}
	}



	vector<pair<unsigned long,unsigned long> > tmpIactVec;

	vector<int> nvec;
	nvec.assign(totalParticle,0);
	for (int i=0 ; i<IactPairTab.size();++i ) {
		int p=IactPairTab[i].first;
		int s=IactPairTab[i].second;

		if (globalpart[p]!=globalpart[s]) {
			nvec[p]++;
			nvec[s]++;
		}
	}


	int localIact=0;
	for (int i=0 ; i<IactPairTab.size();++i )
	{
		int pm = IactPairTab[i].first;
		int ps = IactPairTab[i].second;

		if ( globalpart[pm] == rank && globalpart[ps] ==rank ) {

			tmpIactVec.push_back( make_pair( pm , ps ) );
			localIact++;

		}else if( globalpart[pm] == rank ) {
			if ( nvec[pm] <= nvec[ps] )
				tmpIactVec.push_back( make_pair( pm , ps ) );
		}
		else if ( globalpart[ps] == rank ) {
			if ( nvec[ps] < nvec[pm] )
				tmpIactVec.push_back( make_pair( pm , ps ) );
		}
		//else do nothing
	}
	IactPairTab = tmpIactVec;

	delete[] xadj;
	delete[] adjncy;
	delete[] vtxdist;
	delete[] tpwgts;
	delete[] part;
	delete[] recvcounts;
	delete[] displs;
	delete[] vwgt;
	delete[] adjwgt;
	delete[] globalwgt;
	delete[] xyz;


	cout<<"rank: "<<rank<<"localwgt= "<<localwgt<<" edgecut: "<<edgecut<<" localIact: "<<localIact<<endl;
}

