
#include "vscrecorderpanel.h"
#include <QMimeData>
#include <QDrag>
#include <QString>
#include "factory.hpp"
#include "vscdeviceipc.h"
#include <QPainter>
#include <QMessageBox>

extern Factory *gFactory;

VSCRecorderPanel::VSCRecorderPanel(QWidget *parent)
    : QPushButton(parent)
{
	this->setAcceptDrops(true);
}

void VSCRecorderPanel::dragEnterEvent(QDragEnterEvent *event)
{
    VDC_DEBUG( "%s Event %s \n",__FUNCTION__,
            event->mimeData()->text().toLatin1().data());
    event->acceptProposedAction();
    QWidget::dragEnterEvent(event);
}

void VSCRecorderPanel::dropEvent(QDropEvent *event)
{
	astring strUrl;
    int nId = atoi(event->mimeData()->text().toLatin1().data());
    VDC_DEBUG( "%s Enter in dropEvent id %d\n",__FUNCTION__, nId);
    VDC_DEBUG( "%s %d\n",__FUNCTION__, nId);
    QMessageBox msgBox(this);
    //Set text
    msgBox.setText("Add Camera To NVR ...");
    msgBox.setIcon(QMessageBox::Information);
        //Set predefined icon, icon is show on left side of text.
    msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.jpg"));
        //set inforative text
    //msgBox.setInformativeText("Just show infornation.");
        //Add ok and cancel button.
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        //Set focus of ok button
    msgBox.setDefaultButton(QMessageBox::Ok);


        //execute message box. method exec() return the button value of cliecke button
    int ret = msgBox.exec();

        //User get input from returned value (ret). you can handle it here.
    switch (ret) {
    case QMessageBox::Ok:
       if (gFactory->GetDeviceRtspUrl(strUrl, nId) == TRUE)
	   {
		   //qDebug() << "Item:"  << (QString::fromLocal8Bit(strUrl.c_str()));//just for test
			m_Recorder->AddAnIPCamera();//TODO set the url to the function
	   }
	   else
	   {
		  //Can not get the url
		  //  qDebug() << "Item:"  << (QString::fromLocal8Bit(strUrl.c_str()));//just for test
	   }
       break;
    default:
       // should never be reached
       break;
    }

}


