/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
#include <QMessageBox>
#include "config/clientsetting/vscuser.h"
#include "debug.hpp"
#include "vscloading.hpp"

VSCUser::VSCUser(ClientFactory &pFactory, QWidget *parent)
	: QWidget(parent), m_pFactory(pFactory)
{
	ui.setupUi(this);
	ui.user->setText("admin");
	ui.autoLogin->setToggle(false);
	/* Setup the default value */
	if (m_pFactory.GetAutoLogin() == true)
	{
	        ui.autoLogin->setToggle(true);
	}
	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
}

void VSCUser::applyConfig()
{
	astring strCurUser = "admin";
	astring strCurPasswd;
	astring strPasswd;
	astring strPasswd2;

	astring oldPasswd;
	m_pFactory.GetAdminPasswd(oldPasswd);

	strCurPasswd = ui.curPasswd->text().toStdString();
	strPasswd = ui.newPasswd->text().toStdString();
	strPasswd2 = ui.repeatPasswd->text().toStdString();

	if (m_pFactory.AuthUser(strCurUser, strCurPasswd) == true
		&& strPasswd == strPasswd2)
	{
		m_pFactory.SetAutoLogin(ui.autoLogin->isToggled());

		VSCLoading *loading = new VSCLoading(this);
		loading->Processing(3);
		SimpleCrypt crypt;

		QString strPassCrypt = strPasswd.c_str();
		m_pFactory.SetAdminPasswd(crypt.encryptToString(strPassCrypt).toStdString());
		
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


