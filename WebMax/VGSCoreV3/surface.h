#ifndef _surface_
#define _surface_
//
// CSurface

//-----------------------------------------------------------------
//海水表面的类
//-----------------------------------------------------------------

#include "VGSHeader.h"
#include "CCamera.h"
#include "parameterhandler.h"
#include "software_noisemaker.h"
#include "CCamera.h"

#define skyboxdetail 16	

#define reflrefrdetail 512

struct togglestate
{
	bool	animation,
			view_normals;			
};

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
	FLOAT x, y, z;      // The untransformed, 3D position for the vertex
	DWORD color;        // The vertex color
};

struct SURFACEVERTEX
{
	D3DXVECTOR3	position;	
	float displacement;
};

struct DISPLACEMENT
{
	DWORD displacement;
};

enum rendermode
{
	RM_POINTS = 0,
	RM_WIREFRAME,
	RM_SOLID
};
// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
#define D3DFVF_SURFACEVERTEX (D3DFVF_XYZ|D3DFVF_TEX1) //|D3DFVF_TEX1
#define D3DFVF_DISPLACEMENT (D3DFVF_DIFFUSE)
#define D3DFVF_SURFACEVERTEX_AND_DISPLACEMENT (D3DFVF_XYZ|D3DFVF_DIFFUSE)

class VGS_EXPORT CSurface
{
public:
	CSurface(D3DXVECTOR3* position, D3DXVECTOR3* normal,int gridsize_x, int gridsize_y,
			LPDIRECT3DDEVICE9 device, CCamera* pRenderCamera, parameterhandler *prm, BOOL bSupportTex64);
	~CSurface();
	bool prepare(CCamera* pCamera);
	bool render();
	void render_cutter();
	void render_skybox();
	void calc_efficiency();
	void set_render_mode(int rendermode);
	void set_grid_size(int size_x, int size_y);
	void set_displacement_amplitude(float amplitude);
	float get_height_at(float,float);

	void	Invalidate();
	void	RestoreDeviceObject();
	void	Release();

public:
	char debugdata[500];
	float efficiency;
	D3DXMATRIXA16 range;
	D3DXPLANE	plane, upper_bound, lower_bound;
	LPDIRECT3DTEXTURE9			surf_refraction, surf_reflection;
	LPDIRECT3DSURFACE9			depthstencil;

	CCamera						*projecting_camera,	// the camera that does the actual projection
								*rendering_camera,		// the camera whose frustum the projection is created for
								*observing_camera;		// the camera the geometry is transformed through when rendering.
														// in real use rendering_camera and observing_camera should be the same
														// they can be different for demonstration purposes though
	
	LPDIRECT3DCUBETEXTURE9		sky_cubemap;
	BOOL						m_bSupportGPUNormal;

public:
	parameterhandler *prm;
	bool initbuffers();
	bool getMinMax(D3DXMATRIXA16 *range);
	void CSurface::SetupMatrices(CCamera *camera_view);
	bool CSurface::within_frustum(D3DXVECTOR3 *pos);
	void CSurface::LoadEffect();
	bool initialized, boxfilter;

protected:	
	D3DXVECTOR3	normal, u, v, pos;
	float		min_height,max_height;
	int			gridsize_x, gridsize_y, rendermode;
	bool		plane_within_frustum;
	
	
	software_noisemaker			*software_brus;
		
	LPDIRECT3DVERTEXBUFFER9		surf_software_vertices;	
	LPDIRECT3DINDEXBUFFER9		surf_indicies;
	
	LPDIRECT3DTEXTURE9			surf_fresnel, underwater_fresnel, noise2D;
	LPD3DXEFFECT				surf_software_effect, underwater_software_effect;


	LPDIRECT3DVERTEXBUFFER9		skybox_vertices;
	LPDIRECT3DINDEXBUFFER9		skybox_indicies;
	LPD3DXEFFECT				skybox_effect;
		
	LPDIRECT3DDEVICE9			device;	
	
};

#endif