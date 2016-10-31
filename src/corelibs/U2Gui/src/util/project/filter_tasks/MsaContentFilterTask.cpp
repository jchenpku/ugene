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

#include <U2Core/DNAAlphabet.h>
#include <U2Core/L10n.h>
#include <U2Core/MSAUtils.h>
#include <U2Core/MultipleSequenceAlignmentObject.h>
#include <U2Core/U2SafePoints.h>

#include "MsaContentFilterTask.h"
#include "../ProjectFilterNames.h"

namespace U2 {

//////////////////////////////////////////////////////////////////////////
/// ObjectNameFilterTask
//////////////////////////////////////////////////////////////////////////

MsaContentFilterTask::MsaContentFilterTask(const ProjectTreeControllerModeSettings &settings, const QList<QPointer<Document> > &docs)
    : AbstractProjectFilterTask(settings, ProjectFilterNames::MSA_CONTENT_FILTER_NAME, docs)
{
    filteredObjCountPerIteration = 1;
}

bool MsaContentFilterTask::filterAcceptsObject(GObject *obj) {
    MultipleSequenceAlignmentObject *msaObject = qobject_cast<MultipleSequenceAlignmentObject *>(obj);
    CHECK(NULL != msaObject, false);

    foreach(const QString &pattern, settings.tokensToShow) {
        if (patternFitsMsaAlphabet(msaObject, pattern) && msaContainsPattern(msaObject, pattern)) {
            return true;
        }
    }
    return false;
}

bool MsaContentFilterTask::patternFitsMsaAlphabet(MultipleSequenceAlignmentObject *msaObject, const QString &pattern) {
    SAFE_POINT(NULL != msaObject, L10N::nullPointerError("MSA object"), false);
    SAFE_POINT(!pattern.isEmpty(), "Empty pattern to search", false);

    const DNAAlphabet *alphabet = msaObject->getAlphabet();
    SAFE_POINT(NULL != alphabet, L10N::nullPointerError("MSA alphabet"), false);

    const QByteArray searchStr = pattern.toUpper().toLatin1();
    return alphabet->containsAll(searchStr.constData(), searchStr.length());
}

bool MsaContentFilterTask::msaContainsPattern(MultipleSequenceAlignmentObject *msaObject, const QString &pattern) {
    SAFE_POINT(NULL != msaObject, L10N::nullPointerError("MSA object"), false);
    SAFE_POINT(!pattern.isEmpty(), "Empty pattern to search", false);

    const MultipleSequenceAlignment msa = msaObject->getMsa();
    const QByteArray searchStr = pattern.toUpper().toLatin1();

    for (int i = 0, n = msa->getNumRows(); i < n; ++i) {
        const MultipleSequenceAlignmentRow row = msa->getMsaRow(i);
        for (int j = 0; j < (msa->getLength() - searchStr.length() + 1); ++j) {
            const char c = row->charAt(j);
            int altenateLength = 0;
            if (U2Msa::GAP_CHAR != c && MSAUtils::equalsIgnoreGaps(row, j, searchStr, altenateLength)) {
                return true;
            }
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////
/// ObjectNameFilterTaskFactory
//////////////////////////////////////////////////////////////////////////

AbstractProjectFilterTask * MsaContentFilterTaskFactory::createNewTask(const ProjectTreeControllerModeSettings &settings,
    const QList<QPointer<Document> > &docs) const
{
    return new MsaContentFilterTask(settings, docs);
}

} // namespace U2
