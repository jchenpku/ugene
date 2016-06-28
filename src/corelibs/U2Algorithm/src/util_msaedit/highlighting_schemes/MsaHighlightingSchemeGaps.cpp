/**
 * UGENE - Integrated Bioinformatics Tools.
 * Copyright (C) 2008-2016 UniPro <ugene@unipro.ru>
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

#include <QColor>

#include <U2Core/MultipleSequenceAlignment.h>

#include "MsaHighlightingSchemeGaps.h"

namespace U2 {

const QColor MsaHighlightingSchemeGaps::gapColor = QColor(192, 192, 192);

MsaHighlightingSchemeGaps::MsaHighlightingSchemeGaps(QObject *parent, const MsaHighlightingSchemeFactory *factory, MultipleSequenceAlignmentObject *maObj)
    : MsaHighlightingScheme(parent, factory, maObj)
{

}

void MsaHighlightingSchemeGaps::process(const char refChar, char &seqChar, QColor &color, bool &highlight, int refCharColumn, int refCharRow) const {
    if (seqChar == MAlignment_GapChar) {
        color = gapColor;
        highlight = true;
    } else {
        color = QColor();
        highlight = false;
    }
    MsaHighlightingScheme::process(refChar, seqChar, color, highlight, refCharColumn, refCharRow);
}

MsaHighlightingSchemeGapsFactory::MsaHighlightingSchemeGapsFactory(QObject *parent, const QString &id, const QString &name, DNAAlphabetType alphabetType)
    : MsaHighlightingSchemeFactory(parent, id, name, alphabetType, true)
{

}

MsaHighlightingScheme * MsaHighlightingSchemeGapsFactory::create(QObject *parent, MultipleSequenceAlignmentObject *maObj) const {
    return new MsaHighlightingSchemeGaps(parent, this, maObj);
}

}   // namespace U2