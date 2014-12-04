#include "vscplayback.h"
#include "debug.hpp"
#include <QDesktopWidget>
#include <QApplication>

VSCPlayback::VSCPlayback(QWidget *parent, QTabWidget &pTabbed)
    : QWidget(parent), m_pTabbed(pTabbed), m_currentFocus(-1)
{
    ui.setupUi(this);

    QVBoxLayout* layout = new QVBoxLayout();
    m_pVideo = new VSCVideoWall(this->ui.widget);
    m_pVideo->hide();
    //layout->setSpacing(10);

    layout->addWidget(m_pVideo);
    layout->setMargin(0);

    this->ui.widget->setLayout(layout);

    m_pVideo->show();

    m_pFloating = new QAction(QIcon(tr("images/open.ico")), tr("Floating"), this);
    //m_pUnFloating = new QAction(QIcon(tr("images/open.ico")), tr("UnFloating"), this);
    //connect(m_pFloating, SIGNAL(triggered()), this, SLOT(floatingAction()));
    //connect(m_pUnFloating, SIGNAL(triggered()), this, SLOT(unFloatingAction()));
    SetupConnections();
    createContentMenu();

    m_pPlayControl = new VSCPlayControl();
    m_bPlayControl = FALSE;
    m_pPlayControl->hide();
    //setMouseTracking(true);
}

void VSCPlayback::SetupConnections()
{
    //connect(m_pFloating, SIGNAL(triggered()), this, SLOT(floatingAction()));

    connect(ui.pushButton2x2, SIGNAL(clicked()), m_pVideo, SLOT(SetLayoutMode2x2()));
    connect(ui.pushButton3x3, SIGNAL(clicked()), m_pVideo, SLOT(SetLayoutMode3x3()));
    connect(ui.pushButton4x4, SIGNAL(clicked()), m_pVideo, SLOT(SetLayoutMode4x4()));
    connect(ui.pushButton6, SIGNAL(clicked()), m_pVideo, SLOT(SetLayoutMode6()));
    connect(ui.pushButton1, SIGNAL(clicked()), m_pVideo, SLOT(SetLayoutMode1()));
    connect(ui.pushButton12p1, SIGNAL(clicked()), m_pVideo, SLOT(SetLayoutMode12p1()));
    connect(ui.pushButton5x5, SIGNAL(clicked()), m_pVideo, SLOT(SetLayoutMode5x5()));
    //connect(ui.pushButton6x6, SIGNAL(clicked()), m_pVideo, SLOT(SetLayoutMode6x6()));
    //connect(ui.pushButton8x8, SIGNAL(clicked()), m_pVideo, SLOT(SetLayoutMode8x8()));
    //connect(ui.pushButtonPB, SIGNAL(clicked()), this, SLOT(ShowPlayControl()));
    connect(m_pVideo, SIGNAL(ShowDisplayClicked(int)), this,
                                        SLOT(ShowDisplayClicked(int)));
    connect(m_pVideo, SIGNAL(ShowFloatingClicked()), this,
                                        SLOT(floatingClicked()));
    connect(m_pVideo, SIGNAL(ShowTabbedClicked()), this,
                                        SLOT(TabbedClicked()));
    connect(m_pVideo, SIGNAL(ShowFocusClicked(int)), this,
                                        SLOT(ShowFocusClicked(int)));
    connect(m_pVideo, SIGNAL(Layout1Clicked(int)), this,
                                        SLOT(ShowLayout1Clicked(int)));
}

VSCPlayback::~VSCPlayback()
{
    m_pPlayControl->hide();
    delete m_pPlayControl;
}

void VSCPlayback::ShowPlayControl()
{
    if (m_bPlayControl == FALSE)
    {
        m_bPlayControl = TRUE;
        m_pPlayControl->show();
        int x, y, w, h;
        x = this->x();
        y = this->y();
        w = width();
        h = height(); 
        VDC_DEBUG( "%s rect x:%d y:%d w:%d h:%d \n",__FUNCTION__, x, y, w, h);
        //m_pPlayControl->move(x + w/2 - 200, y + h - 60);
    }else
    {
        m_bPlayControl = FALSE;
        m_pPlayControl->hide();
    }

}

void VSCPlayback::ShowDisplayClicked(int nId)
{
    if (m_bFloated == FALSE)
    {
        return;
    }

    QDesktopWidget *desktop = QApplication::desktop();
    if ( 1==desktop->screenCount()  ) {
        // single monitor - use built in
        showFullScreen();
    } else {
        QRect rect = desktop->screenGeometry(nId);
        //move(rect.topLeft());
        setGeometry(desktop->screenGeometry(nId));
        setWindowState(Qt::WindowFullScreen | windowState());
        move(rect.topLeft());
        resize(rect.width(), rect.height());
    }

}

void VSCPlayback::ShowFocusClicked(int nId)
{
    if (m_currentFocus != nId)
    {
        if (m_currentFocus != -1)
        {
            m_pVideo->SetVideoFocus(m_currentFocus, FALSE);
        }
        m_pVideo->SetVideoFocus(nId, TRUE);
        m_currentFocus = nId;
    }
}

void VSCPlayback::ShowLayout1Clicked(int nId)
{
    m_pVideo->SetLayout1Mode(nId);
}

void VSCPlayback::mouseDoubleClickEvent(QMouseEvent *e)
{
    VDC_DEBUG( "%s mouseDoubleClickEvent\n",__FUNCTION__);
    QWidget::mouseDoubleClickEvent(e);
#if 0
    if(isFullScreen()) {
        //setParent(m_pParent);
        //resize(m_pParent->width(), m_pParent->height());
        //showMaximized();
        this->setWindowState(Qt::WindowMaximized);
    } else {
        //m_pParent = parentWidget();
        //setParent(NULL);
        //showFullScreen();
        this->setWindowState(Qt::WindowFullScreen);
    }
#endif
}

void VSCPlayback::floatingClicked()
{
    if (m_bFloated == TRUE)
    {
        return;
    }

    m_pTabbed.setCurrentWidget(this);
    int thisIndex = m_pTabbed.currentIndex();

    m_pTabbed.removeTab(thisIndex);
    setParent(NULL);
    showMaximized();
    m_bFloated = TRUE;
}

void VSCPlayback::TabbedClicked()
{
    if (m_bFloated == FALSE)
    {
        return;
    }
    m_pTabbed.addTab(this,tr("Playback"));
    m_pTabbed.setCurrentWidget(this);
    //showFullScreen();
    //setParent(m_pParent);
    //resize(m_pParent->width(), m_pParent->height());

    m_bFloated = FALSE;
}


void VSCPlayback::createContentMenu()
{
    //this->addAction(m_pFloating);
    //this->addAction(m_pUnFloating);
    //this->setContextMenuPolicy(Qt::ActionsContextMenu);
}
