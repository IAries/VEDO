#include <njr/ACADXF_Solid.h>
#include <njr/Utility.h>
#include <aries/utility/Vector3df.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>

namespace njrdxf
{

Solid::Solid() : lcon3dFace(0)
{
}

Solid::~Solid()
{
	for_each(lcon3dFace.begin(), lcon3dFace.end(), njr::Delete_ptr());
}

std::list< Face* >& Solid::Get3dFaceContainer()
{
	return lcon3dFace;
}

Cuboid::Cuboid()
{
	for (_int_t i=0; i<6; ++i)
	{
		lcon3dFace.push_back(new Face);
	}

	this->Set
		(1, 1, 1,
		 aries::Vector3df(),
		 aries::Vector3df(1.0, 0.0, 0.0),
		 aries::Vector3df(0.0, 0.0, 1.0), "NJRdefault", red);
}

void Cuboid::Set
	(const _float_t& dWidth, const _float_t& dLength, const _float_t& dHeight,
	 const aries::Vector3df& vP, const aries::Vector3df &vOX, const aries::Vector3df& vOZ, const char* layer, const Color& color)
{
 	std::list<Face *>::iterator i3dFace = lcon3dFace.begin();

	aries::Vector3df LX =            vOX.direction() * dWidth  * 0.5;
    aries::Vector3df LZ =            vOZ.direction() * dHeight * 0.5;
    aries::Vector3df LY = (LZ.cross(LX)).direction() * dLength * 0.5;

	aries::Vector3df v(1.0, 1.0, 1.0);

	// The four vertexes on the top of this ApproximatePlate
    aries::Vector3df Ft[4];
	Ft[0] = v.trans( LX, LY, LZ) + vP;
	Ft[1] = v.trans(-LX, LY, LZ) + vP;
	Ft[2] = v.trans(-LX,-LY, LZ) + vP;
	Ft[3] = v.trans( LX,-LY, LZ) + vP;

	// The four vertexes on the bottom of this ApproximatePlate
	aries::Vector3df Fb[4];
	Fb[0] = v.trans( LX, LY,-LZ) + vP;
	Fb[1] = v.trans(-LX, LY,-LZ) + vP;
	Fb[2] = v.trans(-LX,-LY,-LZ) + vP;
	Fb[3] = v.trans( LX,-LY,-LZ) + vP;

	// Drawing top face of this ApproximatePlate
	(*i3dFace ++)->Set(Ft[0], Ft[1], Ft[2], Ft[3], layer, color);

	// Drawing bottom face of this ApproximatePlate
	(*i3dFace ++)->Set(Fb[0], Fb[1], Fb[2], Fb[3], layer, color);

	// Drawing the four surrounding face of this AppoximatePlate
	(*i3dFace ++)->Set(Ft[0], Fb[0], Fb[1], Ft[1], layer, color);
	(*i3dFace ++)->Set(Ft[1], Fb[1], Fb[2], Ft[2], layer, color);
	(*i3dFace ++)->Set(Ft[2], Fb[2], Fb[3], Ft[3], layer, color);
	(*i3dFace ++)->Set(Ft[3], Fb[3], Fb[0], Ft[0], layer, color);
}

Ellipsoid::Ellipsoid (const _float_t& vfpa)
{
	_float_t alpha;
	_float_t thita;

	fpa = vfpa;

	for (alpha=0.0; alpha<aries::fPI; alpha+=fpa)
	{
		for (thita=0.0; thita<aries::fDoublePI; thita+=fpa)
		{
			lcon3dFace.push_back (new Face);
		}
	}

	this->Set(1.0, 2.0, 3.0, aries::Vector3df(), aries::Vector3df(1.0, 0.0, 0.0), aries::Vector3df(0.0, 0.0, 1.0), "NJRdefault", bylayer);
}

void Ellipsoid::Set
	(const _float_t& dlx, const _float_t& dly, const _float_t& dlz,
	 const aries::Vector3df& vP, const aries::Vector3df& vOX, const aries::Vector3df& vOZ, const char* layer, const Color& color)
{
 	std::list< Face* >::iterator i3dFace = lcon3dFace.begin();

	aries::Vector3df LX =            vOX.direction() * dlx;
    aries::Vector3df LZ =            vOZ.direction() * dly;
    aries::Vector3df LY = (LZ.cross(LX)).direction() * dlz;
	aries::Vector3df Co[4];

	_float_t alpha;
	_float_t thita;

	for (alpha=0.0; alpha<aries::fPI; alpha+=fpa)
	{
		for (thita=0.0; thita<aries::fDoublePI; thita+=fpa)
		{
			Co[0].set_sphere(1.0, alpha         , thita         );
			Co[1].set_sphere(1.0, alpha+fpa*1.05, thita         );
			Co[2].set_sphere(1.0, alpha+fpa*1.05, thita+fpa*1.05);
			Co[3].set_sphere(1.0, alpha         , thita+fpa*1.05);

			(*i3dFace ++)->Set
				(Co[0].trans(LX,LY,LZ) + vP,
				 Co[1].trans(LX,LY,LZ) + vP,
				 Co[2].trans(LX,LY,LZ) + vP,
				 Co[3].trans(LX,LY,LZ) + vP,
				 layer, color               );
		}
	}
}

Sphere::Sphere(const _float_t& vfpa) : Ellipsoid(vfpa)
{
	this->Set(1.0, aries::Vector3df(), aries::Vector3df(1.0, 0.0, 0.0), aries::Vector3df(0.0, 0.0, 1.0), "NJRdefault", red);
}

void Sphere::Set
	(const _float_t &dR,
	 const aries::Vector3df &vP, const aries::Vector3df &vOX, const aries::Vector3df &vOZ, const char *layer, const Color &color)
{
	this->Ellipsoid::Set(dR, dR, dR, vP, vOX, vOZ, layer, color);
}


Cylinder::Cylinder(const _float_t& vfpa )
{
	_float_t thita;
	fpa = vfpa;

	for (thita=0.0; thita<aries::fDoublePI; thita+=fpa)
	{
		lcon3dFace.push_back (new Face);
	}

	this->Set(1.0, 10.0, aries::Vector3df(), aries::Vector3df(1.0, 0.0, 0.0), aries::Vector3df(0.0, 0.0, 1.0), "NJRdefault", red);
}

void Cylinder::Set
	(const _float_t& dR, const _float_t& dH,
	 const aries::Vector3df& vP, const aries::Vector3df& vOX, const aries::Vector3df& vOZ, const char* layer, const Color& color)
{
 	std::list< Face* >::iterator i3dFace = lcon3dFace.begin() ;

	aries::Vector3df LX = vOX.direction();
    aries::Vector3df LZ = vOZ.direction();
    aries::Vector3df LY = LZ.cross(LX);
	aries::Vector3df Co[4];

	_float_t thita;
	for (thita=0.0; thita<aries::fDoublePI; thita+=fpa)
	{
	    Co[0].set_cylinder(dR, thita         ,  dH*0.5);
	    Co[1].set_cylinder(dR, thita+fpa*1.05,  dH*0.5);
	    Co[2].set_cylinder(dR, thita+fpa*1.05, -dH*0.5);
	    Co[3].set_cylinder(dR, thita         , -dH*0.5);

		(*i3dFace ++)->Set
			(Co[0].trans(LX,LY,LZ) + vP,
			 Co[1].trans(LX,LY,LZ) + vP,
			 Co[2].trans(LX,LY,LZ) + vP,
			 Co[3].trans(LX,LY,LZ) + vP,
			 layer, color               );
	}
}

QuasiCylinder::QuasiCylinder (const _float_t& vfpa)
{
	_float_t alpha;
	_float_t thita;
	fpa = vfpa;

	for (thita=0.0; thita<aries::fDoublePI; thita+=fpa)
	{
		lcon3dFace.push_back(new Face);
	}

	for (alpha=0.0; alpha<aries::fPI; alpha+=fpa)
	{
		for (thita=0.0; thita<aries::fDoublePI; thita+=fpa)
		{
			lcon3dFace.push_back(new Face);
		}
	}

	this->Set(1.0, 10.0, aries::Vector3df(), aries::Vector3df(1.0, 0.0, 0.0), aries::Vector3df(0.0, 0.0, 1.0), "NJRdefault", red);
}

void QuasiCylinder::Set
	(const _float_t& dRadius, const _float_t& dHeight,
	 const aries::Vector3df& vP, const aries::Vector3df& vOX, const aries::Vector3df& vOZ, const char* layer, const Color& color)
{
 	std::list< Face* >::iterator i3dFace = lcon3dFace.begin();

	aries::Vector3df LX = vOX.direction();
    aries::Vector3df LZ = vOZ.direction();
    aries::Vector3df LY = LZ.cross(LX);
	aries::Vector3df Co[4];
    aries::Vector3df bottom;

	_float_t alpha;
	_float_t thita;
	for (thita=0.0; thita<aries::fDoublePI; thita+=fpa)
	{
		Co[0].set_cylinder(dRadius, thita         ,  dHeight*0.5);
		Co[1].set_cylinder(dRadius, thita+fpa*1.05,  dHeight*0.5);
		Co[2].set_cylinder(dRadius, thita+fpa*1.05, -dHeight*0.5);
		Co[3].set_cylinder(dRadius, thita         , -dHeight*0.5);

		(*i3dFace ++)->Set
			(Co[0].trans(LX,LY,LZ) + vP,
			 Co[1].trans(LX,LY,LZ) + vP,
			 Co[2].trans(LX,LY,LZ) + vP,
			 Co[3].trans(LX,LY,LZ) + vP,
			 layer, color               );
	}

	for (alpha=0.0; alpha<aries::fPI; alpha+= fpa)
	{
		for (thita=0.0; thita<aries::fDoublePI;thita+= fpa)
		{
			Co[0].set_sphere(dRadius, alpha         ,thita         );
			Co[1].set_sphere(dRadius, alpha+fpa*1.05,thita         );
			Co[2].set_sphere(dRadius, alpha+fpa*1.05,thita+fpa*1.05);
			Co[3].set_sphere(dRadius, alpha         ,thita+fpa*1.05);

			bottom = vP + vOZ * dHeight * ((alpha<aries::fHalfPI) ? 0.5 : -0.5);

			(*i3dFace ++)->Set
				(Co[0].trans(LX,LY,LZ) + bottom,
				 Co[1].trans(LX,LY,LZ) + bottom,
				 Co[2].trans(LX,LY,LZ) + bottom,
				 Co[3].trans(LX,LY,LZ) + bottom,
				 layer, color                   );
		}
	}
}

QuasiPlate::QuasiPlate(const _float_t& vfpa)
{
	fpa = vfpa;
	for (_int_t i=0; i<42; ++i)
	{
		lcon3dFace.push_back (new Face);
	}

	this->Set(10.0, 10.0, 1.0, aries::Vector3df(), aries::Vector3df(1.0, 0.0, 0.0), aries::Vector3df(0.0, 0.0, 1.0), "NJRdefault", red);
}

void QuasiPlate::Set
	(const _float_t& dWidth, const _float_t& dLength, const _float_t& dHeight,
	 const aries::Vector3df &vP, const aries::Vector3df& vOX, const aries::Vector3df &vOZ, const char* layer, const Color &color)
{
 	std::list< Face* >::iterator i3dFace = lcon3dFace.begin();
	register _int_t i;
	register _int_t j;
	register _int_t k;
	_float_t Hwidth  = dWidth  * 0.5;
	_float_t Hlength = dLength * 0.5;
	_float_t Hheight = dHeight * 0.5;

	aries::Vector3df PlateVertex[54] ;
	aries::Vector3df LX = vOX.direction() ;
	aries::Vector3df LZ = vOZ.direction() ;
	aries::Vector3df LY = LZ.cross(LX);

	_float_t Bc[9]
		= { 0.0                     ,
			aries::fHalfPI-0.001      ,
			aries::fHalfPI            ,
			aries::fPI-0.001          ,
			aries::fPI                ,
			aries::fOneAndHalfPI-0.001,
			aries::fOneAndHalfPI      ,
			aries::fDoublePI-0.001    ,
			0.0                      };

	_float_t Qc[6] = {0.0, 0.18*aries::fPI, 0.41*aries::fPI, 0.59*aries::fPI, 0.82*aries::fPI, aries::fPI};

	_float_t Lx[9] = {Hwidth, Hwidth, -Hwidth, -Hwidth, -Hwidth, -Hwidth, Hwidth, Hwidth, Hwidth};

	_float_t Ly[9] = {Hlength, Hlength, Hlength, Hlength, -Hlength, -Hlength, -Hlength, -Hlength, Hlength};

	// Drawing top face of this ApproximatePlate
    (*i3dFace ++)->Set
		((LX* Hwidth)+(LY* Hlength)+(vOZ*Hheight)+vP,
		 (LX*-Hwidth)+(LY* Hlength)+(vOZ*Hheight)+vP,
		 (LX*-Hwidth)+(LY*-Hlength)+(vOZ*Hheight)+vP,
		 (LX* Hwidth)+(LY*-Hlength)+(vOZ*Hheight)+vP,
		 layer, color                                );

	// Drawing bottom face of this ApproximatePlate
    (*i3dFace ++)->Set
		((LX* Hwidth)+(LY* Hlength)+(vOZ*-Hheight)+vP,
		 (LX*-Hwidth)+(LY* Hlength)+(vOZ*-Hheight)+vP,
		 (LX*-Hwidth)+(LY*-Hlength)+(vOZ*-Hheight)+vP,
		 (LX* Hwidth)+(LY*-Hlength)+(vOZ*-Hheight)+vP,
		 layer, color                                 );

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
				 layer, color             );
		}
	}
}

QuasiPlateWithCircularHole::QuasiPlateWithCircularHole(const _float_t& vfpa)
{
	fpa = vfpa;
	for (_int_t i=0; i<42; ++i)
	{
		lcon3dFace.push_back (new Face);
	}

	this->Set(10.0, 10.0, 1.0, aries::Vector3df(), aries::Vector3df(1.0, 0.0, 0.0), aries::Vector3df(0.0, 0.0, 1.0), "NJRdefault", red);
}

void QuasiPlateWithCircularHole::Set
	(const _float_t& dWidth, const _float_t& dLength, const _float_t& dHeight,
	 const aries::Vector3df &vP, const aries::Vector3df& vOX, const aries::Vector3df &vOZ, const char* layer, const Color &color)
{
 	std::list< Face* >::iterator i3dFace = lcon3dFace.begin();
	register _int_t i;
	register _int_t j;
	register _int_t k;
	_float_t Hwidth  = dWidth  * 0.5;
	_float_t Hlength = dLength * 0.5;
	_float_t Hheight = dHeight * 0.5;

	aries::Vector3df PlateVertex[54] ;
	aries::Vector3df LX = vOX.direction() ;
	aries::Vector3df LZ = vOZ.direction() ;
	aries::Vector3df LY = LZ.cross(LX);

	_float_t Bc[9]
		= { 0.0                     ,
			aries::fHalfPI-0.001      ,
			aries::fHalfPI            ,
			aries::fPI-0.001          ,
			aries::fPI                ,
			aries::fOneAndHalfPI-0.001,
			aries::fOneAndHalfPI      ,
			aries::fDoublePI-0.001    ,
			0.0                      };

	_float_t Qc[6] = {0.0, 0.18*aries::fPI, 0.41*aries::fPI ,0.59*aries::fPI , 0.82*aries::fPI, aries::fPI};

	_float_t Lx[9] = {Hwidth, Hwidth, -Hwidth, -Hwidth, -Hwidth, -Hwidth, Hwidth, Hwidth, Hwidth};

	_float_t Ly[9] = {Hlength, Hlength, Hlength, Hlength, -Hlength, -Hlength, -Hlength, -Hlength, Hlength};

	// Drawing top face of this ApproximatePlate
    (*i3dFace ++)->Set
		((LX* Hwidth)+(LY* Hlength)+(vOZ*Hheight)+vP,
		 (LX*-Hwidth)+(LY* Hlength)+(vOZ*Hheight)+vP,
		 (LX*-Hwidth)+(LY*-Hlength)+(vOZ*Hheight)+vP,
		 (LX* Hwidth)+(LY*-Hlength)+(vOZ*Hheight)+vP,
		 layer, color                                );

	// Drawing bottom face of this ApproximatePlate
    (*i3dFace ++)->Set
		((LX* Hwidth)+(LY* Hlength)+(vOZ*-Hheight)+vP,
		 (LX*-Hwidth)+(LY* Hlength)+(vOZ*-Hheight)+vP,
		 (LX*-Hwidth)+(LY*-Hlength)+(vOZ*-Hheight)+vP,
		 (LX* Hwidth)+(LY*-Hlength)+(vOZ*-Hheight)+vP,
		 layer, color                                 );

	// Calculating surrounding vertexes on surrounding faces of this ApproximatePlate
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
				 layer, color             );
		}
	}
}

Polygon::Polygon()
{
}

void Polygon::Set
	(const njr::NJRpolygon& polygon,
	 const aries::Vector3df& vP, const aries::Vector3df& vOX, const aries::Vector3df& vOZ, const char* layer, const Color &color)
{
	if (polygon.area() == 0.0)
	{
		return;
	}

	std::list< Face* >::iterator i3dFace;

	std::for_each(lcon3dFace.begin(), lcon3dFace.end(), njr::Delete_ptr());

	lcon3dFace.clear();

	aries::Vector3df LX = vOX.direction();
    aries::Vector3df LZ = vOZ.direction();
    aries::Vector3df LY = LZ.cross(LX);

	std::vector<aries::Vector3df> vertexes = polygon.vertexes();
	_uint_t num = (_uint_t)(vertexes.size());

	for (_uint_t i=0; i<num; i++)
	{
		vertexes[i] = vertexes[i].trans(LX,LY,LZ) + vP;
	}

	Face *face;

	while ( num >= 3)
	{
		face = new Face;
		lcon3dFace.push_back(face);

		if (num == 3)
		{
			face->Set(vertexes[0], vertexes[1], vertexes[2], vertexes[0], layer, color);

			num = 0;
		}
		else
		{
			face->Set(vertexes[0], vertexes[num-1], vertexes[num-2], vertexes[num-3], layer, color);

			num -= 2;
		}
	}

}

njrdxf::ofstream& operator << (njrdxf::ofstream& dxf, Solid* Solid)
{
 	std::list<Face *>::iterator i3dFace;
	for(i3dFace=Solid->Get3dFaceContainer().begin(); i3dFace!=Solid->Get3dFaceContainer().end(); ++i3dFace)
	{
		dxf << *i3dFace;
	}
	return dxf;
}

}   // namespace njr
