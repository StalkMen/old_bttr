#pragma once

#if defined(DIRECTX11)

IC HRESULT CreateQuery ( ID3DQuery **ppQuery, D3DQUERYTYPE Type)
{
	D3D_QUERY_DESC	desc;
	desc.MiscFlags = 0;
	
	switch (Type)
	{
	case D3DQUERYTYPE_OCCLUSION:
		desc.Query = D3D_QUERY_OCCLUSION;
		break;
	default:
		VERIFY(!"No default.");
	}

	return DEVICE_HW::CRYRAY_RENDER::HW.pRenderDevice->CreateQuery( &desc, ppQuery);
}

IC HRESULT GetData( ID3DQuery *pQuery, void *pData, UINT DataSize )
{
	//	Use D3Dxx_ASYNC_GETDATA_DONOTFLUSH for prevent flushing
	return DEVICE_HW::CRYRAY_RENDER::HW.pRenderContext->GetData(pQuery, pData, DataSize, 0);
}

IC HRESULT BeginQuery( ID3DQuery *pQuery)
{
	DEVICE_HW::CRYRAY_RENDER::HW.pRenderContext->Begin(pQuery);
	return S_OK;
}

IC HRESULT EndQuery( ID3DQuery *pQuery)
{
	DEVICE_HW::CRYRAY_RENDER::HW.pRenderContext->End(pQuery);
	return S_OK;
}

#elif defined(DIRECTX10)

IC HRESULT CreateQuery ( ID3DQuery **ppQuery, D3DQUERYTYPE Type)
{
	D3D_QUERY_DESC	desc;
	desc.MiscFlags = 0;
	
	switch (Type)
	{
	case D3DQUERYTYPE_OCCLUSION:
		desc.Query = D3D_QUERY_OCCLUSION;
		break;
	default:
		VERIFY(!"No default.");
	}

	return DEVICE_HW::CRYRAY_RENDER::HW.pRenderDevice->CreateQuery( &desc, ppQuery);
}

IC HRESULT GetData( ID3DQuery *pQuery, void *pData, UINT DataSize )
{
	//	Use D3Dxx_ASYNC_GETDATA_DONOTFLUSH for prevent flushing
	return pQuery->GetData( pData, DataSize, 0);
}

IC HRESULT BeginQuery( ID3DQuery *pQuery)
{
	pQuery->Begin();
	return S_OK;
}

IC HRESULT EndQuery( ID3DQuery *pQuery)
{
	pQuery->End();
	return S_OK;
}

#else	//	DIRECTX10

IC HRESULT CreateQuery ( ID3DQuery **ppQuery, D3DQUERYTYPE Type)
{
	return DEVICE_HW::CRYRAY_RENDER::HW.pDevice->CreateQuery(Type, ppQuery);
}

IC HRESULT GetData( ID3DQuery *pQuery, void *pData, UINT DataSize )
{
	return pQuery->GetData( pData, DataSize, D3DGETDATA_FLUSH);
}

IC HRESULT BeginQuery( ID3DQuery *pQuery)
{
	return pQuery->Issue( D3DISSUE_BEGIN);
}

IC HRESULT EndQuery( ID3DQuery *pQuery)
{
	return pQuery->Issue( D3DISSUE_END);
}

#endif	//	DIRECTX10