#include <QThread>

#include "vscsearch.h"
#include "vscvwidget.h"

#include <QTableWidgetItem>
#include <QMessageBox>
#include <QApplication>
#include "devicesearcher.h"

extern Factory *gFactory;

BOOL VSCSearch::m_bStarted = FALSE;

class Event:public QEvent
{
public:
    Event(astring IP, astring Port, astring Manufacturer, astring Model, 
	astring FirmwareVersion, astring ONVIFAddress):QEvent(QEvent::User), 
	IP(IP), Port(Port), Manufacturer(Manufacturer), Model(Model), 
	FirmwareVersion(FirmwareVersion),ONVIFAddress(ONVIFAddress){}
    astring IP;
	astring Port; 
	astring Manufacturer; 
	astring Model;
	astring FirmwareVersion; 
	astring ONVIFAddress;
};

VSCSearch::VSCSearch(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	m_nSearchCnt = 0;
	m_bSelectedAll = FALSE;
	m_DeviceSearcher = NULL;
	QList<QHostAddress> hostAddr = DeviceSearcher::getHostAddress();
	QList<QHostAddress>::iterator i;
	for(i=hostAddr.begin();i!=hostAddr.end();i++)
	{
		
		ui.comboBox->addItem((*i).toString());
	}


    SetupConnections();
}

VSCSearch::~VSCSearch()
{
    if (m_bStarted == TRUE)
    {
        StopSearch();
    }
}

void VSCSearch::SetupConnections()
{
    connect( this->ui.pushButtonStart, SIGNAL( clicked() ), this, SLOT(StartSearch()));
    connect( this->ui.pushButtonStop, SIGNAL( clicked() ), this, SLOT(StopSearch()));
    connect( this->ui.pushButtonAdd, SIGNAL( clicked() ), this, SLOT(AddAll()));
	connect( this->ui.pushButtonSelect, SIGNAL( clicked() ), this, SLOT(SelectAll()));
	connect( this, SIGNAL(NewSearchedItem(astring, astring, astring, astring, astring, astring) ), this, SLOT(AddItem(astring, astring, astring, astring, astring, astring)), Qt::QueuedConnection);
	//Qt::QueuedConnection
}

void VSCSearch::OnvifCallProfileS()
{
#if 0
	QMetaObject::invokeMethod(this,"AddItem"
                              ,Q_ARG(astring, deviceIP), Q_ARG(astring, devicePort),
							  Q_ARG(astring, DevInfo.Manufacturer), Q_ARG(astring, DevInfo.Model),
							  Q_ARG(astring, DevInfo.FirmwareVersion), Q_ARG(astring, deviceOnvifAddress));
	QMetaObject::invokeMethod(this,"intTest"
                              ,Q_ARG(int, 5));
	QApplication::postEvent(this, new Event(deviceIP, devicePort, DevInfo.Manufacturer, DevInfo.Model, 
		DevInfo.FirmwareVersion, deviceOnvifAddress));
#endif

    //if(tableWidget ->item(row, col)->checkState() == Qt::Checked)
}

bool VSCSearch::event(QEvent *event)
{
#if 0
	if (event->type() == QEvent::User){
		Event * evt = static_cast<Event*>(event);
		AddItem(evt->IP, evt->Port, evt->Manufacturer, 
			evt->Model, evt->FirmwareVersion, evt->ONVIFAddress);
	}

	return QWidget::event(event);
 #endif
 return true;
}

void VSCSearch::intTest(int a)
{
	VDC_DEBUG( " intTest %d\n", a);
}

void VSCSearch::AddItem(astring IP, astring Port, astring Manufacturer, astring Model, 
	astring FirmwareVersion, astring ONVIFAddress)
{
    
    if (CheckIP((s8 *)IP.c_str()) == true && CheckPort((s8 *)Port.c_str()))
    {
        VDC_DEBUG( " AddItem === > %s exist\n", IP.c_str());
	 return;
    }
    VDC_DEBUG( " AddItem === >\n");
    int insertRow = ui.tableWidget->rowCount();
    ui.tableWidget->insertRow(insertRow);
    QTableWidgetItem *firstCheck = new QTableWidgetItem("");
    firstCheck->setCheckState(Qt::Unchecked);

    ui.tableWidget->setItem(insertRow, 0, firstCheck);
    ui.tableWidget->setItem(insertRow, 1, new QTableWidgetItem(IP.c_str()));
    ui.tableWidget->setItem(insertRow, 2, new QTableWidgetItem(Port.c_str()));
    ui.tableWidget->setItem(insertRow, 3, new QTableWidgetItem(Manufacturer.c_str()));
    ui.tableWidget->setItem(insertRow, 4, new QTableWidgetItem(Model.c_str()));
    ui.tableWidget->setItem(insertRow, 5, new QTableWidgetItem(FirmwareVersion.c_str()));
    ui.tableWidget->setItem(insertRow, 6, new QTableWidgetItem(ONVIFAddress.c_str()));
    ui.progressBar->setValue(m_nSearchCnt ++);	
}

void VSCSearch::SearchCallback(void *pParam, char *pIPAddr, char *pOnvifAddr, int type)
{
    VSCSearch *pSearch = (VSCSearch *)pParam;

    if (pSearch)
    {
        pSearch->SearchCallback1(pIPAddr, pOnvifAddr, type);
    }
}

void VSCSearch::SearchCallback1(char *pIPAddr, char *pOnvifAddr, int type)
{
    VDC_DEBUG( "[ONVIF]: Search IP %s ONVIF Addr %s type %d\n",
                pIPAddr, pOnvifAddr, type);
    std::string strIPAddr = pIPAddr;
    std::string strOnvifAddr = pOnvifAddr;
    QThread *thread = QThread::currentThread();

	if (type != 1)
	{
	    VDC_DEBUG( "[ONVIF]: Search IP %s ONVIF Addr %s type %d ignore\n",
	                pIPAddr, pOnvifAddr, type);
	    return;
	}

    OnvifCallProfileS();
}

static void GetIPAndPort(astring &str, astring & ip, astring & port, astring & onvifAddr)
{
       astring strItem = "http://";
	size_t pos = 0;
	while (1)
	{
	       strItem = "http://";
		size_t p1 = str.find(strItem, pos);
		if (p1 == string::npos) return;
		size_t posIP = p1 + strItem.length();
		strItem = "/";
		size_t p2 = str.find(strItem,posIP);
		if (p2 == string::npos) return;
		string strGetIP = str.substr(posIP, p2 - posIP);
		if (strGetIP.find(".") == string::npos || 
			strGetIP.find("169") == 0)
		{
			/* IPV6 address */
			pos = p2;
			continue;
		}
		string strGetOnvifAddr;
		strItem = "http://";
		if (str.find(strItem, p2) == string::npos)
		{
			strGetOnvifAddr = str .substr(p2);
		}else
		{
			strItem = " ";
			size_t p3 = str.find(strItem, p2);
			if (p3 == string::npos) return;
			strGetOnvifAddr = str.substr(p2, p3 - p2);
		}

	       string strPort = "80";
		size_t posPort = strGetIP.find_first_of(":");
		if (posPort != std::string::npos) 
		{
			strPort = strGetIP.substr(posPort + 1);
			string strIPTemp = strGetIP.substr(0, posPort);
			strGetIP = strIPTemp;
		}
		ip = strGetIP;
		port = strPort;
		onvifAddr = strGetOnvifAddr;
		strItem = "http://";
		if (str.find(strItem, p2) == string::npos)
		{
			break;
		}else
		{
			pos = p2;
			continue;			
		}
	}
	
}

static void GetHardwareModel(astring &str, astring & hdModel)
{
	size_t pos1= 0;
	size_t pos2 = 0;	
	std::string strItem;
	strItem = "hardware/";
	pos2 = str.find(strItem);

	string sHardware = "unknown";
	if (pos2 != string::npos)
	{
		size_t posHardware =  pos2 + strItem.length();
		strItem = "onvif";
		pos1 = str.find(strItem,pos2);
		
		if (pos1 != string::npos)
		{
			sHardware =  str.substr(posHardware,pos1 - posHardware);
			transform(sHardware.begin(), sHardware.end(),sHardware.begin(),(int(*)(int))toupper);
		}
		
	}
	hdModel = sHardware;
}

void VSCSearch::SearchReceiveData(const QHash<QString, QString> &data)
{
    astring ip = "192.168.0.1";
    astring port = "80";
    astring hdModel = "unknown";
    astring OnvifAddr = "/onvif/device_service";
    astring Manufacturer = "";
    astring FirmwareVersion = "";
    astring type = data["types"].toStdString();
	astring strFilter = data["device_ip"].toStdString();
    transform(	type.begin(), type.end(),type.begin(),(int(*)(int))tolower);
	transform(	strFilter.begin(), strFilter.end(),strFilter.begin(),(int(*)(int))tolower);
    if (strFilter.find("network_video_storage") != string::npos)
    {
        VDC_DEBUG("[ONVIF]: it is a NVS device\n");
        return;
    } 
    size_t pos1 = type.find("networkvideotransmitter");
    VDC_DEBUG( "[ONVIF]: Searched ip %s\n", data["device_service_address"].toStdString().c_str());
    if (pos1 == string::npos)
    {
        return;
    }
    astring strServerAddr = data["device_service_address"].toStdString();
    GetIPAndPort(strServerAddr,  ip, port, OnvifAddr);
    astring strDeviceIp = data["device_ip"].toStdString();
    GetHardwareModel(strDeviceIp, hdModel);

    astring OnvifDeviceService = "http://" + ip + ":" + port + OnvifAddr;
#if 0
    DeviceManagement *pDm = new DeviceManagement(OnvifDeviceService.c_str(), 
                            "admin", "12345");
    if (pDm)
    {
    	QHash<QString, QString> deviceInfo = pDm->getDeviceInformation();
       Manufacturer = deviceInfo["mf"].toStdString();
	FirmwareVersion = deviceInfo["firmware_version"].toStdString();
    }
#endif
    
    AddItem(ip, port, Manufacturer, hdModel, FirmwareVersion, OnvifAddr);
    m_nSearchProcess += 2;
    if (m_nSearchProcess == 100)
    {
        m_nSearchProcess = 0;
    }
    ui.progressBar->setValue(m_nSearchProcess);
}

void VSCSearch::StartSearch()
{
    if (m_bStarted == TRUE)
    {
        QMessageBox msgBox(this);
        //Set text
		msgBox.setWindowTitle(tr("Warning"));
        msgBox.setText(tr("Search is In Processing ..."));
            //Set predefined icon, icon is show on left side of text.
        msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));
            //set inforative text
        //msgBox.setInformativeText("Just show infornation.");
            //Add ok and cancel button.
        msgBox.setStandardButtons(QMessageBox::Ok);
            //Set focus of ok button
        msgBox.setDefaultButton(QMessageBox::Ok);

            //execute message box. method exec() return the button value of cliecke button
        int ret = msgBox.exec();

        return;
    }
    m_nSearchProcess = 0;
    m_bStarted = TRUE;
    m_nSearchCnt = 0;
    QHostAddress host(ui.comboBox->currentText());
    m_DeviceSearcher = DeviceSearcher::instance(host);
    connect(m_DeviceSearcher, SIGNAL( receiveData(const QHash<QString, QString> &) ), 
		this, SLOT(SearchReceiveData(const QHash<QString, QString> &)));
    m_DeviceSearcher->sendSearchMsg();

}
void VSCSearch::StopSearch()
{
    if (m_bStarted == FALSE)
    {
        QMessageBox msgBox(this);
        //Set text
		msgBox.setWindowTitle(tr("Warning"));
        msgBox.setText(tr("Search is not Started ..."));
            //Set predefined icon, icon is show on left side of text.
        msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));
            //set inforative text
        //msgBox.setInformativeText("Just show infornation.");
            //Add ok and cancel button.
        msgBox.setStandardButtons(QMessageBox::Ok);
            //Set focus of ok button
        msgBox.setDefaultButton(QMessageBox::Ok);

            //execute message box. method exec() return the button value of cliecke button
        int ret = msgBox.exec();

        return;
    }
    ui.progressBar->setValue(100);
    disconnect(m_DeviceSearcher, SIGNAL( receiveData(const QHash<QString, QString> &) ), 
		this, SLOT(SearchReceiveData(const QHash<QString, QString> &)));
    delete m_DeviceSearcher;
    m_DeviceSearcher = NULL;
    m_bStarted = FALSE;
}

void VSCSearch::updateParamValue(QTableWidgetItem *item, s8 * pParam)
{
    if (pParam && item)
    {
        strcpy(pParam, item->text().toStdString().c_str());
    }
}

void VSCSearch::AddAll()
{

	DeviceParam Param;
	int nId = 0;
	int insertRow = ui.tableWidget->rowCount();
    VDC_DEBUG( "[ONVIF]: Searched %d", insertRow);
	/* Loop to add device */
	for (int i = 0; i < insertRow; i ++)
	{
		if(ui.tableWidget->item(i, 0)->checkState() != Qt::Checked)
		{
			continue;
		}

		/* Update  from UI  */
		updateParamValue(ui.tableWidget->item(i, 1), Param.m_Conf.data.conf.Name);

		updateParamValue(ui.tableWidget->item(i, 1), Param.m_Conf.data.conf.IP);
		updateParamValue(ui.tableWidget->item(i, 2), Param.m_Conf.data.conf.Port);

		updateParamValue(ui.tableWidget->item(i, 6), Param.m_Conf.data.conf.OnvifAddress);
		
		Param.m_Conf.data.conf.UseProfileToken = 0;
		
		Param.m_Conf.data.conf.nSubType = VSC_SUB_DEVICE_ONVIF;

		nId = gFactory->AddDevice(Param);

		VDC_DEBUG( "%s  ID %d\n",__FUNCTION__, nId);
	
	}
	
	//emit CameraTreeUpdated();
}


bool VSCSearch::CheckIP(s8 * ipAddr)
{
	int nId = 0;
	int insertRow = ui.tableWidget->rowCount();
	s8 rowip[1024];
       VDC_DEBUG( "[ONVIF]: Searched %d", insertRow);
	/* Loop to add device */
	for (int i = 0; i < insertRow; i ++)
	{

		/* Update  from UI  */

		updateParamValue(ui.tableWidget->item(i, 1), rowip);
		if (strcmp(ipAddr, rowip) == 0)
		{
		    VDC_DEBUG( "%s  Find IP %s\n",__FUNCTION__, rowip);
		    return true;
		}
	}

	return false;
}

bool VSCSearch::CheckPort(s8 * Port)
{
	int nId = 0;
	int insertRow = ui.tableWidget->rowCount();
	s8 rowPort[1024];
       VDC_DEBUG( "[ONVIF]: Searched %d", insertRow);
	/* Loop to add device */
	for (int i = 0; i < insertRow; i ++)
	{

		/* Update  from UI  */

		updateParamValue(ui.tableWidget->item(i, 2), rowPort);
		if (strcmp(Port, rowPort) == 0)
		{
		    VDC_DEBUG( "%s  Find IP %s\n",__FUNCTION__, rowPort);
		    return true;
		}
	}

	return false;
}

void VSCSearch::SelectAll()
{
	int insertRow = ui.tableWidget->rowCount();
    VDC_DEBUG( "[ONVIF]: Searched %d", insertRow);

	if (m_bSelectedAll == TRUE)
	{
		for (int i = 0; i < insertRow; i ++)
		{
			ui.tableWidget->item(i, 0)->setCheckState(Qt::Unchecked);
		}
		m_bSelectedAll = FALSE;
	}else
	{
		for (int i = 0; i < insertRow; i ++)
		{
			ui.tableWidget->item(i, 0)->setCheckState(Qt::Checked);
		}	
		m_bSelectedAll = TRUE;
	}
}


