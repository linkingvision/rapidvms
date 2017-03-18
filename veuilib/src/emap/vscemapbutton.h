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
#ifndef __VIDEO_EMAP_BUTTON_H__
#define __VIDEO_EMAP_BUTTON_H__

#include <QWidget>
#include <QMenu>
#include <QPushButton>
#include "utility.hpp"
#include "server/factory.hpp"

#define NOMINMAX
#define NOMINMAX 
//#include <inttypes.h>


#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
using  namespace tthread;


class VSCEmapButton : public QPushButton
{
	Q_OBJECT

public:
	VSCEmapButton(QWidget *parent = 0);
	~VSCEmapButton();
	void mousePressEvent(QMouseEvent *event);
	void DragCommon();
public:
	BOOL setId(s32 nId)
	{
		m_nId = nId;
		return TRUE;
	}
private:
	s32 m_nId;


};

#endif // __VIDEO_EMAP_BUTTON_H__
