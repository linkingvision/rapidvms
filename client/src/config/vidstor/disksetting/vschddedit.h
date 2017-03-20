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
#ifndef VSC_HDD_EDIT_H
#define VSC_HDD_EDIT_H

#include <QWidget>
#include "ui_vschddedit.h"
#include "utility.hpp"
#include <QTimer>
#include "config/vidstor/disksetting/vschddoneedit.h"

#define VSC_SUPPORTED_DISKS 128

class VSCHddEdit : public QWidget
{
	Q_OBJECT

public:
	VSCHddEdit(VidStor &stor, QWidget *parent = 0);
	~VSCHddEdit(){}
	
public:
	Ui::VSCHddEdit ui;
	
private:
	VSCHddOneEdit *  m_Disks[VSC_SUPPORTED_DISKS];
	VidStor m_pStor;

    
};

#endif // VSC_HDD_DEVICE_H
