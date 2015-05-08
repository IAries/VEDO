#include <vedo/framework/DOWorld.h>
#include <vedo/framework/DOMap.h>
#include <aries/utility/Constants.h>
#include <algorithm>
#include <iterator>

void svg(const vedo::DOWorld* World, std::string filename)
{
	if (!World->Check())
	{
		std::cout
			<< "Error!! Code: svg.cpp" << std::endl
			<< "        Note: vedo::DOWorld Error" << std::endl;
		exit(0);
	}

	std::ofstream svgf(filename.c_str());

	vedo::_uint_t ul;
	vedo::_uint_t numberDO = World->GetSystemParameter()->GetDONumber();

	std::vector<vedo::DOMap> vDOMap(numberDO);
	std::vector<vedo::DOMap> MTab_Temp, MTab;

	for (ul=0; ul<numberDO; ++ul)
	{
		const vedo::DOStatus* cpdos = World->GetDOStatus(ul);
		const vedo::DOModel* cpdoml = World->GetDOModel(cpdos->GetDOName());
		vDOMap[ul] = vedo::DOMap(ul, cpdos, cpdoml, cpdos->GetVelocity().length());
	}

	remove_copy_if
		(vDOMap.begin(),
		 vDOMap.end(),
		 back_inserter(MTab_Temp),
		 vedo::DOMap::ISConstrained     );
	remove_copy_if
		(MTab_Temp.begin(),
		 MTab_Temp.end(),
		 back_inserter(MTab),
		 vedo::DOMap::ISFixed      );

	vedo::_float_t maxX
		= max_element
			(MTab.begin(), MTab.end(), vedo::DOMap::ComX)->cpdos()->GetPosition().x();

	vedo::_float_t minX
		= min_element
			(MTab.begin(), MTab.end(), vedo::DOMap::ComX)->cpdos()->GetPosition().x();

	vedo::_float_t maxY
		= max_element
			(MTab.begin(), MTab.end(), vedo::DOMap::ComY)->cpdos()->GetPosition().y();

	vedo::_float_t minY
		= min_element
			(MTab.begin(), MTab.end(), vedo::DOMap::ComY)->cpdos()->GetPosition().y();

	vedo::Vector3df center
		(400.0 * (maxX + minX) / (maxX-minX),
		-300.0 * (maxY + minY) / (maxY- minY),
		0.0);

    vedo::Vector3df v(400.0, -300.0, 0.0);

	center = v - center;

	svgf
		<< "<?xml version=\"1.0\" standalone=\"no\"?>"
		<< std::endl
		<< "<!-- created by vedo::DOWorld -->"
		<< std::endl
		<< "<svg viewBox=\""
		<< 800.0 * minX / (maxX - minX) - center.x()
		<< ' '
		<< -600.0 * maxY / (maxY - minY) + center.y()
		<< " 800 600"
		<< "\" "
		<< "xmlns=\"http://www.w3.org/2000/svg\">"
		<< std::endl;

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
			<< std::endl;
	}

	svgf << "</svg>" << std::endl;
	svgf.close();
};
