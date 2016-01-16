#include <QMessageBox>
#include "config/clientsetting/vscuser.h"
#include "debug.hpp"
#include "vscloading.hpp"

VSCUser::VSCUser(ClientFactory &pFactory, QWidget *parent)
	: QWidget(parent), m_pFactory(pFactory)
{
	ui.setupUi(this);
	ui.user->setText("admin");
	ui.autoLogin->setChecked(false);
	/* Setup the default value */
	if (m_pFactory.GetAutoLogin() == true)
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

	astring oldPasswd;
	m_pFactory.GetAdminPasswd(oldPasswd);

	strCurPasswd = ui.curPasswd->text().toStdString();
	strPasswd = ui.newPasswd->text().toStdString();
	strPasswd2 = ui.repeatPasswd->text().toStdString();

	if (strCurPasswd == oldPasswd)
	{
		m_pFactory.SetAutoLogin(ui.autoLogin->isChecked());

		VSCLoading *loading = new VSCLoading(this);
		loading->Processing(3);
		m_pFactory.SetAdminPasswd(strPasswd);
		
		delete loading;
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


