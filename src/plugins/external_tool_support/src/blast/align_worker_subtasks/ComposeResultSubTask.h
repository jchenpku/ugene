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

#ifndef _U2_COMPOSE_RESULT_SUBTASK_H_
#define _U2_COMPOSE_RESULT_SUBTASK_H_

#include <U2Core/Task.h>

#include <U2Lang/DbiDataHandler.h>
#include <U2Lang/DbiDataStorage.h>

namespace U2 {
namespace Workflow {

class BlastAndSwReadTask;

class ComposeResultSubTask : public Task {
    Q_OBJECT
public:
    ComposeResultSubTask(const SharedDbiDataHandler &reference,
                         const QList<SharedDbiDataHandler> &reads,
                         const QList<BlastAndSwReadTask*> subTasks, DbiDataStorage *storage);
    void prepare();
    void run();
    const SharedDbiDataHandler& getAlignment() const;
    const SharedDbiDataHandler& getAnnotations() const;

private:
    BlastAndSwReadTask * getBlastSwTask(int readNum);
    DNASequence getReadSequence(int readNum);
    DNASequence getReferenceSequence();
    U2MsaRowGapModel getReferenceGaps();
    U2MsaRowGapModel getShiftedGaps(int rowNum);
     void insertShiftedGapsIntoReference(MultipleSequenceAlignment &alignment, const U2MsaRowGapModel &gaps);
    void insertShiftedGapsIntoRead(MultipleSequenceAlignment &alignment, int readNum, int rowNum, const U2MsaRowGapModel &gaps);
    void createAlignmentAndAnnotations();
    U2Region getReadRegion(const MultipleSequenceAlignmentRow &readRow, const U2MsaRowGapModel &referenceGapModel) const;
    U2Location getLocation(const U2Region &region, bool isComplement);

private:
    const SharedDbiDataHandler reference;
    const QList<SharedDbiDataHandler> reads;
    const QList<BlastAndSwReadTask*> subTasks;
    DbiDataStorage *storage;
    SharedDbiDataHandler msa;
    SharedDbiDataHandler annotations;
};

} // namespace Workflow
} // namespace U2

#endif // _U2_COMPOSE_RESULT_SUBTASK_H_