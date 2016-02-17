#include <QMessageBox>
#include "config/vidstor/storsetting/vscstoruser.h"
#include "debug.hpp"
#include "vscloading.hpp"
#include "client/storsyncinf.hpp"

VSCStorUser::VSCStorUser(ClientFactory &pFactory, VidStor &stor, QWidget *parent)
	: QWidget(parent), m_pFactory(pFactory), m_pStor(stor)
{
	ui.setupUi(this);
	ui.user->setText("admin");
	
	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
}

void VSCStorUser::applyConfig()
{
	astring strCurPasswd;
	astring strPasswd;
	astring strPasswd2;

	strCurPasswd = ui.curPasswd->text().toStdString();
	strPasswd = ui.newPasswd->text().toStdString();
	strPasswd2 = ui.repeatPasswd->text().toStdString();
	
	
	VSCLoading * pLoading = VSCLoading::Create();
	StorSyncInf syncInf(m_pStor);
	astring pVer;
	astring strInfo;
	syncInf.Connect();

	SimpleCrypt crypt;
	QString strPassCrypt = strPasswd.c_str();
	QString strCurrPassCrypt = strCurPasswd.c_str();
	m_pFactory.SetAdminPasswd(crypt.encryptToString(strPassCrypt).toStdString());
	

	if (strPasswd == strPasswd2 && syncInf.ConfAdminPasswd(
		crypt.encryptToString(strCurrPassCrypt).toStdString(), 
		crypt.encryptToString(strPassCrypt).toStdString()) == true)
	{
		delete pLoading;
		return;
	}
	delete pLoading;
	QMessageBox msgBox(this);
	//Set text
	msgBox.setText(tr("Change Password Failed ..."));
	    //Set predefined icon, icon is show on left side of text.
	msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));

	msgBox.setStandardButtons(QMessageBox::Ok);
	    //Set focus of ok button
	msgBox.setDefaultButton(QMessageBox::Ok);

	    //execute message box. method exec() return the button value of cliecke button
	int ret = msgBox.exec();	

	delete pLoading;
	return;

}


