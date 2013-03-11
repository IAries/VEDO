#include <FrameWork/Interfaces/DOWorld.h>
#include <FrameWork/Interfaces/DOMap.h>
#include <algorithm>
#include <iterator>

using namespace std;

void svg(const DOWorld* World, string filename)
{

	if (!World->Check())
	{
		cerr << "DOWorld Error" << endl;
		exit(0);
	}

	ofstream svgf(filename.c_str());

	unsigned long ul;
	unsigned long numberDO = World->GetSystemParameter()->GetDONumber();

	vector<DOMap> vDOMap(numberDO);
	vector<DOMap> MTab_Temp, MTab;

	for (ul=0; ul<numberDO; ++ul)
	{
		const DOStatus* cpdos = World->GetDOStatus(ul);
		const DOModel* cpdoml = World->GetDOModel(cpdos->GetDOName());
		vDOMap[ul] = DOMap(ul, cpdos, cpdoml, cpdos->GetVelocity().length());
	}

	remove_copy_if
		(vDOMap.begin(),
		 vDOMap.end(),
		 back_inserter(MTab_Temp),
		 DOMap::ISConstrained     );
	remove_copy_if
		(MTab_Temp.begin(),
		 MTab_Temp.end(),
		 back_inserter(MTab),
		 DOMap::ISFixed      );

	double maxX
		= max_element
			(MTab.begin(), MTab.end(), DOMap::ComX)->cpdos()->GetPosition().x();

	double minX
		= min_element
			(MTab.begin(), MTab.end(), DOMap::ComX)->cpdos()->GetPosition().x();

	double maxY
		= max_element
			(MTab.begin(), MTab.end(), DOMap::ComY)->cpdos()->GetPosition().y();

	double minY
		= min_element
			(MTab.begin(), MTab.end(), DOMap::ComY)->cpdos()->GetPosition().y();

	NJRvector3d center
		(400.0 * (maxX + minX) / (maxX-minX),
		-300.0 * (maxY + minY) / (maxY- minY),
		0.0);

    NJRvector3d v(400.0, -300.0, 0.0);

	center = v - center;

	svgf
		<< "<?xml version=\"1.0\" standalone=\"no\"?>"
		<< endl
		<< "<!-- created by DOWorld -->"
		<< endl
		<< "<svg viewBox=\""
		<< 800.0 * minX / (maxX - minX) - center.x()
		<< ' '
		<< -600.0 * maxY / (maxY - minY) + center.y()
		<< " 800 600"
		<< "\" "
		<< "xmlns=\"http://www.w3.org/2000/svg\">"
		<< endl;

	for (ul=0; ul<MTab.size(); ++ul)
	{
		svgf
			<< "<circle cx=\""
			<< 600.0 * (MTab[ul].cpdos()->GetPosition().x()) / (maxY - minY);
		svgf
			<< "\" cy=\""
			<< -600.0 * (MTab[ul].cpdos()->GetPosition().y()) / (maxY - minY);
		svgf
			<< "\" r=\""
			<< 600.0 * (MTab[ul].cpdoml()->GetShapeAttributes().sphere.radius)
				/ (maxY - minY);
		svgf
			<< "\" fill =\"red\"> </circle>"
			<< endl;
	}

	svgf << "</svg>" << endl;
	svgf.close();
};
