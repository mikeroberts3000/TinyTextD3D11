#include "TinyText.h"

#include <d3d11.h>

#using <SlimDX.dll>

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace SlimDX::Direct3D11;

namespace TinyText
{

Context::Context( Device^ direct3D11Device, DeviceContext^ direct3D11DeviceContext, int characterCapacity, [Out] bool% result )
{
    bool internalResult = false;

    mTinyTextContext = new TinyTextContext_c(
        reinterpret_cast< ID3D11Device* >( direct3D11Device->ComPointer.ToPointer() ),
        reinterpret_cast< ID3D11DeviceContext* >( direct3D11DeviceContext->ComPointer.ToPointer() ),
        characterCapacity,
        &internalResult );

    result = internalResult;
}

Context::~Context()
{
    delete mTinyTextContext;
}

bool Context::Print( Viewport^ viewport, System::String^ text, int x, int y )
{
    return Print( viewport, text, x, y, TinyTextContext_c::DefaultColour );
}

bool Context::Print( Viewport^ viewport, System::String^ text, int x, int y, DWORD colour )
{
    D3D11_VIEWPORT d3d11Viewport;

    d3d11Viewport.Width = viewport->Width;
    d3d11Viewport.Height = viewport->Height;
    d3d11Viewport.MinDepth = viewport->MinZ;
    d3d11Viewport.MaxDepth = viewport->MaxZ;
    d3d11Viewport.TopLeftX = viewport->X;
    d3d11Viewport.TopLeftY = viewport->Y;

    char* cString = (char*)(void*)Marshal::StringToHGlobalAnsi( text );
    bool result = mTinyTextContext->Print( d3d11Viewport, text->Length, cString, x, y, colour );
    Marshal::FreeHGlobal( IntPtr( (void*)cString ) );

    return result;
}

bool Context::Render()
{
    return Render( true );
}

bool Context::Render( bool maintainState )
{
    return mTinyTextContext->Render( maintainState );
}

}