#include ".\mymesh.h"

MyMesh::MyMesh(void)
{
	m_pMesh				= NULL;
    m_dwNumMaterials     = 0L;
    m_pMeshMaterials  = NULL;
    m_pMeshTextures      = NULL;
	m_bShowOptimized     = TRUE;
}

MyMesh::~MyMesh(void)
{
}



void	MyMesh::init(LPDIRECT3DDEVICE9	givenDevice) //filename
{
//	DXUtil_GetDXSDKMediaPathCb( m_strInitialDir, sizeof(m_strInitialDir) );
//    _tcscpy( m_strMeshFilename, _T("Tiger.x") );
//	m_pd3dDevice = givenDevice ;
}
void	MyMesh::draw()
{
	for( DWORD i=0; i<m_dwNumMaterials; i++ )
        {
            // Set the material and texture for this subset
//            m_pd3dDevice->SetMaterial( &m_pMeshMaterials[i] );
//            m_pd3dDevice->SetTexture( 0, m_pMeshTextures[i] );
        
            // Draw the mesh subset
            m_pMesh->DrawSubset( i );
        }
}
//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT MyMesh::InitGeometry()
{
LPD3DXBUFFER pD3DXMtrlBuffer;

    // Load the mesh from the specified file
/*    if( FAILED( D3DXLoadMeshFromX( "Tiger.x", D3DXMESH_SYSTEMMEM, 
                                   m_pd3dDevice, NULL, 
                                   &pD3DXMtrlBuffer, NULL, &m_dwNumMaterials, 
                                   &m_pMesh ) ) )
    {
        // If model is not in current folder, try parent folder
        if( FAILED( D3DXLoadMeshFromX( "..\\Tiger.x", D3DXMESH_SYSTEMMEM, 
                                    m_pd3dDevice, NULL, 
                                    &pD3DXMtrlBuffer, NULL, &m_dwNumMaterials, 
                                    &m_pMesh ) ) )
        {
            MessageBox(NULL, "Could not find tiger.x", "Meshes.exe", MB_OK);
            return E_FAIL;
        }
    }

    // We need to extract the material properties and texture names from the 
    // pD3DXMtrlBuffer
    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];
    m_pMeshTextures  = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

    for( DWORD i=0; i<m_dwNumMaterials; i++ )
    {
        // Copy the material
        m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;

        m_pMeshTextures[i] = NULL;
        if( d3dxMaterials[i].pTextureFilename != NULL && 
            lstrlen(d3dxMaterials[i].pTextureFilename) > 0 )
        {
            // Create the texture
            if( FAILED( D3DXCreateTextureFromFile( m_pd3dDevice, 
                                                d3dxMaterials[i].pTextureFilename, 
                                                &m_pMeshTextures[i] ) ) )
            {
                // If texture is not in current folder, try parent folder
                const TCHAR* strPrefix = TEXT("..\\");
                const int lenPrefix = lstrlen( strPrefix );
                TCHAR strTexture[MAX_PATH];
                lstrcpyn( strTexture, strPrefix, MAX_PATH );
                lstrcpyn( strTexture + lenPrefix, d3dxMaterials[i].pTextureFilename, MAX_PATH - lenPrefix );
                // If texture is not in current folder, try parent folder
                if( FAILED( D3DXCreateTextureFromFile( m_pd3dDevice, 
                                                    strTexture, 
                                                    &m_pMeshTextures[i] ) ) )
                {
                    MessageBox(NULL, "Could not find texture map", "Meshes.exe", MB_OK);
                }
            }
        }
    }

    // Done with the material buffer
    pD3DXMtrlBuffer->Release();
*/
    return S_OK;
}

