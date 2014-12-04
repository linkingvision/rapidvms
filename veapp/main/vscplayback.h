#ifndef VSC_PLAYBACK_H
#define VSC_PLAYBACK_H

#include <QWidget>
#include "ui_vscplayback.h"
#include "utility.hpp"
#include "debug.hpp"
#include "vscvwidget.h"
#include "vscvideowall.h"
#include "QTabWidget"
#include "vscplaycontrol.h"

class VSCPlayback : public QWidget
{
    Q_OBJECT

public:
    VSCPlayback(QWidget *parent, QTabWidget &pTabbed);
    ~VSCPlayback();
public:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void createContentMenu();
public:
    void SetupConnections();
	
public slots:
    void floatingClicked();
    void TabbedClicked();
    void ShowDisplayClicked(int nId);
    void ShowFocusClicked(int nId);
    void ShowPlayControl();
    void ShowLayout1Clicked(int nId);
    void mouseMoveEvent(QMouseEvent *e)
    	{
    	    VDC_DEBUG( "%s mouseMoveEvent \n",__FUNCTION__);
    	}
	
public:
    Ui::VSCPlayback ui;
private:
    QWidget *m_pParent;
    QAction *m_pFloating;
    QAction *m_pUnFloating;
    BOOL m_bFloated;
    int m_currentFocus;
    VSCVideoWall * m_pVideo;
    VSCPlayControl * m_pPlayControl;
    BOOL m_bPlayControl;
    QTabWidget &m_pTabbed;
    
};

#endif // VSC_PLAYBACK_H
