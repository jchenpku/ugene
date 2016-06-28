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

#include "MultipleSequenceAlignmentObject.h"

#include <U2Core/DNASequence.h>
#include <U2Core/GHints.h>
#include <U2Core/MultipleSequenceAlignmentExporter.h>
#include <U2Core/MultipleSequenceAlignmentImporter.h>
#include <U2Core/MSAUtils.h>
#include <U2Core/MsaDbiUtils.h>
#include <U2Core/U2AlphabetUtils.h>
#include <U2Core/U2Mod.h>
#include <U2Core/U2MsaDbi.h>
#include <U2Core/U2ObjectDbi.h>
#include <U2Core/U2OpStatusUtils.h>
#include <U2Core/U2SafePoints.h>

namespace U2 {

MsaSavedState::MsaSavedState():lastState(MultipleSequenceAlignment()){}

MultipleSequenceAlignment MsaSavedState::getState() const{
    return lastState;
}

void MsaSavedState::setState(const MultipleSequenceAlignment& state){
    lastState = state;
}

MultipleSequenceAlignmentObject::MultipleSequenceAlignmentObject(const QString& name, const U2EntityRef& msaRef, const QVariantMap& hintsMap, const MultipleSequenceAlignment &alnData)
    : GObject(GObjectTypes::MULTIPLE_SEQUENCE_ALIGNMENT, name, hintsMap), cachedMsa(alnData), savedState(new MsaSavedState)
{
    entityRef = msaRef;

    if (!cachedMsa.isEmpty()) {
        dataLoaded = true;
    }
}

MultipleSequenceAlignmentObject::~MultipleSequenceAlignmentObject(){
    emit si_invalidateAlignmentObject();
    delete savedState;
}

void MultipleSequenceAlignmentObject::setTrackMod(U2TrackModType trackMod, U2OpStatus& os) {
    // Prepare the connection
    DbiConnection con(entityRef.dbiRef, os);
    CHECK_OP(os, );

    U2ObjectDbi* objDbi = con.dbi->getObjectDbi();
    SAFE_POINT(NULL != objDbi, "NULL Object Dbi!",);

    // Set the new status
    objDbi->setTrackModType(entityRef.entityId, trackMod, os);
}

const MultipleSequenceAlignment & MultipleSequenceAlignmentObject::getMAlignment() const {
    ensureDataLoaded();
    return cachedMsa;
}

void MultipleSequenceAlignmentObject::updateCachedMAlignment(const MsaModificationInfo &mi, const QList<qint64> &removedRowIds)
{
    ensureDataLoaded();
    emit si_startMsaUpdating();

    MultipleSequenceAlignment maBefore = cachedMsa;
    QString oldName = maBefore.getName();

    U2OpStatus2Log os;

    if (mi.alignmentLengthChanged) {
        qint64 msaLength = MsaDbiUtils::getMsaLength(entityRef, os);
        SAFE_POINT_OP(os, );
        if (msaLength != cachedMsa.getLength()) {
            cachedMsa.setLength(msaLength);
        }
    }

    if (mi.alphabetChanged) {
        U2AlphabetId alphabet = MsaDbiUtils::getMsaAlphabet(entityRef, os);
        SAFE_POINT_OP(os, );
        if (alphabet.id != cachedMsa.getAlphabet()->getId() && !alphabet.id.isEmpty()) {
            const DNAAlphabet* newAlphabet = U2AlphabetUtils::getById(alphabet);
            cachedMsa.setAlphabet(newAlphabet);
        }
    }

     if (mi.modifiedRowIds.isEmpty() && removedRowIds.isEmpty()) { // suppose that in this case all the alignment has changed
        loadAlignment(os);
        SAFE_POINT_OP(os, );
    } else { // only specified rows were changed
        if (!removedRowIds.isEmpty()) {
            foreach (qint64 rowId, removedRowIds) {
                const int rowIndex = cachedMsa.getRowIndexByRowId(rowId, os);
                SAFE_POINT_OP(os, );
                cachedMsa.removeRow(rowIndex, os);
                SAFE_POINT_OP(os, );
            }
        }
        if (!mi.modifiedRowIds.isEmpty()) {
            MultipleSequenceAlignmentExporter alExporter;
            QList<MsaRowReplacementData> rowsAndSeqs = alExporter.getAlignmentRows(entityRef.dbiRef, entityRef.entityId,
                mi.modifiedRowIds, os);
            SAFE_POINT_OP(os, );
            foreach (const MsaRowReplacementData &data, rowsAndSeqs) {
                const int rowIndex = cachedMsa.getRowIndexByRowId(data.row.rowId, os);
                SAFE_POINT_OP(os, );
                cachedMsa.setRowContent(rowIndex, data.sequence.seq);
                cachedMsa.setRowGapModel(rowIndex, data.row.gaps);
                cachedMsa.renameRow(rowIndex, data.sequence.getName());
            }
        }
    }

    setModified(true);
    if (!mi.middleState) {
        emit si_alignmentChanged(maBefore, mi);

        if (cachedMsa.isEmpty() && !maBefore.isEmpty()) {
            emit si_alignmentBecomesEmpty(true);
        } else if (!cachedMsa.isEmpty() && maBefore.isEmpty()) {
            emit si_alignmentBecomesEmpty(false);
        }

        const QString newName = cachedMsa.getName();
        if (oldName != newName) {
            setGObjectNameNotDbi(newName);
        }
    }
    if (!removedRowIds.isEmpty()) {
        emit si_rowsRemoved(removedRowIds);
    }
    if (cachedMsa.getAlphabet()->getId() != maBefore.getAlphabet()->getId()) {
        emit si_alphabetChanged(mi, maBefore.getAlphabet());
    }
}

void MultipleSequenceAlignmentObject::setMAlignment(const MultipleSequenceAlignment& newMa, MsaModificationInfo mi, const QVariantMap& hints) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );

    U2OpStatus2Log os;
    MsaDbiUtils::updateMsa(entityRef, newMa, os);
    SAFE_POINT_OP(os, );

    mi.hints = hints;
    updateCachedMAlignment(mi);
}

void MultipleSequenceAlignmentObject::copyGapModel(const QList<MultipleSequenceAlignmentRow> &copyRows) {
    const MultipleSequenceAlignment &tmp = getMAlignment();
    const QList<MultipleSequenceAlignmentRow> &oldRows = tmp.getRows();

    SAFE_POINT(oldRows.count() == copyRows.count(), "Different rows count", );

    QMap<qint64, QList<U2MaGap> > newGapModel;
    QList<MultipleSequenceAlignmentRow>::ConstIterator ori = oldRows.begin();
    QList<MultipleSequenceAlignmentRow>::ConstIterator cri = copyRows.begin();
    for (; ori != oldRows.end(); ori++, cri++) {
        newGapModel[ori->getRowId()] = cri->getGapModel();
    }

    U2OpStatus2Log os;
    updateGapModel(newGapModel, os);
}

char MultipleSequenceAlignmentObject::charAt(int seqNum, int pos) const {
    const MultipleSequenceAlignment &msa = getMAlignment();
    return msa.charAt(seqNum, pos);
}

void MultipleSequenceAlignmentObject::saveState(){
    const MultipleSequenceAlignment &msa = getMAlignment();
    emit si_completeStateChanged(false);
    savedState->setState(msa);
}

void MultipleSequenceAlignmentObject::releaseState() {
    if(!isStateLocked()) {
        emit si_completeStateChanged(true);

        MultipleSequenceAlignment maBefore = savedState->getState();
        CHECK(maBefore != getMAlignment(), );
        setModified(true);

        MsaModificationInfo mi;
        emit si_alignmentChanged(maBefore, mi);

        if (cachedMsa.isEmpty() && !maBefore.isEmpty()) {
            emit si_alignmentBecomesEmpty(true);
        } else if (!cachedMsa.isEmpty() && maBefore.isEmpty()) {
            emit si_alignmentBecomesEmpty(false);
        }
    }
}

GObject* MultipleSequenceAlignmentObject::clone(const U2DbiRef& dstDbiRef, U2OpStatus& os, const QVariantMap &hints) const {
    DbiOperationsBlock opBlock(dstDbiRef, os);
    Q_UNUSED(opBlock);
    CHECK_OP(os, NULL);

    GHintsDefaultImpl *gHints = new GHintsDefaultImpl(getGHintsMap());
    gHints->setAll(hints);
    const QString dstFolder = gHints->get(DocumentFormat::DBI_FOLDER_HINT, U2ObjectDbi::ROOT_FOLDER).toString();

    MultipleSequenceAlignment msa = getMAlignment();
    MultipleSequenceAlignmentObject *clonedObj = MultipleSequenceAlignmentImporter::createAlignment(dstDbiRef, dstFolder, msa, os);
    CHECK_OP_EXT(os, delete gHints, NULL);

    clonedObj->setGHints(gHints);
    clonedObj->setIndexInfo(getIndexInfo());
    return clonedObj;
}

void MultipleSequenceAlignmentObject::insertGap(U2Region rows, int pos, int count) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );
    const MultipleSequenceAlignment &msa = getMAlignment();
    int startSeq = rows.startPos;
    int endSeq = startSeq + rows.length;

    QList<qint64> rowIdsToInsert;
    for (int i = startSeq; i < endSeq; ++i) {
        qint64 rowId = msa.getRow(i).getRowId();
        rowIdsToInsert.append(rowId);
    }

    U2OpStatus2Log os;
    MsaDbiUtils::insertGaps(entityRef, rowIdsToInsert, pos, count, os);
    SAFE_POINT_OP(os, );

    MsaModificationInfo mi;
    mi.sequenceListChanged = false;
    mi.modifiedRowIds = rowIdsToInsert;
    updateCachedMAlignment(mi);
}

int MultipleSequenceAlignmentObject::getMaxWidthOfGapRegion( const U2Region &rows, int pos, int maxGaps,
    U2OpStatus &os )
{
    const MultipleSequenceAlignment &msa = getMAlignment( );
    SAFE_POINT_EXT( U2Region( 0, msa.getNumRows( ) ).contains( rows ) && 0 <= pos && 0 <= maxGaps
        && msa.getLength( ) > pos, os.setError( "Illegal parameters of the gap region!" ), 0 );

    const int maxRemovedGaps = qBound(0, maxGaps, msa.getLength() - pos);
    // check if there is nothing to remove
    if ( 0 == maxRemovedGaps ) {
        return 0;
    }

    int removingGapColumnCount = maxRemovedGaps;
    bool isRegionInRowTrailingGaps = true;
    // iterate through given rows to determine the width of the continuous gap region
    for ( int rowCount = rows.startPos; rowCount < rows.endPos( ); ++rowCount ) {
        int gapCountInCurrentRow = 0;
        // iterate through current row bases to determine gap count
        while ( gapCountInCurrentRow < maxRemovedGaps ) {
            const char currentSymbol = msa.charAt( rowCount,
                pos + maxGaps - gapCountInCurrentRow - 1 );
            if ( MAlignment_GapChar != currentSymbol ) {
                break;
            }
            gapCountInCurrentRow++;
        }

        // determine if the given area intersects a row in the area of trailing gaps
        if ( 0 != gapCountInCurrentRow && isRegionInRowTrailingGaps ) {
            int trailingPosition = pos + maxRemovedGaps - gapCountInCurrentRow;
            if ( msa.getLength( ) != trailingPosition ) {
                while ( msa.getLength( ) > trailingPosition && isRegionInRowTrailingGaps ) {
                    isRegionInRowTrailingGaps &= ( MAlignment_GapChar == msa.charAt( rowCount,
                        trailingPosition ) );
                    ++trailingPosition;
                }
            }
        } else if ( isRegionInRowTrailingGaps ) {
            isRegionInRowTrailingGaps = false;
        }

        if ( 0 == gapCountInCurrentRow ) {
            // don't do anything if there is a row without gaps
            return 0;
        }
        removingGapColumnCount = qMin( removingGapColumnCount, gapCountInCurrentRow );
    }

    if ( isRegionInRowTrailingGaps ) {
        if (rows.length == getNumRows() && rows.startPos == 0) {
            return qMin(getLength() - pos, (qint64)maxGaps);
        } else {
            return 0;
        }
    }

    return removingGapColumnCount;
}

int MultipleSequenceAlignmentObject::deleteGap( const U2Region &rows, int pos, int maxGaps, U2OpStatus &os ) {
    SAFE_POINT( !isStateLocked( ), "Alignment state is locked!", 0 );

    const int removingGapColumnCount = getMaxWidthOfGapRegion( rows, pos, maxGaps, os );
    SAFE_POINT_OP( os, 0 );
    if ( 0 == removingGapColumnCount ) {
        return 0;
    } else if ( removingGapColumnCount < maxGaps ) {
        pos += maxGaps - removingGapColumnCount;
    }
    QList<qint64> modifiedRowIds;
    modifiedRowIds.reserve( rows.length );

    MultipleSequenceAlignment msa = getMAlignment( );
    // iterate through given rows to update each of them in DB
    for ( int rowCount = rows.startPos; rowCount < rows.endPos( ); ++rowCount ) {
        msa.removeChars( rowCount, pos, removingGapColumnCount, os );
        CHECK_OP( os, 0 );

        const MultipleSequenceAlignmentRow &row = msa.getRow( rowCount );
        MsaDbiUtils::updateRowGapModel( entityRef, row.getRowId( ), row.getGapModel( ), os );
        CHECK_OP( os, 0 );
        modifiedRowIds << row.getRowId( );
    }
    if (rows.startPos == 0 && rows.length == getNumRows()) {
        // delete columns
        MsaDbiUtils::updateMsaLength( entityRef, getLength() - removingGapColumnCount, os);
        CHECK_OP( os, 0);
    }

    MsaModificationInfo mi;
    mi.sequenceListChanged = false;
    mi.modifiedRowIds = modifiedRowIds;
    updateCachedMAlignment(mi);
    return removingGapColumnCount;
}

void MultipleSequenceAlignmentObject::removeRow(int rowIdx) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );

    const MultipleSequenceAlignment &msa = getMAlignment();
    SAFE_POINT(rowIdx >= 0 && rowIdx < msa.getNumRows(), "Invalid row index!", );
    const MultipleSequenceAlignmentRow& row = msa.getRow(rowIdx);
    qint64 rowId = row.getRowDBInfo().rowId;

    U2OpStatus2Log os;
    MsaDbiUtils::removeRow(entityRef, rowId, os);
    SAFE_POINT_OP(os, );

    MsaModificationInfo mi;
    mi.sequenceContentChanged = false;
    mi.alignmentLengthChanged = false;

    QList<qint64> removedRowIds;
    removedRowIds << rowId;

    updateCachedMAlignment(mi, removedRowIds);
}

void MultipleSequenceAlignmentObject::updateRow(int rowIdx, const QString& name, const QByteArray& seqBytes, const QList<U2MaGap>& gapModel, U2OpStatus& os) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );

    const MultipleSequenceAlignment &msa = getMAlignment();
    SAFE_POINT(rowIdx >= 0 && rowIdx < msa.getNumRows(), "Invalid row index!", );
    const MultipleSequenceAlignmentRow& row = msa.getRow(rowIdx);
    qint64 rowId = row.getRowDBInfo().rowId;

    MsaDbiUtils::updateRowContent(entityRef, rowId, seqBytes, gapModel, os);
    CHECK_OP(os, );

    MsaDbiUtils::renameRow(entityRef, rowId, name, os);
    CHECK_OP(os, );
}

void MultipleSequenceAlignmentObject::setGObjectName(const QString& newName) {
    ensureDataLoaded();
    CHECK(cachedMsa.getName() != newName, );

    if (!isStateLocked()) {
        U2OpStatus2Log os;
        MsaDbiUtils::renameMsa(entityRef, newName, os);
        CHECK_OP(os, );

        updateCachedMAlignment();
    } else {
        GObject::setGObjectName(newName);
        cachedMsa.setName(newName);
    }
}

QList<qint64> getRowsAffectedByDeletion( const MultipleSequenceAlignment &msa,
    const QList<qint64> &removedRowIds )
{
    QList<qint64> rowIdsAffectedByDeletion;
    U2OpStatus2Log os;
    const QList<qint64> msaRows = msa.getRowsIds( );
    int previousRemovedRowIndex = -1;
    foreach ( qint64 removedRowId, removedRowIds ) {
        if ( -1 != previousRemovedRowIndex ) {
            const int currentRemovedRowIndex = msa.getRowIndexByRowId( removedRowId, os );
            SAFE_POINT_OP(os, QList<qint64>( ) );
            SAFE_POINT( currentRemovedRowIndex > previousRemovedRowIndex,
                "Rows order violation!", QList<qint64>( ) );
            const int countOfUnchangedRowsBetween = currentRemovedRowIndex
                - previousRemovedRowIndex - 1;
            if ( 0 < countOfUnchangedRowsBetween ) {
                for ( int middleRowIndex = previousRemovedRowIndex + 1;
                    middleRowIndex < currentRemovedRowIndex; ++middleRowIndex )
                {
                    rowIdsAffectedByDeletion += msaRows[middleRowIndex];
                }
            }
        }
        previousRemovedRowIndex = msa.getRowIndexByRowId( removedRowId, os );
        SAFE_POINT_OP(os, QList<qint64>( ) );
    }
    const int lastDeletedRowIndex = msa.getRowIndexByRowId( removedRowIds.last( ), os );
    SAFE_POINT_OP(os, QList<qint64>( ) );
    if ( lastDeletedRowIndex < msaRows.size( ) - 1 ) { // if the last removed row was not in the bottom of the msa
        rowIdsAffectedByDeletion += msaRows.mid( lastDeletedRowIndex + 1 );
    }
    return rowIdsAffectedByDeletion;
}

template<typename T>
inline QList<T> mergeLists( const QList<T> &first, const QList<T> &second ) {
    QList<T> result = first;
    foreach ( const T &item, second ) {
        if ( !result.contains( item ) ) {
            result.append( item );
        }
    }
    return result;
}

void MultipleSequenceAlignmentObject::removeRegion(int startPos, int startRow, int nBases, int nRows, bool removeEmptyRows, bool track) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );
    QList<qint64> modifiedRowIds;
    const MultipleSequenceAlignment &msa = getMAlignment();
    const QList<MultipleSequenceAlignmentRow> &msaRows = msa.getRows();
    SAFE_POINT(nRows > 0 && startRow >= 0 && startRow + nRows <= msaRows.size() && startPos + nBases <= msa.getLength(), "Invalid parameters!", );
    QList<MultipleSequenceAlignmentRow>::ConstIterator it = msaRows.begin() + startRow;
    QList<MultipleSequenceAlignmentRow>::ConstIterator end = it + nRows;
    for (; it != end; it++) {
        modifiedRowIds << it->getRowId();
    }

    U2OpStatus2Log os;
    MsaDbiUtils::removeRegion(entityRef, modifiedRowIds, startPos, nBases, os);
    SAFE_POINT_OP(os, );

    QList<qint64> removedRows;
    if (removeEmptyRows) {
        removedRows = MsaDbiUtils::removeEmptyRows(entityRef, modifiedRowIds, os);
        SAFE_POINT_OP(os, );
        if ( !removedRows.isEmpty( ) ) { // suppose that if at least one row in msa was removed then
            // all the rows below it were changed
            const QList<qint64> rowIdsAffectedByDeletion = getRowsAffectedByDeletion( msa,
                removedRows );
            foreach ( qint64 removedRowId, removedRows ) { // removed rows ain't need to be update
                modifiedRowIds.removeAll( removedRowId );
            }
            modifiedRowIds = mergeLists( modifiedRowIds, rowIdsAffectedByDeletion );
        }
    }
    if (track || !removedRows.isEmpty()) {
        MsaModificationInfo mi;
        mi.modifiedRowIds = modifiedRowIds;
        updateCachedMAlignment(mi, removedRows);
    }
    if (!removedRows.isEmpty()) {
        emit si_rowsRemoved(removedRows);
    }
}

void MultipleSequenceAlignmentObject::replaceCharacter(int startPos, int rowIndex, char newChar) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );
    const MultipleSequenceAlignment &msa = getMAlignment();
    SAFE_POINT(rowIndex >= 0 && startPos + 1 <= msa.getLength(), "Invalid parameters!", );
    qint64 modifiedRowId = msa.getRow(rowIndex).getRowId();

    //msa.setAlphabet(newAlphabet);

    U2OpStatus2Log os;
    if (newChar != MAlignment_GapChar) {
        MsaDbiUtils::replaceCharacterInRow(entityRef, modifiedRowId, startPos, newChar, os);
    } else {
        MsaDbiUtils::removeRegion(entityRef, QList<qint64>() << modifiedRowId, startPos, 1, os);
        MsaDbiUtils::insertGaps(entityRef, QList<qint64>() << modifiedRowId, startPos, 1, os);
    }
    SAFE_POINT_OP(os, );

    MsaModificationInfo mi;
    mi.sequenceContentChanged = true;
    mi.sequenceListChanged = false;
    mi.alignmentLengthChanged = false;
    mi.modifiedRowIds << modifiedRowId;

    if (newChar != ' ' && !msa.getAlphabet()->contains(newChar)) {
        const DNAAlphabet *alp = U2AlphabetUtils::findBestAlphabet(QByteArray(1, newChar));
        const DNAAlphabet *newAlphabet = U2AlphabetUtils::deriveCommonAlphabet(alp, msa.getAlphabet());
        SAFE_POINT(NULL != newAlphabet, "Common alphabet is NULL!", );

        if (newAlphabet->getId() != msa.getAlphabet()->getId()) {
            MsaDbiUtils::updateMsaAlphabet(entityRef, newAlphabet->getId(), os);
            mi.alphabetChanged = true;
            SAFE_POINT_OP(os, );
        }
    }

    updateCachedMAlignment(mi, QList<qint64>());
}

void MultipleSequenceAlignmentObject::renameRow(int rowIdx, const QString& newName) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );

    const MultipleSequenceAlignment &msa = getMAlignment();
    SAFE_POINT(rowIdx >= 0 && rowIdx < msa.getNumRows(), "Invalid row index!", );
    const MultipleSequenceAlignmentRow& row = msa.getRow(rowIdx);
    qint64 rowId = row.getRowDBInfo().rowId;

    U2OpStatus2Log os;
    MsaDbiUtils::renameRow(entityRef, rowId, newName, os);
    SAFE_POINT_OP(os, );

    MsaModificationInfo mi;
    mi.alignmentLengthChanged = false;
    updateCachedMAlignment(mi);
}

void MultipleSequenceAlignmentObject::crop(U2Region window, const QSet<QString>& rowNames) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );
    const MultipleSequenceAlignment &msa = getMAlignment();

    QList<qint64> rowIds;
    for (int i = 0; i < msa.getNumRows(); ++i) {
        QString rowName = msa.getRow(i).getName();
        if (rowNames.contains(rowName)) {
            qint64 rowId = msa.getRow(i).getRowId();
            rowIds.append(rowId);
        }
    }

    U2OpStatus2Log os;
    MsaDbiUtils::crop(entityRef, rowIds, window.startPos, window.length, os);
    SAFE_POINT_OP(os, );

    updateCachedMAlignment();
}

QList<qint64> MultipleSequenceAlignmentObject::getColumnsWithGaps(int requiredGapCount) const {
    const MultipleSequenceAlignment &msa = getMAlignment();
    const int length = msa.getLength();
    if (GAP_COLUMN_ONLY == requiredGapCount) {
        requiredGapCount = msa.getNumRows();
    }
    QList<qint64> colsForDelete;
    for (int i = 0; i < length; i++) { //columns
        int gapCount = 0;
        for (int j = 0; j < msa.getNumRows(); j++) { //sequences
            if (charAt(j, i) == MAlignment_GapChar) {
                gapCount++;
            }
        }

        if (gapCount >= requiredGapCount) {
            colsForDelete.prepend(i); //invert order
        }
    }
    return colsForDelete;
}

void MultipleSequenceAlignmentObject::deleteColumnWithGaps(int requiredGapCount, U2OpStatus &os) {
    QList<qint64> colsForDelete = getColumnsWithGaps(requiredGapCount);
    if (getLength() == colsForDelete.count()) {
        return;
    }

    QList<U2Region> horizontalRegionsToDelete;
    foreach (qint64 columnNumber, colsForDelete) {
        bool columnMergedWithPrevious = false;
        if (!horizontalRegionsToDelete.isEmpty()) {
            U2Region &lastRegion = horizontalRegionsToDelete.last();
            if (lastRegion.startPos == columnNumber + 1) {
                --lastRegion.startPos;
                ++lastRegion.length;
                columnMergedWithPrevious = true;
            } else if (lastRegion.endPos() == columnNumber) {
                ++lastRegion.length;
                columnMergedWithPrevious = true;
            }
        }

        if (!columnMergedWithPrevious) {
            horizontalRegionsToDelete.append(U2Region(columnNumber, 1));
        }
    }

    QList<U2Region>::const_iterator columns = horizontalRegionsToDelete.constBegin();
    const QList<U2Region>::const_iterator end = horizontalRegionsToDelete.constEnd();

    for (int counter = 0; columns != end; ++columns, counter++) {
        removeRegion((*columns).startPos, 0, (*columns).length, getNumRows(), true, (end - 1 == columns));
        os.setProgress(100 * counter / horizontalRegionsToDelete.size());
    }
    updateCachedMAlignment();
}

void MultipleSequenceAlignmentObject::deleteColumnWithGaps(int requiredGapCount) {
    U2OpStatusImpl os;
    deleteColumnWithGaps(requiredGapCount, os);
    SAFE_POINT_OP(os, );
}

void MultipleSequenceAlignmentObject::updateGapModel(QMap<qint64, QList<U2MaGap> > rowsGapModel, U2OpStatus& os) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );

    const MultipleSequenceAlignment &msa = getMAlignment();

    QList<qint64> modifiedRowIds;
    foreach (qint64 rowId, rowsGapModel.keys()) {
        if (!msa.getRowsIds().contains(rowId)) {
            os.setError("Can't update gaps of a multiple alignment!");
            return;
        }

        MsaDbiUtils::updateRowGapModel(entityRef, rowId, rowsGapModel.value(rowId), os);
        CHECK_OP(os, );
        modifiedRowIds.append(rowId);
    }

    MsaModificationInfo mi;
    mi.sequenceListChanged = false;
    updateCachedMAlignment(mi);
}

QMap<qint64, QList<U2MaGap> > MultipleSequenceAlignmentObject::getGapModel() const {
    QMap<qint64, QList<U2MaGap> > rowsGapModel;
    const MultipleSequenceAlignment &msa = getMAlignment();
    foreach (const MultipleSequenceAlignmentRow& curRow, msa.getRows()) {
        rowsGapModel[curRow.getRowId()] = curRow.getGapModel();
    }
    return rowsGapModel;
}

void MultipleSequenceAlignmentObject::moveRowsBlock(int firstRow, int numRows, int shift)
{
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );

    const MultipleSequenceAlignment &msa = getMAlignment();
    QList<qint64> rowIds = msa.getRowsIds();
    QList<qint64> rowsToMove;

    for (int i = 0; i < numRows; ++i) {
        rowsToMove << rowIds[firstRow + i];
    }

    U2OpStatusImpl os;
    MsaDbiUtils::moveRows(entityRef, rowsToMove, shift, os);
    CHECK_OP(os, );

    updateCachedMAlignment();
}

void MultipleSequenceAlignmentObject::updateRowsOrder(const QList<qint64>& rowIds, U2OpStatus& os) {
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );

    MsaDbiUtils::updateRowsOrder(entityRef, rowIds, os);
    CHECK_OP(os, );

    MsaModificationInfo mi;
    mi.alignmentLengthChanged = false;
    updateCachedMAlignment(mi);
}

int MultipleSequenceAlignmentObject::shiftRegion( int startPos, int startRow, int nBases, int nRows, int shift )
{
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", 0 );
    SAFE_POINT(!isRegionEmpty(startPos, startRow, nBases, nRows), "Region is empty!", 0 );
    SAFE_POINT( 0 <= startPos && 0 <= startRow && 0 < nBases && 0 < nRows,
        "Invalid parameters of selected region encountered", 0 );
    U2OpStatusImpl os;

    int n = 0;
    if (shift > 0) {
        // if some trailing gaps are selected --> save them!
        if (startPos + nBases + shift > getLength()) {
            bool increaseAlignmentLen = true;
            for (int i = startRow; i < startRow + nRows; i++) {
                const MultipleSequenceAlignmentRow& row = getRow(i);
                int rowLen = row.getRowLengthWithoutTrailing();
                if (rowLen >= startPos + nBases + shift) {
                    increaseAlignmentLen = false;
                    break;
                }
            }
            if (increaseAlignmentLen) {
                MsaDbiUtils::updateMsaLength(entityRef, startPos + nBases + shift, os);
                SAFE_POINT_OP( os, 0 );
                updateCachedMAlignment();
            }
        }

        insertGap(U2Region(startRow,nRows), startPos, shift);
        n = shift;
    } else if ( 0 < startPos ) {
        if (0 > startPos + shift) {
            shift = -startPos;
        }
        n = -deleteGap(U2Region(startRow, nRows), startPos + shift, -shift, os);
        SAFE_POINT_OP( os, 0 );
    }
    return n;
}

bool MultipleSequenceAlignmentObject::isRegionEmpty(int startPos, int startRow, int numChars, int numRows) const
{
    const MultipleSequenceAlignment &msa = getMAlignment();
    bool isBlockEmpty = true;
    for (int row = startRow; row < startRow + numRows && isBlockEmpty; ++row ) {
        for( int pos = startPos; pos < startPos + numChars; ++pos ) {
            const MultipleSequenceAlignmentRow& r = msa.getRows().at(row);
            if (r.charAt(pos) != MAlignment_GapChar) {
                isBlockEmpty = false;
                break;
            }
        }
    }
    return isBlockEmpty;
}

const DNAAlphabet* MultipleSequenceAlignmentObject::getAlphabet() const {
    const MultipleSequenceAlignment &msa = getMAlignment();
    return msa.getAlphabet();
}

qint64 MultipleSequenceAlignmentObject::getLength() const {
    const MultipleSequenceAlignment &msa = getMAlignment();
    return msa.getLength();
}

qint64 MultipleSequenceAlignmentObject::getNumRows() const {
    const MultipleSequenceAlignment &msa = getMAlignment();
    return msa.getNumRows();
}

const MultipleSequenceAlignmentRow& MultipleSequenceAlignmentObject::getRow(int row) const {
//    MultipleSequenceAlignment msa = getMAlignment();
//    return msa.getRow(row);
    ensureDataLoaded();
    return cachedMsa.getRow(row);
}

int MultipleSequenceAlignmentObject::getRowPosById(qint64 rowId) const {
    const MultipleSequenceAlignment &msa = getMAlignment();
    return msa.getRowsIds().indexOf(rowId);
}

static bool _registerMeta() {
    qRegisterMetaType<MsaModificationInfo>("MsaModificationInfo");
    return true;
}

bool MsaModificationInfo::registerMeta = _registerMeta();

void MultipleSequenceAlignmentObject::sortRowsByList(const QStringList& order) {
    GTIMER(c, t, "MultipleSequenceAlignmentObject::sortRowsByList");
    SAFE_POINT(!isStateLocked(), "Alignment state is locked!", );

    MultipleSequenceAlignment msa = getMAlignment();
    msa.sortRowsByList(order);
    CHECK(msa.getRowsIds() != cachedMsa.getRowsIds(), );

    U2OpStatusImpl os;
    MsaDbiUtils::updateRowsOrder(entityRef, msa.getRowsIds(), os);
    SAFE_POINT_OP(os, );

    MsaModificationInfo mi;
    mi.alignmentLengthChanged = false;
    mi.sequenceContentChanged = false;
    mi.sequenceListChanged = false;
    updateCachedMAlignment(mi);
}

void MultipleSequenceAlignmentObject::loadDataCore(U2OpStatus &os) {
    DbiConnection con(entityRef.dbiRef, os);
    CHECK_OP(os,);
    loadAlignment(os);
}

void MultipleSequenceAlignmentObject::loadAlignment(U2OpStatus &os) {
    MultipleSequenceAlignmentExporter alExporter;
    cachedMsa = alExporter.getAlignment(entityRef.dbiRef, entityRef.entityId, os);
}

}//namespace