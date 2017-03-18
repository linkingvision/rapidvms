#ifndef __FFMPEG_SCALE_H__
#define __FFMPEG_SCALE_H__
#pragma once

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

#ifndef BOOL
    typedef int BOOL;
#endif
#if 0
#ifndef byte
    typedef char byte;
#endif
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif 

#ifdef WIN32
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "gdiplus.lib")
#endif

extern "C"
{
#include "libswscale/swscale.h"
}

enum SwsAlogrithm
{
    SWS_SA_FAST_BILINEAR    = 0x1,
    SWS_SA_BILINEAR         = 0x2,
    SWS_SA_BICUBIC          = 0x4,
    SWS_SA_X                = 0x8,
    SWS_SA_POINT            = 0x10,
    SWS_SA_AREA             = 0x20,
    SWS_SA_BICUBLIN         = 0x40,
    SWS_SA_GAUSS            = 0x80,
    SWS_SA_SINC             = 0x100,
    SWS_SA_LANCZOS          = 0x200,
    SWS_SA_SPLINE           = 0x400,
};


enum PicFormat
{
    SWS_PF_NONE         = AV_PIX_FMT_NONE,
	SWS_PF_YUV420P = AV_PIX_FMT_YUV420P,
	SWS_PF_YUVJ420P = AV_PIX_FMT_YUVJ420P,
	SWS_PF_NV12 = AV_PIX_FMT_NV12,
	SWS_PF_RGB24 = AV_PIX_FMT_RGB24,
	SWS_PF_BGR24 = AV_PIX_FMT_BGR24,
	SWS_PF_ARGB = AV_PIX_FMT_ARGB,
	SWS_PF_RGBA = AV_PIX_FMT_RGBA,
	SWS_PF_ABGR = AV_PIX_FMT_ABGR,
	SWS_PF_BGRA = AV_PIX_FMT_BGRA,
};


class FFmpegScale
{
public:
    FFmpegScale(void);
    ~FFmpegScale(void);

    void SetAttribute(PicFormat srcFormat, PicFormat dstFormat, SwsAlogrithm enAlogrithm = SWS_SA_FAST_BILINEAR);

    BOOL Scale(
        unsigned char *pSrc, int nSrcW, int nSrcH, int nSrcPicth,
        unsigned char *pDst, int nDstW, int nDstH, int nDstPicth
        );
    BOOL ScaleYUV420(
        unsigned char *pSrcY, int lenY,  unsigned char *pSrcU, int lenU, 
		unsigned char *pSrcV, int lenV,
		int nSrcW, int nSrcH, int nSrcPicth,
        unsigned char *pDst, int nDstW, int nDstH, int nDstPicth
        );

private:

    BOOL Init();

    void DeInit();

    SwsContext* m_pSwsContext;    
    PicFormat m_srcFormat;         
    PicFormat m_dstFormat;       
    SwsAlogrithm m_enAlogrithm;    

    int m_nSrcW, m_nSrcH;       
    int m_nSrcPicth;               
    int m_nSrcSlice[4];            
    int m_nSrcStride[4];            

    int m_nDstW, m_nDstH;          
    int m_nDstPicth;             
    int m_nDstSlice[4];           
    int m_nDstStride[4];          

};

inline FFmpegScale::FFmpegScale(void)
{
    m_pSwsContext = NULL;
    m_srcFormat = SWS_PF_NONE;  
    m_dstFormat = SWS_PF_NONE;  
    m_enAlogrithm = SWS_SA_FAST_BILINEAR;

    m_nSrcW = m_nSrcH = 0;          
    m_nSrcPicth = 0;                
    m_nDstW = m_nDstH = 0;
    m_nDstPicth = 0;
    for (int i=0; i<4; i++)
    {
        m_nSrcSlice[i] = -1;
        m_nSrcStride[i] = 0;
        m_nDstSlice[i] = -1;
        m_nDstStride[i] = 0; 
    }
}

inline FFmpegScale::~FFmpegScale(void)
{
    DeInit();
}

inline void FFmpegScale::SetAttribute(PicFormat srcFormat, PicFormat dstFormat, SwsAlogrithm enAlogrithm)
{
    m_srcFormat = srcFormat;
    m_dstFormat = dstFormat;
    m_enAlogrithm = enAlogrithm;
    DeInit();
}

inline BOOL FFmpegScale::Init()
{
    if (SWS_PF_NONE == m_srcFormat || SWS_PF_NONE == m_dstFormat)
    {
        return FALSE;
    }

    DeInit();

    m_pSwsContext = sws_getContext(
        m_nSrcW,
        m_nSrcH,
        (AVPixelFormat)m_srcFormat,
        m_nDstW,
        m_nDstH,
        (AVPixelFormat)m_dstFormat,
        (int)m_enAlogrithm,
        NULL, 
        NULL, 
        NULL);
    if (NULL == m_pSwsContext)
    {
        return FALSE;
    }

    if (m_srcFormat == SWS_PF_YUV420P)
    {
        m_nSrcSlice[0] = 0;
        m_nSrcSlice[1] = m_nSrcW * m_nSrcH;
        m_nSrcSlice[2] = m_nSrcW * m_nSrcH * 5 / 4;
        m_nSrcSlice[3] = -1;

        m_nSrcStride[0] = m_nSrcW;
        m_nSrcStride[1] = m_nSrcW / 2;
        m_nSrcStride[2] = m_nSrcW / 2;
        m_nSrcStride[3] = 0;

    }
    else
    {
        m_nSrcSlice[0] = 0;
        m_nSrcSlice[1] = -1;
        m_nSrcSlice[2] = -1;
        m_nSrcSlice[3] = -1;

        m_nSrcStride[0] = m_nSrcPicth;
        m_nSrcStride[1] = 0;
        m_nSrcStride[2] = 0;
        m_nSrcStride[3] = 0;
    }

    if (m_dstFormat == SWS_PF_YUV420P)
    {
        m_nDstSlice[0] = 0;
        m_nDstSlice[1] = m_nDstW * m_nDstH;
        m_nDstSlice[2] = m_nDstW * m_nDstH * 5 / 4;
        m_nDstSlice[3] = -1;

        m_nDstStride[0] = m_nDstW;
        m_nDstStride[1] = m_nDstW / 2;
        m_nDstStride[2] = m_nDstW / 2;
        m_nDstStride[3] = 0;

    }
    else
    {
        m_nDstSlice[0] = 0;
        m_nDstSlice[1] = -1;
        m_nDstSlice[2] = -1;
        m_nDstSlice[3] = -1;

        m_nDstStride[0] = m_nDstPicth;
        m_nDstStride[1] = 0;
        m_nDstStride[2] = 0;
        m_nDstStride[3] = 0;
    }
    return TRUE;
}

inline void FFmpegScale::DeInit()
{
    if (NULL != m_pSwsContext)
    {
        sws_freeContext(m_pSwsContext);
    }
    m_pSwsContext = NULL;
}

inline BOOL FFmpegScale::Scale(unsigned char *pSrc, int nSrcW, int nSrcH, int nSrcPicth, unsigned char *pDst, int nDstW, int nDstH, int nDstPicth)
{
    if (nSrcW != m_nSrcW || nSrcH != m_nSrcH || m_nSrcPicth != m_nSrcPicth
        || nDstW != m_nDstW || nDstH != m_nDstH || m_nDstPicth != m_nDstPicth)
    {
        m_nSrcW = nSrcW;
        m_nSrcH = nSrcH;
        m_nSrcPicth = nSrcPicth;
        m_nDstW = nDstW;
        m_nDstH = nDstH;
        m_nDstPicth = nDstPicth;
        DeInit();
    }

    if (NULL == m_pSwsContext && !Init())
    {
        return FALSE;
    }

    unsigned char *srcSlice[4], *dstSlice[4];
    for (int i=0; i<4; i++)
    {
        srcSlice[i] = m_nSrcSlice[i] < 0 ? NULL : (pSrc + m_nSrcSlice[i]);
        dstSlice[i] = m_nDstSlice[i] < 0 ? NULL : (pDst + m_nDstSlice[i]);
    }
    return sws_scale
        (
        m_pSwsContext,
        (uint8_t **)srcSlice,
        m_nSrcStride,
        0,
        m_nSrcH,
        (uint8_t **)dstSlice,
        m_nDstStride
        ) == m_nSrcH;
}

inline BOOL FFmpegScale::ScaleYUV420(
	unsigned char *pSrcY, int lenY,  unsigned char *pSrcU, int lenU, 
	unsigned char *pSrcV, int lenV,
	int nSrcW, int nSrcH, int nSrcPicth,
	unsigned char *pDst, int nDstW, int nDstH, int nDstPicth
	)
{
    if (nSrcW != m_nSrcW || nSrcH != m_nSrcH || nSrcPicth != m_nSrcPicth
        || nDstW != m_nDstW || nDstH != m_nDstH || nDstPicth != m_nDstPicth)
    {
        m_nSrcW = nSrcW;
        m_nSrcH = nSrcH;
        m_nSrcPicth = nSrcPicth;
        m_nDstW = nDstW;
        m_nDstH = nDstH;
        m_nDstPicth = nDstPicth;
        DeInit();
    }


    if (NULL == m_pSwsContext && !Init())
    {
        return FALSE;
    }

    unsigned char *srcSlice[4], *dstSlice[4];
    for (int i=0; i<4; i++)
    {
        dstSlice[i] = m_nDstSlice[i] < 0 ? NULL : (pDst + m_nDstSlice[i]);
    }
	m_nSrcStride[0] = lenY;
	m_nSrcStride[1] = lenU;
	m_nSrcStride[2] = lenV;
	
	srcSlice[0] = pSrcY;
	srcSlice[1] = pSrcU;
	srcSlice[2] = pSrcV;
	srcSlice[3] = NULL;
    return sws_scale
        (
        m_pSwsContext,
        (uint8_t **)srcSlice,
        m_nSrcStride,
        0,
        m_nSrcH,
        (uint8_t **)dstSlice,
        m_nDstStride
        ) == m_nSrcH;
}

#endif /* __FFMPEG_SCALE_H__ */
