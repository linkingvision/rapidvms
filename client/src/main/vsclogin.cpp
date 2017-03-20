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
#include "vsclogin.h"
#include "debug.hpp"


VSCLogin::VSCLogin(QWidget *parent)
    : QDialog(parent), m_isLogin(FALSE)
{
   ui.setupUi(this);
   //setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
   setWindowFlags(windowFlags() &~ Qt::WindowCloseButtonHint); 

   connect(ui.pbLogin, SIGNAL(clicked()), this, SLOT(LoginClicked()));
   connect(ui.pbExit, SIGNAL(clicked()), this, SLOT(ExitClicked()));
   connect(ui.passwd, SIGNAL(returnPressed()), ui.pbLogin, SIGNAL(clicked()));
    


}

void VSCLogin::LoginClicked()
{
	m_isLogin = TRUE;
}
void VSCLogin::ExitClicked()
{
	//m_Type = VSC_IPCGROUP_CONF_CANCEL;
}

BOOL VSCLogin::GetUserPasswd(astring &strUser, astring &strPasswd)
{
 	strUser = ui.username->text().toStdString();
	strPasswd = ui.passwd->text().toStdString();

	return TRUE;
}


void VSCLogin::mousePressEvent(QMouseEvent *event)  
{  
    if (event->button() == Qt::LeftButton) {  
        m_Drag = true;  
        m_DragPosition = event->globalPos() - this->pos();  
        event->accept();  
    }  
}  
  
void VSCLogin::mouseMoveEvent(QMouseEvent *event)  
{  
    if (m_Drag && (event->buttons() && Qt::LeftButton)) {  
        move(event->globalPos() - m_DragPosition);  
        event->accept();  
    }  
}  
  
void VSCLogin::mouseReleaseEvent(QMouseEvent *)  
{  
    m_Drag = false;  
} 



