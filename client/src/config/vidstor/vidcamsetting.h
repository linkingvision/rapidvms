#ifndef __VIDEO_VIDCAM_SETTING_H__
#define __VIDEO_VIDCAM_SETTING_H__

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QMimeData>
#include <QDrag>

#define NOMINMAX
#define NOMINMAX 

#include <QWidget>
#include "utility.hpp"
#include <QTimer>
#include <QMovie>
#include <QtWidgets/QMainWindow>
#include "client/clientfactory.hpp"
#include "ui_vidcamsetting.h"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QApplication>
#include "vscvwidget.h"

class VidCamSetting : public QWidget
{
    Q_OBJECT

public:
	VidCamSetting(VidStor pStor, astring strCam, ClientFactory &pFactory, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VidCamSetting();

signals:
	//void SignalSectionClicked(int row, int column);

public slots:
	//void SlotSectionClicked(int row, int column);

public:
    Ui::VidCamSetting ui;
private:
	ClientFactory &m_pFactory;
	VidStor m_pStor;
	astring m_strCam;
	VSCVWidget *m_pVideo;

};


#endif // __VIDEO_VIDCAM_SETTING_H__
