#ifndef __MEDIA_PIPELINE_H___
#define __MEDIA_PIPELINE_H___
#include <string>
#include <iostream>
#include <gst/gst.h>
#include <gst/rtsp/gstrtsptransport.h>
#include <glib.h>
#include <gst/interfaces/xoverlay.h>
#include <windows.h>

using namespace std;

class mediaPipeline
{
public:
    mediaPipeline(std::string &url);
	~mediaPipeline()
	{
	    if (m_started == false)
		g_main_loop_unref(m_pLoop);
	}
	
public:
	static void init(int argc, char *argv[]);
	static gboolean handleMessage(GstBus *bus, GstMessage *msg, mediaPipeline *data);
	static void on_pad_added (GstElement *element, GstPad *pad, gpointer data);
	static void on_pad_added2 (GstElement *playbin, GstElement *source, gpointer  data);
	
public:
	bool setupPipeline();
	bool cleanPipeline();
	bool addWindows(guintptr handle){handleWin = handle; return true;}
	void run();
	bool stop()
	{
	    m_started = false;
	    if (m_pLoop)
	        g_main_loop_quit (m_pLoop);
	    return true;
	}
	
public:
	GMainLoop *m_pLoop;
	GstElement *m_pVideoSink;
	GstElement *m_pPlaybin2;
    GstElement *m_pPipeline;
    GstBus *m_pBus;
	guintptr handleWin;
	std::string m_url;
private:
	bool m_started;
};

inline mediaPipeline::mediaPipeline(std::string &url)
:m_url(url), m_started(true), m_pLoop(NULL), m_pVideoSink(NULL), 
m_pPlaybin2(NULL), m_pPipeline(NULL), m_pBus(NULL), handleWin(NULL)
{
    m_pLoop = g_main_loop_new (NULL, FALSE);
}

inline void mediaPipeline::init(int argc, char *argv[])
{
    //int argc = 0;
    //char **argv = NULL;
    _putenv("GST_PLUGIN_PATH=gstreamer-0.10");
    gst_init( &argc, &argv);
}

inline void mediaPipeline::on_pad_added (GstElement *element, GstPad *pad, gpointer data)
{
  GstPad *sinkpad;
   GstCaps *new_pad_caps = NULL;
  GstStructure *new_pad_struct = NULL;
  mediaPipeline *mediaPipe = (mediaPipeline *) data;
  
  g_print ("Received new pad '%s' from '%s':\n", 
	GST_PAD_NAME (pad), GST_ELEMENT_NAME (element));

  /* We can now link this pad with the rtsp-decoder sink pad */
  g_print ("Dynamic pad created, linking source/demuxer on_pad_added\n");

  //sinkpad = gst_element_get_static_pad (mediaPipe->m_pDecodebin2, "sink");

  gst_pad_link (pad, sinkpad);

  gst_object_unref (sinkpad);
}

inline void mediaPipeline::on_pad_added2 (GstElement *playbin, GstElement *source, gpointer  data)
{
	g_object_set(source, "latency", 0, NULL);
	g_object_set(source, "do-rtsp-keep-alive", true, NULL);
	//g_object_set(source, "protocols", GST_RTSP_LOWER_TRANS_TCP, NULL);	
	//g_object_set(source, "tcp-timeout", 100000, NULL);
    //g_object_set(source, "location", "OH-Ditang.wmv", NULL);	
}

/* Process messages from GStreamer */
inline gboolean mediaPipeline::handleMessage (GstBus *bus, GstMessage *msg, mediaPipeline *data) 
{
  GError *err;
  gchar *debug_info;
  GstElement *elmSrc;
  g_print ("Message. %d\n", GST_MESSAGE_TYPE (msg));
  
  switch (GST_MESSAGE_TYPE (msg)) {
    case GST_MESSAGE_ERROR:
      gst_message_parse_error (msg, &err, &debug_info);
      g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
      g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
      g_clear_error (&err);
      g_free (debug_info);
      g_main_loop_quit (data->m_pLoop);
      break;
    case GST_MESSAGE_EOS:
      g_print ("End-Of-Stream reached.\n");
      g_main_loop_quit (data->m_pLoop);
      break;
    case GST_MESSAGE_STATE_CHANGED: {

	  g_print ("GST_MESSAGE_STATE_CHANGED. %d\n", GST_MESSAGE_TYPE (msg));
      GstState old_state, new_state, pending_state;
      gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
#if 0
      if (GST_MESSAGE_SRC (msg) == GST_OBJECT (data->m_pPipeline)) {
        if (new_state == GST_STATE_PLAYING) {
          /* Once we are in the playing state, analyze the streams */
          //analyze_streams (data);
		  g_print ("Playing ...\n");
        }
      }
#endif
    } break;
    default:
      break;
  }
  
  /* We want to keep receiving messages */
  return TRUE;
}

inline bool mediaPipeline::cleanPipeline()
{
    if (m_pPipeline)
        gst_element_set_state (m_pPipeline, GST_STATE_NULL);
        gst_object_unref (m_pPipeline);
    //gst_object_unref (m_pBus);
    //gst_object_unref (m_pVideoSink);
    //gst_object_unref (m_pPlaybin2);

    
    return TRUE;
}

inline bool mediaPipeline::setupPipeline()
{
    
    m_pPipeline = gst_pipeline_new ("my_pipeline");

    g_print("Initialize Message Watch...\n" );
    m_pBus = gst_pipeline_get_bus (GST_PIPELINE (m_pPipeline));
    gst_bus_add_watch (m_pBus, (GstBusFunc)handleMessage, this);
    gst_object_unref (m_pBus);
	
#if 0
	m_pRtspSrc = gst_element_factory_make ("rtspsrc", "rtspsrc");

	//m_pRtspSrc = gst_element_factory_make ("fakesrc", "rtspsrc");
	g_object_set(m_pRtspSrc, "location", m_url.c_str(), NULL);
	g_object_set(m_pRtspSrc, "latency", 0, NULL);
	g_object_set(m_pRtspSrc, "do-rtsp-keep-alive", true, NULL);
	g_object_set(m_pRtspSrc, "protocols", GST_RTSP_LOWER_TRANS_TCP, NULL);
	m_pRtpDepay = gst_element_factory_make ("rtph264depay", "rtph264depay");
	m_pH264parse = gst_element_factory_make ("h264parse", "h264parse");
	//m_pDecodebin2 = gst_element_factory_make ("ffdec_h264", "dec");
	m_pDecodebin2 = gst_element_factory_make ("decodebin2", "dec");
	//m_pFFColorspace = gst_element_factory_make ("ffmpegcolorspace", "m_pFFColorspace");
	m_pTimeOverlay = gst_element_factory_make ("clockoverlay", "timeoverlay");
	g_object_set(m_pTimeOverlay, "halign", "left", NULL);
	g_object_set(m_pTimeOverlay, "valign", "bottom", NULL);
	g_object_set(m_pTimeOverlay, "text", "Video", NULL);
	//g_object_set(m_pTimeOverlay, "shaded-background", true, NULL);
	g_object_set(m_pTimeOverlay, "font-desc", "Sans 16", NULL);
	g_object_set(m_pTimeOverlay, "auto-resize", true, NULL);
#else
	m_pVideoSink = gst_element_factory_make ("d3dvideosink", "autovideosink");
	m_pPlaybin2 = gst_element_factory_make ("playbin2", "playbin2");
	g_object_set(m_pPlaybin2, "uri", m_url.c_str(), NULL);
	g_object_set(m_pPlaybin2, "video-sink", m_pVideoSink, NULL);
	g_object_set(m_pPlaybin2, "delay", 0, NULL);
	//g_object_set(m_pVideoSink, "force-aspect-ratio", true, NULL);
	g_object_set(m_pPlaybin2, "force-aspect-ratio", false, NULL);
#endif
	
	gst_x_overlay_set_window_handle(GST_X_OVERLAY(m_pVideoSink),(guintptr)handleWin);
	gst_bin_add_many (GST_BIN (m_pPipeline), m_pPlaybin2, NULL);
	//gst_element_link_many ( m_pDecodebin2, m_pTimeOverlay, 
		//m_pVideoSink, NULL);
	if(! g_signal_connect (m_pPlaybin2, "source-setup", G_CALLBACK (on_pad_added2),this))
	{
	    g_warning ("Linking part (A) with part (B) Fail...");
	}
	return true;
	
}

inline void mediaPipeline::run()
{
	while(m_started)
	{
		setupPipeline();
		if (gst_element_set_state (m_pPipeline, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE             )
		{
			//TODO Clean up
			cleanPipeline();
			Sleep(10000);
			continue;
		}
		
		g_main_loop_run (m_pLoop);
		cleanPipeline();
		//TODO add a clean up
		
	}
}

#endif /* __MEDIA_PIPELINE_H___ */