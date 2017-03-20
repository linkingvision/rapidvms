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
#ifndef __VSC_VID_ITEM_EMAP_CONF_H__
#define __VSC_VID_ITEM_EMAP_CONF_H__

#include "common/vscviditeminf.h"

class VSCVidItemEmapConf : public QObject,public VSCVidItemInf
{
Q_OBJECT
public:
    VSCVidItemEmapConf(ClientFactory &pFactory, QTreeWidgetItem *parent);
    ~VSCVidItemEmapConf();
public:

};

#endif /* __VSC_VID_ITEM_EMAP_CONF_H__ */