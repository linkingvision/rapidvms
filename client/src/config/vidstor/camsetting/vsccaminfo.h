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
#ifndef VSC_CAM_INFO_H
#define VSC_CAM_INFO_H

#include <QWidget>
#include "ui_vsccaminfo.h"
#include "utility.hpp"
#include "client/clientfactory.hpp"
#include <QTimer>


class VSCCamInfo : public QWidget
{
    Q_OBJECT

public:
    VSCCamInfo(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent = 0);
    ~VSCCamInfo(){}
public:
	astring GetCameraName()
	{
		return m_strCamName;
	}

public slots:   
  void applyConfig();
	
public:
    Ui::VSCCamInfo ui;
	
private:
	ClientFactory &m_pFactory;
	VidStor m_pStor;
	astring m_strCam;
	astring m_strCamName;

    
};

#endif // VSC_LICENSE_H
