
#include "vscdevicetrash.h"
#include <QMimeData>
#include <QDrag>
#include "factory.hpp"
#include "vscdeviceipc.h"
#include <QPainter>
#include <QMessageBox>

extern Factory *gFactory;

VSCDeviceTrash::VSCDeviceTrash(QWidget *parent)
    : QPushButton(parent)
{
	this->setAcceptDrops(true);
}

void VSCDeviceTrash::dragEnterEvent(QDragEnterEvent *event)
{
    VDC_DEBUG( "%s Event %s \n",__FUNCTION__,
            event->mimeData()->text().toLatin1().data());
    event->acceptProposedAction();
    QWidget::dragEnterEvent(event);
}

void VSCDeviceTrash::dropEvent(QDropEvent *event)
{
    int nId = atoi(event->mimeData()->text().toLatin1().data());
    VDC_DEBUG( "%s Enter in dropEvent id %d\n",__FUNCTION__, nId);
    VDC_DEBUG( "%s %d\n",__FUNCTION__, nId);
    QMessageBox msgBox(this);
    //Set text
    msgBox.setText("Delete the Camera ...");
    msgBox.setIcon(QMessageBox::Information);
        //Set predefined icon, icon is show on left side of text.
    msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));
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
       gFactory->DelDevice(nId);
       emit CameraDeleted();
       break;
    default:
       // should never be reached
       break;
    }

}


