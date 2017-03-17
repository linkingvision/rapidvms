#ifndef FFMPEG_DXVA2_H_
#define FFMPEG_DXVA2_H_

#define inline __inline

#include "libavformat/avformat.h"
#include "libavformat/avio.h"

#include "libavcodec/avcodec.h"

#include "libavfilter/avfilter.h"

#include "libavutil/avutil.h"
#include "libavutil/dict.h"
#include "libavutil/eval.h"
#include "libavutil/fifo.h"
#include "libavutil/pixfmt.h"
#include "libavutil/rational.h"
#include "libavutil/threadmessage.h"
#include "libavutil/imgutils.h"

#include "libswresample/swresample.h"

#include "libswscale/swscale.h"

enum HWAccelID {
	HWACCEL_NONE = 0,
	HWACCEL_AUTO,
	HWACCEL_VDPAU,
	HWACCEL_DXVA2,
	HWACCEL_VDA,
	HWACCEL_VIDEOTOOLBOX,
	HWACCEL_QSV,
};

typedef struct InputStream
{
	int file_index;
	AVStream *st;
	enum HWAccelID hwaccel_id;//--------------------------
	char  *hwaccel_device;//-----------------------

	/* hwaccel context */
	enum HWAccelID active_hwaccel_id;
	void  *hwaccel_ctx;//----------------------------------
	void(*hwaccel_uninit)(AVCodecContext *s);//-----------------------
	int(*hwaccel_get_buffer)(AVCodecContext *s, AVFrame *frame, int flags);//------------------------
	int(*hwaccel_retrieve_data)(AVCodecContext *s, AVFrame *frame);//----------------------
	enum AVPixelFormat hwaccel_pix_fmt;
	void *(*hwaccel_get_d3d_dev)(AVCodecContext *s, void *pCtx);
	void *hwaccel_cb_ctx;
}InputStream;

int dxva2_init(AVCodecContext *s);
enum AVPixelFormat get_format(AVCodecContext *s, const enum AVPixelFormat *pix_fmts);
int get_buffer(AVCodecContext *s, AVFrame *frame, int flags);
#endif