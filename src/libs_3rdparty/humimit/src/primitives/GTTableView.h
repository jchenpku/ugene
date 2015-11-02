/**
 * UGENE - Integrated Bioinformatics Tools.
 * Copyright (C) 2008-2015 UniPro <ugene@unipro.ru>
 * http://ugene.unipro.ru
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */
#ifndef GTTABLEVIEW_H
#define GTTABLEVIEW_H

#include "GTGlobals.h"
#include <QtCore/QPoint>

#if (QT_VERSION < 0x050000) //Qt 5
#include <QtGui/QTableView>
#else
#include <QtWidgets/QTableView>
#endif

namespace HI{

class HI_EXPORT GTTableView
{
public:
    static QPoint getCellPosition(U2::U2OpStatus &os, QTableView *table, int column, int row);
    static QPoint getCellPoint(U2::U2OpStatus &os, QTableView *table, int row, int column);
    static int rowCount(U2::U2OpStatus &os, QTableView *table);
    static QString data(U2::U2OpStatus &os, QTableView *table, int row, int column);
};
}
#endif // GTTABLEVIEW_H