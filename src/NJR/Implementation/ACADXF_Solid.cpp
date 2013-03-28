#include <NJR/Interfaces/Constants.h>
#include <NJR/Interfaces/ACADXF_Solid.h>
#include <NJR/Interfaces/Utility.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>

namespace NJRDXF
{

Solid::Solid() : lcon3dFace(0)
{
};

Solid::~Solid()
{
	for_each(lcon3dFace.begin(), lcon3dFace.end(), NJR::Delete_ptr());
};

std::list< Face* >& Solid::Get3dFaceContainer()
{
	return lcon3dFace;
};

Cuboid::Cuboid()
{
	for (int i=0; i<6; ++i)
	{
		lcon3dFace.push_back(new Face);
	}

	this->Set (1, 1, 1, ORIGIN, AXIALX, AXIALZ, "NJRdefault", red);
};

void Cuboid::Set
	(const double& dWidth,
	const double& dLength,
	const double& dHeight,
	const NJR::Vector3d& vP,
	const NJR::Vector3d &vOX,
	const NJR::Vector3d& vOZ,
	const char* layer,
	const Color& color)
{
 	std::list<Face *>::iterator i3dFace = lcon3dFace.begin();

	NJR::Vector3d LX =     vOX.direction() * dWidth  * 0.5;
    NJR::Vector3d LZ =     vOZ.direction() * dHeight * 0.5;
    NJR::Vector3d LY = (LZ*LX).direction() * dLength * 0.5;

	NJR::Vector3d v(1.0, 1.0, 1.0);

	// The four vertexes on the top of this ApproximatePlate
    NJR::Vector3d Ft[4];
	Ft[0] = v.Trans( LX, LY, LZ) + vP;
	Ft[1] = v.Trans(-LX, LY, LZ) + vP;
	Ft[2] = v.Trans(-LX,-LY, LZ) + vP;
	Ft[3] = v.Trans( LX,-LY, LZ) + vP;

	// The four vertexes on the bottom of this ApproximatePlate
	NJR::Vector3d Fb[4];
	Fb[0] = v.Trans( LX, LY,-LZ) + vP;
	Fb[1] = v.Trans(-LX, LY,-LZ) + vP;
	Fb[2] = v.Trans(-LX,-LY,-LZ) + vP;
	Fb[3] = v.Trans( LX,-LY,-LZ) + vP;

	// Drawing top face of this ApproximatePlate
	(*i3dFace ++)->Set(Ft[0], Ft[1], Ft[2], Ft[3], layer, color);

	// Drawing bottom face of this ApproximatePlate
	(*i3dFace ++)->Set(Fb[0], Fb[1], Fb[2], Fb[3], layer, color);

	// Drawing the four surrounding face of this AppoximatePlate
	(*i3dFace ++)->Set(Ft[0], Fb[0], Fb[1], Ft[1], layer, color);
	(*i3dFace ++)->Set(Ft[1], Fb[1], Fb[2], Ft[2], layer, color);
	(*i3dFace ++)->Set(Ft[2], Fb[2], Fb[3], Ft[3], layer, color);
	(*i3dFace ++)->Set(Ft[3], Fb[3], Fb[0], Ft[0], layer, color);
};

Ellipsoid::Ellipsoid (const double& vfpa)
{
	double alpha;
	double thita;

	fpa = vfpa;

	for (alpha=0.0; alpha<NJR::dPI; alpha+=fpa)
	{
		for (thita=0.0; thita<NJR::dDoublePI; thita+=fpa)
		{
			lcon3dFace.push_back (new Face);
		}
	}

	this->Set(1.0, 2.0, 3.0, ORIGIN, AXIALX, AXIALZ, "NJRdefault", bylayer);
};

void Ellipsoid::Set
	(const double& dlx,
	const double& dly,
	const double& dlz,
	const NJR::Vector3d& vP,
	const NJR::Vector3d& vOX,
	const NJR::Vector3d& vOZ,
	const char* layer,
	const Color& color)
{
 	std::list< Face* >::iterator i3dFace = lcon3dFace.begin();

	NJR::Vector3d LX =     vOX.direction() * dlx;
    NJR::Vector3d LZ =     vOZ.direction() * dly;
    NJR::Vector3d LY = (LZ*LX).direction() * dlz;
	NJR::Vector3d Co[4];

	double alpha;
	double thita;

	for (alpha=0.0; alpha<NJR::dPI; alpha+=fpa)
	{
		for (thita=0.0; thita<NJR::dDoublePI; thita+=fpa)
		{
			Co[0].SetSphere(1.0,          alpha, thita);
			Co[1].SetSphere(1.0, alpha+fpa*1.05, thita);
			Co[2].SetSphere(1.0, alpha+fpa*1.05, thita+fpa*1.05);
			Co[3].SetSphere(1.0,          alpha, thita+fpa*1.05);

			(*i3dFace ++)->Set
				(Co[0].Trans(LX,LY,LZ) + vP,
				Co[1].Trans(LX,LY,LZ) + vP,
				Co[2].Trans(LX,LY,LZ) + vP,
				Co[3].Trans(LX,LY,LZ) + vP,
				layer,
				color);
		}
	}
};

Sphere::Sphere(const double& vfpa) : Ellipsoid(vfpa)
{
	this->Set(1.0, ORIGIN, AXIALX, AXIALZ, "NJRdefault", red);
};

void Sphere::Set
	(const double &dR,
	const NJR::Vector3d &vP,
	const NJR::Vector3d &vOX,
	const NJR::Vector3d &vOZ,
	const char *layer,
	const Color &color)
{
	this->Ellipsoid::Set(dR, dR, dR, vP, vOX, vOZ, layer, color);
};


Cylinder::Cylinder(const double& vfpa )
{
	double thita;
	fpa = vfpa;

	for (thita=0.0; thita<NJR::dDoublePI; thita+=fpa)
	{
		lcon3dFace.push_back (new Face);
	}

	this->Set(1.0, 10.0, ORIGIN, AXIALX, AXIALZ, "NJRdefault", red);
};

void Cylinder::Set
	(const double& dR,
	const double& dH,
	const NJR::Vector3d& vP,
	const NJR::Vector3d& vOX,
	const NJR::Vector3d& vOZ,
	const char* layer,
	const Color& color)
{
 	std::list< Face* >::iterator i3dFace = lcon3dFace.begin() ;

	NJR::Vector3d LX = vOX.direction();
    NJR::Vector3d LZ = vOZ.direction();
    NJR::Vector3d LY = LZ*LX ;
	NJR::Vector3d Co[4];

	double thita;
	for (thita=0.0; thita<NJR::dDoublePI; thita+=fpa)
	{
	    Co[0].SetCylinder(dR, thita         ,  dH*0.5);
	    Co[1].SetCylinder(dR, thita+fpa*1.05,  dH*0.5);
	    Co[2].SetCylinder(dR, thita+fpa*1.05, -dH*0.5);
	    Co[3].SetCylinder(dR, thita         , -dH*0.5);

		(*i3dFace ++)->Set
			(Co[0].Trans(LX,LY,LZ) + vP,
			Co[1].Trans(LX,LY,LZ) + vP,
			Co[2].Trans(LX,LY,LZ) + vP,
			Co[3].Trans(LX,LY,LZ) + vP,
			layer,color);
	}
};

QuasiCylinder::QuasiCylinder (const double& vfpa)
{
	double alpha;
	double thita;
	fpa = vfpa;

	for (thita=0.0; thita<NJR::dDoublePI; thita+=fpa)
	{
		lcon3dFace.push_back(new Face);
	}

	for (alpha=0.0; alpha<NJR::dPI; alpha+=fpa)
	{
		for (thita=0.0; thita<NJR::dDoublePI; thita+=fpa)
		{
			lcon3dFace.push_back(new Face);
		}
	}

	this->Set(1.0, 10.0, ORIGIN, AXIALX, AXIALZ, "NJRdefault", red);
};

void QuasiCylinder::Set
	(const double& dRadius,
	const double& dHeight,
	const NJR::Vector3d& vP,
	const NJR::Vector3d& vOX,
	const NJR::Vector3d& vOZ,
	const char* layer,
	const Color& color)
{
 	std::list< Face* >::iterator i3dFace = lcon3dFace.begin();

	NJR::Vector3d LX = vOX.direction();
    NJR::Vector3d LZ = vOZ.direction();
    NJR::Vector3d LY = LZ*LX ;
	NJR::Vector3d Co[4];
    NJR::Vector3d bottom;

	double alpha;
	double thita;
	for (thita=0.0; thita<NJR::dDoublePI; thita+=fpa)
	{
		Co[0].SetCylinder(dRadius, thita         ,  dHeight*0.5);
		Co[1].SetCylinder(dRadius, thita+fpa*1.05,  dHeight*0.5);
		Co[2].SetCylinder(dRadius, thita+fpa*1.05, -dHeight*0.5);
		Co[3].SetCylinder(dRadius, thita         , -dHeight*0.5);

		(*i3dFace ++)->Set
			(Co[0].Trans(LX,LY,LZ) + vP,
			Co[1].Trans(LX,LY,LZ) + vP,
			Co[2].Trans(LX,LY,LZ) + vP,
			Co[3].Trans(LX,LY,LZ) + vP,
			layer,
			color);
	}

	for (alpha=0.0; alpha<NJR::dPI; alpha+= fpa)
	{
		for (thita=0.0; thita<NJR::dDoublePI;thita+= fpa)
		{
			Co[0].SetSphere(dRadius, alpha         ,thita);
			Co[1].SetSphere(dRadius, alpha+fpa*1.05,thita);
			Co[2].SetSphere(dRadius, alpha+fpa*1.05,thita+fpa*1.05);
			Co[3].SetSphere(dRadius, alpha         ,thita+fpa*1.05);

			bottom = vP + vOZ * dHeight * ((alpha<NJR::dHalfPI) ? 0.5 : -0.5);

			(*i3dFace ++)->Set
				(Co[0].Trans(LX,LY,LZ) + bottom,
				Co[1].Trans(LX,LY,LZ) + bottom,
				Co[2].Trans(LX,LY,LZ) + bottom,
				Co[3].Trans(LX,LY,LZ) + bottom,
				layer,
				color);
		}
	}
};

QuasiPlate::QuasiPlate(const double& vfpa)
{
	fpa = vfpa;
	for (int i=0; i<42; ++i)
	{
		lcon3dFace.push_back (new Face);
	}

	this->Set(10.0, 10.0, 1.0, ORIGIN, AXIALX, AXIALZ, "NJRdefault", red);
};

void QuasiPlate::Set
	(const double& dWidth,
	const double& dLength,
	const double& dHeight,
	const NJR::Vector3d &vP,
	const NJR::Vector3d& vOX,
	const NJR::Vector3d &vOZ,
	const char* layer,
	const Color &color)
{
 	std::list< Face* >::iterator i3dFace = lcon3dFace.begin();
	register int i;
	register int j;
	register int k;
	double Hwidth  = dWidth  * 0.5;
	double Hlength = dLength * 0.5;
	double Hheight = dHeight * 0.5;

	NJR::Vector3d PlateVertex[54] ;
	NJR::Vector3d LX = vOX.direction() ;
	NJR::Vector3d LZ = vOZ.direction() ;
	NJR::Vector3d LY = LZ*LX ;

	double Bc[9]
		= {0.0,
			NJR::dHalfPI-0.001,
			NJR::dHalfPI,
			NJR::dPI-0.001,
			NJR::dPI,
			NJR::dOneAndHalfPI-0.001,
			NJR::dOneAndHalfPI,
			NJR::dDoublePI-0.001,
			0.0};

	double Qc[6] = {0.0, 0.18*NJR::dPI, 0.41*NJR::dPI ,0.59*NJR::dPI , 0.82*NJR::dPI, NJR::dPI};

	double Lx[9]
		= {Hwidth,
			Hwidth,
			-Hwidth,
			-Hwidth,
			-Hwidth,
			-Hwidth,
			Hwidth,
			Hwidth,
			Hwidth};

	double Ly[9]
		= {Hlength,
			Hlength,
			Hlength,
			Hlength,
			-Hlength,
			-Hlength,
			-Hlength,
			-Hlength,
			Hlength};

	// Drawing top face of this ApproximatePlate
    (*i3dFace ++)->Set
		((LX* Hwidth)+(LY* Hlength)+(vOZ*Hheight)+vP,
		(LX*-Hwidth)+(LY* Hlength)+(vOZ*Hheight)+vP,
		(LX*-Hwidth)+(LY*-Hlength)+(vOZ*Hheight)+vP,
		(LX* Hwidth)+(LY*-Hlength)+(vOZ*Hheight)+vP,
		layer,
		color);

	// Drawing bottom face of this ApproximatePlate
    (*i3dFace ++)->Set
		((LX* Hwidth)+(LY* Hlength)+(vOZ*-Hheight)+vP,
		(LX*-Hwidth)+(LY* Hlength)+(vOZ*-Hheight)+vP,
		(LX*-Hwidth)+(LY*-Hlength)+(vOZ*-Hheight)+vP,
		(LX* Hwidth)+(LY*-Hlength)+(vOZ*-Hheight)+vP,
		layer,
		color);

	/*************************************************************
	 * Calculating surrounding vertexes on surrounding faces of
	 * this ApproximatePlate
	 *************************************************************/
    for (k=0, i=0; i<6; i++)
	{
		for (j=0; j<9; j++, k++)
		{
			PlateVertex[k]
				= (LX * (Hheight * std::sin(Qc[i]) * std::cos(Bc[j]) ) )
				+ (LY * (Hheight * std::sin(Qc[i]) * std::sin(Bc[j]) ) )
				+ (LZ * (Hheight * std::cos(Qc[i])                   ) )
				+ vP
				+ LX*Lx[j]
				+ LY*Ly[j];
		}
	}

	// Drawing surrounding faces of this ApproximatePlate
    for (i=0; i<5; i++)
	{
		for (j=0; j<8; j++)
		{
			(*i3dFace ++)->Set
				(PlateVertex[i*9+j],
				PlateVertex[i*9+j+1],
				PlateVertex[(i+1)*9+j+1],
				PlateVertex[(i+1)*9+j],
				layer,
				color);
		}
	}
};

Polygon::Polygon()
{
};

void Polygon::Set
	(const NJR::NJRpolygon& polygon,
	const NJR::Vector3d& vP,
	const NJR::Vector3d& vOX,
	const NJR::Vector3d& vOZ,
	const char* layer,
	const Color &color)
{
	if (polygon.area() == 0.0)
	{
		return;
	}

	std::list< Face* >::iterator i3dFace;

	std::for_each(lcon3dFace.begin(), lcon3dFace.end(), NJR::Delete_ptr());

	lcon3dFace.clear();

	NJR::Vector3d LX = vOX.direction();
    NJR::Vector3d LZ = vOZ.direction();
    NJR::Vector3d LY = LZ*LX ;


	std::vector<NJR::Vector3d> vertexes = polygon.vertexes();
	unsigned int num = (unsigned int)(vertexes.size());

	for (unsigned int i=0; i<num; i++)
	{
		vertexes[i] = vertexes[i].Trans(LX,LY,LZ) + vP;
	}

	Face *face;

	while ( num >= 3)
	{
		face = new Face;
		lcon3dFace.push_back(face);

		if (num == 3)
		{
			face->Set
				(vertexes[0],
				vertexes[1],
				vertexes[2],
				vertexes[0],
				layer,
				color);

			num = 0;
		}
		else
		{
			face->Set
				(vertexes[0],
				vertexes[num-1],
				vertexes[num-2],
				vertexes[num-3],
				layer,
				color);

			num -= 2;
		}
	}

};



NJRDXF::ofstream& operator << (NJRDXF::ofstream& dxf, Solid* Solid)
{
 	std::list<Face *>::iterator i3dFace;
	for
		(i3dFace=Solid->Get3dFaceContainer().begin();
		i3dFace!=Solid->Get3dFaceContainer().end();
		++i3dFace)
	{
		dxf << *i3dFace;
	}
	return dxf;
};

};   // namespace NJR
