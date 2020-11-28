#include "stdafx.h"

#include "../xrRender/du_sphere.h"
#include "../xrRenderDX10/dx10BufferUtils.h"

/*
Fvector du_sphere_vertices[DU_SPHERE_NUMVERTEX]=
{
	0.0000f,	1.0000f, 	0.0000f,
		0.8944f,	0.4472f, 	0.0000f,
		0.2764f,	0.4472f, 	0.8507f,
		-0.7236f,	0.4472f, 	0.5257f,
		-0.7236f,	0.4472f, 	-0.5257f,
		0.2764f,	0.4472f, 	-0.8507f,
		0.7236f,	-0.4472f,	0.5257f,
		-0.2764f,	-0.4472f,	0.8507f,
		-0.8944f,	-0.4472f,	-0.0000f,
		-0.2764f,	-0.4472f,	-0.8507f,
		0.7236f,	-0.4472f,	-0.5257f,
		0.0000f,	-1.0000f,	0.0000f,
		0.3607f,	0.9327f, 	0.0000f,
		0.6729f,	0.7397f, 	0.0000f,
		0.1115f,	0.9327f, 	0.3431f,
		0.2079f,	0.7397f, 	0.6399f,
		-0.2918f,	0.9327f, 	0.2120f,
		-0.5444f,	0.7397f, 	0.3955f,
		-0.2918f,	0.9327f, 	-0.2120f,
		-0.5444f,	0.7397f, 	-0.3955f,
		0.1115f,	0.9327f, 	-0.3431f,
		0.2079f,	0.7397f, 	-0.6399f,
		0.7844f,	0.5168f, 	0.3431f,
		0.5687f,	0.5168f, 	0.6399f,
		-0.0839f,	0.5168f, 	0.8520f,
		-0.4329f,	0.5168f, 	0.7386f,
		-0.8362f,	0.5168f, 	0.1835f,
		-0.8362f,	0.5168f, 	-0.1835f,
		-0.4329f,	0.5168f, 	-0.7386f,
		-0.0839f,	0.5168f, 	-0.8520f,
		0.5687f,	0.5168f, 	-0.6399f,
		0.7844f,	0.5168f, 	-0.3431f,
		0.9647f,	0.1561f, 	0.2120f,
		0.9051f,	-0.1561f,	0.3955f,
		0.0965f,	0.1561f, 	0.9830f,
		-0.0965f,	-0.1561f,	0.9830f,
		-0.9051f,	0.1561f, 	0.3955f,
		-0.9647f,	-0.1561f,	0.2120f,
		-0.6558f,	0.1561f, 	-0.7386f,
		-0.4998f,	-0.1561f,	-0.8520f,
		0.4998f,	0.1561f, 	-0.8520f,
		0.6558f,	-0.1561f,	-0.7386f,
		0.9647f,	0.1561f, 	-0.2120f,
		0.9051f,	-0.1561f,	-0.3955f,
		0.4998f,	0.1561f, 	0.8520f,
		0.6558f,	-0.1561f,	0.7386f,
		-0.6558f,	0.1561f, 	0.7386f,
		-0.4998f,	-0.1561f,	0.8520f,
		-0.9051f,	0.1561f, 	-0.3955f,
		-0.9647f,	-0.1561f,	-0.2120f,
		0.0965f,	0.1561f, 	-0.9830f,
		-0.0965f,	-0.1561f,	-0.9830f,
		0.4329f,	-0.5168f,	0.7386f,
		0.0839f,	-0.5168f,	0.8520f,
		-0.5687f,	-0.5168f,	0.6399f,
		-0.7844f,	-0.5168f,	0.3431f,
		-0.7844f,	-0.5168f,	-0.3431f,
		-0.5687f,	-0.5168f,	-0.6399f,
		0.0839f,	-0.5168f,	-0.8520f,
		0.4329f,	-0.5168f,	-0.7386f,
		0.8362f,	-0.5168f,	-0.1835f,
		0.8362f,	-0.5168f,	0.1835f,
		0.2918f,	-0.9327f,	0.2120f,
		0.5444f,	-0.7397f,	0.3955f,
		-0.1115f,	-0.9327f,	0.3431f,
		-0.2079f,	-0.7397f,	0.6399f,
		-0.3607f,	-0.9327f,	-0.0000f,
		-0.6729f,	-0.7397f,	-0.0000f,
		-0.1115f,	-0.9327f,	-0.3431f,
		-0.2079f,	-0.7397f,	-0.6399f,
		0.2918f,   	-0.9327f,	-0.2120f,
		0.5444f,	-0.7397f,	-0.3955f,
		0.4795f,	0.8054f,   	0.3484f,
		-0.1832f,	0.8054f,   	0.5637f,
		-0.5927f,	0.8054f,   	-0.0000f,
		-0.1832f,	0.8054f,   	-0.5637f,
		0.4795f,	0.8054f,   	-0.3484f,
		0.9855f,	-0.1699f,	0.0000f,
		0.3045f,	-0.1699f,	0.9372f,
		-0.7973f,	-0.1699f,	0.5792f,
		-0.7973f,	-0.1699f,	-0.5792f,
		0.3045f,	-0.1699f,	-0.9372f,
		0.7973f,	0.1699f,   	0.5792f,
		-0.3045f,	0.1699f,   	0.9372f,
		-0.9855f,	0.1699f,   	-0.0000f,
		-0.3045f,	0.1699f,   	-0.9372f,
		0.7973f,	0.1699f,   	-0.5792f,
		0.1832f,	-0.8054f,	0.5637f,
		-0.4795f,	-0.8054f,	0.3484f,
		-0.4795f,	-0.8054f,	-0.3484f,
		0.1832f,	-0.8054f,	-0.5637f,
		0.5927f,	-0.8054f,	0.0000
};

u16 du_sphere_faces[DU_SPHERE_NUMFACES*3]={
	14,	12,    0,
		72,   13,   12,
		14,   72,   12,
		15,   72,   14,
		22,    1,   13,
		72,   22,   13,
		23,   22,   72,
		15,   23,   72,
		2,   23,   15,
		16,   14,    0,
		73,   15,   14,
		16,   73,   14,
		17,   73,   16,
		24,    2,   15,
		73,   24,   15,
		25,   24,   73,
		17,   25,   73,
		3,   25,   17,
		18,   16,    0,
		74,   17,   16,
		18,   74,   16,
		19,   74,   18,
		26,    3,   17,
		74,   26,   17,
		27,   26,   74,
		19,   27,   74,
		4,   27,   19,
		20,   18,    0,
		75,   19,   18,
		20,   75,   18,
		21,   75,   20,
		28,    4,   19,
		75,   28,   19,
		29,   28,   75,
		21,   29,   75,
		5,   29,   21,
		12,   20,    0,
		76,   21,   20,
		12,   76,   20,
		13,   76,   12,
		30,    5,   21,
		76,   30,   21,
		31,   30,   76,
		13,   31,   76,
		1,   31,   13,
		32,   42,    1,
		77,   43,   42,
		32,   77,   42,
		33,   77,   32,
		60,   10,   43,
		77,   60,   43,
		61,   60,   77,
		33,   61,   77,
		6,   61,   33,
		34,   44,    2,
		78,   45,   44,
		34,   78,   44,
		35,   78,   34,
		52,    6,   45,
		78,   52,   45,
		53,   52,   78,
		35,   53,   78,
		7,   53,   35,
		36,   46,    3,
		79,   47,   46,
		36,   79,   46,
		37,   79,   36,
		54,    7,   47,
		79,   54,   47,
		55,   54,   79,
		37,   55,   79,
		8,   55,   37,
		38,   48,    4,
		80,   49,   48,
		38,   80,   48,
		39,   80,   38,
		56,    8,   49,
		80,   56,   49,
		57,   56,   80,
		39,   57,   80,
		9,   57,   39,
		40,   50,    5,
		81,   51,   50,
		40,   81,   50,
		41,   81,   40,
		58,    9,   51,
		81,   58,   51,
		59,   58,   81,
		41,   59,   81,
		10,   59,   41,
		33,   45,    6,
		82,   44,   45,
		33,   82,   45,
		32,   82,   33,
		23,    2,   44,
		82,   23,   44,
		22,   23,   82,
		32,   22,   82,
		1,   22,   32,
		35,   47,    7,
		83,   46,   47,
		35,   83,   47,
		34,   83,   35,
		25,    3,   46,
		83,   25,   46,
		24,   25,   83,
		34,   24,   83,
		2,   24,   34,
		37,   49,    8,
		84,   48,   49,
		37,   84,   49,
		36,   84,   37,
		27,    4,   48,
		84,   27,   48,
		26,   27,   84,
		36,   26,   84,
		3,   26,   36,
		39,   51,    9,
		85,   50,   51,
		39,   85,   51,
		38,   85,   39,
		29,    5,   50,
		85,   29,   50,
		28,   29,   85,
		38,   28,   85,
		4,   28,   38,
		41,   43,   10,
		86,   42,   43,
		41,   86,   43,
		40,   86,   41,
		31,    1,   42,
		86,   31,   42,
		30,   31,   86,
		40,   30,   86,
		5,   30,   40,
		62,   64,   11,
		87,   65,   64,
		62,   87,   64,
		63,   87,   62,
		53,    7,   65,
		87,   53,   65,
		52,   53,   87,
		63,   52,   87,
		6,   52,   63,
		64,   66,   11,
		88,   67,   66,
		64,   88,   66,
		65,   88,   64,
		55,    8,   67,
		88,   55,   67,
		54,   55,   88,
		65,   54,   88,
		7,   54,   65,
		66,   68,   11,
		89,   69,   68,
		66,   89,   68,
		67,   89,   66,
		57,    9,   69,
		89,   57,   69,
		56,   57,   89,
		67,   56,   89,
		8,   56,   67,
		68,   70,   11,
		90,   71,   70,
		68,   90,   70,
		69,   90,   68,
		59,   10,   71,
		90,   59,   71,
		58,   59,   90,
		69,   58,   90,
		9,   58,   69,
		70,   62,   11,
		91,   63,   62,
		70,   91,   62,
		71,   91,   70,
		61,    6,   63,
		91,   61,   63,
		60,   61,   91,
		71,   60,   91,
		10,   60,   71,
};
*/

void CRenderTarget::accum_point_geom_create()
{
//	u32	dwUsage				= D3DUSAGE_WRITEONLY;

	// vertices
	{
		u32		vCount		= DU_SPHERE_NUMVERTEX;
		u32		vSize		= 3*4;
		//R_CHK	(DEVICE_HW::XRAY::HW.pDevice->CreateVertexBuffer(
//			vCount*vSize,
//			dwUsage,
//			0,
//			D3DPOOL_MANAGED,
//			&g_accum_point_vb,
//			0));
//		BYTE*	pData				= 0;
//		R_CHK						(g_accum_point_vb->Lock(0,0,(void**)&pData,0));
//		CopyMemory				(pData,du_sphere_vertices,vCount*vSize);
//		g_accum_point_vb->Unlock	();

		R_CHK( dx10BufferUtils::CreateVertexBuffer( &g_accum_point_vb, du_sphere_vertices, vCount*vSize ) );
#ifdef USE_DX10
		DEVICE_HW::XRAY::HW.stats_manager.increment_stats_vb			( g_accum_point_vb );
#endif		
	}

	// Indices
	{
		u32		iCount		= DU_SPHERE_NUMFACES*3;

		//BYTE*	pData		= 0;
		//R_CHK				(DEVICE_HW::XRAY::HW.pDevice->CreateIndexBuffer(iCount*2,dwUsage,D3DFMT_INDEX16,D3DPOOL_MANAGED,&g_accum_point_ib,0));
		//R_CHK				(g_accum_point_ib->Lock(0,0,(void**)&pData,0));
		//CopyMemory		(pData,du_sphere_faces,iCount*2);
		//g_accum_point_ib->Unlock	();

		R_CHK( dx10BufferUtils::CreateIndexBuffer( &g_accum_point_ib, du_sphere_faces, iCount*2) );
#ifdef USE_DX10
		DEVICE_HW::XRAY::HW.stats_manager.increment_stats_ib			( g_accum_point_ib );
#endif
	}
}

void CRenderTarget::accum_point_geom_destroy()
{
#ifdef DEBUG
	_SHOW_REF("g_accum_point_ib",g_accum_point_ib);
#endif // DEBUG
#ifdef USE_DX10
	DEVICE_HW::XRAY::HW.stats_manager.decrement_stats_ib(g_accum_point_ib);
#endif	
	_RELEASE(g_accum_point_ib);
#ifdef DEBUG
	_SHOW_REF("g_accum_point_vb",g_accum_point_vb);
#endif // DEBUG
#ifdef USE_DX10
	DEVICE_HW::XRAY::HW.stats_manager.decrement_stats_vb(g_accum_point_vb);
#endif	
	_RELEASE(g_accum_point_vb);
}
