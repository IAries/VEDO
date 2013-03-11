#include <mpi.h>
#include <Iris/Interfaces/NBSGPConsultant.h>
#include <NJRLib/Interfaces/Utility.h>
#include <Common/Interfaces/NBSConsultant.h>
#include <parmetis.h>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <iterator>
#include <limits>

/*****************************************************************************
 * The two undef prevents confliction min and max in std namespace conflicts
 * with the macros in <mpi.h>
 ******************************************************************************/
#undef max
#undef min

using namespace std;

void NBSGPConsultant::GraphPartitionIactPair()
{
	unsigned long totalParticle
		= this->GetDOWorld()->GetSystemParameter()->GetDONumber();

    unsigned long avgParticle = totalParticle / NP;

	unsigned long localParticle
		= (rank != NP-1 ) ? avgParticle : (avgParticle+ totalParticle%NP);

	unsigned long avgIact = IactPairTab.size() / NP;

	vector< vector<unsigned long> > vecAdjncy ;
	vecAdjncy.assign(localParticle, vector<unsigned long>());

	unsigned long mp, sp;

	for (int i=0; i<IactPairTab.size(); ++i)
	{
		mp = IactPairTab[i].first;
		int mr = mp / avgParticle;
		if (mr == NP)
		{
			mr = mr - 1;
		}
		sp = IactPairTab[i].second;
		int sr = sp / avgParticle;
		if (sr == NP)
		{
			sr = sr - 1;
		}
		if (mr == rank)
		{
			vecAdjncy[mp - rank*avgParticle].push_back(sp);
		}
		if (sr == rank)
		{
			vecAdjncy[sp - rank*avgParticle].push_back(mp);
		}
	}

	for (unsigned int i=0; i<vecAdjncy.size(); ++i)
	{
		sort(vecAdjncy[i].begin(), vecAdjncy[i].end());
	}

	// Finish the xyz array
	float* xyz = new float [3 * vecAdjncy.size()];
	for (int i=0; i<vecAdjncy.size(); ++i)
	{
		int gid = i + rank * avgParticle;
		NJRvector3d p = pDOWorld->GetDOStatus(gid)->GetPosition();
		xyz[3*i]   = p.x();
		xyz[3*i+1] = p.y();
		xyz[3*i+2] = p.z();
	}

	// Finish the xadj and adjncy array;
	int totalSize = 0;
	for (int i=0; i<vecAdjncy.size(); ++i)
	{
        totalSize += vecAdjncy[i].size();
	}
	int* adjncy            = new int[totalSize];
	int* adjwgt            = new int[totalSize];
	fill(adjwgt, adjwgt+totalSize, 1);
	int* xadj              = new int[vecAdjncy.size()+1];
	int* vwgt              = new int[vecAdjncy.size()];
	xadj[vecAdjncy.size()] = totalSize;
	int offset             = 0;
	for (int i=0; i<vecAdjncy.size(); ++i)
	{
		copy(vecAdjncy[i].begin(), vecAdjncy[i].end(), adjncy+offset);
		xadj[i] = offset;
		offset += vecAdjncy[i].size();
		vwgt[i] = vecAdjncy[i].size();
	}
	// Finish the vtxdist array
	int* vtxdist = new int[NP+1];
	for(int i=0; i<NP; ++i)
	{
		vtxdist[i] = i * avgParticle;
	}
	vtxdist[NP] = totalParticle;

	int* part = new int[vecAdjncy.size()];

	// wgtflag = 2 represents edge weight neglected
	int wgtflag = 2;

	int numflag = 0;

	int ncon = 1;

	int nparts = NP;

	float *tpwgts = new float[NP];

	fill(tpwgts, tpwgts+NP, 1.0/(double)NP);

	float ubvec = 1.0;

	// Option
	int option = 0;

	int edgecut;

	int ndims = 3;

	MPI_Comm comm;
	MPI_Comm_dup(MPI_COMM_WORLD, &comm);

	int *globalpart = new int[totalParticle];
	int *recvcounts = new int[NP];
	int *displs = new int[NP];
	for (int i=0; i<NP; ++i)
	{
		if (i == NP-1)
		{
			recvcounts[i] = avgParticle + totalParticle%NP;
		}
		else
		{
			recvcounts[i] = avgParticle;
		}
		displs[i] = i * avgParticle;
	}

	int* globalwgt = new int[totalParticle];

	int* pneighcnt = new int[NP*totalParticle];

	int** neighcnt = new int* [NP];

	for (int i=0; i<NP; ++i)
	{
		neighcnt[i] = pneighcnt + i * totalParticle;
	}

	unsigned long *numIactBK = new unsigned long[NP];

	int times = 0;
	double UNB = 100.0;

	while ((times < MAX_CUT) && (UNB > MAX_ALLOWED_UNB))
	{
		/*************************************************
			MPI_Allgatherv(vwgt,vecAdjncy.size(),MPI_INT,globalwgt,recvcounts,displs,MPI_INT,MPI_COMM_WORLD);

			int localwgt=0;
			for (int i=0;i<totalParticle;++i) {
				//if (globalpart[i] == rank)
					localwgt+=globalwgt[i];
			}
			if (rank==0)
			{
				cout << "wgt total" << localwgt << endl;
			}
		**************************************************/

		if (times == 0)
		{
		//ParMETIS_V3_PartKway(vtxdist,xadj,adjncy,vwgt,adjwgt,&wgtflag,&numflag,&ncon,&nparts,
			//tpwgts,&ubvec,&option,&edgecut,part,&comm);
			ParMETIS_V3_PartGeomKway
				(vtxdist,
				xadj,
				adjncy,
				vwgt,
				adjwgt,
				&wgtflag,
				&numflag,
				&ndims,
				xyz,
				&ncon,
				&nparts,
				tpwgts,
				&ubvec,
				&option,
				&edgecut,
				part,
				&comm    );
		//ParMETIS_V3_RefineKway(vtxdist,xadj,adjncy,vwgt,adjwgt,&wgtflag,&numflag,&ncon,&nparts,
		//	tpwgts,&ubvec,&option,&edgecut,part,&comm);
		}
		else
		{
			ParMETIS_V3_PartGeomKway
				(vtxdist,
				xadj,
				adjncy,
				vwgt,
				adjwgt,
				&wgtflag,
				&numflag,
				&ndims,
				xyz,
				&ncon,
				&nparts,
				tpwgts,
				&ubvec,
				&option,
				&edgecut,
				part,
				&comm    );
		//ParMETIS_V3_RefineKway(vtxdist,xadj,adjncy,vwgt,adjwgt,&wgtflag,&numflag,&ncon,&nparts,
		//	tpwgts,&ubvec,&option,&edgecut,part,&comm);
		//ParMETIS_V3_RefineKway(vtxdist,xadj,adjncy,vwgt,adjwgt,&wgtflag,&numflag,&ncon,&nparts,
		//	tpwgts,&ubvec,&option,&edgecut,part,&comm);
		}
		cout << "ith iter : edgecut = " << edgecut << endl;

		MPI_Allgatherv
			(part,
			vecAdjncy.size(),
			MPI_INT,
			globalpart,
			recvcounts,
			displs,
			MPI_INT,
			MPI_COMM_WORLD   );

		MPI_Allgatherv
			(vwgt,
			vecAdjncy.size(),
			MPI_INT,
			globalwgt,
			recvcounts,
			displs,
			MPI_INT,
			MPI_COMM_WORLD   );

		/*int localwgt=0;
		for (int i=0;i<totalParticle;++i) {
			if (globalpart[i] == rank) {
				localwgt+=globalwgt[i];
			}
		}*/

		fill(pneighcnt, pneighcnt+NP*totalParticle, 0);

		//vector<int> nvec;
		//nvec.assign(totalParticle,0);

		for (int i=0; i<IactPairTab.size(); ++i)
		{
			int p = IactPairTab[i].first;
			int s = IactPairTab[i].second;

			if (globalpart[p] != globalpart[s])
			{
				neighcnt[ globalpart[s] ][p]++;
				neighcnt[ globalpart[p] ][s]++;
				//nvec[p]++;
				//nvec[s]++;
			}
		}

		fill(vwgt, vwgt+localParticle, 0);
		fill(numIactBK, numIactBK+NP, 0);
		// dynamic adjust the weight

		int pm, ps;
		for (int i=0; i<IactPairTab.size(); ++i)
		{
			pm = IactPairTab[i].first;
			ps = IactPairTab[i].second;
			// for adjust the node weight
			int mr = pm / avgParticle;
			if (mr == NP)
			{
				mr = mr - 1;
			}
			int sr = ps / avgParticle;
			if (sr == NP)
			{
				sr = sr - 1;
			}
			if (mr == rank)
			{
				if (globalpart[pm] == globalpart[ps])
				{
					vwgt[pm-mr*avgParticle]++;
				}
				else if (neighcnt[ globalpart[ps] ][pm]
						<= neighcnt[ globalpart[pm] ][ps])
				{
					//vwgt[pm-mr*avgParticle]++;
					vwgt[pm-mr*avgParticle] += 2;
				}
			}
			if (sr == rank)
			{
				if (globalpart[pm] == globalpart[ps])
				{
					vwgt[ps-sr*avgParticle]++;
				}
				else if
					(neighcnt[ globalpart[pm] ][ps]
					< neighcnt[ globalpart[ps] ][pm] )
				{
					//vwgt[ps-sr*avgParticle]++;
					vwgt[ps-sr*avgParticle] += 2;
				}
			}

			// for calculate the load balance statistics
			if (globalpart[pm] == globalpart[ps])
			{
				numIactBK[ globalpart[pm] ]++;
			}
			else
			{
				if (neighcnt[ globalpart[ps] ][pm]
					<= neighcnt[ globalpart[pm] ][ps] )
				{
					numIactBK[ globalpart[pm] ]++;
				}
				else
				{
				 	// it means neighcnt[ globalpart[pm] ][ps] < neighcnt[ globalpart[ps] ][pm] )
					numIactBK[ globalpart[ps] ]++;
				}
			}

		}

		times++;
		unsigned long maxIact = *(max_element(numIactBK, numIactBK+NP));
		UNB = (maxIact-avgIact) / static_cast<double>(avgIact);

		//unsigned long minIact=*(min_element(numIactBK,numIactBK+NP));
		//UNB= (avgIact-minIact)/static_cast<double>(avgIact);
		//UNB= (maxIact-minIact)/static_cast<double>(avgIact);

	} //while loop ends

	if ((times >= MAX_CUT) && (UNB > MAX_ALLOWED_UNB))
	{
		TIMES_MAX_CUT_EXCEEDED++;
	}

	vector< pair<unsigned long, unsigned long> > tmpIactVec;
	int localIact = 0;
	for (int i=0; i<IactPairTab.size(); ++i)
	{
		int pm = IactPairTab[i].first;
		int ps = IactPairTab[i].second;

		if ((globalpart[pm] == rank) && (globalpart[ps] ==rank))
		{
			tmpIactVec.push_back(make_pair(pm, ps));
			localIact++;
		}
		else if (globalpart[pm] == rank)
		{
			if (neighcnt[ globalpart[ps] ][pm]
				<= neighcnt[ globalpart[pm] ][ps])
			{
				//if ( nvec[pm] <= nvec[ps] )
				tmpIactVec.push_back(make_pair(pm, ps));
			}
		}
		else if (globalpart[ps] == rank)
		{
			if (neighcnt[ globalpart[pm] ][ps]
				< neighcnt[ globalpart[ps] ][pm])
			{
		 		//if ( nvec[ps] < nvec[pm] )
				tmpIactVec.push_back(make_pair(pm, ps));
			}
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
	delete[] globalpart;
	delete[] xyz;
	delete[] neighcnt ;
	delete[] pneighcnt;
	delete[] numIactBK;

	//cout<<"rank: "<<rank<<"localwgt= "<<localwgt<<" edgecut: "<<edgecut<<" localIact: "<<localIact<<endl;
}
