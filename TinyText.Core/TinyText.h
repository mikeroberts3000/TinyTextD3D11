/*=================================================================================

    PROJECT:        Tiny Text Library for DirectX 10

    AUTHOR:         James Bird (http://www.jb101.co.uk/)

    DESCRIPTION:    A light-weight, quick-to-integrate debug text drawing library

    USAGE:          - Construct a text context, providing a D3D11 device and a
                      maximum character capacity

                    - During each frame, add text to the text context using the
                      'TinyTextContext_c::Print' method

                        - Two versions of this method are available. One takes
                          an additional character count which can be used to
                          avoid buffer overruns

                    - At the end of your frame, call 'TinyTextContext_c::Render'
                      to draw all text to the screen

                        - The default behaviour for this method is to save previous
                          D3D11 device state. This can be overridden with the
                          optional 'bool' argument

                    - During your application shutdown, remember to delete the
                      text context if you created it on the heap

=================================================================================*/
#pragma once

/*---------------------------------------------------------------------------------
    Includes
---------------------------------------------------------------------------------*/
#include <d3d11.h>

/*---------------------------------------------------------------------------------
    TinyTextContext_c
    Represents a text context. For usage, see comments at the top of this file
---------------------------------------------------------------------------------*/
//namespace
//{
class TinyTextContext_c
{
public:
    
    // Constructor - takes a pointer to the direct3d device and the capacity of this
    // context (in characters). Also takes an optional pointer to a boolean that will
    // receive the result of this operation.
    explicit TinyTextContext_c( ID3D11Device * device, ID3D11DeviceContext * deviceContext, size_t characterCapacity, bool * result = NULL );

    // Destructor - releases all GPU resources
    ~TinyTextContext_c( );

    // Default text colour = opaque white
    static const DWORD DefaultColour = 0xFFFFFFFF;
    
    // Print some text to the context - returns 'true' on success or 'false' on failure.
    // 'colour' is expected to be in the form: 0xAABBGGRR.
    bool Print( const D3D11_VIEWPORT & viewport, const char * text, int x, int y, DWORD colour = DefaultColour );
    bool Print( const D3D11_VIEWPORT & viewport, size_t maxCharacterCount, const char * text, int x, int y, DWORD colour = DefaultColour );
    
    // Render the context onto the screen - returns 'true' on success or 'false' on failure
    bool Render( bool maintainState = true );

private:

    // Deliberately not implemented - this object cannot be copied or assigned to
    TinyTextContext_c( const TinyTextContext_c & );
    TinyTextContext_c & operator = ( const TinyTextContext_c & );

    // Initialises the context
    bool Initialise( ID3D11Device * device, ID3D11DeviceContext * deviceContext, size_t characterCapacity );

    // Maps the vertex buffer to CPU memory (if it isn't already mapped)
    bool MapVertexBuffer( );

    // Unmaps the vertex buffer to CPU memory (if it isn't already unmapped)
    void UnmapVertexBuffer( );

    // The Direct3D10 device associated with this text context
    ID3D11Device * m_Device;

    // The Direct3D10 device associated with this text context
    ID3D11DeviceContext * m_DeviceContext;

    // A view of the font texture resource
    ID3D11ShaderResourceView * m_TextureView;

    // The vertex shader
    ID3D11VertexShader * m_VertexShader;

    // The pixel shader
    ID3D11PixelShader * m_PixelShader;

    // The input layout
    ID3D11InputLayout * m_InputLayout;

    // The vertex buffer
    ID3D11Buffer * m_VertexBuffer;

    // The sampler state
    ID3D11SamplerState * m_SamplerState;

    // The depth-stencil state
    ID3D11DepthStencilState * m_DepthStencilState;

    // Number of vertices written to the vertex buffer
    unsigned int m_NumVertices;

    // Total capacity
    const unsigned int m_Capacity;

    // The current write position of the vertex buffer (when mapped to CPU memory)
    DWORD * m_VertexBufferWriteAddress;

};
//}
