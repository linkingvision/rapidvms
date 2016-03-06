#ifndef __VVID_PB_WALL_H__
#define __VVID_PB_WALL_H__

#include <QWidget>
#include <QGridLayout>
#include <map>
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include "server/factory.hpp"
#include "client/clientfactory.hpp"


#define PB_WALL_WIDGET_MAX 16

class VVidPBWidget;
typedef std::map<int, VVidPBWidget *> VVidPBWidgetMap;
class VVidPBControl;
typedef std::map<int, VVidPBControl *> VVidPBControlMap;

class VE_LIBRARY_API VVidPBWall : public QWidget
{
    Q_OBJECT

public:
    VVidPBWall(ClientFactory &pFactory, QWidget *parent = 0);
    ~VVidPBWall();

public:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void createContentMenu();

public:
    void ClearVideoLayout();
    void ClearControlLayout();
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

    void DeviceEvent(int deviceId, VscEventType type);
    void OffAllFocus();
	void ResizeAllVideo();

public slots:
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
	void GetPlayMap(u32 *Map, int length);
	void SetPlayMap(u32 *Map, int length, VideoWallLayoutMode nMode);
	void SetLayout1Mode(int nId);

	void PlayVideoByWindow(u32 nWindow, astring strStor, 
						astring strCam, astring strCamName);
   

signals:
	void ShowFocusClicked(int nId);
	void Layout1Clicked(int nId);

private:
    QWidget *m_pParent;
    VVidPBWidgetMap m_VideoMap;
    VVidPBControlMap m_VideoControlMap;
    QGridLayout *m_pLayout;
    QVBoxLayout* m_pVLayout;
    VideoWallLayoutMode m_VideoWallMode;
    VideoWallLayoutMode m_LastVideoWallMode;

	ClientFactory &m_pFactory;


};

#endif // __VIDEO_WALL_H_
