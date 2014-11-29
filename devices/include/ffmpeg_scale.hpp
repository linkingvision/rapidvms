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

extern "C"
{
#include "libswscale/swscale.h"
}

//Scale算法
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

//视频图像格式
enum PicFormat
{
    SWS_PF_NONE         = PIX_FMT_NONE,
    SWS_PF_YUV420P      = PIX_FMT_YUV420P,
    SWS_PF_RGB24        = PIX_FMT_RGB24,
    SWS_PF_BGR24        = PIX_FMT_BGR24,
    SWS_PF_ARGB         = PIX_FMT_ARGB,
    SWS_PF_RGBA         = PIX_FMT_RGBA,
    SWS_PF_ABGR         = PIX_FMT_ABGR,
    SWS_PF_BGRA         = PIX_FMT_BGRA,
};


class FFmpegScale
{
public:
    FFmpegScale(void);
    ~FFmpegScale(void);

    //设置输入输出图片属性以及Scale算法
    void SetAttribute(PicFormat srcFormat, PicFormat dstFormat, SwsAlogrithm enAlogrithm = SWS_SA_FAST_BILINEAR);

    //Scale
    BOOL Scale(
        unsigned char *pSrc, int nSrcW, int nSrcH, int nSrcPicth,
        unsigned char *pDst, int nDstW, int nDstH, int nDstPicth
        );

private:

    //初始化
    BOOL Init();

    //反初始化
    void DeInit();

    SwsContext* m_pSwsContext;      //SWS对象
    PicFormat m_srcFormat;          //源像素格式
    PicFormat m_dstFormat;          //目标像素格式
    SwsAlogrithm m_enAlogrithm;     //Resize算法

    int m_nSrcW, m_nSrcH;           //源图像宽高
    int m_nSrcPicth;                //源图像第一行数据的长度
    int m_nSrcSlice[4];             //源图像各分量数据起始地址偏移
    int m_nSrcStride[4];            //源图像各分量一行数据的长度

    int m_nDstW, m_nDstH;           //目标图像宽高
    int m_nDstPicth;                //目标图像第一行数据的长度
    int m_nDstSlice[4];             //目标图像各分量数据起始地址偏移
    int m_nDstStride[4];            //目标图像各分量一行数据的长度

};

//构造
FFmpegScale::FFmpegScale(void)
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

//析构
FFmpegScale::~FFmpegScale(void)
{
    DeInit();
}

/***************************************************************************//**
* 函数名称：    SetAttribute
* 功能描述：    设置输入输出图片属性以及Scale算法。
* 参    数：    srcFormat   >> 源图像格式；
* 参    数：    dstFormat   >> 目标图像格式；
* 参    数：    enAlogrithm >> Scale算法；
* 返 回 值：    
*******************************************************************************/
void FFmpegScale::SetAttribute(PicFormat srcFormat, PicFormat dstFormat, SwsAlogrithm enAlogrithm)
{
    m_srcFormat = srcFormat;
    m_dstFormat = dstFormat;
    m_enAlogrithm = enAlogrithm;
    DeInit();
}

/***************************************************************************//**
* 函数名称：    Init
* 功能描述：    初始化。
* 返 回 值：    执行成功返回TRUE，否则返回FALSE。
* 其它说明：    
*******************************************************************************/
BOOL FFmpegScale::Init()
{
    //必须预先设置过输入输出格式
    if (SWS_PF_NONE == m_srcFormat || SWS_PF_NONE == m_dstFormat)
    {
        return FALSE;
    }

    //反初始化
    DeInit();

    //创建sws对象
    m_pSwsContext = sws_getContext(
        m_nSrcW,
        m_nSrcH,
        (PixelFormat)m_srcFormat,
        m_nDstW,
        m_nDstH,
        (PixelFormat)m_dstFormat,
        (int)m_enAlogrithm,
        NULL, 
        NULL, 
        NULL);
    if (NULL == m_pSwsContext)
    {
        return FALSE;
    }

    //初始化源Slice和Stride
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

    //初始化目标Slice和Stride
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

/***************************************************************************//**
* 函数名称：    DeInit
* 功能描述：    反初始化。
* 返 回 值：    
* 其它说明：    
*******************************************************************************/
void FFmpegScale::DeInit()
{
    if (NULL != m_pSwsContext)
    {
        sws_freeContext(m_pSwsContext);
    }
    m_pSwsContext = NULL;
}

/***************************************************************************//**
* 函数名称：    Scale
* 功能描述：    Scale
* 参    数：    pSrc            >> 源图像内存起始地址；
* 参    数：    nSrcW           >> 源图像宽度；
* 参    数：    nSrcH           >> 源图像高度；
* 参    数：    nSrcPicth       >> 源图像每行数据的长度（YUV格式的该值不被采纳）；
* 参    数：    pDst            << 目标图像内存起始地址；
* 参    数：    nDstW           >> 目标图像宽度；
* 参    数：    nDstH           >> 目标图像高度；
* 参    数：    nDstPicth       >> 目标图像每行数据的长度（YUV格式的该值不被采纳）；
* 返 回 值：    执行成功返回TRUE，否则返回FALSE。
*******************************************************************************/
BOOL FFmpegScale::Scale(unsigned char *pSrc, int nSrcW, int nSrcH, int nSrcPicth, unsigned char *pDst, int nDstW, int nDstH, int nDstPicth)
{
    //如果任何参数发生变化，则需要重新初始化
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

    //如果未能成功初始化，返回失败
    if (NULL == m_pSwsContext && !Init())
    {
        return FALSE;
    }

    //真正的Scale操作
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

#endif /* __FFMPEG_SCALE_H__ */
