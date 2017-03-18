/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
#include "d3d_render.h"
//#include "dxerr9.h"
//#pragma comment(lib, "DxErr9.lib")
#include "vvideodraw.hpp"

struct_fmt_table fmt_table[] = {
	{FOURCC_PIX_FMT_YVU420,   (D3DFORMAT)MAKEFOURCC('Y','V','1','2')},
	{FOURCC_PIX_FMT_YUV420P,  (D3DFORMAT)MAKEFOURCC('I','4','2','0')},
	{FOURCC_PIX_FMT_IYUV,     (D3DFORMAT)MAKEFOURCC('I','Y','U','V')},
	{FOURCC_PIX_FMT_YVU410,   (D3DFORMAT)MAKEFOURCC('Y','V','U','9')},
	{FOURCC_PIX_FMT_YUY2,     D3DFMT_YUY2},
	{FOURCC_PIX_FMT_UYVY,     D3DFMT_UYVY},
	{FOURCC_PIX_FMT_BGR32,    D3DFMT_X8R8G8B8},
	{FOURCC_PIX_FMT_RGB32,    D3DFMT_X8B8G8R8},
	{FOURCC_PIX_FMT_BGR24,    D3DFMT_R8G8B8}, /* untested	*/
	{FOURCC_PIX_FMT_BGR16,    D3DFMT_R5G6B5},
	{FOURCC_PIX_FMT_BGR15,    D3DFMT_X1R5G5B5},
	{FOURCC_PIX_FMT_BGR8,     D3DFMT_R3G3B2}, /* untested	*/
};

d3d_render::d3d_render(BOOL bHWAccel)
	: m_d3d_handle(NULL)
	, m_d3d_device(NULL)
	, m_d3d_surface(NULL)
	, m_d3d_backbuf(NULL)
	, m_desktop_fmt(D3DFMT_UNKNOWN)
	, m_hwnd(NULL)
	, m_image_width(0)
	, m_image_height(0)
	, m_cur_backbuf_width(0)
	, m_cur_backbuf_height(0)
	, m_window_aspect(0.0f)
	, m_keep_aspect(false)
	, m_last_error(false)
	, m_bHWAccel(bHWAccel)
{
	ZeroMemory(&m_locked_rect, sizeof(m_locked_rect));
	ZeroMemory(&m_present_params, sizeof(m_present_params));

	CopyInitCache(&m_pCache, 8192);
}

d3d_render::~d3d_render()
{
	destory_render();
}

bool d3d_render::init_render(void* ctx, int w, int h, int pix_fmt)
{
	m_Lock.lock();
	m_hwnd = (HWND)ctx;
	m_image_width = w;
	m_image_height = h;

	m_window_aspect = (float)w / (float)h;

	m_movie_src_fmt = (D3DFORMAT)MAKEFOURCC('Y', 'V', '1', '2');

	m_d3d_handle = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_d3d_handle) {
		printf("Initializing Direct3D failed.\n");
		m_Lock.unlock();
		return false;
	}

	HRESULT hr;
	D3DDISPLAYMODE disp_mode;

	hr = m_d3d_handle->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &disp_mode);
	if (FAILED(hr))
	{
		printf("Reading display mode failed.\n");
		destory_render();
		m_Lock.unlock();
		return false;
	}

	/* Store in m_desktop_fmt the user desktop's colorspace. Usually XRGB. */
	m_desktop_fmt = disp_mode.Format;
	m_cur_backbuf_width = disp_mode.Width;
	m_cur_backbuf_height = disp_mode.Height;

	//printf("Setting backbuffer dimensions to (%dx%d).\n",
	//	disp_mode.Width, disp_mode.Height);

	D3DCAPS9 disp_caps;
	m_BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	hr = m_d3d_handle->GetDeviceCaps(D3DADAPTER_DEFAULT, //D3DADAPTER_DEFAULT
		D3DDEVTYPE_HAL,
		&disp_caps);
	if (FAILED(hr))
	{
		printf("Reading display capabilities failed.\n");
		destory_render();
		m_Lock.unlock();
		return false;
	}
	if (disp_caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		m_BehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE;
	}
	else
	{
		m_BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE;
	}
	/* Store relevant information reguarding caps of device */
	DWORD texture_caps;
	DWORD dev_caps;

	texture_caps                  = disp_caps.TextureCaps;
	dev_caps                      = disp_caps.DevCaps;

	if (!configure_d3d())
	{
		destory_render();
		m_Lock.unlock();
		return false;
	}

	InvalidateRect(m_hwnd, NULL, TRUE);
	m_Lock.unlock();

	return true;
}

bool d3d_render::render_one_frame(RenderFrame* data, int pix_fmt)
{
	if (m_bHWAccel == true && data->hwCmd == VIDEO_HW_NONE)
	{
		return true;
	}
	m_Lock.lock();

	if (m_bHWAccel == true && data->hwCmd == VIDEO_HW_GET_D3D_DEV)
	{
		data->hwD3DDev = m_d3d_device;
		m_Lock.unlock();
		return true;
	}

	if (m_image_width != data->width 
		|| m_image_height != data->height
		|| m_last_error == true)
	{
		m_image_width = data->width;
		m_image_height = data->height;
		if (data->hwCmd == VIDEO_HW_NONE)
		{
			destroy_d3d_surfaces();
			create_d3d_surfaces();
		}
	}
	m_last_error = false;
	HRESULT hr;
	char* src_yuv[3] = { data->data[0], 
		data->data[2], 
		data->data[1] };

	int src_linesize[3] = { data->linesize[0], 
		data->linesize[2], 
		data->linesize[1] };

	/* If the D3D device is uncooperative (not initialized), return success.
		The device will be probed for reinitialization in the next flip_page() */
	if (!m_d3d_device)
	{
		m_Lock.unlock();
		return false;
	}

	/*	Get the video region.	*/
	RECT rect_client = { 0 };
	RECT rect_win = { 0 };
	int width, height;

	GetClientRect(m_hwnd, &rect_client);
	GetClientRect(m_hwnd, &rect_win);

	int win_width = width = rect_client.right - rect_client.left;
	int win_height = height = rect_client.bottom - rect_client.top;
	if (m_keep_aspect)
	{
		int tmpheight = ((float)width / m_window_aspect);
		tmpheight += tmpheight % 2;
		if(tmpheight > height)
		{
			width = ((float)height * m_window_aspect);
			width += width % 2;
		}
		else 
		{
			height = tmpheight;
		}

		/* algin to center.	*/
		rect_client.left += ((win_width - width) / 2);
		rect_client.top += ((win_height - height) / 2);
		rect_client.bottom -= ((win_height - height) / 2);
		rect_client.right -= ((win_width - width) / 2);
	}

	if (data->hwCmd == VIDEO_HW_NONE)
	{
		if ((rect_win.right - rect_win.left) != 
			(m_last_client_rect.right - m_last_client_rect.left) ||
			(rect_win.bottom - rect_win.top) != 
			(m_last_client_rect.bottom - m_last_client_rect.top))
		{
			m_last_client_rect = rect_win;
			destroy_d3d_surfaces();
			m_present_params.BackBufferWidth = m_last_client_rect.right - m_last_client_rect.left;
			m_present_params.BackBufferHeight = m_last_client_rect.bottom - m_last_client_rect.top;
			fill_d3d_presentparams(&m_present_params);

			hr = m_d3d_device->Reset(&m_present_params);
			if (FAILED(hr))
			{
				printf("<vo_direct3d><INFO>Could not reset the D3D device\n");

				m_last_error = true;
				m_Lock.unlock();
				return 0;
			}

			if (!create_d3d_surfaces())
			{
				m_Lock.unlock();
				return 0;
			}

		}
	}
	
	if (data->hwCmd == VIDEO_HW_NONE)
	/* SW decode copy to the surface */
	{
		/* Lock the offscreen surface if it's not already locked. */
		if (!m_locked_rect.pBits && m_d3d_surface)
		{
			hr = m_d3d_surface->LockRect(&m_locked_rect, NULL, 0);
			if (FAILED(hr))
			{
				printf("Reading display capabilities failed.\n");
				m_Lock.unlock();
				return false;
			}
		}
		else
		{
			m_Lock.unlock();
			return false;
		}

		if (!m_locked_rect.pBits)
		{
			m_Lock.unlock();
			return false;
		}

		uint8_t*	dst_yuv[3] = { (uint8_t*)m_locked_rect.pBits,
			(uint8_t*)m_locked_rect.pBits + m_locked_rect.Pitch * m_image_height,
			(uint8_t*)m_locked_rect.pBits + m_locked_rect.Pitch * m_image_height + 
			((m_locked_rect.Pitch >> 1) * (m_image_height >> 1)) };

		int dst_linesize[3] = { m_locked_rect.Pitch, 
			m_locked_rect.Pitch >> 1, 
			m_locked_rect.Pitch >> 1 };
#ifdef DISABLE_COVER
		for (int i = 0; i < m_image_height; i++)
		{
			// copy Y
			memcpy(dst_yuv[0] + i * dst_linesize[0], src_yuv[0] + i * src_linesize[0], src_linesize[0]);
			// copy U
			if (i % 2 == 0)
			{
				memcpy(dst_yuv[1] + (i >> 1) * dst_linesize[1], src_yuv[1] + (i >> 1) * src_linesize[1], src_linesize[1]);
				memcpy(dst_yuv[2] + (i >> 1) * dst_linesize[2], src_yuv[2] + (i >> 1) * src_linesize[2], src_linesize[2]);
			}
		}
#else
	    for (unsigned n = 0; n < 3; n++) {
	        const unsigned d = n > 0 ? 2 : 1;
	        cover_SSE_CopyPlane(dst_yuv[n], dst_linesize[n],
	                      (unsigned char *)src_yuv[n], src_linesize[n],
	                      m_pCache.buffer, m_pCache.size,
	                      (m_image_width+d-1)/d, (m_image_height+d-1)/d, VLC_CPU_SSE_ALL);
	    }
#endif

		/* This unlock is used for both slice_draw path and render_d3d_frame path. */
		hr = m_d3d_surface->UnlockRect();
		if (FAILED(hr))
		{
			m_Lock.unlock();
			printf("Surface unlock failed.\n");
			return false;
		}
	}

	m_locked_rect.pBits = NULL;

	//LPDIRECT3DSURFACE9 d3d = (LPDIRECT3DSURFACE9)(uintptr_t)data->data[3];
	//m_d3d_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	/* Render the scene	*/
	hr = m_d3d_device->BeginScene();
	if (FAILED(hr))
	{
		printf("BeginScene failed.hr=0x%0lX\n", hr);
		m_Lock.unlock();
		return false;
	}
	
	/* HW decode just render the surface */
	if (data->hwCmd == VIDEO_HW_DATA)
	{
		//LPDIRECT3DDEVICE9 *pDevice = NULL;
		//hr = pSurface->GetDevice(&pDevice);
		//m_d3d_device->Clear(0, NULL,D3DCLEAR_TARGET, 0, 0, 0);
		if (m_d3d_backbuf)
		{
			m_d3d_backbuf->Release();
			m_d3d_backbuf = NULL;
		}
		//RECT m_rtViewport;
		//GetClientRect(m_hwnd, &m_rtViewport);
		m_d3d_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_d3d_backbuf);
		hr = m_d3d_device->StretchRect((LPDIRECT3DSURFACE9)(data->data[3]),
			NULL,
			m_d3d_backbuf,
			NULL,//&m_rtViewport,
			D3DTEXF_LINEAR);
	}else
	{
		hr = m_d3d_device->StretchRect(m_d3d_surface,
			NULL,
			m_d3d_backbuf,
			NULL,
			D3DTEXF_LINEAR);		
	}
	if (FAILED(hr))
	{
		printf("Copying frame to the backbuffer failed.hr=0x%0lX\n", hr);
		hr = m_d3d_device->EndScene();
		m_Lock.unlock();
		return false;
	}

	hr = m_d3d_device->EndScene();
	if (FAILED(hr))
	{
		printf("EndScene failed.\n");
		m_Lock.unlock();
		return false;
	}
#if 0
	if (m_keep_aspect)
		hr = m_d3d_device->Present(NULL, &rect_client, NULL, NULL);
	else
		hr = m_d3d_device->Present(NULL, NULL, NULL, NULL);
#else
	hr = m_d3d_device->Present(NULL, NULL, NULL, NULL);
#endif
	if (FAILED(hr))
	{
		printf("EndScene failed.\n");
		m_Lock.unlock();
		return false;
	}
	m_Lock.unlock();

	return true;
}

void d3d_render::re_size(int width, int height)
{
#if 0
	if (m_bHWAccel == true)
	{
		m_Lock.lock();
		m_present_params.BackBufferWidth = width;
		m_present_params.BackBufferHeight = height;
		m_d3d_device->Reset(&m_present_params);
		m_Lock.unlock();
	}
#endif
}

void d3d_render::aspect_ratio(int srcw, int srch, bool enable_aspect)
{
	if (enable_aspect)
	{
		enable_aspect = true;
		m_window_aspect = (float)srcw / (float)srch;
	}
	else
	{
		enable_aspect = false;
	}
}

void d3d_render::destory_render()
{
	destroy_d3d_surfaces();

	if (m_d3d_backbuf)
	{
		m_d3d_backbuf->Release();
		m_d3d_backbuf = NULL;
	}

	/* Destroy the D3D Device. */
	if (m_d3d_device)
	{
		m_d3d_device->Release();
		m_d3d_device = NULL;
	}

	/* Stop the whole D3D. */
	if (m_d3d_handle)
	{
		m_d3d_handle->Release();
		m_d3d_handle = NULL;
	}

	ZeroMemory(&m_locked_rect, sizeof(m_locked_rect));
}

void d3d_render::fill_d3d_presentparams(D3DPRESENT_PARAMETERS *present_params)
{
#if 0
	/* Prepare Direct3D initialization parameters. */
	memset(present_params, 0, sizeof(D3DPRESENT_PARAMETERS));
	present_params->Windowed               = TRUE;
	present_params->SwapEffect             = D3DSWAPEFFECT_COPY;
	present_params->Flags                  = D3DPRESENTFLAG_VIDEO;
	present_params->hDeviceWindow          = m_hwnd; /* w32_common var */
	//    present_params->BackBufferWidth        = 0; /* 防止闪烁, 赋值为0, 原因不详. */
	//    present_params->BackBufferHeight       = 0;
	present_params->MultiSampleType        = D3DMULTISAMPLE_NONE;
	present_params->PresentationInterval   = D3DPRESENT_INTERVAL_ONE;
	present_params->BackBufferFormat       = m_desktop_fmt;
	present_params->BackBufferCount        = 1;
	present_params->EnableAutoDepthStencil = FALSE;
#else
	memset(present_params, 0, sizeof(D3DPRESENT_PARAMETERS));
	present_params->Windowed = TRUE;
	present_params->BackBufferCount = 0;
	present_params->hDeviceWindow = m_hwnd;
	present_params->SwapEffect = D3DSWAPEFFECT_DISCARD;
	present_params->BackBufferFormat = m_desktop_fmt;
	present_params->EnableAutoDepthStencil = FALSE;
	present_params->Flags = D3DPRESENTFLAG_VIDEO;
	present_params->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	present_params->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	present_params->BackBufferWidth = 1920;
	present_params->BackBufferHeight = 1088;
#endif
}

bool d3d_render::configure_d3d()
{
	D3DDISPLAYMODE disp_mode;
	D3DVIEWPORT9 vp = { 0, 0, m_image_width, m_image_height, 0, 1 };
	HRESULT hr;

	destroy_d3d_surfaces();

	/* Get the current desktop display mode, so we can set up a back buffer
	 * of the same format. 
	 */
	hr = m_d3d_handle->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &disp_mode);
	if (FAILED(hr))
	{
		printf("Reading display mode failed.\n");
		destory_render();
		return false;
	}

	/* Write current Desktop's colorspace format in the global storage. */
	m_desktop_fmt = disp_mode.Format;

	if (m_image_width > m_cur_backbuf_width)
		m_cur_backbuf_width = m_image_width;

	if (m_image_height > m_cur_backbuf_height)
		m_cur_backbuf_height = m_image_height;

	fill_d3d_presentparams(&m_present_params);
	if (m_d3d_handle)
	{
		hr = m_d3d_handle->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL, m_hwnd,
			m_BehaviorFlags,
			&m_present_params, &m_d3d_device);
		if (FAILED(hr))
		{
			printf("Creating Direct3D device failed.\n");
			destory_render();
			return false;
		}
	}

	if (!create_d3d_surfaces())
		return false;
#if 1
	hr = m_d3d_device->SetViewport(&vp);
	if (FAILED(hr))
	{
		printf("Setting viewport failed.\n");
		return false;
	}
#endif

	return true;
}

bool d3d_render::create_d3d_surfaces()
{
	//printf("create_d3d_surfaces called.\n");
	if (!m_d3d_device)
		return false;

	HRESULT hr;

	if (!m_d3d_surface)
	{
		hr = m_d3d_device->CreateOffscreenPlainSurface(m_image_width, m_image_height,
			m_movie_src_fmt, D3DPOOL_DEFAULT, &m_d3d_surface, NULL);
		if (FAILED(hr))
		{
			printf("Allocating offscreen surface failed.\n");
			return false;
		}
	}

	if (!m_d3d_backbuf)
	{
		hr = m_d3d_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_d3d_backbuf);
		if (FAILED(hr))
		{
			printf("Allocating backbuffer failed.\n");
			return false;
		}
	}

	/* setup default renderstate */
	IDirect3DDevice9_SetRenderState(m_d3d_device, D3DRS_SRCBLEND, D3DBLEND_ONE);
	IDirect3DDevice9_SetRenderState(m_d3d_device, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	IDirect3DDevice9_SetRenderState(m_d3d_device, D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	IDirect3DDevice9_SetRenderState(m_d3d_device, D3DRS_ALPHAREF, (DWORD)0x0);
	IDirect3DDevice9_SetRenderState(m_d3d_device, D3DRS_LIGHTING, FALSE);
	IDirect3DDevice9_SetSamplerState(m_d3d_device, 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	IDirect3DDevice9_SetSamplerState(m_d3d_device, 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	return true;
}

void d3d_render::destroy_d3d_surfaces()
{
	//printf("destroy_d3d_surfaces called.\n");

	/* Let's destroy the old (if any) D3D Surfaces */
	if (m_locked_rect.pBits)
		m_d3d_surface->UnlockRect();
	m_locked_rect.pBits = NULL;

	if (m_d3d_surface)
		m_d3d_surface->Release();
	m_d3d_surface = NULL;

	if (m_d3d_backbuf)
		m_d3d_backbuf->Release();
	m_d3d_backbuf = NULL;
}

int d3d_render::query_format(uint32_t movie_fmt)
{
	HRESULT hr;
	int i;
	for (i = 0; i < DISPLAY_FORMAT_TABLE_ENTRIES; i++)
	{
		if (fmt_table[i].mplayer_fmt == movie_fmt)
		{
			/* Test conversion from Movie colorspace to
			 * display's target colorspace. */
			hr = m_d3d_handle->CheckDeviceFormatConversion(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				fmt_table[i].fourcc,
				m_desktop_fmt);
			if (FAILED(hr))
			{
				printf("Rejected image format.\n");
				return false;
			}

			m_movie_src_fmt = fmt_table[i].fourcc;
			return true;
		}
	}

	return false;
}

