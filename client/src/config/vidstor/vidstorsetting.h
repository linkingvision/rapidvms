#ifndef __VIDEO_VIDSTOR_SETTING_H__
#define __VIDEO_VIDSTOR_SETTING_H__

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
#include "ui_VidStorSetting.h"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QApplication>

class VidStorSetting : public QWidget
{
    Q_OBJECT

public:
	VidStorSetting(VidStor pStor, ClientFactory &pFactory, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VidStorSetting();

signals:
	//void SignalSectionClicked(int row, int column);

public slots:
	//void SlotSectionClicked(int row, int column);

public:
    Ui::VidStorSetting ui;
private:
	ClientFactory &m_pFactory;
	VidStor m_pStor;

};


#endif // __VIDEO_VIDSTOR_SETTING_H__
