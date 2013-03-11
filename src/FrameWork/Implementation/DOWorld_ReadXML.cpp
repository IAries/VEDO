#include <xercesc/dom/DOMAttr.hpp>
//#include <xercesc/dom/DOMBuilder.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMError.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMLSParser.hpp>
#include <xercesc/dom/DOMLocator.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/parsers/AbstractDOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <iostream>
#include <string>
#include <cstdlib>

XERCES_CPP_NAMESPACE_USE

static const char* trans (const XMLCh* const toTranscode);
static const XMLCh* XMLtrans (const char* toTranscode);

class DOMCountErrorHandler : public DOMErrorHandler
{

public:

	DOMCountErrorHandler():fSawErrors(false)
	{
	};

	~DOMCountErrorHandler()
	{
	};

	// Getter methods
	bool getSawErrors() const
	{
		return fSawErrors;
	};

	// Implementation of the DOM ErrorHandler interface

	bool handleError(const DOMError& domError)
	{
		fSawErrors = true;
		if (domError.getSeverity() == DOMError::DOM_SEVERITY_WARNING)
		{
			std::cerr << std::endl << "Warning at file ";
		}
		else if (domError.getSeverity() == DOMError::DOM_SEVERITY_ERROR)
		{
			std::cerr << std::endl << "Error at file ";
		}
		else
		{
			std::cerr << std::endl << "Fatal Error at file ";
		}

		std::cerr
			<< trans (domError.getLocation()->getURI() )
			<< ", line "
			<< domError.getLocation()->getLineNumber()
			<< ", char "
			<< domError.getLocation()->getColumnNumber()
			<< std::endl
			<< "  Message: "
			<< trans(domError.getMessage())
			<< std::endl;
		return true;
	};

	void resetErrors()
	{
		fSawErrors = false;
	};

private :

	// Unimplemented constructors and operators
	DOMCountErrorHandler(const DOMCountErrorHandler&);
	void operator=(const DOMCountErrorHandler&);

	/**************************************************************************
	 * Private data members
	 *
	 * fSawErrors
	 * This is set if we get any errors, and is queryable via a getter method.
	 * Its used by the main code to suppress output if there are errors.
	 **************************************************************************/
	bool fSawErrors;
};

#include <FrameWork/Interfaces/DOWorld.h>

static const char* trans(const XMLCh* const toTranscode)
{
	static char tempStr[256];
	char *g;
	g = XMLString::transcode(toTranscode);
	strncpy (tempStr, g, 255);
	XMLString::release(&g);
	return tempStr;
};

static const XMLCh* XMLtrans(const char* toTranscode)
{
	static XMLCh tempStr[256];
	XMLString::transcode(toTranscode, tempStr, 255);
	return tempStr;
};

static bool node2data(std::string& des, const DOMNode* node)
{
	if (node == 0)
	{
		return false;
	}
	des = trans(node->getFirstChild()->getNodeValue());
	return true;
};

static bool node2data(double& des, const DOMNode* node)
{
	if (node == 0)
	{
		return false;
	}
	des = atof(trans(node->getFirstChild()->getNodeValue()));
	return true;
};

static bool node2data(unsigned long& des, const DOMNode* node)
{
	if (node == 0)
	{
		return false;
	}
	des = atoi(trans(node->getFirstChild()->getNodeValue()));
	return true;
};

static bool node2data(DOShapeColor& des, const DOMNode* node)
{
	std::string sTmp;
	if (node == 0)
	{
		des = NJRDXF::bylayer;
		return false;
	}

	sTmp = trans (node->getFirstChild()->getNodeValue()) ;

	if (sTmp =="red")
	{
		des = NJRDXF::red;
	}
	else if (sTmp =="yellow")
	{
		des = NJRDXF::yellow;
	}
	else if (sTmp =="green")
	{
		des = NJRDXF::green;
	}
	else if (sTmp =="cyan")
	{
		des = NJRDXF::cyan;
	}
	else if (sTmp =="blue")
	{
		des = NJRDXF::blue;
	}
	else if (sTmp =="magenta")
	{
		des = NJRDXF::magenta;
	}
	else if (sTmp =="bylayer")
	{
		des = NJRDXF::bylayer;
	}
	else
	{
		des = NJRDXF::bylayer;
	}
	return true;
};

static bool node2data
	(DOShapeType& st, DOShapeAttributes& sa, const DOMNode* node)
{
	const DOMElement* element = (DOMElement*) node;
	const DOMElement* shape;

	shape = (DOMElement*) element
		->getElementsByTagName(XMLtrans("Sphere"))->item(0);
	if (shape != 0)
	{
		st = Sphere;
		node2data
			(sa.sphere.radius, shape->getAttributeNode(XMLtrans("Radius")));
		return true;
	}

	shape = (DOMElement*) element
		->getElementsByTagName(XMLtrans("QuasiPlate"))->item(0);
	if (shape != 0)
	{
		st = QuasiPlate;
		node2data(sa.quasiplate.width , shape->getAttributeNode(XMLtrans("Width" )));
		node2data(sa.quasiplate.length, shape->getAttributeNode(XMLtrans("Length")));
		node2data(sa.quasiplate.height, shape->getAttributeNode(XMLtrans("Height")));
		return true;
	}

	shape = (DOMElement*) element
		->getElementsByTagName(XMLtrans("QuasiCylinder"))->item(0);
	if (shape != 0)
	{
		st = QuasiCylinder;
		node2data(sa.quasicylinder.radius, shape->getAttributeNode(XMLtrans("Radius")));
		node2data(sa.quasicylinder.height, shape->getAttributeNode(XMLtrans("Height")));
		return true;
	}

	shape = (DOMElement*) element
		->getElementsByTagName(XMLtrans ("Ellipsoid"))->item(0);
	if (shape != 0)
	{
		st = Ellipsoid;
		node2data(sa.ellipsoid.xlength, shape->getAttributeNode(XMLtrans("XLength")));
		node2data(sa.ellipsoid.ylength, shape->getAttributeNode(XMLtrans("YLength")));
		node2data(sa.ellipsoid.zlength, shape->getAttributeNode(XMLtrans("ZLength")));
		return true;
	}

	shape = (DOMElement*) element
		->getElementsByTagName(XMLtrans ("Polyhedra"))->item(0);
	if (shape != 0)
	{
		st = Polyhedra;
		return true;
	}

	st = NoType;
	return false;
};

static bool node2data(NJRvector3d& des, const DOMNode *node)
{
	double x, y, z;
	if (node == 0)
	{
		return false;
	}

	const DOMElement* vector = (DOMElement*) node;
	node2data(x, vector->getAttributeNode(XMLtrans("x")));
	node2data(y, vector->getAttributeNode(XMLtrans("y")));
	node2data(z, vector->getAttributeNode(XMLtrans("z")));

	des.Set(x, y, z) ;
	return true;
};

static bool node2data(NJRhalfspace& des, const DOMNode *node)
{
	double a, b, c, d;
	std::string sense;
	Sense s;

	if (node == 0)
	{
		return false;
	}

	const DOMElement* halfspace = (DOMElement*) node;
	node2data(a, halfspace->getAttributeNode(XMLtrans("a")));
	node2data(b, halfspace->getAttributeNode(XMLtrans("b")));
	node2data(c, halfspace->getAttributeNode(XMLtrans("c")));
	node2data(d, halfspace->getAttributeNode(XMLtrans("d")));
	node2data(sense, halfspace->getAttributeNode(XMLtrans("sense")));

	if (sense =="E")
	{
		s = E;
	}
	else if (sense =="L")
	{
		s = L;
	}
	else if (sense =="G")
	{
		s = G;
	}

	des.Set(a, b, c, s, d);

	return true;
};

static DOModel* node2doml(const DOMNode* node)
{
	std::string         sDOName;
	std::string         sDOGroup;
	DOShapeColor        cColor;
	std::string         sBehavior;
	std::string         sScope;
	double              dDensity;
	double              dDensityFactor;
	NJRvector3d         vExternalForce;

	DOShapeType         st;
	DOShapeAttributes   sa;

	std::vector<DOMaterialAttribute> cMatOpt(0);

	DOMaterialAttribute DOMatOpt;
	DOMNodeList*        nl_MatOpt;

	unsigned long int i;

	const DOMElement* element = (DOMElement*) node;

	node2data(sDOName  , element->getAttributeNode(XMLtrans("DOName"  )));
	node2data(sDOGroup , element->getAttributeNode(XMLtrans("DOGroup" )));
	node2data(sBehavior, element->getAttributeNode(XMLtrans("Behavior")));
	node2data(dDensity , element->getAttributeNode(XMLtrans("Density" )));
	node2data(cColor   , element->getAttributeNode(XMLtrans("Color"   )));

	if(!node2data(sScope, element->getAttributeNode(XMLtrans("Scope"))))
	{
		if(sBehavior == "mobile")
			sScope = "local";
		else
			sScope = "global";
	}

	if(!node2data(dDensityFactor, element->getAttributeNode(XMLtrans("DensityFactor"))))
	{
		dDensityFactor = 1.0;
	}

	if(!node2data(vExternalForce, element->getElementsByTagName(XMLtrans("ExternalForce"))->item(0)))
	{
		vExternalForce.Set(0.0, 0.0, 0.0);
	};

	node2data(st, sa, element->getElementsByTagName(XMLtrans("Shape"))->item(0));

	nl_MatOpt = element->getElementsByTagName(XMLtrans("MaterialOption"));
	unsigned long ulMaterialOptionSize = nl_MatOpt->getLength();
	for (i=0; i<ulMaterialOptionSize; ++i)
	{
		node2data
			(DOMatOpt.Name,
			 ((DOMElement*) nl_MatOpt->item(i))->getAttributeNode(XMLtrans("Name")));
		node2data
			(DOMatOpt.Value,
			 ((DOMElement*) nl_MatOpt->item(i))->getAttributeNode(XMLtrans("Value")));
		cMatOpt.push_back(DOMatOpt);
	}

	if (st == Polyhedra)
	{
		DOMNodeList* nl_hf = element->getElementsByTagName(XMLtrans("HalfSpace"));
		NJRpolyhedra polyhedra;
		NJRhalfspace hf;

		unsigned long ulHalfSpaceSize = nl_hf->getLength();
		for (i=0; i<ulHalfSpaceSize; ++i)
		{
			node2data (hf, nl_hf->item(i));
			polyhedra.AddConstrain(hf);
		}

		return new DOModel
			(sDOName,
			 sDOGroup,
			 sBehavior,
			 sScope,
			 dDensity,
			 dDensityFactor,
			 vExternalForce,
			 polyhedra,
			 cColor,
			 cMatOpt        );
	}

	return new DOModel
		(sDOName,
		 sDOGroup,
		 sBehavior,
		 sScope,
		 dDensity,
		 dDensityFactor,
		 vExternalForce,
		 st,
		 sa,
		 cColor,
		 cMatOpt        );
};

static DOStatus* node2dos (const DOMNode* node)
{
	std::string sDOName;
	NJRvector3d vPosition;
	NJRvector3d vVelocity;
	NJRvector3d vOrientationX;
	NJRvector3d	vOrientationZ;
	NJRvector3d vAngularVelocity;

	const DOMElement* element = (DOMElement*) node;

	node2data(sDOName         , element->getAttributeNode(XMLtrans("DOName")));
	node2data(vPosition       , element->getElementsByTagName(XMLtrans("Position"))->item(0));
	node2data(vVelocity       , element->getElementsByTagName(XMLtrans("Velocity"))->item(0));
	node2data(vOrientationX   , element->getElementsByTagName(XMLtrans("OrientationX"))->item(0));
	node2data(vOrientationZ   , element->getElementsByTagName(XMLtrans("OrientationZ"))->item(0));
	node2data(vAngularVelocity, element->getElementsByTagName(XMLtrans("AngularVelocity"))->item(0));

	return new DOStatus
		(sDOName,
		 vPosition,
		 vVelocity,
		 vOrientationX,
		 vOrientationZ,
		 vAngularVelocity);
};

static IactModel* node2iactml (const DOMNode* node)
{
	std::string                sMasterGroup;
	std::string                sSlaveGroup;
	std::string                sEquationType;
	std::vector<IactMechanism> svIactMechanisms(0);

	IactMechanism              im;
	DOMNodeList*               nl_IactM;

	unsigned long int i;

	const DOMElement* element = (DOMElement*)node;

	node2data(sMasterGroup , element->getAttributeNode(XMLtrans("MasterGroup" )));
	node2data(sSlaveGroup  , element->getAttributeNode(XMLtrans("SlaveGroup"  )));
	node2data(sEquationType, element->getAttributeNode(XMLtrans("EquationType")));

	nl_IactM = element->getElementsByTagName(XMLtrans("Mechanism"));
	unsigned long ulMechanismSize = nl_IactM->getLength();
	for (i=0; i<ulMechanismSize; ++i)
	{
		node2data
			(im.Name,
			((DOMElement*) nl_IactM->item(i))->getAttributeNode(XMLtrans("Name" )));
		node2data
			(im.Value,
			((DOMElement*) nl_IactM->item(i))->getAttributeNode(XMLtrans("Value")));
		svIactMechanisms.push_back(im);
	}

	return new IactModel
		(sMasterGroup, sSlaveGroup, sEquationType, svIactMechanisms);
};

bool DOWorld::ReadXML(const char* xmlFile)
{
	DOWorld::Clear();
	std::string   sTitle   = "noTitle";
	std::string   sPublish = "noPublish";
	std::string   sNote    = "noNote";
	NJRvector3d   vFieldForce;
	NJRvector3d   vLowerBoundaryZOI;
	NJRvector3d   vUpperBoundaryZOI;
	NJRvector3d   vLowerBoundaryPBC;
	NJRvector3d   vUpperBoundaryPBC;
	double        dStart;
	double        dStop;
	double        dInterval;
	double        dCurrent;
	unsigned long lNumObject;

	register unsigned int i;

	XMLPlatformUtils::Initialize();

	DOMNodeList* nl_iactml;
	DOMNodeList* nl_doml;
	DOMNodeList* nl_dos;
	DOMElement*  element;

	static const XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };

	DOMImplementation* impl
		= DOMImplementationRegistry::getDOMImplementation(gLS);
/*
	// Xerces-C++ 2.x
	DOMBuilder* parser
		= ((createDOMBuilder*)impl)
			->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	parser->setFeature(XMLUni::fgDOMNamespaces           , true);
	parser->setFeature(XMLUni::fgXercesSchema            , true);
	parser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);
	parser->setFeature(XMLUni::fgDOMValidation           , true);
	parser->setFeature(XMLUni::fgDOMDatatypeNormalization, true);

	DOMCountErrorHandler errorHandler;
	parser->setErrorHandler(&errorHandler);

	// Reset error count first
	errorHandler.resetErrors();
*/

	// Xerces-C++ 3.0
	DOMLSParser* parser
		= ((DOMImplementationLS*)impl)
			->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0);

	DOMConfiguration* conf(parser->getDomConfig());
	// Perform namespace processing.
	conf->setParameter(XMLUni::fgDOMNamespaces, true);
	// Enable/Disable validation.
	conf->setParameter(XMLUni::fgDOMValidate, true);
	conf->setParameter(XMLUni::fgXercesSchema, true);
	conf->setParameter(XMLUni::fgXercesSchemaFullChecking, false);
	// Discard comment nodes in the document.
	conf->setParameter(XMLUni::fgDOMComments, false);
	// Enable datatype normalization.
	conf->setParameter(XMLUni::fgDOMDatatypeNormalization, true);
	/***************************************************************************
	 Do not create EntityReference nodes in the DOM tree. No EntityReference
	 nodes will be created, only the nodes corresponding to their fully expanded
	 substitution text will be created.
	 ***************************************************************************/
	conf->setParameter(XMLUni::fgDOMEntities, false);
	// Do not include ignorable whitespace in the DOM tree.
	conf->setParameter(XMLUni::fgDOMElementContentWhitespace, false);
	// We will release the DOM document ourselves.
	conf->setParameter(XMLUni::fgXercesUserAdoptsDOMDocument, true);

	DOMCountErrorHandler errorHandler;
	conf->setParameter (XMLUni::fgDOMErrorHandler, &errorHandler);

	// Reset error count first
	errorHandler.resetErrors();

	XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* doc = 0;

	try
	{
	   // reset document pool
	   parser->resetDocumentPool();
	   doc = parser->parseURI(xmlFile);
	}

	catch (const XMLException& toCatch)
	{
		std::cerr
			<< std::endl
			<< "Error during parsing: '"
			<< xmlFile
			<< std::endl;
	}

	/**************************************************************************
	 * Extract the DOM tree, get the list of all the elements and report the
	 * length as the count of elements.
	 **************************************************************************/

	if (errorHandler.getSawErrors())
	{
		std::cout
			<< std::endl
			<< "Errors occurred, no output available"
			<< std::endl
			<< std::endl;
	   	parser->release();
		XMLPlatformUtils::Terminate();
		return false;
	}

	node2data(sPublish         , doc->getElementsByTagName(XMLtrans("Publish"      ))->item(0));
	node2data(sTitle           , doc->getElementsByTagName(XMLtrans("Title"        ))->item(0));
	node2data(sNote            , doc->getElementsByTagName(XMLtrans("Note"         ))->item(0));
	node2data(lNumObject       , doc->getElementsByTagName(XMLtrans("NumObject"    ))->item(0));
	node2data(vFieldForce      , doc->getElementsByTagName(XMLtrans("FieldForce"   ))->item(0));

	double dBV[3];

	element = (DOMElement *)doc->getElementsByTagName(XMLtrans("ZOI"))->item(0);

	if(!(node2data(dBV[0], element->getAttributeNode(XMLtrans("XMin")))))
	{
		dBV[0] = 0.0;
	};

	if(!(node2data(dBV[1], element->getAttributeNode(XMLtrans("YMin")))))
	{
		dBV[1] = 0.0;
	};

	if(!(node2data(dBV[2], element->getAttributeNode(XMLtrans("ZMin")))))
	{
		dBV[2] = 0.0;
	};

	vLowerBoundaryZOI.Set(dBV[0], dBV[1], dBV[2]);

	if(!(node2data(dBV[0], element->getAttributeNode(XMLtrans("XMax")))))
	{
		dBV[0] = 0.0;
	};

	if(!(node2data(dBV[1], element->getAttributeNode(XMLtrans("YMax")))))
	{
		dBV[1] = 0.0;
	};

	if(!(node2data(dBV[2], element->getAttributeNode(XMLtrans("ZMax")))))
	{
		dBV[2] = 0.0;
	};

	vUpperBoundaryZOI.Set(dBV[0], dBV[1], dBV[2]);

	element = (DOMElement *)doc->getElementsByTagName(XMLtrans("PBC"))->item(0);

	if(!(node2data(dBV[0], element->getAttributeNode(XMLtrans("XMin")))))
	{
		dBV[0] = 0.0;
	};

	if(!(node2data(dBV[1], element->getAttributeNode(XMLtrans("YMin")))))
	{
		dBV[1] = 0.0;
	};

	if(!(node2data(dBV[2], element->getAttributeNode(XMLtrans("ZMin")))))
	{
		dBV[2] = 0.0;
	};

	vLowerBoundaryPBC.Set(dBV[0], dBV[1], dBV[2]);

	if(!(node2data(dBV[0], element->getAttributeNode(XMLtrans("XMax")))))
	{
		dBV[0] = 0.0;
	};

	if(!(node2data(dBV[1], element->getAttributeNode(XMLtrans("YMax")))))
	{
		dBV[1] = 0.0;
	};

	if(!(node2data(dBV[2], element->getAttributeNode(XMLtrans("ZMax")))))
	{
		dBV[2] = 0.0;
	};

	vUpperBoundaryPBC.Set(dBV[0], dBV[1], dBV[2]);

	element
		= (DOMElement *)doc->getElementsByTagName(XMLtrans("TimeControl"))->item(0);

	node2data(dStart     , element->getAttributeNode(XMLtrans("Start"   )));
	node2data(dStop      , element->getAttributeNode(XMLtrans("Stop"    )));
	node2data(dInterval  , element->getAttributeNode(XMLtrans("Interval")));
	node2data(dCurrent   , element->getAttributeNode(XMLtrans("Current" )));

	pSystemParameter
		= new SystemParameter
			(sTitle,
			sNote,
			dStart,
			dStop,
			dInterval,
			dCurrent,
			lNumObject,
			vFieldForce,
			Boundary(vLowerBoundaryZOI, vUpperBoundaryZOI),
			Boundary(vLowerBoundaryPBC, vUpperBoundaryPBC) );

	nl_doml = doc->getElementsByTagName(XMLtrans("DOModel"));
	unsigned long ulDOModelSize = nl_doml->getLength();
	for (i=0; i<ulDOModelSize; ++i)
	{
		cDOModel.push_back(node2doml(nl_doml->item(i)));
	}

	nl_iactml = doc->getElementsByTagName(XMLtrans("IactModel"));
	unsigned long ulIactModelSize = nl_iactml->getLength();
	for (i=0; i<ulIactModelSize; ++i)
	{
		cIactModel.push_back(node2iactml(nl_iactml->item(i)));
	}

	nl_dos = doc->getElementsByTagName(XMLtrans("DOStatus"));
	unsigned long ulDOStatusSize = nl_dos->getLength();
	for (i=0; i<ulDOStatusSize; ++i)
	{
		cDOStatus.push_back(node2dos (nl_dos->item(i)));
	}

	parser->release();
	XMLPlatformUtils::Terminate();
	return DOWorld::Check();
};
