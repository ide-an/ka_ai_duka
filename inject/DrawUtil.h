#pragma once
#include <Windows.h>
//#include <d3d9.h>
namespace ka_ai_duka{
    namespace draw{
        
        // Primitives supported by draw-primitive API
        typedef enum _D3DPRIMITIVETYPE {
            D3DPT_POINTLIST             = 1,
            D3DPT_LINELIST              = 2,
            D3DPT_LINESTRIP             = 3,
            D3DPT_TRIANGLELIST          = 4,
            D3DPT_TRIANGLESTRIP         = 5,
            D3DPT_TRIANGLEFAN           = 6,
            D3DPT_FORCE_DWORD           = 0x7fffffff, /* force 32-bit size enum */
        } D3DPRIMITIVETYPE;
        const DWORD D3DFVF_RESERVED0        = 0x001;
        const DWORD D3DFVF_POSITION_MASK    = 0x400E;
        const DWORD D3DFVF_XYZ              = 0x002;
        const DWORD D3DFVF_XYZRHW           = 0x004;
        const DWORD D3DFVF_XYZB1            = 0x006;
        const DWORD D3DFVF_XYZB2            = 0x008;
        const DWORD D3DFVF_XYZB3            = 0x00a;
        const DWORD D3DFVF_XYZB4            = 0x00c;
        const DWORD D3DFVF_XYZB5            = 0x00e;
        const DWORD D3DFVF_XYZW             = 0x4002;
        const DWORD D3DFVF_NORMAL           = 0x010;
        const DWORD D3DFVF_PSIZE            = 0x020;
        const DWORD D3DFVF_DIFFUSE          = 0x040;
        const DWORD D3DFVF_SPECULAR         = 0x080;
        const DWORD D3DFVF_TEXCOUNT_MASK    = 0xf00;
        const DWORD D3DFVF_TEXCOUNT_SHIFT   = 8;
        const DWORD D3DFVF_TEX0             = 0x000;
        const DWORD D3DFVF_TEX1             = 0x100;
        const DWORD D3DFVF_TEX2             = 0x200;
        const DWORD D3DFVF_TEX3             = 0x300;
        const DWORD D3DFVF_TEX4             = 0x400;
        const DWORD D3DFVF_TEX5             = 0x500;
        const DWORD D3DFVF_TEX6             = 0x600;
        const DWORD D3DFVF_TEX7             = 0x700;
        const DWORD D3DFVF_TEX8             = 0x800;
        const DWORD FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
        struct FVFVertex{
            float x,y,z;
            float rhw;
            DWORD diffuse;
        };
        class DrawUtil{
        //private:
        public:
            int** d3d_device;
            HRESULT(__stdcall *begin_scene)(int**);
            HRESULT(__stdcall *end_scene)(int**);
            HRESULT(__stdcall *draw_primitive_up)(int**, DWORD, DWORD, const void *, DWORD);
            HRESULT(__stdcall *set_vertex_shader)(int**, DWORD);
            HRESULT(__stdcall *get_vertex_shader)(int**, DWORD *);
        public:
            DrawUtil(int** d3d_device) : d3d_device(d3d_device)
            {
                begin_scene       = reinterpret_cast<HRESULT(__stdcall *)(int**)>(*(*d3d_device+34));
                end_scene         = reinterpret_cast<HRESULT(__stdcall *)(int**)>(*(*d3d_device+35));
                draw_primitive_up = reinterpret_cast<HRESULT(__stdcall *)(int**, DWORD, DWORD, const void *, DWORD)>(*(*d3d_device+72));
                set_vertex_shader = reinterpret_cast<HRESULT(__stdcall *)(int**, DWORD)>(*(*d3d_device+76));
                get_vertex_shader = reinterpret_cast<HRESULT(__stdcall *)(int**, DWORD *)>(*(*d3d_device+77));
            }
            void BeginScene(void)
            {
                begin_scene(d3d_device);
            }
            void EndScene(void)
            {
                end_scene(d3d_device);
            }
            void DrawRect(float x1, float y1, float x2, float y2, DWORD color)
            {
                DWORD prev_shader;
                get_vertex_shader(d3d_device, &prev_shader);
                set_vertex_shader(d3d_device, FVF);
                struct FVFVertex vertices[4];
                ::ZeroMemory(vertices, sizeof(vertices));
                for(int i=0;i<4;i++){
                    vertices[i].x = (i==0 || i==3) ? x1 : x2;
                    vertices[i].y = i<3 ? y1 : y2;
                    vertices[i].z = 0.0f;
                    vertices[i].rhw = 1.0f;
                    vertices[i].diffuse = color;
                }
                draw_primitive_up(d3d_device, D3DPT_TRIANGLEFAN, 1, vertices, sizeof(struct FVFVertex));
                set_vertex_shader(d3d_device, prev_shader);
            }
            static DWORD D3DColorARGB(char a, char r, char g, char b)
            {
                return ((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)));
            }
        };
    }
}
