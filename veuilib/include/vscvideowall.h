
#ifndef __VIDEO_WALL_H_
#define __VIDEO_WALL_H_

#include <QWidget>
#include <QGridLayout>
#include <map>
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include "server/factory.hpp"
#include "client/clientfactory.hpp"


#define VIDEO_WALL_WIDGET_MAX 36

class VSCVWidget;
typedef std::map<int, VSCVWidget *> VideoWidgetMap;

class VE_LIBRARY_API VSCVideoWall : public QWidget
{
    Q_OBJECT

public:
    VSCVideoWall(ClientFactory &pFactory, QWidget *parent = 0);
    ~VSCVideoWall();

public:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void createContentMenu();

public:
    void ClearVideoLayout();
    void SetupVideoLayout4x4();
    void SetupVideoLayout2x2();
    void SetupVideoLayout3x3();
    void SetupVideoLayout1();
    void SetupVideoLayout12p1();
    void SetupVideoLayout6();
    void SetupVideoLayout5x5();
    void SetupVideoLayout6x6();
    void SetupVideoLayout8x8();
    void SetupVideoLayout1x3();
    void SetupVideoLayout1p6();
    void StopVideoBeforeSetLayout();
    void SetVideoFocus(int nId, BOOL on);
#if 0
    void mouseMoveEvent(QMouseEvent *e)
    	{
    	    VDC_DEBUG( "%s mouseMoveEvent \n",__FUNCTION__);
    	}
#endif
    void DeviceEvent(int deviceId, VscEventType type);
    void OffAllFocus();
	void ResizeAllVideo();

public slots:
    void floatingAction();
    void unFloatingAction();
    void SetLayoutMode4x4(){SetLayoutMode(LAYOUT_MODE_4X4);}
    void SetLayoutMode3x3(){SetLayoutMode(LAYOUT_MODE_3X3);}
    void SetLayoutMode2x2(){SetLayoutMode(LAYOUT_MODE_2X2);}
    void SetLayoutMode6(){SetLayoutMode(LAYOUT_MODE_6);}
    void SetLayoutMode12p1(){SetLayoutMode(LAYOUT_MODE_12p1);}
    void SetLayoutMode1(){SetLayoutMode(LAYOUT_MODE_1);}
    void SetLayoutMode5x5(){SetLayoutMode(LAYOUT_MODE_5x5);}
    void SetLayoutMode6x6(){SetLayoutMode(LAYOUT_MODE_6x6);}
    void SetLayoutMode8x8(){SetLayoutMode(LAYOUT_MODE_8x8);}
    void SetLayoutMode1x3(){SetLayoutMode(LAYOUT_MODE_1x3);}
    void SetLayoutMode1p6(){SetLayoutMode(LAYOUT_MODE_1p6);}
    void VideoSwitchWith(int nSrcId, int nDstId);

public:
	void UpdateVideoWallLayout();
	void UpdateVideoWallLayout2();
	void SetLayoutMode(VideoWallLayoutMode nMode);
	void GetLayoutMode(VideoWallLayoutMode &nMode)
	{
	    nMode = m_VideoWallMode;
	}
	void GetPlayMap(std::map<int, VidViewWindow> &playMap);
	void SetPlayMap(std::map<int, VidViewWindow> &playMap, VideoWallLayoutMode nMode);
	void SetLayout1Mode(int nId);

	void PlayVideoByWindow(u32 nWindow, astring strStor, 
						astring strCam, astring strCamName);
	void ShowVideoInfo(bool bEnable);
   

signals:
	void ShowDisplayClicked(int nId);
	void ShowFloatingClicked();
	void ShowTabbedClicked();
	void ShowControlPanelClicked();
	void ShowFocusClicked(int nId);
	void Layout1Clicked(int nId);
	void ShowViewClicked(std::string strId);
	void PlaybackClicked(std::string strStor, std::string strId, std::string strName);
	void MotionDetectClicked(std::string strStor, std::string strId, std::string strName);

public:
    bool Start();
    bool Stop();
private:
    QWidget *m_pParent;
    VideoWidgetMap m_VideoMap;
    QGridLayout *m_pLayout;
    QAction *m_pFloating;
    QAction *m_pUnFloating;
    BOOL m_bFloated;
    VideoWallLayoutMode m_VideoWallMode;
    VideoWallLayoutMode m_LastVideoWallMode;

	ClientFactory &m_pFactory;


};

#endif // __VIDEO_WALL_H_
