#include "scene.h"

scene::scene(HINSTANCE* hInst, HWND* hWnd)
{
	m_d3d			= NULL;
	m_d3dDevice		= NULL;
	m_pVB			= NULL;
	m_hInst			= hInst;
	m_hWnd			= hWnd;
//	m_pFont			= new CD3DFont(_T("Arial"), 12, D3DFONT_BOLD);
	m_EyePosition.x	= 0.0f;
	m_EyePosition.y	= 20.0f;
	m_EyePosition.z	= -10.0f;
	m_LookAt.x		= 0.0f;
	m_LookAt.y		= 0.0f;
	m_LookAt.z		= 0.0f;
	r				= 0;
	g				= 0;
	b				= 0;
}

scene::~scene(void)
{
}

bool	scene::init3D()
{
	if ((m_d3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{
		return false;
	}

	D3DDISPLAYMODE			d3ddm;

	if (FAILED(m_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return false;
	}

	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed					= TRUE;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat			= D3DFMT_UNKNOWN;//d3ddm.Format;
	d3dpp.EnableAutoDepthStencil	= TRUE;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;

	if (FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, *m_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &m_d3dDevice)))		//3d-Device erstellen
	{
		return false;
	}
	//CULLING DISABLE
	//m_d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_d3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);	//Z-BUFFER???
//	m_d3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

//	m_d3dDevice->CreateVertexBuffer(sizeof(CUSTOMVERTEX)*3, 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVB, NULL);

	
	return true;
}

void	scene::setupMatrices()
{
	m_d3dDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	D3DXMatrixLookAtLH(&m_matView,
		&D3DXVECTOR3(m_EyePosition.x, m_EyePosition.y, m_EyePosition.z),
		&D3DXVECTOR3(m_LookAt.x, m_LookAt.y, m_LookAt.z),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_d3dDevice->SetTransform(D3DTS_VIEW, &m_matView);
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI/4, 1.0f, 1.0f, 10000.0f);
	m_d3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
	D3DXMatrixIdentity(&m_matWorld);

	m_d3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_d3dDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_COLORVALUE( 0.25, 0.25, 0.25, 1.0 ) );
/*
	D3DXMatrixTranslation(&m_matWorld, 0.5f, 0.5f, 0.5f);
	D3DXMatrixTranslation(&m_matWorld, 0.5f, 0.5f, 0.5f);
	D3DXMatrixTranslation(&m_matWorld, 0.5f, 0.5f, 0.5f);
	D3DXMatrixTranslation(&m_matWorld, 0.5f, 0.5f, 0.5f);
	D3DXMatrixTranslation(&m_matWorld, 0.5f, 0.5f, 0.5f);
	D3DXMatrixTranslation(&m_matWorld, 0.5f, 0.5f, 0.5f);
	D3DXMatrixTranslation(&m_matWorld, 0.5f, 0.5f, 0.5f);
	D3DXMatrixTranslation(&m_matWorld, 0.5f, 0.5f, 0.5f);
	*/
	D3DXMatrixTranslation(&m_matWorld, 4, 4, 4);
	//D3DXMatrixIdentity(&MatRot);

}

void	scene::sdraw()
{
	
	m_d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0L);
	//m_d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET/*|D3DCLEAR_ZBUFFER*/, /*D3DCOLOR_XRGB(0,0,255)*/ 0x000000ff, 1.0f, 0L);
	
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));

//	m_vecDir			= D3DXVECTOR3(/*cosf(timeGetTime()/360.0f)**/0.0f, -1.0f, /*sinf(timeGetTime()/360.0f)**/0.0f);
	m_light.Type		= D3DLIGHT_POINT;
	m_light.Diffuse.r	= 0.0f;
	m_light.Diffuse.g	= 1.0f;
	m_light.Diffuse.b	= 0.0f;
	m_light.Range		= 100.0f;
	m_light.Position.x += 0.1f;
	m_light.Position.y += 1.0f;
	m_light.Position.z = 5.0f;

	m_light.Direction.x = 0.0f;
	m_light.Direction.y = 5.0f;
	m_light.Direction.z = 0.0f;
	m_light.Attenuation1 = 0.4f;
	m_light.Phi			= 1.0f;
	m_light.Theta		= 0.5;
	m_light.Falloff		= 1;
	m_d3dDevice->SetLight(2, &m_light);
	m_d3dDevice->BeginScene();
	m_d3dDevice->LightEnable(0, TRUE);

/*	ZeroMemory(&m_mtrl, sizeof(D3DMATERIAL9));
	
	m_mtrl.Diffuse.r	= m_mtrl.Ambient.r = m_mtrl.Specular.r = 1.0f;
	m_mtrl.Diffuse.g	= m_mtrl.Ambient.g = m_mtrl.Specular.g = 1.0f;
	m_mtrl.Diffuse.b	= m_mtrl.Ambient.b = m_mtrl.Specular.b = 1.0f;
	m_mtrl.Diffuse.a	= m_mtrl.Ambient.a = m_mtrl.Specular.a = 1.0f;
	m_mtrl.Emissive.r = 0.1f;
	m_mtrl.Power = 5;

	m_d3dDevice->SetMaterial(&m_mtrl);*/
	//lightning
	//m_d3dDevice->S
	



	/*
	m_light.Direction.x = 0.0f;
	m_light.Direction.y = 1.0f;
	m_light.Direction.z = 0.0f;
	D3DXVec3Normalize((D3DXVECTOR3*)&m_light.Direction, &m_vecDir);
	m_light.Specular.r = 1.0f;
	m_light.Specular.g = 1.0f;
	m_light.Specular.b = 1.0f;
	m_light.Specular.a = 1.0f;

	m_light.Range		= 1000.0f;
	m_light.Falloff		= 2;

	m_light.Ambient.r	= 0.0f;
	m_light.Ambient.g	= 0.0f;
	m_light.Ambient.b	= 0.0f;
	m_light.Ambient.a	= 0.0f;

	m_light.Diffuse.r	= 1.0f;
	m_light.Diffuse.g	= 1.0f;
	m_light.Diffuse.b	= 1.0f;
	m_light.Diffuse.a	= 1.0f;
	
	m_light.Phi			= 0.4f;
	m_light.Theta		= 0.1;
	m_light.Attenuation0 = 1;
	m_light.Attenuation1 = 1;
	m_light.Attenuation2 = 0;*/
	
/*	m_light.Direction.x = 1.0f;
	m_light.Direction.y = -1.0f;
	m_light.Direction.z = 1.0f;
*/	
	
	
	
	//m_vecDir	= D3DXVECTOR3(0.5f, 0.0f, 0.5f);

/*	m_d3dDevice->SetLight(0, &m_light);
	m_d3dDevice->LightEnable(0, TRUE);
	m_d3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_d3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);*/
//	m_d3dDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	
	//m_d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_d3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	
	
//	m_d3dDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);

	/*
	IDirect3DTexture9*	pTexture;
	*/
	//D3DXMatrixIdentity(&m_matWorld);
	setupMatrices();
	
	
	

	
}

void	scene::edraw()
{
		m_d3dDevice->EndScene();
		m_d3dDevice->Present(NULL, NULL, NULL, NULL);
}

void	scene::drawMesh(MyMesh* givenMesh)
{
	
	for( DWORD i=0; i<givenMesh->m_dwNumMaterials; i++ )
        {
            // Set the material and texture for this subset
            m_d3dDevice->SetMaterial( &givenMesh->m_pMeshMaterials[i] );
            m_d3dDevice->SetTexture( 0, givenMesh->m_pMeshTextures[i] );
    
			
            // Draw the mesh subset
			
            givenMesh->m_pMesh->DrawSubset( i );
			//givenMesh->m_pMesh->SetAttributeTable
			
        }

}

void	scene::initMesh(MyMesh* givenMesh)
{
	LPD3DXBUFFER pD3DXMtrlBuffer;

    // Load the mesh from the specified file
    if( FAILED( D3DXLoadMeshFromX( "Tiger.x", D3DXMESH_SYSTEMMEM, 
                                   m_d3dDevice, NULL, 
                                   &pD3DXMtrlBuffer, NULL, &givenMesh->m_dwNumMaterials, 
                                   &givenMesh->m_pMesh ) ) )
    {
        // If model is not in current folder, try parent folder
        if( FAILED( D3DXLoadMeshFromX( "..\\Tiger.x", D3DXMESH_SYSTEMMEM, 
                                    m_d3dDevice, NULL, 
                                    &pD3DXMtrlBuffer, NULL, &givenMesh->m_dwNumMaterials, 
                                    &givenMesh->m_pMesh ) ) )
        {
            MessageBox(NULL, "Could not find tiger.x", "Meshes.exe", MB_OK);
//            return E_FAIL;
        }
    }

    // We need to extract the material properties and texture names from the 
    // pD3DXMtrlBuffer
    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    givenMesh->m_pMeshMaterials = new D3DMATERIAL9[givenMesh->m_dwNumMaterials];
    givenMesh->m_pMeshTextures  = new LPDIRECT3DTEXTURE9[givenMesh->m_dwNumMaterials];

    for( DWORD i=0; i<givenMesh->m_dwNumMaterials; i++ )
    {
        // Copy the material
        givenMesh->m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        givenMesh->m_pMeshMaterials[i].Ambient = givenMesh->m_pMeshMaterials[i].Diffuse;

        givenMesh->m_pMeshTextures[i] = NULL;
        if( d3dxMaterials[i].pTextureFilename != NULL && 
            lstrlen(d3dxMaterials[i].pTextureFilename) > 0 )
        {
            // Create the texture
            if( FAILED( D3DXCreateTextureFromFile( m_d3dDevice, 
                                                d3dxMaterials[i].pTextureFilename, 
                                                &givenMesh->m_pMeshTextures[i] ) ) )
            {
                // If texture is not in current folder, try parent folder
                const TCHAR* strPrefix = TEXT("..\\");
                const int lenPrefix = lstrlen( strPrefix );
                TCHAR strTexture[MAX_PATH];
                lstrcpyn( strTexture, strPrefix, MAX_PATH );
                lstrcpyn( strTexture + lenPrefix, d3dxMaterials[i].pTextureFilename, MAX_PATH - lenPrefix );
                // If texture is not in current folder, try parent folder
                if( FAILED( D3DXCreateTextureFromFile( m_d3dDevice, 
                                                    strTexture, 
                                                    &givenMesh->m_pMeshTextures[i] ) ) )
                {
                    MessageBox(NULL, "Could not find texture map", "Meshes.exe", MB_OK);
                }
            }
        }
    }
//	givenMesh->m_pMesh->g

    // Done with the material buffer
    pD3DXMtrlBuffer->Release();

//    return S_OK;
}

void	scene::draw(masterClass* givenObject)
{

	m_light.Position.x = m_EyePosition.x;
	m_light.Position.y = m_EyePosition.y;
	m_light.Position.z = m_EyePosition.z;

	m_d3dDevice->SetLight(0, &m_light);
	m_d3dDevice->LightEnable(0, TRUE);
//	m_d3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//	m_d3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
	
	for (int count=0; count < givenObject->getNumPolys(); count++)
	{
		CUSTOMVERTEX vertices[] =
		{
			{
				givenObject->getPoly(count)->p1->x, givenObject->getPoly(count)->p1->y, givenObject->getPoly(count)->p1->z, /*0xffff0000*/ //0x00000000,
			},{
				givenObject->getPoly(count)->p2->x, givenObject->getPoly(count)->p2->y, givenObject->getPoly(count)->p2->z, /*0xff00ff00*/ //0x00000000,
			},{
				givenObject->getPoly(count)->p3->x, givenObject->getPoly(count)->p3->y, givenObject->getPoly(count)->p3->z, /*0xff00ffff*/ //0x00000000,
			},/*{
				givenObject->getPoly(count+1)->p1->x, givenObject->getPoly(count+1)->p1->y, givenObject->getPoly(count+1)->p1->z, 0x00ffffff,
			},{
				givenObject->getPoly(count+1)->p2->x, givenObject->getPoly(count+1)->p2->y, givenObject->getPoly(count+1)->p2->z, 0x00ffffff,
			},{
				givenObject->getPoly(count+1)->p3->x, givenObject->getPoly(count+1)->p3->y, givenObject->getPoly(count+1)->p3->z, 0x00ffffff,
			},*/
		};
		//count++;

/*	CUSTOMVERTEX vertices[] =
	{
		{
			0.0f, 0.0f, 0.0f, 0xffff0000,
		},{
			1.0f, 0.0f, 0.0f, 0xff00ff00,
		},{
			1.0f, 1.0f, 1.0f, 0xff00ffff,
		},
	};
*/	
		
		VOID* pVertices;
		m_pVB->Lock(0, sizeof(CUSTOMVERTEX), (void**)&pVertices, 0);
		memcpy(pVertices, vertices, sizeof(CUSTOMVERTEX));
		m_pVB->Unlock();


		

		m_d3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
		//m_d3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		m_d3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		//if (FAILED(m_d3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 3))) exit(1);
		if (FAILED(m_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vertices, sizeof(CUSTOMVERTEX)))) exit(1);

		//m_pFont->DrawText(2, 0, D3DCOLOR_ARGB(255,255,255,0), "hello");
	}
}