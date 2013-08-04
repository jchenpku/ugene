/**
 * UGENE - Integrated Bioinformatics Tools.
 * Copyright (C) 2008-2013 UniPro <ugene@unipro.ru>
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

#include <QtGui/QLineEdit>
#include <QtGui/QToolButton>
#include <QtGui/QComboBox>
#include <QtGui/QMessageBox>

#include <U2Core/AppContext.h>

#include "ui/ui_SearchGenbankSequenceDialog.h"

#include "SearchGenbankSequenceDialogController.h"


namespace U2 {

SearchGenbankSequenceDialogController::SearchGenbankSequenceDialogController(QWidget *p) : QDialog(p), searchTask(NULL)
{
    ui = new Ui_SearchGenbankSequenceDialog();
    ui->setupUi(this);

    queryBlockController = new QueryBuilderController(this);

    connect(ui->searchButton, SIGNAL(clicked()), SLOT(sl_searchButtonClicked()) );
    connect( AppContext::getTaskScheduler(), SIGNAL(si_stateChanged(Task*)), SLOT(sl_taskStateChanged(Task*)) );


}

SearchGenbankSequenceDialogController::~SearchGenbankSequenceDialogController()
{


}

void SearchGenbankSequenceDialogController::addQueryBlockWidget(QWidget *w)
{
    ui->queryBuilderBox->layout()->addWidget(w);
}

void SearchGenbankSequenceDialogController::removeQueryBlockWidget(QWidget *w)
{
    ui->queryBuilderBox->layout()->removeWidget(w);
}

void SearchGenbankSequenceDialogController::setQueryText(const QString &queryText)
{
    ui->queryEdit->setText(queryText);
}

void SearchGenbankSequenceDialogController::sl_searchButtonClicked()
{
    QString query = ui->queryEdit->toPlainText();

    if (query.isEmpty()) {
        return;
    }
    

    //searchTask = new EntrezSearchTask("nucleotide", query);
    QString qUrl(EntrezUtils::NCBI_ESEARCH_URL.arg("nucleotide").arg(query));
    searchResultHandler.reset( new ESearchResultHandler() );
    searchTask = new EntrezQueryTask(searchResultHandler.data(), qUrl );

    AppContext::getTaskScheduler()->registerTopLevelTask(searchTask);

}


void SearchGenbankSequenceDialogController::sl_taskStateChanged( Task* task )
{
    if (task->getState() == Task::State_Finished ) {
        if (task == searchTask) {
            ui->treeWidget->clear();
            const QStringList& results =  searchResultHandler->getIdList();
            if (results.size() == 0) {
                QMessageBox::information(this, windowTitle(), tr("No results found corresponding to the query.") );
            } else {
                QString ids = results.join(",");
                QString query(EntrezUtils::NCBI_ESUMMARY_URL.arg("nucleotide").arg(ids));
                summaryResultHandler.reset( new ESummaryResultHandler() );
                summaryTask = new EntrezQueryTask( summaryResultHandler.data() , query);
                AppContext::getTaskScheduler()->registerTopLevelTask(summaryTask);
            }
            searchTask = NULL;
        } else if (task == summaryTask) {
            
            assert(summaryResultHandler);
            const QList<EntrezSummary>& results = summaryResultHandler->getResults();
            
            foreach (const EntrezSummary& desc, results ) {
                QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
                item->setText(0, desc.name);
                item->setText(1, desc.title );
                item->setText(2, QString("%1").arg(desc.size) );
                ui->treeWidget->addTopLevelItem( item );   
            }
            summaryTask = NULL;
        }
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////


QueryBlockWidget::QueryBlockWidget(QueryBuilderController* controller, bool first)
    : conditionBox(NULL), termBox(NULL), queryEdit(NULL)
{
    QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    layout->setMargin(0);

    if (first) {
        QLabel* label = new QLabel("Term:");
        layout->addWidget(label);
    } else {
        conditionBox = new QComboBox(this);
        conditionBox->addItems(ctx.rules);
        connect(conditionBox, SIGNAL(currentIndexChanged(int)), controller, SLOT(sl_updateQuery()) );
        layout->addWidget(conditionBox);
    }

    termBox = new QComboBox(this);
    termBox->addItem("All fields");
    termBox->addItems(ctx.fields);
    connect(termBox, SIGNAL(currentIndexChanged(int)), controller, SLOT(sl_updateQuery()) );

    queryEdit = new QLineEdit(this);
    connect(queryEdit,  SIGNAL(textEdited(const QString&)), controller, SLOT(sl_updateQuery()) );


    layout->addWidget(termBox);
    layout->addWidget(queryEdit);

    if (first) {
        QToolButton* addBlockButton = new QToolButton();
        addBlockButton->setText("+");
        layout->addWidget(addBlockButton);
        connect(addBlockButton,  SIGNAL(clicked()), controller, SLOT(sl_addQueryBlockWidget()) );

    } else {
        QToolButton* removeBlockButton = new QToolButton();
        removeBlockButton->setText("-");
        layout->addWidget(removeBlockButton);
        connect(removeBlockButton,  SIGNAL(clicked()), controller, SLOT(sl_removeQueryBlockWidget()) );
    }

    setLayout(layout);


}


QueryBlockWidget::~QueryBlockWidget()
{

}



QString QueryBlockWidget::getQuery()
{
    QString query;

    query = queryEdit->text();

    if ( query.isEmpty() ) {
        return query;
    }

    if (termBox->currentIndex() != 0) {
        query += "[" + termBox->currentText() + "]";
    }

    if (conditionBox) {
        query.prepend( " " + conditionBox->currentText() + " " );
    }


    return query;

}

////////////////////////////////////////////////////////////////////////////////////////////////////

QueryBuilderController::QueryBuilderController(SearchGenbankSequenceDialogController *p) : QObject(p), parentController(p)
{
    QueryBlockWidget* widget = new QueryBlockWidget(this, true);
    parentController->addQueryBlockWidget(widget);
    queryBlockWidgets.append(widget);
}

QueryBuilderController::~QueryBuilderController()
{

}


void QueryBuilderController::sl_addQueryBlockWidget()
{    
    QueryBlockWidget* widget = new QueryBlockWidget(this, false);
    parentController->addQueryBlockWidget(widget);
    queryBlockWidgets.append(widget);

}

void QueryBuilderController::sl_removeQueryBlockWidget()
{
    QToolButton* callbackButton = qobject_cast<QToolButton*> ( sender() );
    assert(callbackButton);

    QueryBlockWidget* queryBlockWidget = qobject_cast<QueryBlockWidget*> (callbackButton->parentWidget() );
    assert(queryBlockWidget);

    parentController->removeQueryBlockWidget(queryBlockWidget);
    queryBlockWidgets.removeAll(queryBlockWidget);

    queryBlockWidget->deleteLater();

    sl_updateQuery();


}


void QueryBuilderController::sl_updateQuery()
{
    QString query;
    foreach (QueryBlockWidget* w, queryBlockWidgets) {
        query += w->getQuery();
    }

    parentController->setQueryText(query);

}



} //namespace 
