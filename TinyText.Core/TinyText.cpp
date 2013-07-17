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

/*---------------------------------------------------------------------------------
    Includes
---------------------------------------------------------------------------------*/
#include "TinyText.h"
#include <d3dx11.h>

/*---------------------------------------------------------------------------------
    Constants
---------------------------------------------------------------------------------*/
//namespace
//{
    // The font texture data, encoded as a monochrome bitmap
    const unsigned char TextTexture[]           = { 66 , 77 , 62 , 8 , 0 , 0 , 0 , 0 , 0 , 0 , 62 , 0 , 0 , 0 , 40 , 0 , 0 , 0 , 128 , 0 , 0 , 0 , 128 , 0 , 0 , 0 , 1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 8 , 0 , 0 , 18 , 11 , 0 , 0 , 18 , 11 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 255 , 255 , 255 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 63 , 191 , 192 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 8 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 8 , 34 , 13 , 132 , 64 , 131 , 251 , 252 , 16 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 8 , 17 , 7 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 8 , 8 , 159 , 193 , 1 , 64 , 192 , 136 , 140 , 62 , 0 , 0 , 0 , 0 , 0 , 0 , 24 , 17 , 7 , 2 , 129 , 64 , 32 , 80 , 146 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 34 , 13 , 129 , 1 , 64 , 64 , 32 , 98 , 62 , 4 , 0 , 0 , 0 , 0 , 0 , 30 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 4 , 0 , 128 , 0 , 0 , 0 , 33 , 8 , 7 , 132 , 36 , 144 , 240 , 124 , 58 , 54 , 4 , 15 , 128 , 0 , 0 , 0 , 6 , 8 , 8 , 68 , 36 , 145 , 8 , 132 , 70 , 73 , 0 , 0 , 0 , 0 , 0 , 0 , 24 , 8 , 8 , 68 , 36 , 145 , 0 , 132 , 66 , 72 , 15 , 15 , 129 , 0 , 128 , 128 , 33 , 8 , 8 , 68 , 36 , 145 , 0 , 124 , 66 , 63 , 16 , 140 , 66 , 0 , 64 , 64 , 30 , 12 , 136 , 70 , 36 , 145 , 8 , 4 , 66 , 9 , 16 , 10 , 64 , 0 , 0 , 0 , 0 , 11 , 7 , 133 , 199 , 96 , 240 , 120 , 66 , 54 , 31 , 137 , 66 , 33 , 192 , 192 , 8 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 16 , 136 , 196 , 64 , 128 , 32 , 28 , 32 , 2 , 2 , 1 , 1 , 224 , 4 , 120 , 65 , 15 , 7 , 200 , 128 , 64 , 64 , 34 , 16 , 0 , 1 , 0 , 128 , 0 , 8 , 0 , 34 , 0 , 0 , 4 , 65 , 64 , 32 , 32 , 8 , 0 , 0 , 0 , 64 , 192 , 16 , 56 , 20 , 4 , 15 , 194 , 32 , 128 , 192 , 34 , 4 , 15 , 128 , 0 , 33 , 32 , 32 , 72 , 8 , 0 , 4 , 0 , 0 , 0 , 0 , 28 , 2 , 0 , 0 , 0 , 65 , 32 , 64 , 56 , 20 , 0 , 2 , 4 , 33 , 64 , 64 , 8 , 1 , 0 , 0 , 0 , 129 , 32 , 128 , 8 , 34 , 0 , 1 , 2 , 66 , 160 , 160 , 0 , 0 , 130 , 1 , 1 , 0 , 193 , 0 , 48 , 65 , 0 , 0 , 129 , 130 , 160 , 160 , 64 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 4 , 15 , 193 , 130 , 160 , 160 , 32 , 14 , 15 , 134 , 1 , 192 , 240 , 48 , 12 , 28 , 0 , 0 , 2 , 68 , 17 , 16 , 60 , 4 , 4 , 1 , 2 , 32 , 8 , 16 , 16 , 34 , 0 , 136 , 4 , 36 , 17 , 16 , 34 , 4 , 4 , 1 , 5 , 80 , 8 , 120 , 16 , 77 , 0 , 136 , 0 , 0 , 0 , 0 , 34 , 63 , 143 , 2 , 133 , 144 , 248 , 132 , 16 , 81 , 15 , 143 , 128 , 128 , 130 , 8 , 34 , 10 , 4 , 2 , 133 , 81 , 8 , 128 , 16 , 81 , 16 , 136 , 65 , 0 , 129 , 240 , 34 , 63 , 136 , 4 , 69 , 145 , 8 , 128 , 60 , 77 , 16 , 136 , 66 , 0 , 129 , 16 , 34 , 17 , 9 , 4 , 66 , 33 , 8 , 132 , 16 , 34 , 16 , 136 , 68 , 7 , 241 , 16 , 0 , 59 , 134 , 8 , 33 , 192 , 248 , 120 , 16 , 28 , 16 , 136 , 66 , 0 , 129 , 16 , 31 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 15 , 143 , 129 , 0 , 129 , 240 , 33 , 15 , 135 , 195 , 225 , 240 , 240 , 120 , 60 , 30 , 0 , 0 , 0 , 128 , 130 , 8 , 33 , 16 , 136 , 68 , 34 , 17 , 8 , 132 , 66 , 33 , 4 , 2 , 0 , 0 , 0 , 0 , 31 , 16 , 136 , 68 , 34 , 17 , 0 , 128 , 64 , 32 , 4 , 2 , 1 , 0 , 128 , 240 , 1 , 15 , 135 , 195 , 225 , 241 , 248 , 252 , 126 , 63 , 4 , 2 , 1 , 0 , 129 , 8 , 30 , 0 , 128 , 64 , 32 , 17 , 8 , 132 , 66 , 33 , 4 , 2 , 1 , 0 , 129 , 8 , 0 , 15 , 7 , 195 , 193 , 224 , 240 , 120 , 60 , 30 , 4 , 2 , 1 , 0 , 129 , 8 , 4 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 4 , 2 , 1 , 0 , 128 , 240 , 8 , 2 , 2 , 132 , 192 , 0 , 32 , 32 , 20 , 0 , 0 , 0 , 1 , 0 , 128 , 16 , 0 , 1 , 1 , 3 , 33 , 32 , 64 , 16 , 8 , 18 , 4 , 2 , 0 , 0 , 0 , 160 , 32 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 8 , 1 , 2 , 128 , 0 , 64 , 32 , 4 , 30 , 15 , 224 , 160 , 8 , 28 , 2 , 28 , 0 , 0 , 1 , 0 , 192 , 160 , 32 , 4 , 17 , 0 , 0 , 162 , 121 , 8 , 158 , 34 , 39 , 143 , 0 , 0 , 0 , 0 , 32 , 4 , 2 , 129 , 0 , 161 , 40 , 132 , 74 , 32 , 36 , 8 , 139 , 130 , 1 , 112 , 62 , 4 , 4 , 129 , 0 , 160 , 152 , 84 , 38 , 32 , 60 , 8 , 68 , 66 , 1 , 8 , 33 , 4 , 9 , 1 , 3 , 163 , 201 , 232 , 210 , 16 , 36 , 8 , 74 , 35 , 225 , 8 , 33 , 4 , 18 , 15 , 231 , 161 , 32 , 144 , 40 , 8 , 39 , 30 , 74 , 34 , 17 , 8 , 33 , 4 , 20 , 1 , 7 , 161 , 16 , 136 , 68 , 8 , 20 , 8 , 73 , 34 , 17 , 112 , 62 , 0 , 8 , 129 , 7 , 161 , 8 , 132 , 34 , 0 , 20 , 8 , 72 , 162 , 17 , 16 , 0 , 4 , 7 , 129 , 3 , 227 , 1 , 128 , 192 , 8 , 20 , 8 , 136 , 163 , 225 , 16 , 8 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 15 , 143 , 4 , 66 , 1 , 16 , 8 , 15 , 2 , 2 , 132 , 16 , 65 , 252 , 124 , 31 , 0 , 0 , 3 , 162 , 0 , 224 , 8 , 16 , 130 , 5 , 68 , 16 , 65 , 0 , 66 , 33 , 4 , 8 , 64 , 0 , 0 , 0 , 8 , 16 , 133 , 5 , 66 , 32 , 64 , 128 , 66 , 33 , 4 , 8 , 65 , 1 , 16 , 48 , 8 , 16 , 133 , 5 , 65 , 64 , 64 , 64 , 66 , 33 , 4 , 8 , 65 , 1 , 32 , 64 , 8 , 16 , 136 , 133 , 64 , 128 , 160 , 32 , 66 , 33 , 4 , 8 , 65 , 1 , 64 , 64 , 8 , 16 , 136 , 136 , 33 , 65 , 16 , 16 , 124 , 31 , 4 , 12 , 65 , 1 , 192 , 64 , 8 , 16 , 144 , 72 , 34 , 34 , 8 , 8 , 64 , 1 , 15 , 11 , 129 , 1 , 32 , 64 , 127 , 16 , 144 , 72 , 36 , 18 , 8 , 4 , 64 , 1 , 4 , 8 , 1 , 1 , 16 , 64 , 0 , 16 , 144 , 72 , 36 , 18 , 9 , 252 , 64 , 1 , 4 , 8 , 0 , 1 , 0 , 64 , 63 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 3 , 8 , 0 , 1 , 0 , 64 , 32 , 16 , 7 , 132 , 33 , 192 , 240 , 132 , 126 , 65 , 0 , 0 , 1 , 1 , 0 , 64 , 32 , 16 , 8 , 68 , 32 , 129 , 8 , 136 , 64 , 65 , 16 , 135 , 0 , 0 , 0 , 0 , 32 , 16 , 16 , 68 , 32 , 128 , 8 , 144 , 64 , 65 , 17 , 136 , 131 , 162 , 17 , 240 , 60 , 16 , 17 , 196 , 32 , 128 , 8 , 160 , 64 , 73 , 18 , 144 , 68 , 66 , 34 , 8 , 32 , 30 , 16 , 7 , 224 , 128 , 8 , 192 , 64 , 73 , 18 , 144 , 72 , 162 , 64 , 8 , 32 , 16 , 16 , 4 , 32 , 128 , 8 , 160 , 64 , 85 , 18 , 144 , 72 , 34 , 128 , 8 , 32 , 16 , 16 , 4 , 32 , 128 , 8 , 144 , 64 , 85 , 20 , 144 , 72 , 35 , 225 , 240 , 63 , 16 , 8 , 68 , 32 , 128 , 8 , 136 , 64 , 99 , 20 , 144 , 72 , 34 , 18 , 0 , 0 , 31 , 135 , 132 , 33 , 192 , 248 , 132 , 64 , 65 , 24 , 136 , 136 , 34 , 18 , 0 , 28 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 16 , 135 , 4 , 66 , 18 , 8 , 34 , 2 , 7 , 3 , 129 , 0 , 192 , 116 , 58 , 8 , 0 , 0 , 3 , 131 , 225 , 240 , 2 , 2 , 8 , 132 , 65 , 0 , 32 , 140 , 70 , 0 , 14 , 8 , 64 , 0 , 0 , 0 , 2 , 2 , 0 , 132 , 64 , 128 , 16 , 132 , 66 , 8 , 16 , 8 , 71 , 193 , 225 , 224 , 12 , 31 , 0 , 132 , 64 , 128 , 16 , 132 , 66 , 8 , 39 , 8 , 68 , 34 , 17 , 16 , 2 , 18 , 15 , 7 , 128 , 64 , 240 , 132 , 66 , 4 , 42 , 143 , 196 , 36 , 1 , 8 , 2 , 10 , 8 , 4 , 0 , 65 , 16 , 132 , 66 , 2 , 42 , 136 , 68 , 36 , 1 , 8 , 34 , 10 , 8 , 4 , 0 , 33 , 16 , 0 , 0 , 2 , 42 , 136 , 71 , 196 , 1 , 8 , 28 , 6 , 8 , 2 , 0 , 33 , 16 , 40 , 0 , 34 , 38 , 132 , 132 , 36 , 1 , 8 , 0 , 2 , 15 , 129 , 131 , 224 , 224 , 16 , 36 , 28 , 16 , 132 , 132 , 36 , 1 , 8 , 20 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 15 , 3 , 4 , 34 , 17 , 16 , 20 , 4 , 0 , 131 , 97 , 224 , 240 , 120 , 60 , 30 , 0 , 0 , 7 , 193 , 225 , 224 , 62 , 14 , 17 , 68 , 130 , 17 , 8 , 132 , 66 , 33 , 14 , 135 , 64 , 0 , 0 , 0 , 20 , 17 , 8 , 132 , 66 , 17 , 8 , 132 , 66 , 33 , 17 , 136 , 195 , 128 , 129 , 240 , 20 , 1 , 4 , 2 , 162 , 17 , 8 , 132 , 66 , 33 , 16 , 136 , 68 , 64 , 129 , 0 , 20 , 14 , 2 , 1 , 2 , 17 , 8 , 132 , 66 , 33 , 16 , 136 , 68 , 64 , 128 , 128 , 62 , 16 , 1 , 2 , 129 , 224 , 240 , 120 , 60 , 30 , 16 , 136 , 68 , 64 , 128 , 64 , 20 , 17 , 8 , 130 , 64 , 0 , 0 , 0 , 0 , 0 , 16 , 136 , 68 , 64 , 128 , 32 , 20 , 14 , 20 , 66 , 64 , 64 , 64 , 72 , 76 , 0 , 0 , 0 , 4 , 64 , 128 , 16 , 0 , 4 , 8 , 1 , 128 , 128 , 32 , 48 , 50 , 18 , 2 , 2 , 4 , 66 , 128 , 16 , 32 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 4 , 1 , 4 , 65 , 129 , 16 , 32 , 24 , 7 , 131 , 128 , 96 , 224 , 112 , 96 , 28 , 0 , 0 , 3 , 128 , 128 , 224 , 62 , 4 , 8 , 68 , 64 , 129 , 16 , 136 , 16 , 34 , 6 , 7 , 192 , 0 , 0 , 0 , 33 , 4 , 8 , 72 , 32 , 130 , 9 , 4 , 16 , 65 , 2 , 8 , 67 , 128 , 129 , 8 , 33 , 10 , 8 , 72 , 32 , 130 , 9 , 4 , 16 , 65 , 15 , 8 , 68 , 64 , 1 , 8 , 33 , 10 , 8 , 72 , 32 , 130 , 9 , 4 , 16 , 65 , 16 , 135 , 196 , 64 , 129 , 8 , 33 , 17 , 8 , 72 , 35 , 2 , 9 , 4 , 12 , 65 , 32 , 0 , 68 , 64 , 129 , 8 , 62 , 17 , 8 , 72 , 32 , 130 , 9 , 4 , 16 , 65 , 32 , 7 , 131 , 128 , 129 , 136 , 32 , 32 , 136 , 68 , 64 , 129 , 16 , 136 , 16 , 34 , 32 , 0 , 4 , 64 , 129 , 112 , 32 , 0 , 9 , 67 , 128 , 128 , 224 , 112 , 16 , 28 , 32 , 2 , 4 , 64 , 128 , 0 , 32 , 4 , 2 , 0 , 0 , 130 , 112 , 80 , 16 , 8 , 16 , 133 , 4 , 64 , 129 , 48 , 0 , 2 , 4 , 4 , 64 , 97 , 200 , 32 , 96 , 4 , 15 , 2 , 3 , 128 , 128 , 200 , 8 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 8 , 31 , 143 , 199 , 227 , 241 , 8 , 132 , 66 , 28 , 15 , 12 , 3 , 129 , 224 , 240 , 8 , 16 , 8 , 4 , 2 , 1 , 8 , 132 , 66 , 16 , 16 , 130 , 0 , 130 , 17 , 8 , 8 , 16 , 8 , 4 , 2 , 1 , 248 , 132 , 66 , 16 , 16 , 130 , 0 , 130 , 17 , 8 , 8 , 16 , 8 , 4 , 2 , 1 , 8 , 252 , 126 , 16 , 16 , 133 , 0 , 130 , 17 , 8 , 8 , 30 , 15 , 7 , 131 , 193 , 8 , 132 , 66 , 16 , 16 , 133 , 0 , 130 , 17 , 8 , 8 , 16 , 8 , 4 , 2 , 1 , 8 , 132 , 66 , 16 , 16 , 136 , 128 , 130 , 17 , 8 , 8 , 16 , 8 , 4 , 2 , 0 , 144 , 72 , 36 , 16 , 16 , 136 , 128 , 130 , 17 , 8 , 12 , 16 , 8 , 4 , 2 , 0 , 144 , 72 , 36 , 16 , 16 , 144 , 64 , 130 , 17 , 8 , 8 , 31 , 143 , 199 , 227 , 240 , 96 , 48 , 24 , 16 , 16 , 128 , 0 , 130 , 17 , 72 , 16 , 0 , 5 , 1 , 0 , 128 , 144 , 0 , 76 , 16 , 0 , 0 , 0 , 128 , 192 , 32 , 0 , 17 , 2 , 0 , 129 , 0 , 96 , 72 , 50 , 28 , 16 , 136 , 131 , 128 , 192 , 16 , 8 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 8 , 16 , 136 , 71 , 2 , 16 , 64 , 112 , 66 , 8 , 2 , 4 , 1 , 0 , 128 , 64 , 8 , 16 , 136 , 64 , 130 , 16 , 64 , 136 , 70 , 8 , 4 , 2 , 1 , 0 , 128 , 64 , 8 , 16 , 136 , 64 , 130 , 16 , 65 , 4 , 70 , 8 , 4 , 2 , 1 , 0 , 128 , 64 , 8 , 31 , 143 , 192 , 131 , 240 , 65 , 4 , 74 , 8 , 8 , 1 , 1 , 0 , 128 , 64 , 8 , 16 , 136 , 64 , 130 , 16 , 65 , 4 , 74 , 8 , 8 , 1 , 1 , 1 , 64 , 64 , 8 , 16 , 136 , 64 , 130 , 16 , 1 , 4 , 82 , 8 , 8 , 1 , 1 , 1 , 64 , 64 , 8 , 9 , 4 , 128 , 129 , 32 , 65 , 4 , 98 , 8 , 8 , 1 , 1 , 2 , 32 , 64 , 8 , 9 , 4 , 128 , 129 , 32 , 64 , 136 , 98 , 8 , 8 , 1 , 1 , 2 , 32 , 64 , 8 , 6 , 7 , 0 , 0 , 224 , 64 , 112 , 66 , 8 , 4 , 2 , 1 , 4 , 80 , 192 , 8 , 9 , 2 , 0 , 0 , 64 , 64 , 32 , 140 , 0 , 4 , 2 , 2 , 128 , 128 , 64 , 8 , 6 , 1 , 0 , 128 , 128 , 64 , 64 , 114 , 20 , 2 , 4 , 1 , 0 , 64 , 32 };

    // The size of the font texture data in bytes
    const unsigned int  TextTextureByteCount    = 2110;

    // The width (in pixels) of the texture
    const unsigned int  TextTextureWidth        = 128;

    // The height (in pixels) of the texture
    const unsigned int  TextTextureHeight       = 128;

    // Number of bytes per character. 1 byte for X coordinate, 1 byte for Y coordinate, and 1 byte whose upper 4-bits contains the y-offset, and the lower 4-bits contains the height
    const unsigned int  CharacterByteCount      = 3;

    // Total number of characters
    const unsigned int  CharacterCount          = 256;

    // Width of each character (using a fixed-width font)
    const unsigned int  CharacterWidth          = 8;

    // The character data
    const unsigned char CharacterData[]         = { 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 72 , 124 , 208 , 108 , 24 , 41 , 36 , 120 , 35 , 0 , 37 , 41 , 9 , 36 , 41 , 18 , 36 , 41 , 27 , 36 , 41 , 81 , 118 , 35 , 81 , 0 , 43 , 90 , 0 , 43 , 18 , 120 , 69 , 108 , 94 , 71 , 117 , 115 , 162 , 45 , 124 , 113 , 36 , 124 , 161 , 9 , 105 , 55 , 99 , 34 , 41 , 108 , 34 , 41 , 117 , 34 , 41 , 0 , 47 , 41 , 9 , 46 , 41 , 18 , 46 , 41 , 27 , 46 , 41 , 36 , 46 , 41 , 99 , 24 , 41 , 45 , 46 , 41 , 81 , 104 , 86 , 27 , 105 , 87 , 99 , 94 , 71 , 72 , 120 , 83 , 36 , 105 , 71 , 72 , 46 , 41 , 81 , 45 , 41 , 90 , 45 , 41 , 99 , 44 , 41 , 108 , 44 , 41 , 117 , 44 , 41 , 0 , 57 , 41 , 9 , 56 , 41 , 18 , 56 , 41 , 27 , 56 , 41 , 36 , 56 , 41 , 45 , 56 , 41 , 54 , 56 , 41 , 63 , 56 , 41 , 72 , 56 , 41 , 81 , 55 , 41 , 90 , 55 , 41 , 0 , 77 , 41 , 99 , 54 , 41 , 108 , 54 , 41 , 117 , 54 , 41 , 0 , 67 , 41 , 9 , 66 , 41 , 18 , 66 , 41 , 27 , 66 , 41 , 36 , 66 , 41 , 45 , 66 , 41 , 54 , 66 , 41 , 72 , 12 , 43 , 54 , 105 , 55 , 99 , 12 , 43 , 54 , 120 , 19 , 54 , 124 , 193 , 99 , 115 , 34 , 54 , 113 , 86 , 63 , 66 , 41 , 45 , 113 , 86 , 72 , 66 , 41 , 81 , 111 , 86 , 81 , 65 , 41 , 45 , 96 , 88 , 90 , 65 , 41 , 99 , 64 , 41 , 27 , 0 , 43 , 108 , 64 , 41 , 117 , 64 , 41 , 36 , 113 , 86 , 27 , 113 , 86 , 18 , 113 , 86 , 90 , 95 , 88 , 81 , 95 , 88 , 9 , 113 , 86 , 0 , 114 , 86 , 63 , 96 , 56 , 63 , 113 , 86 , 117 , 102 , 86 , 108 , 102 , 86 , 99 , 102 , 86 , 27 , 96 , 88 , 90 , 104 , 86 , 36 , 24 , 43 , 0 , 0 , 28 , 63 , 24 , 43 , 63 , 120 , 51 , 27 , 126 , 208 , 108 , 24 , 41 , 0 , 127 , 208 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 117 , 118 , 208 , 108 , 24 , 41 , 99 , 118 , 208 , 108 , 118 , 208 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 108 , 24 , 41 , 81 , 122 , 208 , 108 , 24 , 41 , 108 , 24 , 41 , 90 , 121 , 208 , 9 , 76 , 41 , 0 , 106 , 71 , 18 , 96 , 56 , 117 , 94 , 39 , 9 , 96 , 56 , 45 , 0 , 43 , 18 , 76 , 41 , 27 , 124 , 33 , 72 , 96 , 40 , 63 , 105 , 39 , 99 , 109 , 101 , 90 , 118 , 82 , 9 , 126 , 113 , 36 , 96 , 40 , 18 , 126 , 17 , 27 , 120 , 35 , 27 , 76 , 41 , 108 , 109 , 37 , 117 , 109 , 37 , 108 , 115 , 34 , 0 , 97 , 88 , 36 , 76 , 41 , 63 , 124 , 113 , 45 , 120 , 163 , 0 , 121 , 37 , 45 , 105 , 39 , 9 , 120 , 101 , 45 , 76 , 41 , 54 , 76 , 41 , 63 , 76 , 41 , 72 , 76 , 41 , 36 , 0 , 11 , 18 , 0 , 11 , 9 , 0 , 11 , 63 , 12 , 11 , 54 , 12 , 11 , 45 , 12 , 11 , 81 , 75 , 41 , 81 , 24 , 58 , 36 , 12 , 11 , 27 , 12 , 11 , 18 , 12 , 11 , 9 , 12 , 11 , 0 , 13 , 11 , 117 , 0 , 11 , 99 , 0 , 11 , 72 , 0 , 11 , 90 , 75 , 41 , 63 , 0 , 11 , 54 , 0 , 11 , 72 , 24 , 11 , 54 , 24 , 11 , 45 , 24 , 11 , 27 , 24 , 11 , 72 , 105 , 71 , 99 , 74 , 41 , 18 , 24 , 11 , 117 , 12 , 11 , 108 , 12 , 11 , 81 , 12 , 11 , 108 , 0 , 11 , 108 , 74 , 41 , 117 , 74 , 41 , 0 , 87 , 41 , 9 , 86 , 41 , 18 , 86 , 41 , 27 , 86 , 41 , 36 , 86 , 41 , 90 , 24 , 26 , 72 , 113 , 86 , 54 , 96 , 88 , 45 , 86 , 41 , 54 , 86 , 41 , 63 , 86 , 41 , 72 , 86 , 41 , 81 , 85 , 41 , 90 , 85 , 41 , 99 , 84 , 41 , 108 , 84 , 41 , 117 , 84 , 41 , 117 , 24 , 41 , 36 , 36 , 41 , 45 , 36 , 41 , 54 , 36 , 41 , 63 , 36 , 41 , 72 , 36 , 41 , 18 , 105 , 71 , 90 , 111 , 86 , 81 , 35 , 41 , 90 , 35 , 41 , 54 , 46 , 41 , 63 , 46 , 41 , 9 , 24 , 43 , 0 , 25 , 43 , 90 , 12 , 43 };

    // The shaders
    const char          Shaders[]               = "Texture2D font : register( t0 ); SamplerState fontSampler { Filter = MIN_MIP_MAG_POINT; }; struct VertexIn { float2 pos : POSITIONT; uint2 texCoord : TEXCOORD0; float4 colour : COLOR0; }; struct VertexOut { float4 pos : SV_Position; float2 texCoord : TEXCOORD0; float4 colour : TEXCOORD1; }; VertexOut VSMain( VertexIn input ) { VertexOut output; output.pos = float4( input.pos, 0.0f, 1.0f ); output.colour = input.colour; output.texCoord = input.texCoord / float2( 128.0f, 128.0f ); return output; } float4 PSMain( VertexOut input ) : SV_Target0 { float fontValue = font.SampleLevel( fontSampler, input.texCoord, 0 ); if ( fontValue < 1.0f ) discard; return fontValue.xxxx * input.colour; }";

    // Total number of vertices for each character
    const unsigned int NumVerticesPerCharacter = 6;

    // Meaningful description of each element of the vertex stream for
    // a single character
    enum VertexStreamElements
    {
        Triangle0_Vertex0_Position_X,
        Triangle0_Vertex0_Position_Y,
        Triangle0_Vertex0_UV,
        Triangle0_Vertex0_Colour,

        Triangle0_Vertex1_Position_X,
        Triangle0_Vertex1_Position_Y,
        Triangle0_Vertex1_UV,
        Triangle0_Vertex1_Colour,

        Triangle0_Vertex2_Position_X,
        Triangle0_Vertex2_Position_Y,
        Triangle0_Vertex2_UV,
        Triangle0_Vertex2_Colour,

        Triangle1_Vertex0_Position_X,
        Triangle1_Vertex0_Position_Y,
        Triangle1_Vertex0_UV,
        Triangle1_Vertex0_Colour,

        Triangle1_Vertex1_Position_X,
        Triangle1_Vertex1_Position_Y,
        Triangle1_Vertex1_UV,
        Triangle1_Vertex1_Colour,

        Triangle1_Vertex2_Position_X,
        Triangle1_Vertex2_Position_Y,
        Triangle1_Vertex2_UV,
        Triangle1_Vertex2_Colour,

        NumVertexElementsPerCharacter
    };
//}

/*---------------------------------------------------------------------------------
    Private Implementation
---------------------------------------------------------------------------------*/
//namespace
//{

    /*---------------------------------------------------------------------------------
        PreviousState_c
        Represents the previous state of the graphics card
    ---------------------------------------------------------------------------------*/
    class PreviousState_c
    {
    private:

        // All of the state that is captured and restored between calls to 'Render'
        ID3D11VertexShader * prevVertexShader;
        ID3D11PixelShader * prevPixelShader;
        ID3D11ShaderResourceView * prevTextureView;
        ID3D11SamplerState * prevSampler;
        ID3D11InputLayout * prevInputLayout;
        ID3D11Buffer * prevVertexBuffer;
        ID3D11GeometryShader * prevGeometryShader;
        ID3D11BlendState * prevBlendState;
        float prevBlendFactor[4];
        UINT prevSampleMask;
        ID3D11DepthStencilState * prevDepthStencilState;
        UINT prevStencilRef;
        UINT numViewports;
        ID3D11RasterizerState * prevRasterizerState;
        UINT prevVertexStride;
        UINT prevVertexOffset;
        D3D11_PRIMITIVE_TOPOLOGY prevTopology;

    public:

        // Constructor
        PreviousState_c( );

        // Destructor
        ~PreviousState_c( );
        
        // Captures the current state of the specified device
        void Capture( ID3D11DeviceContext * deviceContext );

        // Restores the previously captured state of the specified device
        void Restore( ID3D11DeviceContext * deviceContext );

    private:

        // Releases any interfaces that have been obtained
        void Release( );
    };

    /*---------------------------------------------------------------------------------
        PreviousState_c::PreviousState_c
        Constructor
    ---------------------------------------------------------------------------------*/
    PreviousState_c::PreviousState_c( )
        : prevVertexShader( 0 ), prevPixelShader( 0 ), prevTextureView( 0 ), prevSampler( 0 ), prevInputLayout( 0 ),
          prevVertexBuffer( 0 ), prevGeometryShader( 0 ), prevBlendState( 0 ), prevSampleMask( 0xffffffff ),
          prevDepthStencilState( 0 ), prevStencilRef( 0 ), prevRasterizerState( 0 ), prevVertexStride( 0 ),
          prevVertexOffset( 0 ), prevTopology( D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED ), numViewports( 1 )
    {
        prevBlendFactor[3] = prevBlendFactor[2] = prevBlendFactor[1] = prevBlendFactor[0] = 0.0f;
    }

    /*---------------------------------------------------------------------------------
        PreviousState_c::~PreviousState_c
        Destructor
    ---------------------------------------------------------------------------------*/
    PreviousState_c::~PreviousState_c( )
    {
        Release( );
    }

    /*---------------------------------------------------------------------------------
        PreviousState_c::Capture
        Captures the current state of the specified device
    ---------------------------------------------------------------------------------*/
    void PreviousState_c::Capture( ID3D11DeviceContext * deviceContext )
    {
        deviceContext->GSGetShader( &prevGeometryShader, NULL, 0 );
        deviceContext->VSGetShader( &prevVertexShader, NULL, 0 );
        deviceContext->PSGetShader( &prevPixelShader, NULL, 0 );
        deviceContext->PSGetShaderResources( 0, 1, &prevTextureView );
        deviceContext->PSGetSamplers( 0, 1, &prevSampler );
        deviceContext->IAGetInputLayout( &prevInputLayout );
        deviceContext->IAGetVertexBuffers( 0, 1, &prevVertexBuffer, &prevVertexStride, &prevVertexOffset );
        deviceContext->IAGetPrimitiveTopology( &prevTopology );
        deviceContext->OMGetBlendState( &prevBlendState, prevBlendFactor, &prevSampleMask );
        deviceContext->OMGetDepthStencilState( &prevDepthStencilState, &prevStencilRef );
        deviceContext->RSGetState( &prevRasterizerState );
    }

    /*---------------------------------------------------------------------------------
        PreviousState_c::Restore
        Restores the previously captured state of the specified device
    ---------------------------------------------------------------------------------*/
    void PreviousState_c::Restore( ID3D11DeviceContext * deviceContext )
    {
        deviceContext->GSSetShader( prevGeometryShader, NULL, 0 );
        deviceContext->VSSetShader( prevVertexShader, NULL, 0 );
        deviceContext->PSSetShader( prevPixelShader, NULL, 0 );
        deviceContext->PSSetShaderResources( 0, 1, &prevTextureView );
        deviceContext->PSSetSamplers( 0, 1, &prevSampler );
        deviceContext->IASetInputLayout( prevInputLayout );
        deviceContext->IASetVertexBuffers( 0, 1, &prevVertexBuffer, &prevVertexStride, &prevVertexOffset );
        deviceContext->IASetPrimitiveTopology( prevTopology );
        deviceContext->OMSetBlendState( prevBlendState, prevBlendFactor, prevSampleMask );
        deviceContext->OMSetDepthStencilState( prevDepthStencilState, prevStencilRef );
        deviceContext->RSSetState( prevRasterizerState );

        Release( );
    }

    /*---------------------------------------------------------------------------------
        PreviousState_c::Release
        Releases any interfaces that have been obtained
    ---------------------------------------------------------------------------------*/
    void PreviousState_c::Release( )
    {
        if ( prevVertexShader )
        {
            prevVertexShader->Release( );
            prevVertexShader = 0;
        }

        if ( prevPixelShader )
        {
            prevPixelShader->Release( );
            prevPixelShader = 0;
        }

        if ( prevTextureView )
        {
            prevTextureView->Release( );
            prevTextureView = 0;
        }

        if ( prevSampler )
        {
            prevSampler->Release( );
            prevSampler = 0;
        }
        
        if ( prevInputLayout )
        {
            prevInputLayout->Release( );
            prevInputLayout = 0;
        }
        
        if ( prevVertexBuffer )
        {
            prevVertexBuffer->Release( );
            prevVertexBuffer = 0;
        }

        if ( prevGeometryShader )
        {
            prevGeometryShader->Release( );
            prevGeometryShader = 0;
        }
        
        if ( prevBlendState )
        {
            prevBlendState->Release( );
            prevBlendState = 0;
        }
        
        if ( prevDepthStencilState )
        {
            prevDepthStencilState->Release( );
            prevDepthStencilState = 0;
        }

        if ( prevRasterizerState )
        {
            prevRasterizerState->Release( );
            prevRasterizerState = 0;
        }
    }

    /*---------------------------------------------------------------------------------
        CreateTextureView
        Creates a shader resource view of the font texture
    ---------------------------------------------------------------------------------*/
    ID3D11ShaderResourceView * CreateTextureView( ID3D11Device * device )
    {
        ID3D11Resource * texture = 0;
        
        if ( FAILED ( D3DX11CreateTextureFromMemory( device, TextTexture, TextTextureByteCount, 0, 0, &texture, 0 ) ) || !texture )
        {
            return 0;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC textureDesc;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        textureDesc.Texture2D.MipLevels = 1;
        textureDesc.Texture2D.MostDetailedMip = 0;

        ID3D11ShaderResourceView * textureView = 0;    
     
        HRESULT hr = device->CreateShaderResourceView(texture, &textureDesc, &textureView );
        texture->Release( );
        
        if ( FAILED ( hr ) )
        {
            return 0;
        }

        return textureView;
    }

    /*---------------------------------------------------------------------------------
        CompileShader
        Compiles a specified function of the font shader for the specified shader model
    ---------------------------------------------------------------------------------*/
    ID3D10Blob * CompileShader( const char * function, const char * target )
    {
        // Compile the shader
        ID3D10Blob *bytecode = 0, *errors = 0;
        HRESULT hr = D3DX11CompileFromMemory(Shaders, sizeof( Shaders ), 0, 0, 0, function, target, 0, 0, 0, &bytecode, &errors, 0 );

        if ( errors != 0 )
        {
            OutputDebugStringA( ( char * ) errors->GetBufferPointer( ) );
            errors->Release( );
        }

        if ( FAILED ( hr ) )
        {
            return 0;
        }

        return bytecode;
    }

    /*---------------------------------------------------------------------------------
        CreateVertexShader
        Creates a vertex shader which will be used to render the font characters
    ---------------------------------------------------------------------------------*/
    ID3D11VertexShader * CreateVertexShader( ID3D10Blob * bytecode, ID3D11Device * device )
    {
        // Create the shader
        ID3D11VertexShader * shader = 0;
        HRESULT hr = device->CreateVertexShader(bytecode->GetBufferPointer( ), bytecode->GetBufferSize( ), NULL, &shader );

        if ( FAILED ( hr ) )
        {
            return 0;
        }
        
        return shader;
    }

    /*---------------------------------------------------------------------------------
        CreatePixelShader
        Creates a pixel shader that will be used to render the font characters
    ---------------------------------------------------------------------------------*/
    ID3D11PixelShader * CreatePixelShader( ID3D11Device * device )
    {
        // Compile the shader
        ID3D10Blob * bytecode = CompileShader("PSMain", "ps_4_0");
        if ( !bytecode )
        {
            return 0;
        }

        // Create the shader
        ID3D11PixelShader * shader = 0;
        HRESULT hr = device->CreatePixelShader(bytecode->GetBufferPointer( ), bytecode->GetBufferSize( ), NULL, &shader );

        bytecode->Release( );

        if ( FAILED ( hr ) )
        {
            return 0;
        }
        
        return shader;
    }

    /*---------------------------------------------------------------------------------
        CreateInputLayout
        Creates an input layout object which will be used to describe the character quad
        vertices
    ---------------------------------------------------------------------------------*/
    ID3D11InputLayout * CreateInputLayout( ID3D10Blob * byteCode, ID3D11Device * device )
    {
        D3D11_INPUT_ELEMENT_DESC desc[] =
        {
            { "POSITIONT", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R16G16_UINT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        ID3D11InputLayout * inputLayout = 0;

        HRESULT hr = device->CreateInputLayout( desc, 3, byteCode->GetBufferPointer( ), byteCode->GetBufferSize( ), &inputLayout );
        if ( FAILED ( hr ) )
        {
            return 0;
        }
        
        return inputLayout;
    }

    /*---------------------------------------------------------------------------------
        CreateVertexBuffer
        Creates a dynamic vertex buffer which will be filled with font characters on a
        per-frame basis
    ---------------------------------------------------------------------------------*/
    ID3D11Buffer * CreateVertexBuffer( ID3D11Device * device, size_t characterCapacity )
    {
        // Compute vertex buffer size
        size_t bufferSize = characterCapacity * ( NumVertexElementsPerCharacter * sizeof( DWORD ) );

        // Create vertex buffer
        D3D11_BUFFER_DESC desc;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.ByteWidth = bufferSize;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        desc.Usage = D3D11_USAGE_DYNAMIC;
            
        ID3D11Buffer * buffer = 0;
        
        if ( FAILED ( device->CreateBuffer( &desc, 0, &buffer ) ) )
        {
            return 0;
        }
        
        return buffer;
    }

    /*---------------------------------------------------------------------------------
        CreateSamplerState
        Creates a sampler state which will be used by the pixel shader when sampling
        the font texture
    ---------------------------------------------------------------------------------*/
    ID3D11SamplerState * CreateSamplerState( ID3D11Device * device )
    {
        D3D11_SAMPLER_DESC desc;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.BorderColor[0] = 0.0f;
        desc.BorderColor[1] = 0.0f;
        desc.BorderColor[2] = 0.0f;
        desc.BorderColor[3] = 0.0f;
        desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        desc.MaxAnisotropy = 0;
        desc.MinLOD = 0.0f;
        desc.MaxLOD = 0.0f;
        desc.MipLODBias = 0.0f;

        ID3D11SamplerState * samplerState;
        if ( FAILED ( device->CreateSamplerState( &desc, &samplerState ) ) )
        {
            return 0;
        }

        return samplerState;
    }

    /*---------------------------------------------------------------------------------
        CreateDepthStencilState
        Creates the depth stencil state that will be used for rendering the font characters
    ---------------------------------------------------------------------------------*/
    ID3D11DepthStencilState * CreateDepthStencilState( ID3D11Device * device )
    {
        D3D11_DEPTH_STENCIL_DESC desc;
        desc.DepthEnable = FALSE;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
        desc.StencilEnable = FALSE;
        desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
        desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
        desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        
        ID3D11DepthStencilState * depthStencilState;
        if ( FAILED ( device->CreateDepthStencilState( &desc, &depthStencilState ) ) )
        {
            return 0;
        }

        return depthStencilState;
    }

    /*---------------------------------------------------------------------------------
        EncodePositionCoord
        Encodes the specified position coordinate into a dword that will be inserted
        into the vertex stream
    ---------------------------------------------------------------------------------*/
    DWORD EncodePositionCoord( float pos )
    {
        return *( ( DWORD * ) &pos );
    }

    /*---------------------------------------------------------------------------------
        EncodeUVCoords
        Encodes the specified u and v coordinate into a dword that will be inserted
        into the vertex stream
    ---------------------------------------------------------------------------------*/
    DWORD EncodeUVCoords( int u, int v )
    {
        DWORD result = u & 0x0000FFFF;
        result |= ( v << 16 ) & 0xFFFF0000;
        return result;
    }
//}

//namespace
//{
    /*---------------------------------------------------------------------------------
        TinyTextContext_c::Initialise
        Initialises this object
    ---------------------------------------------------------------------------------*/
    bool TinyTextContext_c::Initialise( ID3D11Device * device, ID3D11DeviceContext * deviceContext, size_t characterCapacity )
    {
        // Validate arguments
        if ( !device )
        {
            return false;
        }

        // Compile the shader
        ID3D10Blob * vertexShaderByteCode = CompileShader("VSMain", "vs_4_0");
        if ( !vertexShaderByteCode )
        {
            return false;
        }

        // Create vertex shader + input layout
        ID3D11VertexShader * vertexShader = CreateVertexShader( vertexShaderByteCode, device );
        ID3D11InputLayout * inputLayout = CreateInputLayout( vertexShaderByteCode, device );
        vertexShaderByteCode->Release( );

        if ( !vertexShader )
        {
            return false;
        }

        if ( !inputLayout )
        {
            vertexShader->Release( );
            return false;
        }

        // Create a view of the font texture
        ID3D11ShaderResourceView * textureView = CreateTextureView( device );

        if ( !textureView )
        {
            vertexShader->Release( );
            inputLayout->Release( );
            return false;
        }

        // Create pixel shader
        ID3D11PixelShader * pixelShader = CreatePixelShader( device );
        if ( !pixelShader )
        {
            vertexShader->Release( );
            inputLayout->Release( );
            textureView->Release( );
            return false;
        }

        // Create vertex buffer
        ID3D11Buffer * vertexBuffer = CreateVertexBuffer( device, characterCapacity );
        if ( !vertexBuffer )
        {
            vertexShader->Release( );
            inputLayout->Release( );
            textureView->Release( );
            pixelShader->Release( );
            return false;
        }

        // Create the sampler state
        ID3D11SamplerState * samplerState = CreateSamplerState( device );
        if ( !samplerState )
        {
            vertexShader->Release( );
            inputLayout->Release( );
            textureView->Release( );
            pixelShader->Release( );
            vertexBuffer->Release( );
            return false;
        }

        // Create depth-stencil state
        ID3D11DepthStencilState * depthStencilState = CreateDepthStencilState( device );
        if ( !depthStencilState )
        {
            vertexShader->Release( );
            inputLayout->Release( );
            textureView->Release( );
            pixelShader->Release( );
            vertexBuffer->Release( );
            samplerState->Release( );
            return false;
        }

        // Success - set object state and return success code
        m_Device = device;
        m_DeviceContext = deviceContext;
        m_VertexShader = vertexShader;
        m_InputLayout = inputLayout;
        m_TextureView = textureView;
        m_PixelShader = pixelShader;
        m_VertexBuffer = vertexBuffer;
        m_SamplerState = samplerState;
        m_DepthStencilState = depthStencilState;
    
        return true;
    }

    /*---------------------------------------------------------------------------------
        TinyTextContext_c::TinyTextContext_c
        Constructor - takes an optional pointer to a boolean that will receive the
        result of this operation
    ---------------------------------------------------------------------------------*/
    TinyTextContext_c::TinyTextContext_c( ID3D11Device * device, ID3D11DeviceContext * deviceContext, size_t characterCapacity, bool * resultPtr )
    :   m_Device( 0 ),
        m_DeviceContext( 0 ),
        m_TextureView( 0 ),
        m_VertexShader( 0 ),
        m_PixelShader( 0 ),
        m_InputLayout( 0 ),
        m_VertexBuffer( 0 ),
        m_SamplerState( 0 ),
        m_DepthStencilState( 0 ),
        m_NumVertices( 0 ),
        m_Capacity( characterCapacity ),
        m_VertexBufferWriteAddress( 0 )
    {
        bool result = Initialise( device, deviceContext, m_Capacity );
        if ( resultPtr )
        {
            *resultPtr = result;
        }
    }

    /*---------------------------------------------------------------------------------
        TinyTextContext_c::~TinyTextContext_c
        Private destructor - call 'TinyTextContext_c::Destroy' to destroy a text context
    ---------------------------------------------------------------------------------*/
    TinyTextContext_c::~TinyTextContext_c( )
    {
        UnmapVertexBuffer( );

        if ( m_TextureView )
        {
            m_TextureView->Release( );
        }

        if ( m_VertexShader )
        {
            m_VertexShader->Release( );
        }

        if ( m_PixelShader )
        {
            m_PixelShader->Release( );
        }

        if ( m_InputLayout )
        {
            m_InputLayout->Release( );
        }

        if ( m_VertexBuffer )
        {
            m_VertexBuffer->Release( );
        }

        if ( m_SamplerState )
        {
            m_SamplerState->Release( );
        }

        if ( m_DepthStencilState )
        {
            m_DepthStencilState->Release( );
        }
    }

    /*---------------------------------------------------------------------------------
        TinyTextContext_c::Print
        Print some text to the context. Colour is of form 0xAABBGGRR
    ---------------------------------------------------------------------------------*/
    bool TinyTextContext_c::Print( const D3D11_VIEWPORT & viewport, const char * text, int x, int y, DWORD colour )
    {
        return Print( viewport, 0xffffffff, text, x, y, colour );
    }

    /*---------------------------------------------------------------------------------
        TinyTextContext_c::Print
        Print some text to the context. Colour is of form 0xAABBGGRR
    ---------------------------------------------------------------------------------*/
    bool TinyTextContext_c::Print( const D3D11_VIEWPORT & viewport, size_t maxCharacterCount, const char * text, int x, int y, DWORD colour )
    {
        // If we haven't yet mapped the vertex buffer to CPU memory, then map it now
        if ( !MapVertexBuffer( ) )
        {
            return false;
        }

        // Add characters to the vertex buffer
        char currentChar;

        while ( ( currentChar = *( text++ ) ) != 0 && maxCharacterCount-- )
        {
            // If we have reached capacity already, return false immediately
            if ( ( m_NumVertices / NumVerticesPerCharacter ) == m_Capacity )
            {
                return false;
            }

            // Extract character data
            int u = CharacterData[ ( currentChar * CharacterByteCount ) + 0 ];
            int v = CharacterData[ ( currentChar * CharacterByteCount ) + 1 ];
            int height = CharacterData[ ( currentChar * CharacterByteCount ) + 2 ] & 0x0F;
            int yoffset = ( CharacterData[ ( currentChar * CharacterByteCount ) + 2 ] ) >> 4;
            int charY = y + yoffset;
        
            // Compute bottom-left and top-right vertices of the character
            float bottomLeftX = ( ( 2 * x ) / float( viewport.Width ) ) - 1.0f;
            float bottomLeftY = ( ( -2 * ( charY + height ) ) / float( viewport.Height ) ) + 1.0f;
            int bottomLeftU = u;
            int bottomLeftV = v + height;

            float topRightX = ( ( 2 * ( x + CharacterWidth ) ) / float( viewport.Width ) ) - 1.0f;
            float topRightY = ( ( -2 * ( charY ) ) / float( viewport.Height ) ) + 1.0f;
            int topRightU = u + CharacterWidth;
            int topRightV = v;
        
            // Add triangle vertices for this character to the vertex buffer
            m_VertexBufferWriteAddress[ Triangle0_Vertex0_Position_X ] = EncodePositionCoord(bottomLeftX);
            m_VertexBufferWriteAddress[ Triangle0_Vertex0_Position_Y ] = EncodePositionCoord(bottomLeftY);
            m_VertexBufferWriteAddress[ Triangle0_Vertex0_UV ] = EncodeUVCoords(bottomLeftU, bottomLeftV);
            m_VertexBufferWriteAddress[ Triangle0_Vertex0_Colour ] = colour;

            m_VertexBufferWriteAddress[ Triangle0_Vertex1_Position_X ] = EncodePositionCoord(bottomLeftX);
            m_VertexBufferWriteAddress[ Triangle0_Vertex1_Position_Y ] = EncodePositionCoord(topRightY);
            m_VertexBufferWriteAddress[ Triangle0_Vertex1_UV ] = EncodeUVCoords(bottomLeftU, topRightV);
            m_VertexBufferWriteAddress[ Triangle0_Vertex1_Colour ] = colour;

            m_VertexBufferWriteAddress[ Triangle0_Vertex2_Position_X ] = EncodePositionCoord(topRightX);
            m_VertexBufferWriteAddress[ Triangle0_Vertex2_Position_Y ] = EncodePositionCoord(bottomLeftY);
            m_VertexBufferWriteAddress[ Triangle0_Vertex2_UV ] = EncodeUVCoords(topRightU, bottomLeftV);
            m_VertexBufferWriteAddress[ Triangle0_Vertex2_Colour ] = colour;

            m_VertexBufferWriteAddress[ Triangle1_Vertex0_Position_X ] = EncodePositionCoord(topRightX);
            m_VertexBufferWriteAddress[ Triangle1_Vertex0_Position_Y ] = EncodePositionCoord(topRightY);
            m_VertexBufferWriteAddress[ Triangle1_Vertex0_UV ] = EncodeUVCoords(topRightU, topRightV);
            m_VertexBufferWriteAddress[ Triangle1_Vertex0_Colour ] = colour;

            m_VertexBufferWriteAddress[ Triangle1_Vertex1_Position_X ] = EncodePositionCoord(topRightX);
            m_VertexBufferWriteAddress[ Triangle1_Vertex1_Position_Y ] = EncodePositionCoord(bottomLeftY);
            m_VertexBufferWriteAddress[ Triangle1_Vertex1_UV ] = EncodeUVCoords(topRightU, bottomLeftV);
            m_VertexBufferWriteAddress[ Triangle1_Vertex1_Colour ] = colour;

            m_VertexBufferWriteAddress[ Triangle1_Vertex2_Position_X ] = EncodePositionCoord(bottomLeftX);
            m_VertexBufferWriteAddress[ Triangle1_Vertex2_Position_Y ] = EncodePositionCoord(topRightY);
            m_VertexBufferWriteAddress[ Triangle1_Vertex2_UV ] = EncodeUVCoords(bottomLeftU, topRightV);
            m_VertexBufferWriteAddress[ Triangle1_Vertex2_Colour ] = colour;
        
            // Update vertex buffer write position
            m_VertexBufferWriteAddress += NumVertexElementsPerCharacter;

            // Update vertex count
            m_NumVertices += NumVerticesPerCharacter;

            // Update current x position
            x += CharacterWidth;
        }

        return true;
    }

    /*---------------------------------------------------------------------------------
        TinyTextContext_c::Render
        Render the context onto the screen
    ---------------------------------------------------------------------------------*/
    bool TinyTextContext_c::Render( bool maintainState )
    {
        // If we haven't got a device, then we cannot continue
        if ( !m_Device ) return false;

        // If we haven't got a render-target, then we cannot continue
        ID3D11RenderTargetView * rtv = 0;
        m_DeviceContext->OMGetRenderTargets( 1, &rtv, 0 );
        if ( rtv == 0 ) return false;
        rtv->Release( ); rtv = 0;

        // Ensure the vertex buffer isn't mapped
        UnmapVertexBuffer( );
   
        // Save previous device state
        PreviousState_c state;
        if ( maintainState )
        {
            state.Capture( m_DeviceContext );
        }
        
        // Setup render state
        UINT vertexStride = ( NumVertexElementsPerCharacter / NumVerticesPerCharacter ) * sizeof( DWORD );
        UINT vertexOffset = 0;

        m_DeviceContext->VSSetShader( m_VertexShader, NULL, 0 );
        m_DeviceContext->GSSetShader( 0, NULL, 0 );
        m_DeviceContext->PSSetShader( m_PixelShader, NULL, 0 );
        m_DeviceContext->PSSetShaderResources( 0, 1, &m_TextureView );
        m_DeviceContext->PSSetSamplers( 0, 1, &m_SamplerState );
        m_DeviceContext->IASetInputLayout( m_InputLayout );
        m_DeviceContext->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &vertexStride, &vertexOffset );
        m_DeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
        m_DeviceContext->OMSetDepthStencilState( m_DepthStencilState, 0 );
        m_DeviceContext->OMSetBlendState( 0, 0, 0xffffffff );
        m_DeviceContext->RSSetState( 0 );
        
        // Render the font
        m_DeviceContext->Draw(m_NumVertices, 0);

        // Restore previous render state
        if ( maintainState )
        {
            state.Restore( m_DeviceContext );
        }

        return true;
    }

    /*---------------------------------------------------------------------------------
        TinyTextContext_c::MapVertexBuffer
        Maps the vertex buffer to CPU memory (if it isn't already mapped)
    ---------------------------------------------------------------------------------*/
    bool TinyTextContext_c::MapVertexBuffer( )
    {
        // If we haven't got a device, then we cannot continue
        if ( !m_DeviceContext ) return false;

        // If we haven't got a vertex buffer, then we cannot continue
        if ( !m_VertexBuffer ) return false;
    
        if ( m_VertexBufferWriteAddress == 0 )
        {
            m_NumVertices = 0;

            //m_VertexBuffer->Map( D3D11_MAP_WRITE_DISCARD, 0, ( void ** ) &m_VertexBufferWriteAddress );

            D3D11_MAPPED_SUBRESOURCE mappedSubresource;
            ZeroMemory( &mappedSubresource, sizeof( D3D11_MAPPED_SUBRESOURCE ) );

            HRESULT hr = m_DeviceContext->Map( m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource );
            m_VertexBufferWriteAddress = (DWORD*)mappedSubresource.pData;

            if ( FAILED ( hr ) )
            {
                return false;
            }
        }

        return true;
    }

    /*---------------------------------------------------------------------------------
        TinyTextContext_c::UnmapVertexBuffer
        Unmaps the vertex buffer to CPU memory (if it isn't already unmapped)
    ---------------------------------------------------------------------------------*/
    void TinyTextContext_c::UnmapVertexBuffer( )
    {
        if ( m_VertexBufferWriteAddress != 0 )
        {
            m_VertexBufferWriteAddress = 0;

            if ( m_VertexBuffer )
            {
                // m_VertexBuffer->Unmap( );
                m_DeviceContext->Unmap( m_VertexBuffer, 0 );
            }
        }
    }
//}