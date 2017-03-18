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
#ifndef __VSC_LOADING_HPP__
#define __VSC_LOADING_HPP__

#include <QDialog>
#include <QTimer>
#include <QLabel>
#include <QMovie>
#include <QThread>
#include "server/factory.hpp"
#include "utility.hpp"
#include "debug.hpp"

using namespace UtilityLib;

class VE_LIBRARY_API VSCLoading : public QWidget
{
	Q_OBJECT
public:
	VSCLoading(QWidget *parent);
	~VSCLoading();
public:
	void Processing(int cnt);
	static VSCLoading * Create();
private:
	QLabel * m_label;
	QMovie * m_movie;
};


#endif // __VSC_LOADING_HPP__
