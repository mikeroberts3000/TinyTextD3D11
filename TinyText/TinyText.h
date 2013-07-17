#pragma once

#include "TinyText.Core/TinyText.h"

#using <SlimDX.dll>

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace SlimDX::Direct3D11;

namespace TinyText
{

#pragma managed
public ref class Context
{
public:
    Context( Device^ direct3D11Device, DeviceContext^ direct3D11DeviceContext, int characterCapacity, [Out] bool% result );
    ~Context();

    // Print some text to the context - returns 'true' on success or 'false' on failure.
    // 'colour' is expected to be in the form: 0xAABBGGRR.
    bool Print( Viewport^ viewport, System::String^ text, int x, int y );
    bool Print( Viewport^ viewport, System::String^ text, int x, int y, DWORD colour );

    // Render the context onto the screen - returns 'true' on success or 'false' on failure
    bool Render();
    bool Render( bool maintainState );

private:
    TinyTextContext_c* mTinyTextContext;
};

}