#ifndef _ASSEMBLER_H
#define _ASSEMBLER_H

/******************************************************************************
 * IDOAssembler is the most complicated part of IDO. It uses four desinged
 * patterns conpect to design. They are Factory, Decorator, Builder, and
 * template
 ******************************************************************************/

#include <FrameWork/Interfaces/CDCFactoryDecorator.h>
#include <FrameWork/Interfaces/DOCFactoryDecorator.h>
#include <FrameWork/Interfaces/ISCFactoryDecorator.h>
#include <FrameWork/Interfaces/Interaction.h>
#include <list>

namespace vedo
{

class Assembler
{

public:

	Assembler();

	inline void AddDO(DOFactory* pdof)
	{
		dofd.AddDOCFactory(pdof);
	};

	inline void AddCD(CDFactory* pcdf)
	{
		cdfd.AddCDCFactory(pcdf);
	};

	inline void AddIS(ISFactory *pisf)
	{
		isfd.AddISCFactory(pisf);
	};

	Interaction* CreateInteraction
		(DiscreteObject* pdo1,
		DiscreteObject* pdo2,
		const IactModel* cpiactml) const;

	DiscreteObject* CreateDiscreteObject
		(const DOModel* cpdoml, const DOStatus* cpdos) const;

private:

	CDCFactoryDecorator cdfd;
	DOCFactoryDecorator dofd;
	ISCFactoryDecorator isfd;

	Assembler& operator = (Assembler&);
	Assembler (const Assembler&);

};

};   // namespace vedo

#endif // _ASSEMBLER_H
