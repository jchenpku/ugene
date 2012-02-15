/**
 * UGENE - Integrated Bioinformatics Tools.
 * Copyright (C) 2008-2012 UniPro <ugene@unipro.ru>
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

#ifndef _U2_CAP3_WORKER_H_
#define _U2_CAP3_WORKER_H_

#include <U2Lang/LocalDomain.h>
#include <U2Lang/WorkflowUtils.h>
#include "CAP3SupportTask.h"

namespace U2 {

namespace LocalWorkflow {


class CAP3Prompter : public PrompterBase<CAP3Prompter> {
    Q_OBJECT
public:
    CAP3Prompter(Actor* p = 0);
protected:
    QString composeRichDoc();
};

class CAP3Worker : public BaseWorker {
    Q_OBJECT
public:
    CAP3Worker(Actor* a);
    
    virtual void init();
    virtual bool isReady();
    virtual Task* tick();
    virtual bool isDone();
    virtual void cleanup();
    
private slots:
    void sl_taskFinished();

protected:
    CommunicationChannel *input, *output;
    QString resultName,transId;
    CAP3SupportTaskSettings cfg;
    
}; 

class CAP3WorkerFactory : public DomainFactory {
public:
    static const QString ACTOR_ID;
    static void init();
    CAP3WorkerFactory() : DomainFactory(ACTOR_ID) {}
    virtual Worker* createWorker(Actor* a) {return new CAP3Worker(a);}
};

} // Workflow namespace
} // U2 namespace

#endif // _U2_CAP3_WORKER_H_
