#include <QMessageBox>
#include "vscuser.h"
#include "hdddevice.hpp"
#include "debug.hpp"
#include "vschddone.h"
#include "factory.hpp"

extern Factory *gFactory;

VSCUser::VSCUser(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	ui.user->setText("admin");
	ui.autoLogin->setChecked(false);
	gFactory->GetUserData(m_Param);
	/* Setup the default value */
	if (m_Param.data.conf.AutoLogin  == 1)
	{
	        ui.autoLogin->setChecked(true);
	}
	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
}




void VSCUser::applyConfig()
{
	astring strCurPasswd;
	astring strPasswd;
	astring strPasswd2;

	astring oldPasswd = m_Param.data.conf.Passwd;

	strCurPasswd = ui.curPasswd->text().toStdString();
	strPasswd = ui.newPasswd->text().toStdString();
	strPasswd2 = ui.repeatPasswd->text().toStdString();

	if (strCurPasswd == oldPasswd)
	{
		if (ui.autoLogin->isChecked() == true)
		{
			m_Param.data.conf.AutoLogin  = 1;
		}else
		{
			m_Param.data.conf.AutoLogin  = 0;
		}
		if (strPasswd.length() > 0 && strPasswd == strPasswd2)
		{
			strcpy(m_Param.data.conf.Passwd, strPasswd.c_str());
		}
		gFactory->SetUserData(m_Param);
		return;
	}

	QMessageBox msgBox(this);
	//Set text
	msgBox.setText(tr("Password not correct ..."));
	    //Set predefined icon, icon is show on left side of text.
	msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));

	msgBox.setStandardButtons(QMessageBox::Ok);
	    //Set focus of ok button
	msgBox.setDefaultButton(QMessageBox::Ok);

	    //execute message box. method exec() return the button value of cliecke button
	int ret = msgBox.exec();	
	return;

}


