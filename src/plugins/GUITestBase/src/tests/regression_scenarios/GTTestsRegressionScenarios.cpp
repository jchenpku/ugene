/**
 * UGENE - Integrated Bioinformatics Tools.
 * Copyright (C) 2008-2014 UniPro <ugene@unipro.ru>
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

#include "GTTestsRegressionScenarios.h"

#include "api/GTAction.h"
#include "api/GTCheckBox.h"
#include "api/GTClipboard.h"
#include "api/GTComboBox.h"
#include "api/GTFile.h"
#include "api/GTFileDialog.h"
#include "api/GTGlobals.h"
#include "api/GTKeyboardDriver.h"
#include "api/GTKeyboardUtils.h"
#include "api/GTLineEdit.h"
#include "api/GTMenu.h"
#include "api/GTMouseDriver.h"
#include "api/GTSpinBox.h"
#include "api/GTTableView.h"
#include "api/GTToolbar.h"
#include "api/GTTreeWidget.h"
#include "api/GTWidget.h"

#include "GTUtilsAnnotationsTreeView.h"
#include "GTUtilsBookmarksTreeView.h"
#include "GTUtilsDialog.h"
#include "GTUtilsEscClicker.h"
#include "GTUtilsLog.h"
#include "GTUtilsMdi.h"
#include "GTUtilsMsaEditorSequenceArea.h"
#include "GTUtilsOptionsPanel.h"
#include "GTUtilsPhyTree.h"
#include "GTUtilsProject.h"
#include "GTUtilsProjectTreeView.h"
#include "GTUtilsSequenceView.h"
#include "GTUtilsTaskTreeView.h"
#include "GTUtilsWorkflowDesigner.h"

#include "runnables/qt/MessageBoxFiller.h"
#include "runnables/qt/PopupChooser.h"
#include "runnables/ugene/corelibs/U2Gui/AlignShortReadsDialogFiller.h"
#include "runnables/ugene/corelibs/U2Gui/AppSettingsDialogFiller.h"
#include "runnables/ugene/corelibs/U2Gui/ConvertAssemblyToSAMDialogFiller.h"
#include "runnables/ugene/corelibs/U2Gui/CreateAnnotationWidgetFiller.h"
#include "runnables/ugene/corelibs/U2Gui/DownloadRemoteFileDialogFiller.h"
#include "runnables/ugene/corelibs/U2Gui/EditAnnotationDialogFiller.h"
#include "runnables/ugene/corelibs/U2Gui/EditQualifierDialogFiller.h"
#include "runnables/ugene/corelibs/U2Gui/EditSequenceDialogFiller.h"
#include "runnables/ugene/corelibs/U2Gui/FindTandemsDialogFiller.h"
#include "runnables/ugene/corelibs/U2Gui/ImportBAMFileDialogFiller.h"
#include "runnables/ugene/corelibs/U2Gui/PositionSelectorFiller.h"
#include "runnables/ugene/corelibs/U2Gui/RangeSelectionDialogFiller.h"
#include "runnables/ugene/corelibs/U2Gui/RemovePartFromSequenceDialogFiller.h"
#include "runnables/ugene/corelibs/U2Gui/util/ProjectTreeItemSelectorDialogBaseFiller.h"
#include "runnables/ugene/corelibs/U2View/ov_msa/BuildTreeDialogFiller.h"
#include "runnables/ugene/corelibs/U2View/ov_msa/ExtractSelectedAsMSADialogFiller.h"
#include "runnables/ugene/corelibs/U2View/ov_msa/LicenseAgreemntDialogFiller.h"
#include "runnables/ugene/corelibs/U2View/utils_smith_waterman/SmithWatermanDialogBaseFiller.h"
#include "runnables/ugene/plugins/annotator/FindAnnotationCollocationsDialogFiller.h"
#include "runnables/ugene/plugins/dna_export/ExportMSA2MSADialogFiller.h"
#include "runnables/ugene/plugins/dotplot/BuildDotPlotDialogFiller.h"
#include "runnables/ugene/plugins/dotplot/DotPlotDialogFiller.h"
#include "runnables/ugene/plugins/workflow_designer/WorkflowMetadialogFiller.h"
#include "runnables/ugene/plugins/external_tools/TCoffeeDailogFiller.h"
#include "runnables/ugene/plugins/workflow_designer/StartupDialogFiller.h"
#include "runnables/ugene/plugins/workflow_designer/WizardFiller.h"
#include "runnables/ugene/plugins_3rdparty/kalign/KalignDialogFiller.h"
#include "runnables/ugene/plugins_3rdparty/umuscle/MuscleDialogFiller.h"
#include "runnables/ugene/ugeneui/ConvertAceToSqliteDialogFiller.h"
#include "runnables/ugene/ugeneui/CreateNewProjectWidgetFiller.h"
#include "runnables/ugene/ugeneui/NCBISearchDialogFiller.h"
#include "runnables/ugene/ugeneui/SelectDocumentFormatDialogFiller.h"
#include "runnables/ugene/ugeneui/SequenceReadingModeSelectorDialogFiller.h"
#include "runnables/ugene/ugeneui/SaveProjectDialogFiller.h"

#include <U2Core/AppContext.h>
#include <U2Core/ExternalToolRegistry.h>
#include <U2Core/U2OpStatusUtils.h>

#include <U2View/ADVConstants.h>
#include <U2View/ADVSingleSequenceWidget.h>
#include <U2View/AnnotatedDNAViewFactory.h>
#include <U2View/MSAEditor.h>

namespace U2 {

namespace GUITest_regression_scenarios {

GUI_TEST_CLASS_DEFINITION(test_0734) {
    //1. Open "_common_data/fasta/test.TXT".
    GTUtilsProject::openFiles(os, testDir + "_common_data/fasta/test.TXT");

    //2. Open "_common_data/clustal/test_alignment.aln".
    GTUtilsProject::openFiles(os, testDir + "_common_data/clustal/test_alignment.aln");

    //Expected state: two documents are opened in the project view; MSA Editor are shown with test_alignment.
    QTreeWidgetItem *seqDoc = GTUtilsProjectTreeView::findItem(os, "test.TXT");
    QTreeWidgetItem *msaDoc = GTUtilsProjectTreeView::findItem(os, "test_alignment.aln");
    QWidget *msaView = GTUtilsMdi::activeWindow(os);
    CHECK(NULL != seqDoc && NULL != msaDoc, );
    CHECK(NULL != msaView, );

    //3. Drag'n'drop "Sequence4" object of "test.TXT" document from the project tree to the MSA Editor.
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << MSAE_MENU_LOAD << "Sequence from current project"));
    GTUtilsDialog::waitForDialog(os, new ProjectTreeItemSelectorDialogBaseChecker(os, "[s] Sequence4"));
    GTMenu::showContextMenu(os, GTUtilsMdi::activeWindow(os));
    GTGlobals::sleep();

    //Expected state: UGENE does not crash; a new "Sequence4" row appears in the alignment.
    QStringList names = GTUtilsMSAEditorSequenceArea::getNameList(os);
    CHECK_SET_ERR(names.size() == 4, QString("Sequence count mismatch. Expected: %1. Actual: %2").arg(4).arg(names.size()));
    CHECK_SET_ERR(names.last() == "Sequence4",
        QString("Inserted sequence name mismatch. Expected: %1. Actual: %2").arg("Sequence4").arg(names.last()));
}

GUI_TEST_CLASS_DEFINITION(test_0928) {
    // 1. Open "samples/FASTA/human_T1.fa".
    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");

    class OkClicker : public Filler {
    public:
        OkClicker(U2OpStatus& _os) : Filler(_os, "ORFDialogBase"){}
        virtual void run() {
            QWidget *w = QApplication::activeWindow();
            CHECK(NULL != w, );
            QDialogButtonBox *buttonBox = w->findChild<QDialogButtonBox*>(QString::fromUtf8("buttonBox"));
            CHECK(NULL != buttonBox, );
            QPushButton *button = buttonBox->button(QDialogButtonBox::Ok);
            CHECK(NULL != button, );
            GTWidget::click(os, button);
        }
    };

    // 2. Click "Find ORFs" button on the sequence view toolbar.
    // Expected state: ORF dialog appears. UGENE does not crash.
    // 3. Click "Ok" button and wait for the end of the task.
    GTUtilsDialog::waitForDialog(os, new OkClicker(os));
    GTWidget::click(os, GTAction::button(os, "Find ORFs"));
    GTGlobals::sleep();

    // Expected state: 837 orfs are found.
    QTreeWidgetItem *item = GTUtilsAnnotationsTreeView::findItem(os, "orf");
    CHECK(NULL != item, );
    CHECK_SET_ERR(item->childCount() == 837, QString("ORFs count mismatch. Expected: %1. Actual: %2").arg(837).arg(item->childCount()));
}

GUI_TEST_CLASS_DEFINITION(test_0986) {

    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
    GTGlobals::sleep(3000);

    class EscClicker : public Filler {
    public:
        EscClicker(U2OpStatus& _os) : Filler(_os, "SmithWatermanDialogBase"){}
        virtual void run(){
            GTGlobals::sleep();
#ifdef Q_OS_MAC
            GTKeyboardDriver::keyRelease(os, GTKeyboardDriver::key["shift"]);
            GTKeyboardDriver::keyRelease(os, GTKeyboardDriver::key["cmd"]);
            GTWidget::click(os,GTWidget::findWidget(os,"bttnCancel"));
#else
            GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["esc"]);
#endif
        }
    };

    GTUtilsDialog::waitForDialog(os, new EscClicker(os));
    GTGlobals::sleep(500);
    GTKeyboardDriver::keyPress(os, GTKeyboardDriver::key["shift"]);
    GTKeyboardDriver::keyClick(os, 'f', GTKeyboardDriver::key["ctrl"]);
    GTKeyboardDriver::keyRelease(os, GTKeyboardDriver::key["shift"]);
    GTGlobals::sleep(3000);

    GTGlobals::sleep(5000);
}

GUI_TEST_CLASS_DEFINITION(test_0986_1) {

    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
    GTGlobals::sleep(3000);

    SmithWatermanDialogFiller *filler = new SmithWatermanDialogFiller(os);
    filler->button = SmithWatermanDialogFiller::Cancel;
    GTUtilsDialog::waitForDialog(os, filler);

    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << ADV_MENU_ANALYSE << "find_pattern_smith_waterman_action", GTGlobals::UseMouse));
    GTMenu::showMainMenu(os, MWMENU_ACTIONS);
    GTGlobals::sleep(5000);

    GTGlobals::sleep(5000);
}

GUI_TEST_CLASS_DEFINITION(test_0986_2) {

    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
    GTGlobals::sleep(3000);

    GTRegionSelector::RegionSelectorSettings regionSelectorSettings(1, 2);
    GTUtilsDialog::waitForDialog(os, new SmithWatermanDialogFiller(os, "ATCG", regionSelectorSettings));

    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << ADV_MENU_ANALYSE << "find_pattern_smith_waterman_action", GTGlobals::UseMouse));
    GTMenu::showMainMenu(os, MWMENU_ACTIONS);
    GTGlobals::sleep(5000);

    GTGlobals::sleep(5000);
}

GUI_TEST_CLASS_DEFINITION(test_0986_3) {

    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
    GTGlobals::sleep(3000);

    GTRegionSelector::RegionSelectorSettings regionSelectorSettings(1, 2);
    GTUtilsDialog::waitForDialog(os, new SmithWatermanDialogFiller(os, "ATCGAT", regionSelectorSettings));

    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << ADV_MENU_ANALYSE << "find_pattern_smith_waterman_action", GTGlobals::UseMouse));
    GTMenu::showMainMenu(os, MWMENU_ACTIONS);
    GTGlobals::sleep(5000);

    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "human_T1.fa"));
    GTMouseDriver::click(os);
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["delete"]);

    GTGlobals::sleep(5000);
}

GUI_TEST_CLASS_DEFINITION(test_0986_4) {

    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
    GTGlobals::sleep(3000);

    GTRegionSelector::RegionSelectorSettings regionSelectorSettings(1, 2);
    GTUtilsDialog::waitForDialog(os, new SmithWatermanDialogFiller(os, "ATCGAT", regionSelectorSettings));

    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << ADV_MENU_ANALYSE << "find_pattern_smith_waterman_action", GTGlobals::UseMouse));
    GTMenu::showMainMenu(os, MWMENU_ACTIONS);
    GTGlobals::sleep(5000);

    GTUtilsMdi::click(os, GTGlobals::Close);
    GTGlobals::sleep(5000);
}

GUI_TEST_CLASS_DEFINITION(test_1001) {

    GTUtilsProject::openFiles(os, dataDir+"samples/FASTA/human_T1.fa");
    GTUtilsProject::openFiles(os, testDir+"_common_data/fasta/human_T1_cutted.fa");
    GTGlobals::sleep();

    GTUtilsDialog::waitForDialog(os, new DotPlotFiller(os, 4));
    GTWidget::click(os, GTWidget::findWidget(os, "build_dotplot_action_widget"));
    GTGlobals::sleep();

    GTGlobals::sleep(15000);
}

GUI_TEST_CLASS_DEFINITION(test_1001_1) {

    GTUtilsProject::openFiles(os, dataDir+"samples/FASTA/human_T1.fa");
    GTUtilsProject::openFiles(os, testDir+"_common_data/fasta/human_T1_cutted.fa");
    GTGlobals::sleep();

    GTUtilsDialog::waitForDialog(os, new DotPlotFiller(os, 100, 50));
    GTWidget::click(os, GTWidget::findWidget(os, "build_dotplot_action_widget"));
    GTGlobals::sleep();

    GTGlobals::sleep(5000);
}

GUI_TEST_CLASS_DEFINITION(test_1001_2) {

    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
    GTUtilsProject::openFiles(os, testDir+"_common_data/fasta/human_T1_cutted.fa");
    GTGlobals::sleep();

    GTUtilsDialog::waitForDialog(os, new DotPlotFiller(os, 99, 99, true));
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << ADV_MENU_ANALYSE << "build_dotplot_action", GTGlobals::UseMouse));
    GTMenu::showMainMenu(os, MWMENU_ACTIONS);
    GTGlobals::sleep();

    GTGlobals::sleep(5000);
}

GUI_TEST_CLASS_DEFINITION(test_1001_3) {

    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
    GTUtilsProject::openFiles(os, testDir+"_common_data/fasta/human_T1_cutted.fa");
    GTGlobals::sleep();

    GTUtilsDialog::waitForDialog(os, new DotPlotFiller(os, 99, 99, true));
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << ADV_MENU_ANALYSE << "build_dotplot_action", GTGlobals::UseMouse));
    GTMenu::showMainMenu(os, MWMENU_ACTIONS);
    GTGlobals::sleep(5000);

    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "human_T1.fa"));
    GTMouseDriver::click(os);
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["delete"]);

    GTGlobals::sleep(5000);
}

GUI_TEST_CLASS_DEFINITION(test_1001_4) {

    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
    GTUtilsProject::openFiles(os, testDir+"_common_data/fasta/human_T1_cutted.fa");
    GTGlobals::sleep();

    GTUtilsDialog::waitForDialog(os, new DotPlotFiller(os, 99, 99, true));
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << ADV_MENU_ANALYSE << "build_dotplot_action", GTGlobals::UseMouse));
    GTMenu::showMainMenu(os, MWMENU_ACTIONS);
    GTGlobals::sleep(5000);

    GTUtilsMdi::click(os, GTGlobals::Close);

    GTGlobals::sleep(5000);
}

GUI_TEST_CLASS_DEFINITION(test_1015) {

    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
    GTGlobals::sleep();

    GTUtilsDialog::waitForDialog(os, new DotPlotFiller(os, 3));
    GTWidget::click(os, GTWidget::findWidget(os, "build_dotplot_action_widget"));
    GTGlobals::sleep();

    GTUtilsMdi::click(os, GTGlobals::Close);

    GTGlobals::sleep(5000);
}

GUI_TEST_CLASS_DEFINITION(test_1015_1) {

    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
    GTGlobals::sleep();

    GTUtilsDialog::waitForDialog(os, new DotPlotFiller(os, 30, 50));
    GTWidget::click(os, GTWidget::findWidget(os, "build_dotplot_action_widget"));
    GTGlobals::sleep();

    GTUtilsMdi::click(os, GTGlobals::Close);
    GTUtilsMdi::click(os, GTGlobals::Minimize);

    GTGlobals::sleep(5000);
}

GUI_TEST_CLASS_DEFINITION(test_1015_2) {

    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
    GTGlobals::sleep();

    GTUtilsDialog::waitForDialog(os, new DotPlotFiller(os, 100, 50, true));
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << ADV_MENU_ANALYSE << "build_dotplot_action", GTGlobals::UseMouse));
    GTMenu::showMainMenu(os, MWMENU_ACTIONS);
    GTGlobals::sleep();

    GTUtilsMdi::click(os, GTGlobals::Close);
    GTUtilsMdi::click(os, GTGlobals::Close);

    GTGlobals::sleep(5000);
}

GUI_TEST_CLASS_DEFINITION(test_1015_3) {

    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
    GTGlobals::sleep();

    GTUtilsDialog::waitForDialog(os, new DotPlotFiller(os, 100, 50, true));
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << ADV_MENU_ANALYSE << "build_dotplot_action", GTGlobals::UseMouse));
    GTMenu::showMainMenu(os, MWMENU_ACTIONS);
    GTGlobals::sleep();

    GTUtilsMdi::click(os, GTGlobals::Close);
    GTUtilsMdi::click(os, GTGlobals::Close);

    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "human_T1.fa"));
    GTMouseDriver::click(os);
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["delete"]);

    GTGlobals::sleep(5000);
}

GUI_TEST_CLASS_DEFINITION(test_1015_4) {

    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
    GTGlobals::sleep();

    GTUtilsDialog::waitForDialog(os, new DotPlotFiller(os, 100, 50, true));
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << ADV_MENU_ANALYSE << "build_dotplot_action", GTGlobals::UseMouse));
    GTMenu::showMainMenu(os, MWMENU_ACTIONS);
    GTGlobals::sleep();

    GTUtilsMdi::click(os, GTGlobals::Close);
    GTUtilsMdi::click(os, GTGlobals::Minimize);

    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "human_T1.fa"));
    GTMouseDriver::click(os);
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["delete"]);

    GTGlobals::sleep(5000);
}

GUI_TEST_CLASS_DEFINITION(test_1021) {

    for (int i=0; i<2; i++) {
        // 1) Open data\samples\FASTA\human_T1.fa
        GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
        GTGlobals::sleep();

        // 2) Click "build dotplot" tooltip
        // 3) Click OK in opened dotplot dialog
        GTUtilsDialog::waitForDialog(os, new DotPlotFiller(os, 100));
        GTWidget::click(os, GTWidget::findWidget(os, "build_dotplot_action_widget"));
        GTGlobals::sleep();

        // 4) Click on human_T1.fa project tree view item
        GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "human_T1.fa"));
        GTMouseDriver::click(os);

        // 5) Press delete key
        GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["delete"]);
        GTGlobals::sleep();

        // Expected state: there are no empty MDI window opened, no bookmarks
        GTGlobals::sleep();
        QWidget* activeWindow = GTUtilsMdi::activeWindow(os, GTGlobals::FindOptions(false));
        CHECK_SET_ERR(activeWindow == NULL, "there is active window");

        QTreeWidget* bookmarksTree = GTUtilsBookmarksTreeView::getTreeWidget(os);
        CHECK_SET_ERR(bookmarksTree != NULL, "bookmarksTreeWidget is NULL");

        int bookmarksCount = bookmarksTree->topLevelItemCount();
        CHECK_SET_ERR(bookmarksCount == 0, "there are bookmarks");
    }

//    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["F4"], GTKeyboardDriver::key["alt"]);
//    GTGlobals::sleep();
}

GUI_TEST_CLASS_DEFINITION(test_1021_1) {

    for (int i=0; i<3; i++) {
        // 1) Open data\samples\FASTA\human_T1.fa
        GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
        GTGlobals::sleep();

        // 2) Click "build dotplot" tooltip
        // 3) Click OK in opened dotplot dialog
        GTUtilsDialog::waitForDialog(os, new DotPlotFiller(os, 100));
        GTWidget::click(os, GTWidget::findWidget(os, "build_dotplot_action_widget"));
        GTGlobals::sleep();

        // 4) Click on human_T1.fa project tree view item
        GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "human_T1.fa"));
        GTMouseDriver::click(os);

        // 5) Press delete key
        GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["delete"]);
        GTGlobals::sleep();

        // Expected state: there are no empty MDI window opened, no bookmarks
        GTGlobals::sleep();
        QWidget* activeWindow = GTUtilsMdi::activeWindow(os, GTGlobals::FindOptions(false));
        CHECK_SET_ERR(activeWindow == NULL, "there is active window");

        QTreeWidget* bookmarksTree = GTUtilsBookmarksTreeView::getTreeWidget(os);
        CHECK_SET_ERR(bookmarksTree != NULL, "bookmarksTreeWidget is NULL");

        int bookmarksCount = bookmarksTree->topLevelItemCount();
        CHECK_SET_ERR(bookmarksCount == 0, "there are bookmarks");
    }

//    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["F4"], GTKeyboardDriver::key["alt"]);
//    GTGlobals::sleep();
}

GUI_TEST_CLASS_DEFINITION(test_1021_2) {

    for (int i=0; i<2; i++) {
        // 1) Open data\samples\FASTA\human_T1.fa
        GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
        GTGlobals::sleep();

        // 2) Click "build dotplot" tooltip
        // 3) Click OK in opened dotplot dialog
        GTUtilsDialog::waitForDialog(os, new DotPlotFiller(os, 100));
        GTWidget::click(os, GTWidget::findWidget(os, "build_dotplot_action_widget"));
        GTGlobals::sleep();

        // 4) Click on human_T1.fa project tree view item
        GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "human_T1.fa"));
        GTMouseDriver::click(os);

        // 5) Press delete key
        GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["delete"]);
        GTGlobals::sleep();

        // Expected state: there are no empty MDI window opened, no bookmarks
        GTGlobals::sleep();
        QWidget* activeWindow = GTUtilsMdi::activeWindow(os, GTGlobals::FindOptions(false));
        CHECK_SET_ERR(activeWindow == NULL, "there is active window");

        QTreeWidget* bookmarksTree = GTUtilsBookmarksTreeView::getTreeWidget(os);
        CHECK_SET_ERR(bookmarksTree != NULL, "bookmarksTreeWidget is NULL");

        int bookmarksCount = bookmarksTree->topLevelItemCount();
        CHECK_SET_ERR(bookmarksCount == 0, "there are bookmarks");
    }

//    GTMenu::clickMenuItem(os, GTMenu::showMainMenu(os, MWMENU_FILE), ACTION__EXIT);
//    GTGlobals::sleep();

//    GTGlobals::sleep();
}

GUI_TEST_CLASS_DEFINITION(test_1021_3) {

    for (int i=0; i<2; i++) {
        // 1) Open data\samples\FASTA\human_T1.fa
        GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
        GTGlobals::sleep();

        // 2) Click "build dotplot" tooltip
        // 3) Click OK in opened dotplot dialog
        GTUtilsDialog::waitForDialog(os, new DotPlotFiller(os, 120, 100, true));
        GTWidget::click(os, GTWidget::findWidget(os, "build_dotplot_action_widget"));
        GTGlobals::sleep();

        if (i==0) {
            GTUtilsMdi::click(os, GTGlobals::Minimize);
            GTGlobals::sleep();
        }

        // 4) Click on human_T1.fa project tree view item
        GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "human_T1.fa"));
        GTMouseDriver::click(os);

        // 5) Press delete key
        GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["delete"]);
        GTGlobals::sleep();

        // Expected state: there are no empty MDI window opened, no bookmarks
        GTGlobals::sleep();
        QWidget* activeWindow = GTUtilsMdi::activeWindow(os, GTGlobals::FindOptions(false));
        CHECK_SET_ERR(activeWindow == NULL, "there is active window");

        QTreeWidget* bookmarksTree = GTUtilsBookmarksTreeView::getTreeWidget(os);
        CHECK_SET_ERR(bookmarksTree != NULL, "bookmarksTreeWidget is NULL");

        int bookmarksCount = bookmarksTree->topLevelItemCount();
        CHECK_SET_ERR(bookmarksCount == 0, "there are bookmarks");
    }

//    GTMenu::clickMenuItem(os, GTMenu::showMainMenu(os, MWMENU_FILE), ACTION__EXIT);
//    GTGlobals::sleep();

//    GTGlobals::sleep();
}

GUI_TEST_CLASS_DEFINITION(test_1021_4) {

    for (int i=0; i<2; i++) {
        // 1) Open data\samples\FASTA\human_T1.fa
        GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
        GTGlobals::sleep();

        // 2) Click "build dotplot" tooltip
        // 3) Click OK in opened dotplot dialog
        GTUtilsDialog::waitForDialog(os, new DotPlotFiller(os, 110, 100, true));
        GTWidget::click(os, GTWidget::findWidget(os, "build_dotplot_action_widget"));
        GTGlobals::sleep();

        if (i==0) {
            GTUtilsMdi::click(os, GTGlobals::Minimize);
            GTGlobals::sleep();
        }

        // 4) Click on human_T1.fa project tree view item
        GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "human_T1.fa"));
        GTMouseDriver::click(os);

        // 5) Press delete key
        GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["delete"]);
        GTGlobals::sleep();

        // Expected state: there are no empty MDI window opened, no bookmarks
        GTGlobals::sleep();
        QWidget* activeWindow = GTUtilsMdi::activeWindow(os, GTGlobals::FindOptions(false));
        CHECK_SET_ERR(activeWindow == NULL, "there is active window");

        QTreeWidget* bookmarksTree = GTUtilsBookmarksTreeView::getTreeWidget(os);
        CHECK_SET_ERR(bookmarksTree != NULL, "bookmarksTreeWidget is NULL");

        int bookmarksCount = bookmarksTree->topLevelItemCount();
        CHECK_SET_ERR(bookmarksCount == 0, "there are bookmarks");
    }

//    GTMenu::clickMenuItem(os, GTMenu::showMainMenu(os, MWMENU_FILE), ACTION__EXIT);
//    GTGlobals::sleep();
}

GUI_TEST_CLASS_DEFINITION(test_1022) {

    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
    GTGlobals::sleep();

    GTUtilsDialog::waitForDialog(os, new DotPlotFiller(os, 3));
    GTWidget::click(os, GTWidget::findWidget(os, "build_dotplot_action_widget"));
    GTGlobals::sleep();

    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "human_T1.fa"));
    GTMouseDriver::click(os);

    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["delete"]);
    GTGlobals::sleep();

    GTGlobals::sleep(5000);
}

GUI_TEST_CLASS_DEFINITION(test_1199) {
//1. Open any samples/PDB/1CF7.pdb file.
    GTFileDialog::openFile(os, dataDir+"samples/PDB/", "1CF7.PDB");

//2. Select document in project tree view. Press 'Delete'
    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "1CF7.PDB"));
    GTMouseDriver::click(os);
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["delete"]);
        
//Expected state: document removed, UGENE not crashes.
    GTUtilsProject::checkProject(os, GTUtilsProject::Empty);
}

GUI_TEST_CLASS_DEFINITION(test_1083) {
//1. open files data\samples\FASTA\human_T1.fa
    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
//2. select sequence oject in projrct tree view. press delete
    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "human_T1.fa"));
    GTMouseDriver::click(os);
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["delete"]);
//Expected state: UGENE is not crashed
    GTUtilsProject::checkProject(os, GTUtilsProject::Empty);
}

GUI_TEST_CLASS_DEFINITION(test_1093) {
//    1. do menu "tools->Align to refrence->Align short reads"

//    2. fill next fields in appeared dialog:
//        Refrence sequence "_common_data\scenarios\_regression\1093\refrence.fa"
//        SAM output checkbox {set checked}
//        Short reads add next file: "_common_data\scenarios\_regression\1093\read.fa"

//    3. Press 'Align'
//    Expected state: message window appears "The short reads can't be mapped to the reference sequence!"
    AlignShortReadsFiller::Parameters parameters(testDir + "_common_data/scenarios/_regression/1093/",
                                                 "refrence.fa",
                                                 testDir + "_common_data/scenarios/_regression/1093/",
                                                 "read.fa");
    GTUtilsDialog::waitForDialog(os, new AlignShortReadsFiller(os, &parameters)) ;
    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller(os, QMessageBox::Ok));

    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_TOOLS), QStringList() << "Align to reference" << "Align short reads");

    GTGlobals::sleep(5000);
//The short reads can't be mapped to the reference sequence!
}

GUI_TEST_CLASS_DEFINITION(test_1107){//commit GUIInitionalChecks
//1) Open an MSA file (e.g. _common_data\scenarios\msa\ma2_gapped.aln)

    GTFileDialog::openFile(os, testDir+"_common_data/scenarios/msa/", "ma2_gapped.aln");
//2) Menu File->Close Project
//3) Press No in the Save current project dialog
    GTUtilsDialog::waitForDialog(os, new SaveProjectDialogFiller(os, QDialogButtonBox::No));
    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_FILE), QStringList()<<ACTION_PROJECTSUPPORT__CLOSE_PROJECT);
//Expected state: UGENE not crashes
}
GUI_TEST_CLASS_DEFINITION(test_1113){//commit AboutDialogController.cpp
//1. Open UGENE
//2. Press F1
    class EscClicker : public Filler {
    public:
        EscClicker(U2OpStatus& _os) : Filler(_os, "AboutDialog"){}
        virtual void run(){

            GTGlobals::sleep();
#ifdef Q_OS_MAC
            GTKeyboardDriver::keyRelease(os,GTKeyboardDriver::key["f1"]);
#endif

            QWidget* dialog = QApplication::activeModalWidget();
//getting an info string
            QWidget *w = new QWidget();
            w=dialog->findChild<QWidget*>("about_widget");
            CHECK_SET_ERR(w!=NULL, "aboutWidget not found");

            QObject *parent = new QObject();
            parent= w->findChild<QObject*>("parent");
            CHECK_SET_ERR(parent!=NULL, "parentObject not found");

            QObject *child = new QObject();
            child=parent->findChild<QObject*>();
            CHECK_SET_ERR(child!=NULL, "childObject not found");

            QString text = child->objectName();
            CHECK_SET_ERR(text.contains("64-bit")||text.contains("32-bit"),text);
#ifdef Q_OS_MAC
            GTWidget::click(os,GTWidget::findWidget(os,"close_button"));
#else
            GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["esc"]);
#endif
        }
    };
    GTGlobals::sleep(1000);
    GTUtilsDialog::waitForDialog(os, new EscClicker(os));
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["f1"]);
    GTGlobals::sleep(1000);
//Expected state: About dialog appeared, shown info includes platform info (32/64)

}

GUI_TEST_CLASS_DEFINITION(test_1113_1){//commit AboutDialogController.cpp
//1. Open UGENE
//2. Help->About
    class EscClicker : public Filler {
    public:
        EscClicker(U2OpStatus& _os) : Filler(_os, "AboutDialog"){}
        virtual void run(){
            GTGlobals::sleep();
#ifdef Q_OS_MAC
            GTMouseDriver::release(os);
#endif
            QWidget* dialog = QApplication::activeModalWidget();
//getting an info string
            QWidget *w = new QWidget();
            w=dialog->findChild<QWidget*>("about_widget");
            CHECK_SET_ERR(w!=NULL, "aboutWidget not found");

            QObject *parent = new QObject();
            parent= w->findChild<QObject*>("parent");
            CHECK_SET_ERR(parent!=NULL, "parentObject not found");

            QObject *child = new QObject();
            child=parent->findChild<QObject*>();
            CHECK_SET_ERR(child!=NULL, "childObject not found");

            QString text = child->objectName();
            CHECK_SET_ERR(text.contains("64-bit")||text.contains("32-bit"),text);
#ifdef Q_OS_MAC
            GTWidget::click(os,GTWidget::findWidget(os,"close_button"));
#else
            GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["esc"]);
#endif
        }
    };

    GTUtilsDialog::waitForDialog(os, new EscClicker(os));
    QMenu *menu = GTMenu::showMainMenu(os, MWMENU_HELP);
    GTMenu::clickMenuItemByName(os, menu, QStringList() << ACTION__ABOUT);
    GTGlobals::sleep(1000);
//Expected state: About dialog appeared, shown info includes platform info (32/64)

}

GUI_TEST_CLASS_DEFINITION(test_1165){
//1. Open file "data/samples/CLUSTALW/COI.aln"
    GTFileDialog::openFile(os, dataDir+"samples/CLUSTALW/", "COI.aln");
//2. open context menu in msa, "Add"->"Sequence from file...",
//3. browse the file "data/samples/FASTA/human_T1.fa".
    QWidget *nameList;
    nameList=GTWidget::findWidget(os,"msa_editor_name_list");

    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << "MSAE_MENU_LOAD_SEQ"<<"Sequence from file"));
    GTFileDialogUtils *ob = new GTFileDialogUtils(os, dataDir + "/samples/FASTA/", "human_T1.fa");
    GTUtilsDialog::waitForDialog(os, ob);

    GTMenu::showContextMenu(os,nameList);
//4. Then choose any sequence in sequence names area (except that which you've just added), press "Delete"
//Expected state: UGENE not crashes
    GTWidget::click(os,nameList);
    GTGlobals::sleep(500);
    GTKeyboardDriver::keyClick(os,GTKeyboardDriver::key["delete"]);
}

GUI_TEST_CLASS_DEFINITION(test_1189){
//1) Open samples/FASTA/human_T1.fa
    GTFileDialog::openFile(os, dataDir + "samples/FASTA", "human_T1.fa");
//2) Press Ctrl+F
    GTKeyboardDriver::keyClick(os, 'f', GTKeyboardDriver::key["ctrl"]);
    GTGlobals::sleep(500);
//3) Select any region of the sequence
    GTUtilsDialog::waitForDialog(os, new selectSequenceRegionDialogFiller(os,100,200));
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList()<<"Select"<< "Sequence region"));
    GTMenu::showContextMenu(os, GTWidget::findWidget(os,"ADV_single_sequence_widget_0"));
//4) Select "Selected region" in "Region" combobox of "Search in" area.
    QComboBox *regBox =(QComboBox*)GTWidget::findWidget(os, "boxRegion");
    GTComboBox::setCurrentIndex(os,regBox,2);
//5) Ensure that two lineedits became visible and contain correct region
    QLineEdit *start = (QLineEdit*)GTWidget::findWidget(os,"editStart");
    CHECK_SET_ERR(start->isVisible(), "editStart line is not visiable");
    CHECK_SET_ERR(start->text()=="100","Wrong startValue. Current value is: "+start->text());

    QLineEdit *end = (QLineEdit*)GTWidget::findWidget(os,"editEnd");
    CHECK_SET_ERR(end->isVisible(), "editEnd line is not visiable");
    CHECK_SET_ERR(end->text()=="201","Wrong endValue. Current value is: "+end->text());
}

GUI_TEST_CLASS_DEFINITION(test_1189_1){
//1) Open samples/FASTA/human_T1.fa
    GTFileDialog::openFile(os, dataDir + "samples/FASTA", "human_T1.fa");
//2) Press Ctrl+F
    GTKeyboardDriver::keyClick(os, 'f', GTKeyboardDriver::key["ctrl"]);
    GTGlobals::sleep(500);
//3) Select any region of the sequence
    GTWidget::click(os, GTWidget::findWidget(os,"ADV_single_sequence_widget_0"));
    QPoint p;
    p=GTMouseDriver::getMousePosition();
    p.setX(p.x()-100);
    GTMouseDriver::moveTo(os,p);
    GTMouseDriver::press(os);
    p.setX(p.x()+200);
    GTMouseDriver::moveTo(os,p);
    GTMouseDriver::release(os);
//4) Select "Selected region" in "Region" combobox of "Search in" area.
    QComboBox *regBox =(QComboBox*)GTWidget::findWidget(os, "boxRegion");
    GTComboBox::setCurrentIndex(os,regBox,2);
//5) Ensure that two lineedits became visible and contain correct region
    QLineEdit *start = (QLineEdit*)GTWidget::findWidget(os,"editStart");
    CHECK_SET_ERR(start->isVisible(), "editStart line is not visiable");

    QLineEdit *end = (QLineEdit*)GTWidget::findWidget(os,"editEnd");
    CHECK_SET_ERR(end->isVisible(), "editEnd line is not visiable");
}

GUI_TEST_CLASS_DEFINITION(test_1190){//add AlignShortReadsFiller

//1) Align shortreads with genome aligner
//Tools -> Align to reference -> Align short reads
    AlignShortReadsFiller::Parameters parameters(testDir + "_common_data/fasta/",
                                                 "N.fa",
                                                 testDir + "_common_data/fasta/",
                                                 "RAW.fa");
    GTUtilsDialog::waitForDialog(os, new AlignShortReadsFiller(os, &parameters)) ;
    QMenu *menu = GTMenu::showMainMenu(os, MWMENU_TOOLS);
    GTLogTracer l;

    GTMenu::clickMenuItemByName(os, menu, QStringList() << "Align to reference" << "Align short reads");

    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller(os,QMessageBox::Ok));
    GTGlobals::sleep(10000);

    GTUtilsLog::check(os, l);

    //}
//Reference sequence: _common_data/fasta/N.fa  /home/vmalin/ugene/trunk/test/_common_data/fasta/N.ugenedb
//Short reads: _common_data/reads/shortreads15Mb.fasta

//Click "Start"

//2) wait for dialog to appear, click "OK"

//repeat these steps 3 times, UGENE shouldn't crash
}
GUI_TEST_CLASS_DEFINITION(test_1212){
//    1. Open any sequence. (human_t1.fa)
    GTFileDialog::openFile(os, dataDir + "samples/FASTA", "human_T1.fa");
//    2. Do "Select->Sequence Region..."
//    3. In single selection mode enter any region
//    4. Press "Go" and UGENE hangs up/crashes
    GTUtilsDialog::waitForDialog(os, new selectSequenceRegionDialogFiller(os,"100..200"));
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList()<<"Select"<< "Sequence region"));
    GTMenu::showContextMenu(os, GTWidget::findWidget(os,"ADV_single_sequence_widget_0"));
//    Expected: region is selected
    ADVSingleSequenceWidget *w=(ADVSingleSequenceWidget*)GTWidget::findWidget(os,"ADV_single_sequence_widget_0");
    CHECK_SET_ERR(!w->getSequenceSelection()->isEmpty(), "No selected region");
}

GUI_TEST_CLASS_DEFINITION(test_1212_1){
//    1. Open any sequence. (human_t1.fa)
    GTFileDialog::openFile(os, dataDir + "samples/FASTA", "human_T1.fa");
//    2. Do "Select->Sequence Region..."
//    3. In single selection mode enter any region
//    4. Press "Go" and UGENE hangs up/crashes
    GTUtilsDialog::waitForDialog(os, new selectSequenceRegionDialogFiller(os,"0..199950"));
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList()<<"Select"<< "Sequence region"));
    GTMenu::showContextMenu(os, GTWidget::findWidget(os,"ADV_single_sequence_widget_0"));
//    Expected: region is selected
    ADVSingleSequenceWidget *w=(ADVSingleSequenceWidget*)GTWidget::findWidget(os,"ADV_single_sequence_widget_0");
    CHECK_SET_ERR(!w->getSequenceSelection()->isEmpty(), "No selected region");

}

GUI_TEST_CLASS_DEFINITION(test_1252){
//    1. Open human_t1.fa
    GTFileDialog::openFile(os, dataDir + "samples/FASTA", "human_T1.fa");
//    2. Find any pattern. A new annotation document is created
    GTUtilsOptionsPanel::runFindPatternWithHotKey("TTTTTAAAAA", os);

    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "Annotations"));
    QTreeWidgetItem *item = GTUtilsAnnotationsTreeView::findItem(os, "misc_feature");
    GTMouseDriver::moveTo(os, GTTreeWidget::getItemCenter(os, item));
//    3. Delete found annotations from human_t1 annotations tree
    GTMouseDriver::click(os);
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["delete"]);
//    4. Delete created annotations document
    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller(os, QMessageBox::No));
    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "MyDocument.gb"));
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList()<<ACTION_PROJECT__REMOVE_SELECTED));
    GTMouseDriver::click(os, Qt::RightButton);
    GTGlobals::sleep(500);
//check delition of annotation document
   CHECK_SET_ERR(GTUtilsProjectTreeView::findItem(os, "Annotations", GTGlobals::FindOptions(false))==NULL, "Annotations document not deleted");
//    5. Click search again
    GTWidget::click(os, GTWidget::findWidget(os,"btnSearch"));
    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "Annotations"));
    item = GTUtilsAnnotationsTreeView::findItem(os, "misc_feature");
    GTMouseDriver::moveTo(os, GTTreeWidget::getItemCenter(os, item));
// delete annotations manually to cache MessageBox
    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller(os, QMessageBox::No));
    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "MyDocument.gb"));
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList()<<ACTION_PROJECT__REMOVE_SELECTED));
    GTMouseDriver::click(os, Qt::RightButton);
//    Expected: pattern is found and annotation is stored in a new document

}

GUI_TEST_CLASS_DEFINITION(test_1252_1){
//DIFFERENCE: DEL KEY IS USED TO DELETE ANNOTATION DOCUMENT
//    1. Open human_t1.fa
    GTFileDialog::openFile(os, dataDir + "samples/FASTA", "human_T1.fa");
//    2. Find any pattern. A new annotation document is created
    GTUtilsOptionsPanel::runFindPatternWithHotKey("TTTTTAAAAA", os);

    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "Annotations"));
    QTreeWidgetItem *item = GTUtilsAnnotationsTreeView::findItem(os, "misc_feature");
    GTMouseDriver::moveTo(os, GTTreeWidget::getItemCenter(os, item));
//    3. Delete found annotations from human_t1 annotations tree
    GTMouseDriver::click(os);
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["delete"]);
//    4. Delete created annotations document
    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller(os, QMessageBox::No));
    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "MyDocument.gb"));
    GTMouseDriver::click(os);
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["delete"]);
    GTGlobals::sleep(500);
//check delition of annotation document
    CHECK_SET_ERR(GTUtilsProjectTreeView::findItem(os, "Annotations", GTGlobals::FindOptions(false))==NULL, "Annotations document not deleted");
//    5. Click search again
    GTWidget::click(os, GTWidget::findWidget(os,"btnSearch"));
    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "Annotations"));
    item = GTUtilsAnnotationsTreeView::findItem(os, "misc_feature");
    GTMouseDriver::moveTo(os, GTTreeWidget::getItemCenter(os, item));
// delete annotations manually to cache MessageBox
    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller(os, QMessageBox::No));
    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "MyDocument.gb"));
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList()<<ACTION_PROJECT__REMOVE_SELECTED));
    GTMouseDriver::click(os, Qt::RightButton);
//    Expected: pattern is found and annotation is stored in a new document
}

GUI_TEST_CLASS_DEFINITION(test_1255){
//1. Open human_T1.fa sequence
    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
//2. Open Find Pattern on the Option Panel. Enter a vaild pattern
    GTKeyboardDriver::keyClick(os, 'f', GTKeyboardDriver::key["ctrl"]);
    GTGlobals::sleep(200);
    GTKeyboardDriver::keySequence(os, "TA");
//3. Input invalid annotation name (empty, too long, illegal)
    GTWidget::click(os, GTWidget::findWidget(os, "titleWidget"));
    for (int i=0; i<15; i++){
        GTKeyboardDriver::keyClick(os,GTKeyboardDriver::key["down"]);
        GTGlobals::sleep(50);
    }

    QLabel *label;
    label = (QLabel*)GTWidget::findWidget(os,"lblErrorMessage");

    QLineEdit *annotationNameEdit;
    annotationNameEdit=(QLineEdit *)GTWidget::findWidget(os,"annotationNameEdit");

//3.1 Input invalid annotation name (empty)
    GTLineEdit::setText(os,annotationNameEdit,"");
    QString s=label->text();
    CHECK_SET_ERR(s.contains("empty"),"Error message is: "+s);
    GTGlobals::sleep(500);

    for (int i=0; i<5; i++){
        GTKeyboardDriver::keyClick(os,GTKeyboardDriver::key["down"]);
        GTGlobals::sleep(50);
    }

//3.2 Input invalid annotation name (illegal)

    GTLineEdit::setText(os,annotationNameEdit," test");
    s=label->text();
    CHECK_SET_ERR(s.contains("Illegal"),"Error message is: "+s);
    GTGlobals::sleep(500);

    for (int i=0; i<5; i++){
        GTKeyboardDriver::keyClick(os,GTKeyboardDriver::key["down"]);
        GTGlobals::sleep(50);
    }
//3.3 Input invalid annotation name (illegal)
    GTLineEdit::setText(os,annotationNameEdit,"Too long annotation name");
    s=label->text();
    CHECK_SET_ERR(s.contains("too long"),"Error message is: "+s);
    GTGlobals::sleep(500);
//Expected: error message appears if the annotations are invalid
}

GUI_TEST_CLASS_DEFINITION(test_1262) {
    //1. Open human_t1.fa
    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
    //2. Find any pattern. A new annotation document is created
    GTUtilsOptionsPanel::runFindPatternWithHotKey("AGGAAAAAATGCTAAGGGCAGCCAGAGAGAGGTCAGG", os);

    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList()<<ACTION_PROJECT__REMOVE_SELECTED));
    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller(os, QMessageBox::No));

    //3. Delete created annotations document
    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "MyDocument.gb"));
    GTMouseDriver::click(os, Qt::RightButton);
    //4. Click search again
    GTWidget::click(os, GTWidget::findWidget(os, "btnSearch"));

    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "Annotations"));
    QTreeWidgetItem *item = GTUtilsAnnotationsTreeView::findItem(os, "misc_feature");
    GTMouseDriver::moveTo(os, GTTreeWidget::getItemCenter(os, item));

    //delete new doc
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList()<<ACTION_PROJECT__REMOVE_SELECTED));
    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller(os, QMessageBox::No));

    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "MyDocument.gb"));
    GTMouseDriver::click(os, Qt::RightButton);


}

GUI_TEST_CLASS_DEFINITION(test_1475) {
    GTUtilsDialog::waitForDialog(os, new SelectDocumentFormatDialogFiller(os));
    GTFileDialog::openFile(os, testDir + "_common_data/raw_sequence/", "NC_000117.txt");
}

GUI_TEST_CLASS_DEFINITION(test_1508) {
    //1. Open COI2.fa as an alignment
    GTUtilsDialog::waitForDialog(os, new SequenceReadingModeSelectorDialogFiller(os, SequenceReadingModeSelectorDialogFiller::Join));
    GTFileDialog::openFile(os, testDir+"_common_data/fasta/", "COI2.fa");

    //2. {MSA Editor context menu} -> Align -> Align with MUSCLE
    //3. Choose the mode "Refine only"
    GTUtilsDialog::waitForDialog(os, new MuscleDialogFiller(os, MuscleDialogFiller::Refine));
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << MSAE_MENU_ALIGN << "Align with muscle", GTGlobals::UseMouse));
    GTWidget::click(os, GTUtilsMdi::activeWindow(os), Qt::RightButton);
    GTGlobals::sleep();
}

GUI_TEST_CLASS_DEFINITION(test_1527) {
    //1. Open COI2.aln as an alignment
    GTFileDialog::openFile(os, dataDir+"samples/CLUSTALW/", "COI.aln");
    GTGlobals::sleep();

    //2. {MSA Editor context menu} -> Align -> Align profile to profile with MUSCLE
    //3. Select empty "test.aln" in the profile browsing dialog.
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << MSAE_MENU_ALIGN << "Align profile to profile with MUSCLE", GTGlobals::UseMouse));
    GTUtilsDialog::waitForDialog(os, new GTFileDialogUtils(os, testDir + "_common_data/regression/1527/", "test.aln"));
    GTMenu::showContextMenu(os, GTUtilsMdi::activeWindow(os));

}

GUI_TEST_CLASS_DEFINITION(test_1527_1) {
//1. Open COI2.aln as an alignment
GTFileDialog::openFile(os, dataDir+"samples/CLUSTALW/", "COI.aln");
GTGlobals::sleep();

//2. {MSA Editor context menu} -> Align -> Align sequences to profile with MUSCLE
//3. Select empty "test.aln" in the profile browsing dialog.
GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << MSAE_MENU_ALIGN << "Align sequences to profile with MUSCLE", GTGlobals::UseMouse));
GTUtilsDialog::waitForDialog(os, new GTFileDialogUtils(os, testDir + "_common_data/regression/1527/", "test.aln"));
GTMenu::showContextMenu(os, GTUtilsMdi::activeWindow(os));
}

GUI_TEST_CLASS_DEFINITION( test_1622 )
{
    // 1. Open document "ma.aln"
    GTFileDialog::openFile( os, testDir + "_common_data/scenarios/msa", "ma.aln" );

    // 2. Save the initial content
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 11, 17 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep(200);
    const QString initialContent = GTClipboard::text( os );

    // 2.1. Remove selection
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["esc"] );
    GTGlobals::sleep(200);
    GTUtilsMSAEditorSequenceArea::checkSelectedRect( os, QRect( ) );

    // 3. Select a region in the sequence area
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 5, 4 ), QPoint( 10, 12 ) );

    // 4. Shift the region
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 6, 7 ), QPoint( 11, 7 ) );

    // 5. Obtain undo button
    QAbstractButton *undo = GTAction::button( os, "msa_action_undo" );

    // 6. Undo shifting, e.g. alignment should restore to the init state
    GTWidget::click( os, undo );

    // 7. Check the undone state
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 11, 17 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep(200);
    const QString undoneContent = GTClipboard::text( os );
    CHECK_SET_ERR( undoneContent == initialContent,
        "Undo works wrong. Found text is: " + undoneContent );
}

GUI_TEST_CLASS_DEFINITION( test_1688 ) {
    // 1) Open file "_common_data/scenarios/_regression/1688/sr100.000.fa"
    // Expected state: UGENE show error, not crashed
    GTLogTracer l;

    GTUtilsDialog::waitForDialog( os, new SequenceReadingModeSelectorDialogFiller( os,
        SequenceReadingModeSelectorDialogFiller::Separate ) );
    GTFileDialog::openFile( os, testDir + "_common_data/scenarios/_regression/1688/", "sr100.000.fa");
    GTGlobals::sleep( 60000 );
    GTUtilsLog::check( os, l );
}

GUI_TEST_CLASS_DEFINITION( test_1700 )
{
    //    1. Open file "https://ugene.unipro.ru/tracker/secure/attachment/12864/pdb1a07.ent.gz".
    GTFileDialog::openFile(os, testDir + "_common_data/pdb/", "pdb1a07.ent.gz");

    // Expected state: UGENE displays sequence and 3d structure
    GTUtilsMdi::findWindow(os, "pdb1a07.ent.gz");


    // 2. In context menu of 3d view: {Render style -> Ball-and-Stick}
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList()<< "Render Style"<< "Ball-and-Stick"));
    QMenu* menu = GTMenu::showContextMenu(os, GTWidget::findWidget(os, "1-1A07"));

    //3) In context menu of 3d view: {Coloring scheme -> <Any different from current scheme>}
    //Repeat step 3) many times

    bool curSchemeFlag = true;
    for(int i = 0; i < 10; i++) {
        if(curSchemeFlag) {
            GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << "Coloring Scheme" << "Chemical Elements"));
            curSchemeFlag = false;
        }
        else {
            GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << "Coloring Scheme" << "Secondary Structure"));
            curSchemeFlag = true;
        }
        GTMenu::showContextMenu(os, GTWidget::findWidget(os, "1-1A07"));
    }
}

GUI_TEST_CLASS_DEFINITION( test_1701 ) {
    //1. Open 2 PDB files("_common_data/pdb/1A5H.pdb" and "_common_data/pdb/1CF7.pdb")
    //2. In each of them set {Render style -> Ball and Stick}
    //3. Close one of the views
    //Expected state: UGENE works fine. The opened view is displayed correctly(no black screen instead of molecule).

    GTFileDialog::openFile( os, testDir + "_common_data/pdb", "1A5H.pdb");
    GTFileDialog::openFile( os, testDir + "_common_data/pdb", "1CF7.pdb");

    QWidget* pdb1 = GTWidget::findWidget(os, "1-1A5H");
    QWidget* pdb2 = GTWidget::findWidget(os, "2-1CF7");
    CHECK_SET_ERR( NULL != pdb1, "No 1A5H view");
    CHECK_SET_ERR( NULL != pdb2, "No 1CF7 view");

    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList()<< "Render Style"<< "Ball-and-Stick"));
    GTMenu::showContextMenu(os, pdb2);

    GTGlobals::sleep(500);
    QPixmap before = QPixmap::grabWidget(pdb2, pdb2->rect());

    GTMouseDriver::moveTo( os, GTUtilsProjectTreeView::getItemCenter(os, "1A5H.pdb"));
    GTMouseDriver::doubleClick(os);

    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList()<< "Render Style"<< "Ball-and-Stick"));
    GTMenu::showContextMenu(os, pdb1);

    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_ACTIONS), QStringList()<<"Close active view", GTGlobals::UseKey);

    GTGlobals::sleep(500);
    QPixmap after = QPixmap::grabWidget(pdb2, pdb2->rect());

    GTGlobals::sleep(500);
    CHECK_SET_ERR( before.toImage() == after.toImage(), "The view has changed");
}

GUI_TEST_CLASS_DEFINITION( test_1703 )
{
    // 1. Open document "ma.aln"
    GTFileDialog::openFile( os, testDir + "_common_data/scenarios/msa", "ma.aln" );

    // 2. Select some row in the name list area
    GTUtilsMSAEditorSequenceArea::click( os, QPoint( -5, 6 ) );
    GTGlobals::sleep(200);
    GTUtilsMSAEditorSequenceArea::checkSelectedRect( os, QRect( 0, 6, 12, 1 ) );

    GTKeyboardDriver::keyPress(os,GTKeyboardDriver::key["shift"]);
    // 3. Select the upper row
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["up"] );
    GTGlobals::sleep(500);
    GTUtilsMSAEditorSequenceArea::checkSelectedRect( os, QRect( 0, 5, 12, 2 ) );

    // 4. Select the bottom row
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["down"] );
    GTGlobals::sleep(500);
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["down"] );
    GTGlobals::sleep(500);
    GTKeyboardDriver::keyRelease(os,GTKeyboardDriver::key["shift"]);
    GTUtilsMSAEditorSequenceArea::checkSelectedRect( os, QRect( 0, 6, 12, 2 ) );
}

GUI_TEST_CLASS_DEFINITION(test_1708){
    //1. Open COI.aln or HIV-1.aln from samples
    GTFileDialog::openFile(os, dataDir + "samples/CLUSTALW", "COI.aln");

    GTUtilsMSAEditorSequenceArea::selectArea(os, QPoint(0,0), QPoint(9,1));
    GTKeyboardDriver::keyClick(os, 'c', GTKeyboardDriver::key["ctrl"]);
    GTGlobals::sleep(500);
    QString initAln = GTClipboard::text(os);

    QString expectedAln("TAAGACTT-C\n"
                        "TAAG-CTTAC");

    //2. Align with KAlign
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList()<<MSAE_MENU_ALIGN<<"align_with_kalign", GTGlobals::UseKey));
    GTUtilsDialog::waitForDialog(os, new KalignDialogFiller(os,10));
    GTMenu::showContextMenu(os, GTUtilsMdi::activeWindow(os));
#ifdef Q_OS_MAC
    GTGlobals::sleep(10000);
#endif
    GTWidget::click(os, GTUtilsMdi::activeWindow(os));
    GTUtilsMSAEditorSequenceArea::selectArea(os, QPoint(0,0), QPoint(9,1));
    GTKeyboardDriver::keyClick(os, 'c', GTKeyboardDriver::key["ctrl"]);
    GTGlobals::sleep(500);
    QString changedAln = GTClipboard::text(os);
    CHECK_SET_ERR(changedAln==expectedAln, "Unexpected alignment\n" + changedAln);

    QAbstractButton *undo= GTAction::button(os,"msa_action_undo");

    //3. Press Undo
    GTWidget::click(os,undo);
    GTWidget::click(os, GTUtilsMdi::activeWindow(os));
    GTUtilsMSAEditorSequenceArea::selectArea(os, QPoint(0,0), QPoint(9,1));
    GTKeyboardDriver::keyClick(os, 'c', GTKeyboardDriver::key["ctrl"]);
    GTGlobals::sleep(500);
    changedAln = GTClipboard::text(os);

    CHECK_SET_ERR(changedAln==initAln, "Undo works wrong\n" + changedAln);
}

GUI_TEST_CLASS_DEFINITION(test_1720){
//1. Use menu {File->Access remote database...}
    GTUtilsDialog::waitForDialog(os, new RemoteDBDialogFiller(os, "D11266", 0));
    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_FILE), QStringList()<<ACTION_PROJECTSUPPORT__ACCESS_REMOTE_DB, GTGlobals::UseKey);

    GTLogTracer l;
    GTGlobals::sleep(8000);//some time needed for request
//2. Fill field "Resource ID" with value D11266. Click "OK"

//3. Use menu {File->Access remote database...}
    GTUtilsDialog::waitForDialog(os, new RemoteDBDialogFiller(os, "D11266", 0));
    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_FILE), QStringList()<<ACTION_PROJECTSUPPORT__ACCESS_REMOTE_DB, GTGlobals::UseKey);

    GTGlobals::sleep(8000);
//4. Fill field "Resource ID" with value D11266. Click "OK"
    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "D11266.gb"));
    GTUtilsLog::check(os,l);
//Expected state: project view with document "D11266.gb", no error messages in log appear
}

GUI_TEST_CLASS_DEFINITION(test_1786){
    // 1. Use menu {File->Access remote database...}
    // 2. Select database UniProt(DAS)
    // 3. Fill resource id: 1CRN. Press ok
    GTUtilsDialog::waitForDialog(os, new RemoteDBDialogFiller(os, "1CRN", 7));
    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_FILE), QStringList()<<ACTION_PROJECTSUPPORT__ACCESS_REMOTE_DB, GTGlobals::UseKey);
    GTGlobals::sleep(8000);
    // Expected state: document P01542_das.gb appeared in project
    GTUtilsProjectTreeView::findItemByText(os,
        GTUtilsProjectTreeView::getTreeWidget(os),
        "P01542_das.gb",
        GTGlobals::FindOptions(true));
    }

GUI_TEST_CLASS_DEFINITION(test_1808) {
    QDir outputDir(testDir + "_common_data/scenarios/sandbox");
    const QString outputFilePath = outputDir.absolutePath( ) + "/test_1808.gb";

    // 1.Open WD
    GTUtilsWorkflowDesigner::openWorkflowDesigner(os);

    // 2.Create schema "Read Annotations"->"Write annotations"
    GTUtilsWorkflowDesigner::addAlgorithm(os, "Read Annotations");
    GTUtilsWorkflowDesigner::addAlgorithm(os, "Write annotations");
    WorkflowProcessItem *read = GTUtilsWorkflowDesigner::getWorker(os, "Read Annotations");
    WorkflowProcessItem *write = GTUtilsWorkflowDesigner::getWorker(os, "Write annotations");
    GTUtilsWorkflowDesigner::connect(os, read, write);

    // 3.Select any input and output file
    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, "Read Annotations"));
    GTMouseDriver::click(os);
    GTUtilsWorkflowDesigner::setDatasetInputFile(os, dataDir + "samples/Genbank", "sars.gb");
    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, "Write Annotations"));
    GTMouseDriver::click(os);
    GTUtilsWorkflowDesigner::setParameter(os, "Output file", outputFilePath, GTUtilsWorkflowDesigner::textValue);

    // 4.Validate schema
    // Expected state: No warning about empty input sequence slot
    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller(os, QMessageBox::Ok, "Well done!"));
    GTWidget::click( os,GTAction::button( os,"Validate workflow" ) );
}

GUI_TEST_CLASS_DEFINITION(test_1811) {
    GTUtilsDialog::waitForDialog(os, new RemoteDBDialogFiller(os, "Q9IGQ6", 8));
    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_FILE), QStringList()<<ACTION_PROJECTSUPPORT__ACCESS_REMOTE_DB, GTGlobals::UseKey);

    GTGlobals::sleep(8000);//some time needed for request
}
GUI_TEST_CLASS_DEFINITION(test_1811_1) {
    GTUtilsDialog::waitForDialog(os, new RemoteDBDialogFiller(os, "A0N8V2", 5));
    GTUtilsDialog::waitForDialog(os, new SelectDocumentFormatDialogFiller(os));
    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_FILE), QStringList()<<ACTION_PROJECTSUPPORT__ACCESS_REMOTE_DB, GTGlobals::UseKey);

    GTGlobals::sleep(10000);//some time needed for request
    GTUtilsDocument::isDocumentLoaded(os, "A0N8V2.txt");
}

GUI_TEST_CLASS_DEFINITION( test_1813 )
{
    // 1) Select the menu {File->Access remote database}
    // 2) Fill the "Fetch Data from Remote Database" dialog with the next values:
    //      Resource ID: I7G8J3
    //      Database: UniProt (DAS)
    // 3) Press "OK"
    GTUtilsDialog::waitForDialog(os, new RemoteDBDialogFiller(os, "I7G8J3", 7));
    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_FILE), QStringList()<<ACTION_PROJECTSUPPORT__ACCESS_REMOTE_DB, GTGlobals::UseMouse);

    // Expected: the sequence view with I7G8J3 sequence is opened. UGENE does not crash.
    GTGlobals::sleep(5000);
    GTUtilsTaskTreeView::waitTaskFinidhed(os);
    GTUtilsDocument::isDocumentLoaded(os, "I7G8J3_das.gb");
    GTUtilsDocument::checkDocument(os, "I7G8J3_das.gb", AnnotatedDNAViewFactory::ID);
}

GUI_TEST_CLASS_DEFINITION( test_1821 ) {
    QString workflowOutputDirPath( testDir + "_common_data/scenarios/sandbox" );
    QDir workflowOutputDir( workflowOutputDirPath );

    GTUtilsDialog::waitForDialog( os, new StartupDialogFiller( os,
        workflowOutputDir.absolutePath( ) ) );

    //1. Open WD
    QMenu* menu=GTMenu::showMainMenu( os, MWMENU_TOOLS );
    GTMenu::clickMenuItemByName( os, menu, QStringList( ) << "Workflow Designer" );
    GTGlobals::sleep( 500 );

    //2. Select "Align sequences with MUSCLE"
    GTUtilsWorkflowDesigner::addSample( os, "Align sequences with MUSCLE" );
    GTGlobals::sleep( 500 );

    //3. Change the value of the scale spinbox. E.g. set it to 75%
    QComboBox *scaleCombo = dynamic_cast<QComboBox *>( GTWidget::findWidget( os, "wdScaleCombo" ) );
    CHECK_SET_ERR( NULL != scaleCombo, "Unable to find scale combobox!" );
    GTComboBox::setIndexWithText( os, scaleCombo, "75%" );

    //4. Store the scheme to some file using "Save scheme as" button
    GTUtilsDialog::waitForDialog( os, new WorkflowMetaDialogFiller( os,
        workflowOutputDir.absolutePath( ) + "/" + "test.uwl", "Scheme") );
    GTMenu::clickMenuItemByName( os, GTMenu::showMainMenu( os, MWMENU_ACTIONS ),
        QStringList( ) <<  "Save workflow action", GTGlobals::UseKey );

    //5. Close WD
    GTUtilsMdi::click( os, GTGlobals::Close );
    GTMouseDriver::click( os );

    //6. Open the file containing the saved scheme using "Open" button
    GTFileDialog::openFile( os, workflowOutputDirPath, "test.uwl" );

    //Expected state: scheme is opened in WD, its scale is 75%
    scaleCombo = dynamic_cast<QComboBox *>( GTWidget::findWidget( os, "wdScaleCombo" ) );
    CHECK_SET_ERR( NULL != scaleCombo, "Unable to find scale combobox!" );
    CHECK_SET_ERR( scaleCombo->currentText( ) == "75%", "Unexpected scale value!" );
}

GUI_TEST_CLASS_DEFINITION( test_1859 ) {
    QString workflowOutputDirPath( testDir + "_common_data/scenarios/sandbox" );
    QDir workflowOutputDir( workflowOutputDirPath );
    const QString outputFilePath = workflowOutputDir.absolutePath( ) + "/test.gb";

    // 1) Open Workflow Designer
    GTUtilsWorkflowDesigner::openWorkflowDesigner( os );

    const QString annReaderName = "Read Annotations";
    const QString annWriterName = "Write Annotations";

    // 2) Build workflow of elements: "Write Annotations" and "Read Annotations"
    GTUtilsWorkflowDesigner::addAlgorithm( os, annReaderName );
    GTUtilsWorkflowDesigner::addAlgorithm( os, annWriterName );

    WorkflowProcessItem *annReader = GTUtilsWorkflowDesigner::getWorker( os, annReaderName );
    WorkflowProcessItem *annWriter = GTUtilsWorkflowDesigner::getWorker( os, annWriterName );

    GTUtilsWorkflowDesigner::connect(os, annReader, annWriter );

    // 3) Set output file

    GTMouseDriver::moveTo( os, GTUtilsWorkflowDesigner::getItemCenter( os, annWriterName ) );
    GTMouseDriver::click( os );
    GTUtilsWorkflowDesigner::setParameter( os, "Output file", outputFilePath,
        GTUtilsWorkflowDesigner::textValue );

    // 4) Set input file
    GTMouseDriver::moveTo( os, GTUtilsWorkflowDesigner::getItemCenter( os, annReaderName ) );
    GTMouseDriver::click( os );
    GTUtilsWorkflowDesigner::setDatasetInputFile( os, dataDir + "samples/Genbank", "sars.gb" );

    GTLogTracer lt;

    // 5) Run workflow
    GTWidget::click( os,GTAction::button( os,"Run workflow" ) );
    GTGlobals::sleep( );

    // 6) Block file for writing
    PermissionsSetter permSetter;
    QFile::Permissions p = QFile::WriteOwner | QFile::WriteUser | QFile::WriteGroup
        | QFile::WriteOther;
    bool res = permSetter.setPermissions( outputFilePath, ~p );
    CHECK_SET_ERR( res, "Can't set permissions" );

    // 7) Run workflow again
    GTWidget::click( os,GTAction::button( os,"Run workflow" ) );
    GTGlobals::sleep( );

    CHECK_SET_ERR(lt.hasError(), "no error in log");
}

GUI_TEST_CLASS_DEFINITION(test_1860) {
    // 1) Open Workflow Designer
    GTUtilsDialog::waitForDialog(os, new StartupDialogFiller(os));
    QMenu *menu=GTMenu::showMainMenu( os, MWMENU_TOOLS );
    GTMenu::clickMenuItemByName( os, menu, QStringList( ) << "Workflow Designer" );

    const QString textReaderName = "Read Plain Text";
    const QString textWriterName = "Write Plain Text";

    //2) Add elements: "Write plain text" and "Read plain text"
    GTUtilsWorkflowDesigner::addAlgorithm( os, textReaderName);
    GTUtilsWorkflowDesigner::addAlgorithm( os, textWriterName);

    WorkflowProcessItem *textReader = GTUtilsWorkflowDesigner::getWorker( os, textReaderName);
    WorkflowProcessItem *textWriter = GTUtilsWorkflowDesigner::getWorker( os, textWriterName);

    //3) Connect them to each other
    GTUtilsWorkflowDesigner::connect(os, textReader, textWriter);

    //4) Try to set output file

    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, textWriterName));
    GTMouseDriver::click(os);
    GTUtilsWorkflowDesigner::setParameter(os, "Output file", "test", GTUtilsWorkflowDesigner::textValue );
    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, textReaderName));
    GTMouseDriver::click(os);
}

GUI_TEST_CLASS_DEFINITION(test_1865) {
    // 1) Open Workflow Designer
    GTUtilsDialog::waitForDialog(os, new StartupDialogFiller(os));
    QMenu *menu=GTMenu::showMainMenu( os, MWMENU_TOOLS );
    GTMenu::clickMenuItemByName( os, menu, QStringList( ) << "Workflow Designer" );

    const QString sequenceWriterName = "Write Sequence";

    //2) Add elements: "Write sequence"
    GTUtilsWorkflowDesigner::addAlgorithm( os, sequenceWriterName);

    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, sequenceWriterName));
    GTMouseDriver::click(os);
    
    }

GUI_TEST_CLASS_DEFINITION( test_1883 ){
// 1. Open file "data/samples/CLUSTALW/COI.aln"
    GTFileDialog::openFile( os, testDir + "_common_data/scenarios/msa", "ma.aln" );
// 2. Select any area in sequence view
    const int startRowNumber = 6;
    const int alignmentLength = 12;
    GTUtilsMSAEditorSequenceArea::click( os, QPoint( -5, startRowNumber ) );
    GTGlobals::sleep( 200 );
    GTUtilsMSAEditorSequenceArea::checkSelectedRect( os, QRect( 0, startRowNumber,
        alignmentLength, 1 ) );
// 3. Use context menu: "Edit -> Replace selected rows with complement" or "reverse" or "reverse-complement"
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << MSAE_MENU_EDIT
        << "replace_selected_rows_with_reverse") );
    GTMouseDriver::click(os, Qt::RightButton);
// Expected state: the bases in the selected area were replaced accordingly to the chosen variant
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep(200);
    QString selectionContent = GTClipboard::text( os );
    CHECK_SET_ERR( "AATTATTAGACT" == selectionContent, "MSA changing is failed" );
// 4. Press "Ctrl + Z"
    GTKeyboardDriver::keyClick( os, 'z', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep(200);
// Expected result: all rows in the selection were restored
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep(200);
    selectionContent = GTClipboard::text( os );
    CHECK_SET_ERR( "TCAGATTATTAA" == selectionContent, "MSA changing is failed" );
}

GUI_TEST_CLASS_DEFINITION( test_1884 )
{
    // 1. Open document "ma.aln"
    GTFileDialog::openFile( os, testDir + "_common_data/scenarios/msa", "ma.aln" );

    // 2. Select some row in the name list area
    const int startRowNumber = 6;
    const int alignmentLength = 12;
    GTUtilsMSAEditorSequenceArea::click( os, QPoint( -5, startRowNumber ) );
    GTGlobals::sleep( 200 );
    GTUtilsMSAEditorSequenceArea::checkSelectedRect( os, QRect( 0, startRowNumber,
        alignmentLength, 1 ) );

    // 3. Select all the upper rows and make some extra "selection"
    const int extraUpperSelectionCount = 3;
    GTKeyboardDriver::keyPress( os, GTKeyboardDriver::key["shift"] );
    GTGlobals::sleep(200);
    for ( int i = 0; i < extraUpperSelectionCount; ++i ) {
        GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["up"] );
        GTGlobals::sleep(200);
    }
    const int upperSequencesCount = 4;
    GTUtilsMSAEditorSequenceArea::checkSelectedRect( os, QRect( 0, startRowNumber-extraUpperSelectionCount,
        alignmentLength, upperSequencesCount ) );

    // 4. Decrease the selection
    const int deselectionCount = extraUpperSelectionCount - 1;
    for ( int i = 0; i < deselectionCount; ++i ) {
        GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["down"] );
        GTGlobals::sleep(200);
    }
    GTUtilsMSAEditorSequenceArea::checkSelectedRect( os, QRect( 0, startRowNumber-1,
        alignmentLength, upperSequencesCount - deselectionCount ) );
    GTKeyboardDriver::keyRelease( os, GTKeyboardDriver::key["shift"] );
}

GUI_TEST_CLASS_DEFINITION( test_1886_1 )
{
    // 1. Open document "ma.aln"
    GTFileDialog::openFile( os, testDir + "_common_data/scenarios/msa", "ma.aln" );

    // 2. Select a region in the sequence area
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 5, 4 ), QPoint( 10, 12 ) );

    // 3. Shift the region but don't release left mouse button
    const QPoint mouseDragPosition( 7, 7 );
    GTUtilsMSAEditorSequenceArea::moveTo( os, mouseDragPosition );
    GTMouseDriver::press( os );
    GTUtilsMSAEditorSequenceArea::moveTo( os, mouseDragPosition + QPoint( 3, 0 ) );

    // 4. Call context menu
    GTUtilsDialog::waitForDialog( os, new GTUtilsEscClicker( os, "msa sequence area context menu" ) );
    GTMouseDriver::click( os, Qt::RightButton );
    GTGlobals::sleep( 200 );
    GTUtilsMSAEditorSequenceArea::checkSelectedRect( os, QRect( QPoint( 8, 4 ), QPoint( 13, 12 ) ) );

    // 5. Release left mouse button
    GTMouseDriver::release( os );
    GTGlobals::sleep( 200 );

    // 6. Insert gaps with the Space button
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["space"] );
    GTGlobals::sleep( 500 );
    GTUtilsMSAEditorSequenceArea::checkSelectedRect( os, QRect( QPoint( 14, 4 ), QPoint( 19, 12 ) ) );
}

GUI_TEST_CLASS_DEFINITION( test_1886_2 )
{
    // 1. Open document "ma.aln"
    GTFileDialog::openFile( os, testDir + "_common_data/scenarios/msa", "ma.aln" );

    // 2. Select a region in the sequence area
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 5, 4 ), QPoint( 10, 12 ) );

    // 3. Shift the region but don't release left mouse button
    const QPoint mouseDragPosition( 7, 7 );
    GTUtilsMSAEditorSequenceArea::moveTo( os, mouseDragPosition );
    GTMouseDriver::press( os );
    GTUtilsMSAEditorSequenceArea::moveTo( os, mouseDragPosition + QPoint( 3, 0 ) );

    // 4. Replace selected rows with reverse
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << MSAE_MENU_EDIT
        << "replace_selected_rows_with_reverse") );
    GTMouseDriver::click(os, Qt::RightButton);

    // 5. Obtain selection
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep(200);
    const QString selectionContent = GTClipboard::text( os );
    CHECK_SET_ERR( "--TGAC\n--TGAT\n--AGAC\n--AGAT\n--AGAT\n"
        "--TGAA\n--CGAT\n--CGAT\n--CGAT" == selectionContent, "MSA changing is failed" );
}

GUI_TEST_CLASS_DEFINITION( test_1897 ) {
    //1) Run UGENE
    //2) Open data/samples/CLUSTALW/COI.aln
    GTFileDialog::openFile( os, dataDir + "samples/CLUSTALW", "COI.aln" );

    //3) Selected any region on alignment view using mouse or keyboard
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 40, 4 ), QPoint( 50, 12 ) );

    //4) Choose Highlighting/Gaps action from context-menu
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList( ) << "Highlighting"
        << "Gaps" ) );
    GTMouseDriver::click( os, Qt::RightButton );

    //5) Look at Hightlighting/Gaps action again
    QAction *action = GTAction::findActionByText( os, "Gaps" );
    //Expected state: It must be checked
    CHECK_SET_ERR( action->isChecked( ), "Action has to be checked!" );
}

GUI_TEST_CLASS_DEFINITION( test_1919 )
{
    //1) Create the WD scheme: File list -> File conversions.
    //2) Set input file: a BAM file (e.g _common_data/bam/scerevisiae.bam).
    //3) Set the result format of the converter: BAM
    //4) Run the scheme.
    //Expected: the scheme is finished well, no errors appeared.

    GTLogTracer l;

    QMenu *menu=GTMenu::showMainMenu( os, MWMENU_TOOLS );
    GTMenu::clickMenuItemByName( os, menu, QStringList() << "Workflow Designer" );

    GTUtilsWorkflowDesigner::addAlgorithm( os, "File List" );
    GTUtilsWorkflowDesigner::addAlgorithm( os, "File Format Conversion" );

    WorkflowProcessItem* fileList = GTUtilsWorkflowDesigner::getWorker(os, "File List");
    WorkflowProcessItem* fileConversion = GTUtilsWorkflowDesigner::getWorker(os, "File Format Conversion");

    GTUtilsWorkflowDesigner::connect(os, fileList, fileConversion);

    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, "File List"));
    GTMouseDriver::click(os);
    GTUtilsWorkflowDesigner::setDatasetInputFile(os, testDir + "_common_data/bam", "scerevisiae.bam");

    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, "File Format Conversion"));
    GTMouseDriver::click(os);
    GTUtilsWorkflowDesigner::setParameter(os, "Document format", 1, GTUtilsWorkflowDesigner::comboValue);

    // add setting source url in input data

    //GTWidget::click(os,GTAction::button(os,"Run workflow"));

    GTUtilsLog::check(os, l);
}

GUI_TEST_CLASS_DEFINITION( test_1921 )
{
//    1. Click rapidly on the "Show/hide log" button several (20) times (or use the Alt+3 hotkey).
    GTGlobals::sleep(2000);
    for (int i = 0; i < 20; i++) {
        GTKeyboardDriver::keyClick(os, '3', GTKeyboardDriver::key["alt"]);
        GTGlobals::sleep(20);
    }
//    Expected state: UGENE not crashes.
}

GUI_TEST_CLASS_DEFINITION( test_2006 )
{
    const int MSA_WIDTH = 30;
    const int MSA_HEIGHT = 3;

    // 1. Open "data/samples/CLUSTAL/COI.aln" and save it's part to a string
    GTFileDialog::openFile( os, dataDir + "samples/CLUSTALW/", "COI.aln" );
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( MSA_WIDTH, MSA_HEIGHT ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep(200);
    const QString initialMsaContent = GTClipboard::text( os );
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["esc"] );

    // 2. Select the second symbol in the first line
    const QPoint initialSelectionPos( 1, 0 );
    GTUtilsMSAEditorSequenceArea::click( os, initialSelectionPos );
    GTGlobals::sleep(200);

    // 3. Drag it to the first symbol in the first line
    const QPoint mouseDragPosition( 1, 0 );
    GTUtilsMSAEditorSequenceArea::moveTo( os, mouseDragPosition );
    GTMouseDriver::press( os );
    GTGlobals::sleep( 200 );
    GTUtilsMSAEditorSequenceArea::moveTo( os, mouseDragPosition + QPoint( 0, 0 ) );
    GTMouseDriver::release( os );
    GTGlobals::sleep( 200 );

    // 4. Check that the content has not been changed
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( MSA_WIDTH, MSA_HEIGHT ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep(200);
    const QString finalMsaContent = GTClipboard::text( os );
    CHECK_SET_ERR( initialMsaContent == finalMsaContent, "MSA has unexpectedly changed" );

    // 5. Check that "Undo" and "Redo" buttons are disabled
    const QAbstractButton *undo = GTAction::button( os, "msa_action_undo" );
    CHECK_SET_ERR( !undo->isEnabled( ), "Undo button is unexpectedly enabled" );
    const QAbstractButton *redo = GTAction::button( os, "msa_action_redo" );
    CHECK_SET_ERR( !redo->isEnabled( ), "Redo button is unexpectedly enabled" );
}

GUI_TEST_CLASS_DEFINITION( test_2007 )
{
    // 1. Open "_common_data/scenarios/msa/ma_empty_line.aln (and save it to string)
    GTFileDialog::openFile( os, testDir + "_common_data/scenarios/msa", "ma_empty_line.aln" );
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 11, 4 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep(200);
    const QString initialMsaContent = GTClipboard::text( os );
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["esc"] );

    // 2. Select the fourth column of the second, third and fourth lines
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 3, 1 ), QPoint( 3, 3 ) );
    GTGlobals::sleep(200);

    // 3. Drag it to the left.
    const QPoint mouseDragPosition( 3, 2 );
    GTUtilsMSAEditorSequenceArea::moveTo( os, mouseDragPosition );
    GTMouseDriver::press( os );
    GTGlobals::sleep( 200 );
    GTUtilsMSAEditorSequenceArea::moveTo( os, mouseDragPosition - QPoint( 1, 0 ) );
    GTMouseDriver::release( os );
    GTGlobals::sleep( 200 );

    // Expected state: nothing happens
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 11, 4 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep(200);
    const QString finalMsaContent = GTClipboard::text( os );
    CHECK_SET_ERR( initialMsaContent == finalMsaContent, "MSA has changed unexpectedly!" );
}

GUI_TEST_CLASS_DEFINITION( test_2012 ){
    //1. Open {data/samples/CLUSTALW/COI.aln}.
    GTFileDialog::openFile( os, dataDir + "samples/CLUSTALW/", "COI.aln" );

    //2. Open the "General" tab on the options panel.
    GTWidget::click(os, GTWidget::findWidget(os, "OP_MSA_GENERAL"));
    GTGlobals::sleep(500);
    QLineEdit *sequencelineEdit = (QLineEdit*)GTWidget::findWidget(os, "sequenceLineEdit");
    CHECK(NULL != sequencelineEdit, );

    GTLineEdit::setText(os, sequencelineEdit, "m");
    GTGlobals::sleep(500);

    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["down"]);
    GTGlobals::sleep(500);

    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["down"]);
    GTGlobals::sleep(500);

    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["down"]);
    GTGlobals::sleep(500);

    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["enter"]);
    GTGlobals::sleep(500);

    QString selectedSequence = sequencelineEdit->text();
    CHECK("Mecopoda_elongata__Sumatra_" == selectedSequence, );

    //3. Set focus to the reference sequence name line edit and enter 'm' into it.
}

GUI_TEST_CLASS_DEFINITION( test_2021_1 )
{
    // 1. Open document "ma.aln" and save it to string
    GTFileDialog::openFile( os, testDir + "_common_data/scenarios/msa", "ma.aln" );
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 11, 17 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep(200);
    const QString initialMsaContent = GTClipboard::text( os );
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["esc"] );

    // 2. Select a character in the sequence area
    const QPoint initialSelectionPos( 10, 10 );
    GTUtilsMSAEditorSequenceArea::click( os, initialSelectionPos );
    GTGlobals::sleep(200);

    // 3. Add gaps by pressing "Space" key
    const int totalShiftCount = 3;
    int shiftCounter = 0;
    QPoint currentSelection = initialSelectionPos;
    while ( shiftCounter++ < totalShiftCount ) {
        GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["space"] );
        GTGlobals::sleep(200);
    }
    currentSelection.rx( ) += totalShiftCount;

    // 4. Remove gaps with "Backspace" key
    shiftCounter = 0;
    while ( shiftCounter++ < totalShiftCount ) {
        GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["back"] );
        GTGlobals::sleep(200);
        currentSelection.rx( ) -= 1;
        GTUtilsMSAEditorSequenceArea::checkSelectedRect( os, QRect( currentSelection,
            currentSelection ) );
    }

    // 5. Check that alignment content has returned to initial state
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 11, 17 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep(200);
    const QString finalMsaContent = GTClipboard::text( os );
    CHECK_SET_ERR( initialMsaContent == finalMsaContent, "MSA has unexpectedly changed" );
}

GUI_TEST_CLASS_DEFINITION( test_2021_2 )
{
    // 1. Open document "ma.aln" and save it to string
    GTFileDialog::openFile( os, testDir + "_common_data/scenarios/msa", "ma.aln" );
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 11, 17 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep(200);
    const QString initialMsaContent = GTClipboard::text( os );
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["esc"] );

    // 2. Select a region in the sequence area
    const QPoint initialSelectionLeftTop( 5, 5 );
    const QPoint initialSelectionRightBottom( 11, 10 );
    const int selectionWidth = initialSelectionRightBottom.x( ) - initialSelectionLeftTop.x( ) + 1;
    GTUtilsMSAEditorSequenceArea::selectArea( os, initialSelectionLeftTop,
        initialSelectionRightBottom );
    GTGlobals::sleep(200);

    // 3. Add gaps by pressing "Space" key
    const int totalShiftCount = 3;
    int shiftCounter = 0;
    QPoint currentSelectionLeftTop = initialSelectionLeftTop;
    QPoint currentSelectionRightBottom = initialSelectionRightBottom;
    while ( shiftCounter++ < totalShiftCount ) {
        GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["space"] );
        GTGlobals::sleep( 200 );
    }
    currentSelectionLeftTop.rx( ) += totalShiftCount * selectionWidth;
    currentSelectionRightBottom.rx( ) += totalShiftCount * selectionWidth;
    GTUtilsMSAEditorSequenceArea::checkSelectedRect( os, QRect( currentSelectionLeftTop,
        currentSelectionRightBottom ) );

    // 4. Remove gaps with "Backspace" key
    shiftCounter = 0;
    while ( shiftCounter++ < totalShiftCount ) {
        GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["back"] );
        GTGlobals::sleep( 200 );
        currentSelectionLeftTop.rx( ) -= selectionWidth;
        currentSelectionRightBottom.rx( ) -= selectionWidth;
        GTUtilsMSAEditorSequenceArea::checkSelectedRect( os, QRect( currentSelectionLeftTop,
            currentSelectionRightBottom ) );
    }

    // 5. Check that alignment content has returned to initial state
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 11, 17 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep( 200 );
    const QString finalMsaContent = GTClipboard::text( os );
    CHECK_SET_ERR( initialMsaContent == finalMsaContent, "MSA has unexpectedly changed" );
}

GUI_TEST_CLASS_DEFINITION( test_2021_3 )
{
    // 1. Open document "ma2_gap_8_col.aln"
    GTFileDialog::openFile( os, testDir + "_common_data/scenarios/msa", "ma2_gap_8_col.aln" );

    // 2. Select a column in the sequence area
    GTUtilsMSAEditorSequenceArea::moveTo( os, QPoint( 0, 0 ) );
    GTMouseDriver::click( os );
    GTUtilsMSAEditorSequenceArea::selectColumnInConsensus( os, 9 );
    GTGlobals::sleep( 200 );

    // 3. Remove gap columns by pressing "Backspace" key
    const int totalShiftCount = 6;
    int shiftCounter = 0;
    while ( shiftCounter++ < totalShiftCount ) {
        GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["back"] );
        GTGlobals::sleep( 200 );
    }

    // 4. Check that all the empty columns were removed
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 14, 9 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep( 200 );
    const QString finalMsaContent = GTClipboard::text( os );
    CHECK_SET_ERR(  "AAGCTTCTTTTAA--\nAAGTTACTAA-----\nTAG---TTATTAA--\nAAGC---TATTAA--\n"
                    "TAGTTATTAA-----\nTAGTTATTAA-----\nTAGTTATTAA-----\nAAGCTTT---TAA--\n"
                    "A--AGAATAATTA--\nAAGCTTTTAA-----" == finalMsaContent,
                    "Unexpected MSA content has occurred" );
}

GUI_TEST_CLASS_DEFINITION( test_2021_4 )
{
    // 1. Open document "ma2_gap_8_col.aln" and save it to string
    GTFileDialog::openFile( os, testDir + "_common_data/scenarios/msa", "ma2_gap_8_col.aln" );
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 20, 9 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep(200);
    const QString initialMsaContent = GTClipboard::text( os );
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["esc"] );

    // 2. Select a region of trailing gaps
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 17, 4 ), QPoint( 19, 6 ) );

    // 3. Press "Backspace"
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["back"] );
    GTGlobals::sleep( 200 );

    // 4. Check that MSA does not changed
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 20, 9 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep( 200 );
    const QString finalMsaContent = GTClipboard::text( os );
    CHECK_SET_ERR( initialMsaContent == finalMsaContent, "MSA has unexpectedly changed" );

    // 5. Check that "Undo" and "Redo" buttons are disabled
    const QAbstractButton *undo = GTAction::button( os, "msa_action_undo" );
    CHECK_SET_ERR( !undo->isEnabled( ), "Undo button is unexpectedly enabled" );
    const QAbstractButton *redo = GTAction::button( os, "msa_action_redo" );
    CHECK_SET_ERR( !redo->isEnabled( ), "Redo button is unexpectedly enabled" );
}

GUI_TEST_CLASS_DEFINITION( test_2021_5 )
{
    // 1. Open document "ma2_gap_8_col.aln"
    GTFileDialog::openFile( os, testDir + "_common_data/scenarios/msa", "ma2_gap_8_col.aln" );

    // 2. Select a column in the sequence area
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 9, 0 ), QPoint( 16, 9 ) );

    // 3. Remove gap columns by pressing "Backspace" key
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["back"] );
    GTGlobals::sleep( 200 );

    // 4. Check that all the empty columns were removed
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 14, 9 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep( 200 );
    const QString finalMsaContent = GTClipboard::text( os );
    CHECK_SET_ERR(  "AAGCTTCTTTTAA--\nAAGTTACTAA-----\nTAG---TTATTAA--\nAAGC---TATTAA--\n"
        "TAGTTATTAA-----\nTAGTTATTAA-----\nTAGTTATTAA-----\nAAGCTTT---TAA--\n"
        "A--AGAATAATTA--\nAAGCTTTTAA-----" == finalMsaContent,
        "Unexpected MSA content has occurred" );
}

GUI_TEST_CLASS_DEFINITION( test_2021_6 )
{
    // 1. Open "data/samples/CLUSTAL/COI.aln".
    GTFileDialog::openFile( os, dataDir + "samples/CLUSTALW/", "COI.aln" );

    //2. Set cursor to the position 45 of the first line (after gaps).
    const QPoint initialSelectionPos( 44, 0 );
    GTUtilsMSAEditorSequenceArea::click( os, initialSelectionPos );
    GTGlobals::sleep(200);

    //3. Press BACKSPACE.
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["back"] );
    GTGlobals::sleep( 200 );

    // 4. Expected state: the gap was deleted, selection moves to the previous symbol.
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 43, 0 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep( 200 );
    const QString finalMsaContent = GTClipboard::text( os );
    CHECK_SET_ERR(  "TAAGACTTCTAATTCGAGCCGAATTAGGTCAACCAGGATAC--C" == finalMsaContent,
                    "Unexpected MSA content has occurred" );
}

GUI_TEST_CLASS_DEFINITION( test_2021_7 )
{
    // 1. Open "data/samples/CLUSTAL/COI.aln".
    GTFileDialog::openFile( os, dataDir + "samples/CLUSTALW/", "COI.aln" );

    //2. Select the 45 and 46 of the second line (two symbols after gaps).
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 44, 1 ), QPoint( 45, 1 ) );
    GTGlobals::sleep(200);

    //3. Press BACKSPACE.
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["back"] );
    GTGlobals::sleep( 200 );

    // 4. Expected state: the gap was deleted, selection moves to the previous symbol.
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 1 ), QPoint( 44, 1 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep( 200 );
    const QString finalMsaContent = GTClipboard::text( os );
    CHECK_SET_ERR(  "TAAGCTTACTAATCCGGGCCGAATTAGGTCAACCTGGTTAT-CTA" == finalMsaContent,
                    "Unexpected MSA content has occurred" );

}

GUI_TEST_CLASS_DEFINITION( test_2021_8 )
{
    // 1. Open "data/samples/CLUSTAL/COI.aln".
    GTFileDialog::openFile( os, dataDir + "samples/CLUSTALW/", "COI.aln" );

    //2. Select the 45 and 46 of the second line (two symbols after gaps).
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 44, 1 ), QPoint( 45, 1 ) );
    GTGlobals::sleep(200);

    //3. Press BACKSPACE.
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["back"] );
    GTGlobals::sleep( 200 );

    // 4. Expected state: the gap was deleted, selection moves to the previous symbol.
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 1 ), QPoint( 44, 1 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep( 200 );
    const QString finalMsaContent = GTClipboard::text( os );
    CHECK_SET_ERR(  "TAAGCTTACTAATCCGGGCCGAATTAGGTCAACCTGGTTAT-CTA" == finalMsaContent,
                    "Unexpected MSA content has occurred" );
}

GUI_TEST_CLASS_DEFINITION( test_2021_9 )
{
    // 1. Open "data/samples/CLUSTAL/COI.aln".
    GTFileDialog::openFile( os, dataDir + "samples/CLUSTALW/", "COI.aln" );

    //2. Select the 45 and 46 of the second line (two symbols after gaps).
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 44, 2 ), QPoint( 46, 2 ) );
    GTGlobals::sleep(200);

    //3. Press BACKSPACE.
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["back"] );
    GTGlobals::sleep( 200 );

    // 4. Expected state: the gap was deleted, selection moves to the previous symbol.
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 2 ), QPoint( 46, 2 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep( 200 );
    const QString finalMsaContent = GTClipboard::text( os );
    CHECK_SET_ERR(  "TTAGTTTATTAATTCGAGCTGAACTAGGTCAACCAGGCTATTTAATT" == finalMsaContent,
                    "Unexpected MSA content has occurred" );
}

GUI_TEST_CLASS_DEFINITION(test_2024){
//    1. Open WD
    GTUtilsWorkflowDesigner::openWorkflowDesigner(os);

//    2. Add element "Local BLAST Search"
    GTUtilsWorkflowDesigner::addAlgorithm( os, "Local BLAST Search" );
    GTMouseDriver::moveTo(os,GTUtilsWorkflowDesigner::getItemCenter(os, "blast"));
    GTMouseDriver::click(os);

//    Expected state: element has parameters "gap cost" and "match score"
    GTUtilsWorkflowDesigner::setParameter(os, "gap cost", 2, GTUtilsWorkflowDesigner::comboValue);
    GTUtilsWorkflowDesigner::setParameter(os, "Match scores", 1, GTUtilsWorkflowDesigner::comboValue);

}

GUI_TEST_CLASS_DEFINITION( test_2026 ) {
    // 1. Open data/samples/CLUSTALW/COI.aln
    GTFileDialog::openFile( os, dataDir + "samples/CLUSTALW/", "COI.aln" );

    // 2. Select Montana_montana in name list
    GTUtilsMSAEditorSequenceArea::selectSequence(os, QString("Montana_montana"));

    // 3. Press shift, click down_arrow 4 times. Release shift.
    GTKeyboardDriver::keyPress(os, GTKeyboardDriver::key["shift"]);
    for (int i = 0; i < 4; ++i) {
        GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["down"] );
        GTGlobals::sleep( 500 );
    }
    GTKeyboardDriver::keyRelease(os, GTKeyboardDriver::key["shift"]);
    GTGlobals::sleep(3000);

    // Expected state: 5 sequences are selected
    CHECK_SET_ERR( 5 == GTUtilsMSAEditorSequenceArea::getSelectedSequencesNum(os),
        "Unexpected number of selected sequences");
    CHECK_SET_ERR( GTUtilsMSAEditorSequenceArea::isSequenceSelected(os, QString("Montana_montana")),
        "Expected sequence is not selected");
    CHECK_SET_ERR( GTUtilsMSAEditorSequenceArea::isSequenceSelected(os, QString("Zychia_baranovi")),
        "Expected sequence is not selected");

    // 4. Release shift. Press shift again and click down arrow
    GTKeyboardDriver::keyPress(os, GTKeyboardDriver::key["shift"]);
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["down"] );
    GTGlobals::sleep( 500 );
    GTKeyboardDriver::keyRelease(os, GTKeyboardDriver::key["shift"]);
    GTGlobals::sleep(3000);

    // Expected state: 6 sequences selected
    CHECK_SET_ERR( 6 == GTUtilsMSAEditorSequenceArea::getSelectedSequencesNum(os),
        "Unexpected number of selected sequences");
    CHECK_SET_ERR( GTUtilsMSAEditorSequenceArea::isSequenceSelected(os, QString("Montana_montana")),
        "Expected sequence is not selected");
    CHECK_SET_ERR( GTUtilsMSAEditorSequenceArea::isSequenceSelected(os, QString("Zychia_baranovi")),
        "Expected sequence is not selected");
    CHECK_SET_ERR( GTUtilsMSAEditorSequenceArea::isSequenceSelected(os, QString("Tettigonia_viridissima")),
        "Expected sequence is not selected");
}

GUI_TEST_CLASS_DEFINITION( test_2030 ) {
    // 1. Open {_common_data/scenarios/msa/ma_one_line.aln}
    GTFileDialog::openFile(os, testDir + "_common_data/scenarios/msa/", "ma_one_line.aln");

    // 2. Set cursor to the end of the line
    const QPoint endLinePos( 11, 0 );
    GTUtilsMSAEditorSequenceArea::click( os, endLinePos );
    GTGlobals::sleep(200);

    // 3. Insert four gaps with SPACE.
    for (int i = 0; i < 4; ++i) {
        GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["space"] );
        GTGlobals::sleep( 500 );
    }

    // 4. Set cursor to the beginning of the line
    const QPoint begLinePos( 0, 0 );
    GTUtilsMSAEditorSequenceArea::click( os, begLinePos );
    GTGlobals::sleep(200);

    // 5. Delete one symbol with DELETE
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["delete"] );
    GTGlobals::sleep( 200 );

    // Expected state: line length is 15.
    CHECK_SET_ERR( 15 == GTUtilsMSAEditorSequenceArea::getLength( os ),
        "Unexpected MSA length!" );
}

GUI_TEST_CLASS_DEFINITION( test_2032 ) {
    // 1. Open {_common_data/fasta/abcd.fa} as separate sequences
    GTUtilsDialog::waitForDialog(os, new SequenceReadingModeSelectorDialogFiller(os, SequenceReadingModeSelectorDialogFiller::Separate));
    GTFileDialog::openFile(os, testDir + "_common_data/fasta/", "abcd.fa");
    GTGlobals::sleep(500);

    // Expected result: order of sequences in the project view is {d, a, c, b}
    QTreeWidgetItem *documentItem = GTUtilsProjectTreeView::findItemByText(os,
        GTUtilsProjectTreeView::getTreeWidget(os),
        "abcd.fa",
        GTGlobals::FindOptions(true));

    CHECK_SET_ERR(NULL != documentItem, "No document in the project view!");
    CHECK_SET_ERR(4 == documentItem->childCount(), "Unexpected number of objects in the document!");
    CHECK_SET_ERR("[s] d" == documentItem->child(0)->text(0), "Unexpected name of the object in the project view!");
    CHECK_SET_ERR("[s] a" == documentItem->child(1)->text(0), "Unexpected name of the object in the project view!");
    CHECK_SET_ERR("[s] c" == documentItem->child(2)->text(0), "Unexpected name of the object in the project view!");
    CHECK_SET_ERR("[s] b" == documentItem->child(3)->text(0), "Unexpected name of the object in the project view!");

    // Expected result: order of sequences in the sequences view is {d, a, c, b}
    ADVSingleSequenceWidget *seq0 = dynamic_cast<ADVSingleSequenceWidget*>(GTWidget::findWidget(os, "ADV_single_sequence_widget_0"));
    ADVSingleSequenceWidget *seq1 = dynamic_cast<ADVSingleSequenceWidget*>(GTWidget::findWidget(os, "ADV_single_sequence_widget_1"));
    ADVSingleSequenceWidget *seq2 = dynamic_cast<ADVSingleSequenceWidget*>(GTWidget::findWidget(os, "ADV_single_sequence_widget_2"));
    ADVSingleSequenceWidget *seq3 = dynamic_cast<ADVSingleSequenceWidget*>(GTWidget::findWidget(os, "ADV_single_sequence_widget_3"));

    CHECK_SET_ERR(NULL != seq0, "Failed to find a sequence widget!");
    CHECK_SET_ERR(NULL != seq1, "Failed to find a sequence widget!");
    CHECK_SET_ERR(NULL != seq2, "Failed to find a sequence widget!");
    CHECK_SET_ERR(NULL != seq3, "Failed to find a sequence widget!");

    CHECK_SET_ERR("d" == seq0->getSequenceObject()->getSequenceName(), "Unexpected sequence name!");
    CHECK_SET_ERR("a" == seq1->getSequenceObject()->getSequenceName(), "Unexpected sequence name!");
    CHECK_SET_ERR("c" == seq2->getSequenceObject()->getSequenceName(), "Unexpected sequence name!");
    CHECK_SET_ERR("b" == seq3->getSequenceObject()->getSequenceName(), "Unexpected sequence name!");
}

GUI_TEST_CLASS_DEFINITION( test_2049 ){
    
    GTFileDialog::openFile(os, dataDir + "samples/FASTA/", "human_T1.fa");
    
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList()<<"Codon table"));
    GTWidget::click(os, GTWidget::findWidget(os, "AminoToolbarButton"));
    GTGlobals::sleep(500);
    QWidget* w = GTWidget::findWidget(os, "Codon table widget");
    int ititHeight = GTWidget::findWidget(os, "Leucine (Leu)",w)->geometry().height();

    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList()<<"3. The Yeast Mitochondrial Code"));
    GTWidget::click(os, GTWidget::findWidget(os, "AminoToolbarButton"));
    GTGlobals::sleep(500);

    w = GTWidget::findWidget(os, "Codon table widget");
    int finalHeight = GTWidget::findWidget(os, "Leucine (Leu)",w)->geometry().height();
    CHECK_SET_ERR(ititHeight != finalHeight, "codone table not changed");
    }

GUI_TEST_CLASS_DEFINITION( test_2070 ){
    GTLogTracer lt;
    GTUtilsDialog::waitForDialog(os, new RemoteDBDialogFiller(os, "Q9IGQ6", 7));
    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_FILE), QStringList()<<ACTION_PROJECTSUPPORT__ACCESS_REMOTE_DB, GTGlobals::UseKey);
    GTGlobals::sleep();
    GTUtilsTaskTreeView::waitTaskFinidhed(os);

    GTUtilsDocument::isDocumentLoaded(os, "Q9IGQ6_das.gb");
    GTUtilsDocument::checkDocument(os, "Q9IGQ6_das.gb", AnnotatedDNAViewFactory::ID);
    
    GTUtilsLog::check(os, lt);
}
GUI_TEST_CLASS_DEFINITION( test_2077 ){
    // 1) Open WD

    // 2) Add elements "Read Sequence" and "Write sequence" to the scheme
    // 3) Connect "Read Sequence" to "Write sequence"

    GTUtilsWorkflowDesigner::openWorkflowDesigner(os);

    GTUtilsWorkflowDesigner::addAlgorithm( os, "Read Sequence" );
    GTUtilsWorkflowDesigner::addAlgorithm( os, "Write Sequence" );

    WorkflowProcessItem *seqReader = GTUtilsWorkflowDesigner::getWorker( os, "Read Sequence" );
    WorkflowProcessItem *seqWriter = GTUtilsWorkflowDesigner::getWorker( os, "Write Sequence" );

    GTUtilsWorkflowDesigner::connect( os, seqReader, seqWriter );

    // 4) Add file "human_T1.fa" to "Read Sequence" twice 

    GTMouseDriver::moveTo( os, GTUtilsWorkflowDesigner::getItemCenter( os, "Read Sequence" ) );
    GTMouseDriver::click( os );
    QString dirPath = dataDir + "samples/FASTA/";
    GTUtilsWorkflowDesigner::setDatasetInputFile( os, dirPath, "human_T1.fa" );

    GTMouseDriver::moveTo( os, GTUtilsWorkflowDesigner::getItemCenter( os, "Read Sequence" ) );
    GTMouseDriver::click( os );
    QString dirPath1 = dataDir + "samples/FASTA/";
    GTUtilsWorkflowDesigner::setDatasetInputFile( os, dirPath1, "human_T1.fa" );

    // 5) Validate scheme
    GTUtilsDialog::waitForDialog( os, new MessageBoxDialogFiller( os, QMessageBox::Ok,
        "Please fix issues listed in the error list (located under workflow)." ) );
    GTWidget::click( os,GTAction::button( os,"Validate workflow" ) );
    GTGlobals::sleep( 200 );

    //Expected state: The "File not found" error has appeared in the "Error list"
    GTUtilsWorkflowDesigner::checkErrorList( os, "file '../human_T1.fa' was specified several times" );
    }

GUI_TEST_CLASS_DEFINITION( test_2089 )
{
    // 1. Start UGENE with a new *.ini file.
    GTUtilsDialog::waitForDialog(os, new StartupDialogFiller(os, "!@#$%^&*()_+\";:?/", false));

    // 2. Open WD
    // 3. Set any folder without write access as workflow output directory. Click OK.
    QMenu *menu = GTMenu::showMainMenu(os, MWMENU_TOOLS);
    GTMenu::clickMenuItemByName(os, menu, QStringList() << "Workflow Designer");
}

GUI_TEST_CLASS_DEFINITION( test_2100_1 ){
    //1. Open COI.aln
    GTFileDialog::openFile(os, dataDir + "samples/CLUSTALW/", "COI.aln");

    //2. Select Hetrodes_pupus_EF540832
    GTUtilsMSAEditorSequenceArea::selectSequence(os, QString("Hetrodes_pupus_EF540832"));

    //2. Click toolbutton "Enable collapsing"
    GTWidget::click(os, GTToolbar::getWidgetForActionName(os, GTToolbar::getToolbar(os, "mwtoolbar_activemdi"), "Enable collapsing"));

    //Expected state: Hetrodes_pupus_EF540832 is still selected
    CHECK_SET_ERR( GTUtilsMSAEditorSequenceArea::isSequenceSelected(os, QString("Hetrodes_pupus_EF540832")),
                   "Required sequence is not selected");
}

GUI_TEST_CLASS_DEFINITION( test_2100_2 ){
    //1. Open COI.aln
    GTFileDialog::openFile(os, dataDir + "samples/CLUSTALW/", "COI.aln");

    //2. Click toolbutton "Enable collapsing"
    GTWidget::click(os, GTToolbar::getWidgetForActionName(os, GTToolbar::getToolbar(os, "mwtoolbar_activemdi"), "Enable collapsing"));

    //expected state: Mecopoda_elongata__Ishigaki__J and Mecopoda_elongata__Sumatra_ are collapsed
    CHECK_SET_ERR( !GTUtilsMSAEditorSequenceArea::isSequenceVisible(os, QString("Mecopoda_elongata__Sumatra_")),
                   "Required sequence is not collapsed");

    //3. Select Mecopoda_sp.__Malaysia_
    GTUtilsMSAEditorSequenceArea::selectSequence(os, QString("Mecopoda_sp.__Malaysia_"));

    //4. Expand Mecopoda_elongata__Ishigaki__J


    //Expected state: Mecopoda_sp.__Malaysia_ is still selected
    CHECK_SET_ERR( GTUtilsMSAEditorSequenceArea::isSequenceSelected(os, QString("Mecopoda_sp.__Malaysia_")),
                   "Expected sequence is not selected");
}

GUI_TEST_CLASS_DEFINITION( test_2100_3 ){
    //1. Open COI.aln
    GTFileDialog::openFile(os, dataDir+"samples/CLUSTALW/", "COI.aln");

    //2. Click toolbutton "Enable collapsing"
    GTWidget::click(os, GTToolbar::getWidgetForActionName(os, GTToolbar::getToolbar(os, "mwtoolbar_activemdi"), "Enable collapsing"));

    //3. Select Mecopoda_sp.__Malaysia_
    GTUtilsMSAEditorSequenceArea::selectSequence(os, QString("Mecopoda_sp.__Malaysia_"));

    //4. Expand Select Mecopoda_sp.__Malaysia_
    //Expected state: Mecopoda_elongata__Ishigaki__J and Mecopoda_elongata__Sumatra_ are selected

}

GUI_TEST_CLASS_DEFINITION( test_2122 ){
//    1. Open some sequence translated to amino
    GTFileDialog::openFile(os, testDir + "_common_data/fasta", "AMINO.fa");
//    2. Click Option panel button "das annotations"
    GTWidget::click(os,GTWidget::findWidget(os, "OP_DAS"));
    GTGlobals::sleep(500);
//    3. Click "show more options"
    GTWidget::click(os,GTWidget::findWidget(os,"lblShowMoreLess"),Qt::LeftButton,QPoint(10,10));
//    Expected state:Algorithm settings, DAS features sourses, Annotation settings submenus appeared
    QWidget* settingsContainerWidget = GTWidget::findWidget(os,"settingsContainerWidget");
    QWidget* sourcesContainerWidget = GTWidget::findWidget(os,"sourcesContainerWidget");
    QWidget* annotationsSettingsContainerWidget = GTWidget::findWidget(os,"annotationsSettingsContainerWidget");

    CHECK_SET_ERR(settingsContainerWidget->isVisible(), "settingsContainerWidget is not visible");
    CHECK_SET_ERR(sourcesContainerWidget->isVisible(), "sourcesContainerWidget is not visible");
    CHECK_SET_ERR(annotationsSettingsContainerWidget->isVisible(), "annotationsSettingsContainerWidget is not visible");
}

GUI_TEST_CLASS_DEFINITION( test_2124 ) {
    // 1. Open "data/samples/CLUSTALW/ty3.aln.gz".
    GTFileDialog::openFile( os, dataDir + "samples/CLUSTALW/", "ty3.aln.gz" );

    // 2. Call the context menu on the sequence area.
    const QString colorSchemeName = "Scheme";
    GTUtilsDialog::waitForDialog( os, new PopupChooser( os, QStringList( ) << "Colors"
        << "Custom schemes" << "Create new color scheme" ) );
    GTUtilsDialog::waitForDialog( os, new NewColorSchemeCreator( os, colorSchemeName,
        NewColorSchemeCreator::amino ) );
    GTMouseDriver::click( os, Qt::RightButton );

    // 3. Create a new color scheme for the amino alphabet.
    GTUtilsDialog::waitForDialog( os, new PopupChooser( os, QStringList( ) << "Colors"
        << "Custom schemes" << colorSchemeName ) );
    GTMouseDriver::click( os, Qt::RightButton );
}

GUI_TEST_CLASS_DEFINITION(test_2091) {
    //1. Open "data/samples/CLUSTALW/COI.aln".
    GTFileDialog::openFile(os, dataDir+"samples/CLUSTALW/", "COI.aln");

    //Expected state: document are opened in the project view; MSA Editor are shown with test_alignment.
    QTreeWidgetItem *msaDoc = GTUtilsProjectTreeView::findItem(os, "COI.aln");
    QWidget *msaView = GTUtilsMdi::activeWindow(os);
    CHECK(NULL != msaDoc, );
    CHECK(NULL != msaView, );

    //2. Select any sequence.
    GTUtilsMSAEditorSequenceArea::click(os, QPoint( -5, 4));
    QStringList originalNames = GTUtilsMSAEditorSequenceArea::getNameList(os);


    //3. Call context menu on the name list area, select the {Edit -> Remove current sequence} menu item.
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << MSAE_MENU_EDIT << "Remove current sequence"));
    GTMouseDriver::click(os, Qt::RightButton);
    //Expected state: the sequence is removed.
    QStringList modifiedNames = GTUtilsMSAEditorSequenceArea::getNameList(os);

    CHECK_SET_ERR(originalNames.length()-modifiedNames.length() == 1, "The number of sequences remained unchanged.");
    CHECK_SET_ERR(!modifiedNames.contains("Montana_montana"), "Removed sequence is present in multiple alignment.");
}

GUI_TEST_CLASS_DEFINITION(test_2093_1) {
//    1. Run a scheme, e.g. "Call variants with SAMtools" from the NGS samples (or any other like read->write).
    GTUtilsWorkflowDesigner::openWorkflowDesigner(os);

    // Simple scheme: read file list.
    GTUtilsWorkflowDesigner::addAlgorithm(os, "File list");
    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, "File list"));
    GTMouseDriver::click(os);
    QString dirPath = dataDir + "samples/FASTA/";
    GTUtilsWorkflowDesigner::setDatasetInputFile(os, dirPath, "human_T1.fa");

    QToolBar* wdToolbar = GTToolbar::getToolbar(os, "mwtoolbar_activemdi");
    CHECK_SET_ERR(wdToolbar, "Toolbar not found");
    GTToolbar::getWidgetForActionName(os, wdToolbar, "Run workflow");

    GTGlobals::sleep();

//    2. Select "Load schema" button on the dashboard menu line.
    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller(os, QMessageBox::Discard));

    // We must click to the "Load schema" button on dashboard's toolbar.
    // The follow code is incorrect, it should be fixed.
    QWidget* loadSchemaButton = GTWidget::findWidget(os, "Load work");
    CHECK_SET_ERR(loadSchemaButton, "Load schema button not found");
    GTWidget::click(os, loadSchemaButton);

    GTGlobals::sleep();

//    Expected result: the scheme with parameters is loaded.
    WorkflowProcessItem* wdElement = GTUtilsWorkflowDesigner::getWorker(os, "File list");
    CHECK_SET_ERR(wdElement, "Schema wasn't loaded");
}

GUI_TEST_CLASS_DEFINITION(test_2093_2) {
    // 1. Open WD.
    GTUtilsWorkflowDesigner::openWorkflowDesigner(os);

    // 2. Open any shema with the "Load workflow" button on the toolbar (not the "Open" button!)
    QString schemaPath = testDir + "_common_data/scenarios/workflow designer/222.uwl";
    GTUtilsDialog::waitForDialog(os, new GTFileDialogUtils(os, schemaPath));

    QToolBar* wdToolbar = GTToolbar::getToolbar(os, "mwtoolbar_activemdi");
    CHECK_SET_ERR(wdToolbar, "Toolbar not found");
    QWidget* loadButton = GTToolbar::getWidgetForActionName(os, wdToolbar, "Load workflow");
    CHECK_SET_ERR(loadButton, "Load button not found");
    GTWidget::click(os, loadButton);

    GTGlobals::sleep();

    // Expected result: the scheme with parameters is loaded.

    WorkflowProcessItem* wdElement = GTUtilsWorkflowDesigner::getWorker(os, "Read sequence");
    CHECK_SET_ERR(wdElement, "Schema wasn't loaded");
}


GUI_TEST_CLASS_DEFINITION( test_2128 )
{
    // 1. Open document "ma.aln" and save it to string
    GTFileDialog::openFile( os, testDir + "_common_data/scenarios/msa", "ma.aln" );
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 11, 17 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep(200);
    const QString initialMsaContent = GTClipboard::text( os );
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["esc"] );

    // 2. Select a region in the sequence area
    QRect currentSelection( QPoint( 2, 5 ), QPoint( 8, 11 ) );
    GTUtilsMSAEditorSequenceArea::selectArea( os, currentSelection.topLeft( ),
        currentSelection.bottomRight( ) );

    // 3. Add gaps by pressing "Ctrl + Space" key
    const int totalShiftCount = 3;
    for ( int shiftCounter = 0; shiftCounter < totalShiftCount; ++shiftCounter ) {
        GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["space"],
            GTKeyboardDriver::key["ctrl"] );
        GTGlobals::sleep( 200 );
        currentSelection.moveRight( currentSelection.right( ) + 1 );
        GTGlobals::sleep( 200 );
        GTUtilsMSAEditorSequenceArea::checkSelectedRect( os, currentSelection );
    }

    // 4. Remove gaps with "Ctrl + Backspace" key
    for ( int shiftCounter = 0; shiftCounter < totalShiftCount; ++shiftCounter ) {
        GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["back"],
            GTKeyboardDriver::key["ctrl"] );
        GTGlobals::sleep( 200 );
        currentSelection.moveLeft( currentSelection.left( ) - 1 );
        GTGlobals::sleep( 200 );
        GTUtilsMSAEditorSequenceArea::checkSelectedRect( os, currentSelection );
    }

    // 5. Check that alignment content has returned to initial state
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 11, 17 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep(200);
    const QString finalMsaContent = GTClipboard::text( os );
    CHECK_SET_ERR( initialMsaContent == finalMsaContent, "MSA has unexpectedly changed" );
}

GUI_TEST_CLASS_DEFINITION( test_2128_1 )
{
    // 1. Open "data/samples/CLUSTAL/COI.aln".
    GTFileDialog::openFile( os, dataDir + "samples/CLUSTALW/", "COI.aln" );

    //2. Select the six first symbols in the first sequence and press Space. 
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 5, 0 ) );
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["space"] );
    GTGlobals::sleep( 2000 );

    // Expected state: The first symbol T is on the 7 position.
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["esc"] );
    GTGlobals::sleep(200);
    const QPoint initialSelectionPos( 6, 0 );
    GTUtilsMSAEditorSequenceArea::click( os, initialSelectionPos );
    GTGlobals::sleep( 2000 );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep( 200 );
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["esc"] );
    GTGlobals::sleep(200);
    const QString finalMsaContent = GTClipboard::text( os );
    CHECK_SET_ERR(  "T" == finalMsaContent,
                    "Unexpected MSA content has occurred" );

    //3. Press the Ctrl + Space. 
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 6, 0 ), QPoint( 12, 0 ) );
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["space"], GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep( 200 );

    // Expected state: The first symbol T is on the 8 position.
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["esc"] );
    GTGlobals::sleep(200);
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 7, 0 ), QPoint( 7, 0 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep( 200 );
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["esc"] );
    GTGlobals::sleep(200);
    const QString finalMsaContent1 = GTClipboard::text( os );
    CHECK_SET_ERR(  "T" == finalMsaContent1,
                    "Unexpected MSA content has occurred" );
    
    //4. Press the Ctrl + Backspace. 
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 7, 0 ), QPoint( 13, 0 ) );
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["back"], GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep( 200 );

    // Expected state: The first symbol T is on the 8 position.
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["esc"] );
    GTGlobals::sleep(200);
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 6, 0 ), QPoint( 6, 0 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep( 200 );
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["esc"] );
    GTGlobals::sleep(200);
    const QString finalMsaContent2 = GTClipboard::text( os );
    CHECK_SET_ERR(  "T" == finalMsaContent2,
                    "Unexpected MSA content has occurred" );

    //5. Press the Backspace. 
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 6, 0 ), QPoint( 12, 0 ) );
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["back"] );
    GTGlobals::sleep( 200 );

    // Expected state: The first symbol T is on the 1 position.
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["esc"] );
    GTGlobals::sleep(200);
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 0, 0 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep( 200 );
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["esc"] );
    GTGlobals::sleep(200);
    const QString finalMsaContent3 = GTClipboard::text( os );
    CHECK_SET_ERR(  "T" == finalMsaContent3,
                    "Unexpected MSA content has occurred" );
}

GUI_TEST_CLASS_DEFINITION( test_2138 )
{
    //1. Open _common_data/scenarios/_regression/2138/1.fa
    //2. Press "Join sequences into alignment..." radio button
    //3. Press "OK" button

    GTUtilsDialog::waitForDialog(os, new SequenceReadingModeSelectorDialogFiller(os, SequenceReadingModeSelectorDialogFiller::Join));
    GTUtilsProject::openFiles(os, testDir + "_common_data/scenarios/_regression/2138/1.fa");

    GTGlobals::sleep(1000);
    //Expected state: alignment has been opened and whole msa alphabet is amino
    bool isAmino = GTUtilsMSAEditorSequenceArea::hasAminoAlphabet(os);
    CHECK_SET_ERR(true == isAmino, "Aligment has wrong alphabet type");
}

GUI_TEST_CLASS_DEFINITION( test_2140 )
{
    //1. Use main menu {Tools->Dna assembly->convert UGENE assembly data base to SAM format}
    //2. Select any .fasta file instead of .ugenedb file
    //3. Click "convert"
    //Expected state: UGENE not crashes. Error message is written to log

    GTLogTracer l;
    GTUtilsDialog::waitForDialog(os, new ConvertAssemblyToSAMDialogFiller(os, dataDir + "samples/FASTA/", "human_T1.fa"));
    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_TOOLS),
                          QStringList() << "DNA assembly" << "Convert UGENE Assembly data base to SAM format");

    CHECK_SET_ERR(l.hasError() == true, "There is no error message in log");
}

GUI_TEST_CLASS_DEFINITION( test_2144 )
{
//    1. Open Workflow Designer.
    GTUtilsWorkflowDesigner::openWorkflowDesigner(os);

//    2. Open the NGS sample scheme "Call variants with SAM tools".
    GTUtilsWorkflowDesigner::addSample(os, "Call variants with SAMtools");

//    3. Fill input data, e.g.:
//        "data/samples/Assembly/chrM.sam" as input to "Read Assembly SAM/BAM" element;
//        "data/samples/Assembly/chrM.fa" as input to "Read Sequence" element;
    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, "Read Assembly (BAM/SAM)"));
    GTMouseDriver::click(os);
    GTUtilsWorkflowDesigner::setDatasetInputFile(os, dataDir + "samples/Assembly/", "chrM.sam");

    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, "Read Sequence"));
    GTMouseDriver::click(os);
    GTUtilsWorkflowDesigner::setDatasetInputFile(os, dataDir + "samples/Assembly/", "chrM.fa");

//    4. Chose "Estimate" option in tool bar.
//       "Estimate" option is available only for NGS samples (except "Extract transcript sequence").
//    Expected state: Estimation dialog appears and provides information about approximate time of workflow run.
    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller(os,
                                                                QMessageBox::Close,
                                                                "Approximate estimation time of the workflow run is" ));
    GTWidget::click(os, GTAction::button(os, "Estimate workflow"));
}

GUI_TEST_CLASS_DEFINITION( test_2150 ){
    // 1. Open Workflow Designer.
    GTUtilsWorkflowDesigner::openWorkflowDesigner(os);
    // 2. Open the "Align sequences with MUSCLE" sample scheme.
    GTUtilsWorkflowDesigner::addSample(os, "Align sequences with MUSCLE");

    // 3. Set "data/samples/CLUSTALW/ty3.aln.gz" as the input file.
    GTMouseDriver::moveTo(os,GTUtilsWorkflowDesigner::getItemCenter(os,"Read alignment"));
    GTMouseDriver::click(os);
    GTUtilsWorkflowDesigner::setDatasetInputFile(os, dataDir + "samples/CLUSTALW/", "ty3.aln.gz");

    // 4. Set some name to the result file.
    QTableView* table = qobject_cast<QTableView*>(GTWidget::findWidget(os,"table"));
    CHECK_SET_ERR(table,"tableView not found");
    GTMouseDriver::moveTo(os,GTUtilsWorkflowDesigner::getItemCenter(os,"Write alignment"));
    GTMouseDriver::click(os);
    GTMouseDriver::moveTo(os,GTTableView::getCellPosition(os,table,1,1));
    GTMouseDriver::click(os);
    QString s = QFileInfo(testDir + "_common_data/scenarios/sandbox/").absoluteFilePath();
    GTKeyboardDriver::keySequence(os, s+"/2150_0001.sto");
    GTWidget::click(os,GTUtilsMdi::activeWindow(os));

    // 5. Run the workflow.
    GTWidget::click(os,GTAction::button(os,"Run workflow"));
    GTGlobals::sleep(5000);
    //GTUtilsTaskTreeView::waitTaskFinidhed(os,1000);

    // 6. During the workflow execution open the "Tasks" panel in the bottom, find in the task tree the "MUSCLE alignment" subtask and cancel it.
    GTUtilsTaskTreeView::cancelTask(os, "MUSCLE alignment");
}

GUI_TEST_CLASS_DEFINITION( test_2152 ){
    //1. Open WD
    //2. Create a scheme with the following elements: File list, Assembly Sequences with CAP3
    //3. Put _common_data/cap3/region1.fa,
    //       _common_data/cap3/region2.fa,
    //       _common_data/cap3/region3.fa,
    //       _common_data/cap3/region4.fa as an input sequences
    //4. Run the scheme
    //Expected state: the dashboard appears, the WD task has been finished without errors.

    GTLogTracer l;
    GTUtilsWorkflowDesigner::openWorkflowDesigner(os);

    GTUtilsWorkflowDesigner::addAlgorithm( os, "File List" );
    GTUtilsWorkflowDesigner::addAlgorithm( os, "Assembly Sequences with CAP3" );

    WorkflowProcessItem* fileList = GTUtilsWorkflowDesigner::getWorker(os, "File List");
    WorkflowProcessItem* fileCAP3 = GTUtilsWorkflowDesigner::getWorker(os, "Assembly Sequences with CAP3");

    GTUtilsWorkflowDesigner::connect(os, fileList, fileCAP3);

    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, "CAP3"));
    GTMouseDriver::click(os);
    GTUtilsWorkflowDesigner::setParameter(os,"Output file", QDir().absoluteFilePath(sandBoxDir + "out.ace"),GTUtilsWorkflowDesigner::textValue);

    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, "File List"));
    GTMouseDriver::click(os);
    GTUtilsWorkflowDesigner::setDatasetInputFile(os, testDir + "_common_data/cap3", "region1.fa");
    GTUtilsWorkflowDesigner::setDatasetInputFile(os, testDir + "_common_data/cap3", "region2.fa");
    GTUtilsWorkflowDesigner::setDatasetInputFile(os, testDir + "_common_data/cap3", "region3.fa");
    GTUtilsWorkflowDesigner::setDatasetInputFile(os, testDir + "_common_data/cap3", "region4.fa");

    GTWidget::click(os,GTAction::button(os,"Run workflow"));
    GTUtilsTaskTreeView::waitTaskFinidhed(os);

    GTUtilsLog::check(os, l);
}

GUI_TEST_CLASS_DEFINITION( test_2156 ){
//    1. Open "data/samples/CLUSTALW/COI.aln".
    GTFileDialog::openFile(os, dataDir + "samples/CLUSTALW/","COI.aln");
//    2. Select six symbols (45-50) of the first line.
    GTUtilsMSAEditorSequenceArea::selectArea(os,QPoint(44,0),QPoint(49,0));
//    3. Press BACKSPACE.
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["back"]);
//    Expected state: three gaps before the selected area are removed.
    GTWidget::click(os, GTUtilsMdi::activeWindow(os));
    GTUtilsMSAEditorSequenceArea::selectArea(os,QPoint(41,0),QPoint(44,0));
    GTKeyboardDriver::keyClick(os, 'c', GTKeyboardDriver::key["ctrl"]);
    QString clipboardText = GTClipboard::text(os);
    CHECK_SET_ERR(clipboardText == "CTAA", QString("Expected: CTAA, found: %1").arg(clipboardText) );

}

GUI_TEST_CLASS_DEFINITION( test_2157 )
{
//    1. Open file "https://ugene.unipro.ru/tracker/secure/attachment/12864/pdb1a07.ent.gz".
    GTFileDialog::openFile(os, testDir + "_common_data/pdb/", "pdb1a07.ent.gz");

//    Expected state: UGENE displays sequence and 3d structure
    GTUtilsMdi::findWindow(os, "pdb1a07.ent.gz");
}

GUI_TEST_CLASS_DEFINITION( test_2160 )
{
    // 1. Open document "ma.aln"
    GTFileDialog::openFile( os, testDir + "_common_data/scenarios/msa", "ma.aln" );

    // 2. Select whole msa
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 11, 17 ) );

    // 3. Shift msa
    const QPoint mouseDragPosition( 7, 7 );
    GTUtilsMSAEditorSequenceArea::moveTo( os, mouseDragPosition );
    GTMouseDriver::press( os );
    GTGlobals::sleep( 200 );
    GTUtilsMSAEditorSequenceArea::moveTo( os, mouseDragPosition + QPoint( 3, 0 ) );
    GTMouseDriver::release( os );
    GTGlobals::sleep( 200 );

    // 4. Press "Delete" key
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["delete"] );
    GTGlobals::sleep( 200 );

    // 6. Check that alignment has not changed
    GTUtilsMSAEditorSequenceArea::checkSelectedRect( os, QRect( QPoint( 3, 0 ), QPoint( 14, 17 ) ) );
    CHECK_SET_ERR( 15 == GTUtilsMSAEditorSequenceArea::getLength( os ),
        "MSA length unexpectedly changed" );
    CHECK_SET_ERR( 18 == GTUtilsMSAEditorSequenceArea::getNameList( os ).size( ),
        "MSA row count unexpectedly changed" );
}

GUI_TEST_CLASS_DEFINITION( test_1924 )
{   
    //1. Open any sequence
    GTFileDialog::openFile( os, dataDir + "samples/FASTA/", "human_T1.fa");
    GTGlobals::sleep();

    //2. Use context menu on the sequence     {Edit sequence -> Insert subsequence}
    //3. Fill in "atcgtac" or any valid sequence containing lower case
    Runnable *filler = new InsertSequenceFiller(os,
        "atcgtac"
        );
    GTUtilsDialog::waitForDialog(os, filler);
    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_ACTIONS), QStringList() <<  ADV_MENU_EDIT << ACTION_EDIT_INSERT_SUBSEQUENCE, GTGlobals::UseKey);
    GTGlobals::sleep();

    //4. Click OK
    //Expected state: subsequence inserted
    //Bug state: Warning message is shown first

    int sequenceLength = GTUtilsSequenceView::getLengthOfSequence(os);
    CHECK_SET_ERR(sequenceLength == 199957, "Sequence length is " + QString::number(sequenceLength) + ", expected 199957");

    QString sequenceBegin = GTUtilsSequenceView::getBeginOfSequenceAsString(os, 7);
    CHECK_SET_ERR(sequenceBegin == "ATCGTAC", "Sequence starts with <" + sequenceBegin + ">, expected ATCGTAC");

}

GUI_TEST_CLASS_DEFINITION(test_1986){
//1. Run UGENE
//2. Use main toolbar { File -> Search NCBI Genbank }
    GTUtilsDialog::waitForDialog(os, new NCBISearchDialogFiller(os, "human", false,5));

    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_FILE), QStringList()<<ACTION_PROJECTSUPPORT__SEARCH_GENBANK);
    GTGlobals::sleep();
//Expected state: the "NCBI Sequence Search" dialog has appeared

//3. Type "human" to the request string

//4. In the dialog's right bottom corner set "Result limit" to 5

//5. Press the "Search" button

//Expected state: only 5 results has appeared in the "Results" list

//6. Choose some result sequence

//7. Press the "Download" button

//Expected state: the "Fetch Data from Remote Database" dialog has appeared, it has the "Output format" combobox

//8. Select "fasta" output format

//9. Press "OK"
    GTUtilsTaskTreeView::waitTaskFinidhed(os);
    QTreeWidget* treeWidget = GTUtilsProjectTreeView::getTreeWidget(os);
    QList<QTreeWidgetItem*> treeItems = GTTreeWidget::getItems(treeWidget->invisibleRootItem());
    QString text = treeItems.takeFirst()->text(0);
    CHECK_SET_ERR(text.contains(".fasta"),text);

//Expected state: the chosen sequence has been downloaded, saved in FASTA format and displayed in sequence view
}
GUI_TEST_CLASS_DEFINITION( test_2163 ) {
    // 1. Open "_common_data/fasta/AMINO.fa".
    GTFileDialog::openFile(os, testDir + "_common_data/fasta/", "AMINO.fa");

    // 2. Open the DAS widget on the options panel
    GTWidget::click(os, GTWidget::findWidget(os, "OP_DAS"));
    GTGlobals::sleep(500);
    QWidget *dasPanel = GTWidget::findWidget(os, "DasOptionsPanelWidget");
    CHECK(NULL != dasPanel, );
   
    //3. Press "Fetch IDs".
    QWidget *searchIdsButton = GTWidget::findWidget(os, "searchIdsButton");
    GTWidget::click(os, searchIdsButton);
    GTGlobals::sleep(500);
   
   //4. Double click on the results table.
    GTUtilsTaskTreeView::waitTaskFinidhed(os);
    QTableWidget *idList = qobject_cast<QTableWidget*>(GTWidget::findWidget(os, "idList"));

    GTGlobals::sleep();

    QPoint p1 = idList->mapFromGlobal(GTTableView::getCellPosition(os, idList, 0, 0));
    GTMouseDriver::moveTo(os, GTTableView::getCellPosition(os, idList, 0, 0 ));
    QString value1 = idList->itemAt(p1)->text();
    GTMouseDriver::doubleClick(os);


    // 4. Try to delete value from table
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["delete"]);

    //Expected state: value not deleted
    QString value2 = idList->itemAt(p1)->text();
    CHECK_SET_ERR(value1 == value2, "Results differ!");
    GTUtilsTaskTreeView::waitTaskFinidhed(os);
}

GUI_TEST_CLASS_DEFINITION( test_2164 ) {
    // 1. Open "_common_data/fasta/AMINO.fa".
    GTFileDialog::openFile(os, testDir + "_common_data/fasta/", "AMINO.fa");

    // 2. Open the DAS widget on the options panel
    GTWidget::click(os, GTWidget::findWidget(os, "OP_DAS"));
    GTGlobals::sleep(500);
    QWidget *dasPanel = GTWidget::findWidget(os, "DasOptionsPanelWidget");
    CHECK(NULL != dasPanel, );
    
    //3. Press "Fetch IDs".
    QWidget *searchIdsButton = GTWidget::findWidget(os, "searchIdsButton");
    GTWidget::click(os, searchIdsButton);
    GTGlobals::sleep(500);
    
    //Expected: the "Fetch ID" button is disabled until the task is finished or canceled.
    CHECK_SET_ERR(!searchIdsButton->isEnabled(), "Fetch IDs is enabled!");
    TaskScheduler* scheduller = AppContext::getTaskScheduler();
    GTGlobals::sleep(5000);
    while(!scheduller->getTopLevelTasks().isEmpty()){
        GTGlobals::sleep();
    }
    CHECK_SET_ERR(searchIdsButton->isEnabled(), "Fetch IDs is disabled!");
    //4. Right click on the results table.
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() <<"Fetch IDs"));
    GTWidget::click(os, GTWidget::findWidget(os, "idList"), Qt::RightButton);

    //Expected: the context menu action "Fetch IDs" is disabled until the task is finished or canceled.
    CHECK_SET_ERR(!searchIdsButton->isEnabled(), "Fetch IDs is enabled!");
    GTGlobals::sleep(500);
    TaskScheduler* scheduller1 = AppContext::getTaskScheduler();
    GTGlobals::sleep(5000);
    while(!scheduller1->getTopLevelTasks().isEmpty()){
       GTGlobals::sleep();
    }
    CHECK_SET_ERR(searchIdsButton->isEnabled(), "Fetch IDs is disabled!");
    GTGlobals::sleep(500);
}

GUI_TEST_CLASS_DEFINITION( test_2165 ) {
    GTLogTracer l;
    //1. Open human_t1
    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");

    //2. Copy the whole sequence to the clipboard
    GTWidget::click(os, GTWidget::findWidget(os, "annotated_DNA_scrollarea"));
    GTUtilsDialog::waitForDialog(os, new selectSequenceRegionDialogFiller(os));
    GTWidget::click(os, GTWidget::findWidget(os, "select_range_action"));
    GTGlobals::sleep(500);

    GTKeyboardDriver::keyClick(os, 'c', GTKeyboardDriver::key["ctrl"]);

    //3. Past the whole sequence to the find pattern field
    GTWidget::click(os, GTWidget::findWidget(os, "OP_FIND_PATTERN"));
    GTWidget::click(os, GTWidget::findWidget(os, "textPattern"));

    GTKeyboardDriver::keyClick(os, 'v', GTKeyboardDriver::key["ctrl"]);
    GTGlobals::sleep(500);

    //4. Press the "Search" button
    GTWidget::click(os, GTWidget::findWidget(os, "btnSearch"));
    GTGlobals::sleep(500);
    GTUtilsTaskTreeView::waitTaskFinidhed(os);

    //Expected: UGENE finds the sequence or shows a error message
    CHECK_SET_ERR(l.hasError() == true, "Error message expected in log");
}

GUI_TEST_CLASS_DEFINITION( test_2167 ) {
    // 1. Open "_common_data/fasta/AMINO.fa".
    GTFileDialog::openFile(os, testDir + "_common_data/fasta/", "AMINO.fa");

    // 2. Open the DAS widget on the options panel
    GTWidget::click(os, GTWidget::findWidget(os, "OP_DAS"));
    GTGlobals::sleep(500);
    QWidget *dasPanel = GTWidget::findWidget(os, "DasOptionsPanelWidget");
    CHECK_SET_ERR(NULL != dasPanel, "DasOptionsPanelWidget is NULL!");

    // 3. Click "Fetch ids".
    QWidget *searchIdsButton = GTWidget::findWidget(os, "searchIdsButton");
    GTWidget::click(os, searchIdsButton);
    GTGlobals::sleep(500);

    GTUtilsTaskTreeView::waitTaskFinidhed(os);

    // 4. Select a result id in the table.
    QTableWidget* resultsView = qobject_cast<QTableWidget*>(GTWidget::findWidget(os, "idList"));
    CHECK_SET_ERR(NULL != resultsView, "Results widget is NULL!");
    QPoint p1 = resultsView->mapFromGlobal(GTTableView::getCellPosition(os, resultsView, 0, 3));
    GTWidget::click(os, resultsView, Qt::LeftButton, p1);

    int count1 = resultsView->model()->rowCount();
    QString value1 = resultsView->itemAt(p1)->text();

    // 5. Click "Fetch ids" once again.
    GTWidget::click(os, searchIdsButton);
    GTGlobals::sleep(500);

    GTUtilsTaskTreeView::waitTaskFinidhed(os);

    // Expected: the table contains only the last results. There are no previous items and additional empty lines.
    QPoint p2 = resultsView->mapFromGlobal(GTTableView::getCellPosition(os, resultsView, 0, 3));
    GTWidget::click(os, resultsView, Qt::LeftButton, p2);

    int count2 = resultsView->model()->rowCount();
    QString value2 = resultsView->itemAt(p2)->text();

    CHECK_SET_ERR(count1 == count2, "There are empty rows!");
    CHECK_SET_ERR(value1 == value2, "Results differ!");
}

GUI_TEST_CLASS_DEFINITION( test_2169 ) {
    // 1. Open "data/PDB/1CF7.pdb".
    GTFileDialog::openFile(os, dataDir + "samples/PDB", "1CF7.PDB");

    // 2. Open the DAS widget on the options panel
    GTWidget::click(os, GTWidget::findWidget(os, "OP_DAS"));
    GTGlobals::sleep(500);

    QWidget *dasPanel = GTWidget::findWidget(os, "DasOptionsPanelWidget");
    CHECK(NULL != dasPanel, );
    QWidget *regionSelector = GTWidget::findWidget(os, "range_selector");
    CHECK(NULL != regionSelector, );

    GTWidget::click(os, GTWidget::findWidget(os,"ADV_single_sequence_widget_0"));
    GTGlobals::sleep(500);
    CHECK_SET_ERR(dasPanel->isEnabled(), "DAS panel is disabled for amino acid sequence");
    CHECK_SET_ERR(regionSelector->isEnabled(), "DAS panel child is disabled for amino acid sequence");

    GTWidget::click(os, GTWidget::findWidget(os,"ADV_single_sequence_widget_2"));
    GTGlobals::sleep(500);
    CHECK_SET_ERR(!dasPanel->isEnabled(), "DAS panel is enabled for nucleic acid sequence");
    CHECK_SET_ERR(!regionSelector->isEnabled(), "DAS panel child is enabled for nucleic acid sequence");
}

GUI_TEST_CLASS_DEFINITION( test_2171 )
{
    //1. Open file _common_data/fasta/AMINO.fa
    GTFileDialog::openFile( os, testDir + "_common_data/fasta", "AMINO.fa");
    //2. Open DAS option panel
    GTWidget::click( os, GTWidget::findWidget( os, "OP_DAS" ) );
    //3. Click fetch IDs
    GTWidget::click( os, GTWidget::findWidget( os, "searchIdsButton" ) );
    //4. Cancel task  "Fetch IDs"
    GTUtilsTaskTreeView::cancelTask(os, "Get an ID for the sequence");
    //Expected state: task canceled
    QTreeWidget *taskTreeWidget = GTUtilsTaskTreeView::getTreeWidget(os);
    CHECK_SET_ERR(taskTreeWidget->invisibleRootItem()->childCount() == 0, "Task is not canceled");
}

GUI_TEST_CLASS_DEFINITION( test_2172 ) {
    GTLogTracer logTracer;

    // 1. Open file _common_data/fasta/amino_multy.fa
    GTUtilsDialog::waitForDialog( os, new SequenceReadingModeSelectorDialogFiller( os ) );
    const QString filePath = testDir + "_common_data/fasta/amino_multy.fa";
    GTUtilsProject::openFiles(os, QList<QUrl>( ) << filePath );
    GTGlobals::sleep( );

    // 2. Open DAS option panel
    GTWidget::click( os, GTWidget::findWidget( os, "OP_DAS" ) );
    GTGlobals::sleep( 200 );

    // 3. Check that warning message exists
    const QLabel *warnignLabel = qobject_cast<QLabel*>( GTWidget::findWidget( os, "hintLabel" ) );
    CHECK_SET_ERR( NULL != warnignLabel, "Cannot find the hint label" );

    QString warningText = warnignLabel->text( );
    CHECK_SET_ERR( warningText.startsWith( "Warning" ), "No warning message found" );

    // 4. Press the "Fetch IDs" button
    QPushButton *fetchButton = qobject_cast<QPushButton *>( GTWidget::findWidget( os,
        "searchIdsButton" ) );
    CHECK_SET_ERR( NULL != fetchButton, "Cannot find the \"Fetch IDs\" button" );
    fetchButton->click( );
    GTGlobals::sleep( 200 );

    // 5. Check error message in log
    CHECK_SET_ERR( logTracer.hasError( ), "Error message expected in log" );
}

GUI_TEST_CLASS_DEFINITION( test_2174 ) {
    //1. Open file _common_data/fasta/AMINO.fa
    GTFileDialog::openFile( os, testDir + "_common_data/fasta/", "AMINO.fa");
    GTGlobals::sleep();

    //2. Open DAS option panel
    GTWidget::click(os,GTWidget::findWidget(os, "OP_DAS"));
    GTGlobals::sleep(500);

    //3. Select region 1-2
    GTRegionSelector::RegionSelectorSettings regionSelectorSettings(1, 2);
    RegionSelector *regionSelector = qobject_cast<RegionSelector*>(GTWidget::findWidget(os, "range_selector"));
    GTRegionSelector::setRegion(os, regionSelector, regionSelectorSettings);

    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["enter"] );
    GTGlobals::sleep( 200 );

    //Expected state: warning message appeares
    
    QLabel *warnignLabel = qobject_cast<QLabel*>(GTWidget::findWidget(os, "hintLabel"));
    CHECK_SET_ERR(warnignLabel != NULL, "Cannot find the hint label");

    QString warningText = warnignLabel->text();
    CHECK_SET_ERR( warningText.startsWith("Warning"), "No warning message found");
}

GUI_TEST_CLASS_DEFINITION( test_2186 ) {
//    1. Open file _common_data/fasta/amino_multy.fa
    GTUtilsDialog::waitForDialog(os, new SequenceReadingModeSelectorDialogFiller(os, SequenceReadingModeSelectorDialogFiller::Separate));
    GTFileDialog::openFile( os, testDir + "_common_data/fasta", "amino_multy.fa");
    GTGlobals::sleep(500);
//    2. Open the DAS widget on the options panel
    GTWidget::click(os,GTWidget::findWidget(os,"ADV_single_sequence_widget_0"));
    GTWidget::click(os,GTWidget::findWidget(os, "OP_DAS"));
    GTGlobals::sleep(500);
//    3. Select first sequence, select region 1-100
    GTRegionSelector::RegionSelectorSettings regionSelectorSettings(1, 100);
    RegionSelector *regionSelector = qobject_cast<RegionSelector*>(GTWidget::findWidget(os, "range_selector"));
    GTRegionSelector::setRegion(os, regionSelector, regionSelectorSettings);

    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["enter"] );
    GTGlobals::sleep( 200 );
//    4. Click fetch IDs
    GTWidget::click( os, GTWidget::findWidget( os, "searchIdsButton" ) );
    TaskScheduler* scheduller = AppContext::getTaskScheduler();
    while(!scheduller->getTopLevelTasks().isEmpty()){
        GTGlobals::sleep();
    }
//    Expected state:2 IDs are found
    GTGlobals::sleep();
    QTableWidget* idList = qobject_cast<QTableWidget*>(GTWidget::findWidget(os, "idList"));
    CHECK_SET_ERR(idList,"idList widget not found");
    CHECK_SET_ERR(idList->rowCount()==2,QString("idList contains %1 rows, expected 2").arg(idList->rowCount()));

//    5. select second sequence
    GTWidget::click(os,GTWidget::findWidget(os,"ADV_single_sequence_widget_1"));
    GTGlobals::sleep(500);
//    Expected state: IDs table on option panel cleared
     CHECK_SET_ERR(idList->rowCount()==0,QString("idList contains %1 rows, expected 0").arg(idList->rowCount()));


}

GUI_TEST_CLASS_DEFINITION( test_2188 ) {
    class Helper {
    public:
        Helper(const QString &dataDir, const QString &testDir, U2OpStatus &os) {
            dir = testDir + "_common_data/scenarios/sandbox/";
            fileName = "regression_test_2188.fa";
            url = dir + fileName;
            bool copied = QFile::copy(dataDir + "samples/FASTA/human_T1.fa", url);
            if (!copied) {
                os.setError("Can not copy the file");
                url = "";
            }
        }

        QString url;
        QString dir;
        QString fileName;
    };

    // 1. Open the file "data/samples/FASTA/human_T1.fa"
    Helper helper(dataDir, testDir, os);
    CHECK_OP(os, );
    GTFileDialog::openFile( os, helper.dir, helper.fileName );
    GTGlobals::sleep(500);

    // 2. At the same time open the file with a text editor
    // 3. Change something and save
    // Expected state: Dialog suggesting file to reload has appeared in UGENE
    // 4. Press "Yes to All" button
    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller(os, QMessageBox::Yes));
    QFile file(helper.url);
    bool opened = file.open(QIODevice::Append);
    CHECK_SET_ERR(opened, "Can not open the file for writing");
    qint64 writed = file.write("AAAAAAAAAA");
    CHECK_SET_ERR(10 == writed, "Can not write to the file");
    file.close();
    GTGlobals::sleep(6000);

    // Expected state: All the sequences were reloaded and displayed correctly in sequence view
    int length = GTUtilsSequenceView::getLengthOfSequence(os);
    CHECK_OP(os, );
    CHECK_SET_ERR(199960 == length, "The file lenght is wrong");
}

GUI_TEST_CLASS_DEFINITION( test_2187 ) {
    //1. Open "data/samples/FASTA/human_T1.fa"
    //d:\src\ugene\trunk\test\_common_data\scenarios\_regression\2187\seq.fa

    GTFileDialog::openFile(os, testDir + "_common_data/scenarios/_regression/2187/", "seq.fa");
    GTUtilsDocument::checkDocument(os, "seq.fa");

    //2. Open {Actions -> Analyze -> Find tandems...}
    //3. Click ok

    Runnable * tDialog = new FindTandemsDialogFiller(os, testDir + "_common_data/scenarios/sandbox/result_2187.gb");
    GTUtilsDialog::waitForDialog(os, tDialog);

    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << ADV_MENU_ANALYSE
        << "find_tandems_action", GTGlobals::UseMouse));
    GTMenu::showMainMenu(os, MWMENU_ACTIONS);
    GTGlobals::sleep();

    QTreeWidget *treeWidget = GTUtilsAnnotationsTreeView::getTreeWidget(os);
    CHECK_SET_ERR(treeWidget != NULL, "Tree widget is NULL");

    QTreeWidgetItem *annotationsRoot = GTUtilsAnnotationsTreeView::findItem(os, "repeat_unit");
    GTMouseDriver::moveTo(os, GTTreeWidget::getItemCenter(os, annotationsRoot->child(0)));
    GTMouseDriver::doubleClick(os);

    Runnable *filler = new EditAnnotationChecker(os, "repeat_unit", "join(251..251,252..252,253..253,254..254,255..255,256..256,257..257,258..258,259..259)");
    GTUtilsDialog::waitForDialog(os, filler);

    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["F2"]);
    GTGlobals::sleep();

    GTUtilsMdi::click(os, GTGlobals::Close);
    GTMouseDriver::click(os);
}

GUI_TEST_CLASS_DEFINITION( test_2202 )
{
//    1. Open Workflow Designer first time (e.g. remove UGENE.ini before it).
//    Expected: the dialog appears.

//    2. Write the path to the directory which does not exist.

//    3. Click OK.
//    Expected: the directory is created, there are no error messages about write access.

    QDir workflowOutputDir(testDir + "_common_data/scenarios/sandbox/regression_test_2202/1/2/3/4/");
    CHECK_SET_ERR(!workflowOutputDir.exists(), "Dir already exists");

    GTUtilsDialog::waitForDialog(os, new StartupDialogFiller(os, workflowOutputDir.absolutePath()));
    QMenu* menu = GTMenu::showMainMenu(os, MWMENU_TOOLS);

    GTMenu::clickMenuItemByName(os, menu, QStringList() << "Workflow Designer");

    GTGlobals::sleep();
    CHECK_SET_ERR(workflowOutputDir.exists(), "Dir wasn't created");
}

GUI_TEST_CLASS_DEFINITION( test_2224 )
{
    // 1. Open document "ma.aln"
    GTFileDialog::openFile( os, testDir + "_common_data/scenarios/msa", "ma.aln" );

    // 2. Save the initial content
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 11, 17 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep(200);
    const QString initialContent = GTClipboard::text( os );

    // 2.1. Remove selection
    GTKeyboardDriver::keyClick( os, GTKeyboardDriver::key["esc"] );
    GTGlobals::sleep(200);
    GTUtilsMSAEditorSequenceArea::checkSelectedRect( os, QRect( ) );

    // 3. Select a region in the sequence area
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 4 ), QPoint( 7, 12 ) );
    GTGlobals::sleep(500);

    // 4. Shift the region in one base
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 6, 7 ), QPoint( 7, 7 ) );

    // 5. Click in the leftmost column of the selected region
    GTUtilsMSAEditorSequenceArea::click( os, QPoint( 1, 7 ) );
#ifdef Q_OS_MAC
    GTGlobals::sleep(1000);
    GTMouseDriver::click(os);
#endif
    // 7. Check the result state
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 0, 0 ), QPoint( 11, 17 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep(200);
    const QString undoneContent = GTClipboard::text( os );
    CHECK_SET_ERR( undoneContent == initialContent,
        "Scribbling works wrong. Found text is:\n" + undoneContent );
}

GUI_TEST_CLASS_DEFINITION( test_2224_flow )
{
    // 1. Open file {_common_data/scenarios/msa/ma.aln"
    GTFileDialog::openFile( os, testDir + "_common_data/scenarios/msa", "ma.aln" );

    // copy the initial MSA
    GTUtilsMSAEditorSequenceArea::selectArea(os, QPoint(0, 0), QPoint(11, 17));
    GTKeyboardDriver::keyClick(os, 'c', GTKeyboardDriver::key["ctrl"]);
    GTGlobals::sleep(500);
    QString initialMSA = GTClipboard::text(os);
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["esc"]);

    // 2. Select some region starting from the 1st column and ending in the 8th one
    GTUtilsMSAEditorSequenceArea::selectArea(os, QPoint(0, 2), QPoint(7, 6));

    // 3. Drag it to the right in one column
    GTUtilsMSAEditorSequenceArea::selectArea(os, QPoint(4, 4), QPoint(5, 4));

    // 4. Click left mouse button in the leftmost side of the selected region
    GTUtilsMSAEditorSequenceArea::moveTo(os, QPoint(1, 4));
    GTMouseDriver::click(os);

    // copy the result MSA
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["esc"]);
    GTUtilsMSAEditorSequenceArea::selectArea(os, QPoint(0, 0), QPoint(11, 17));
    GTKeyboardDriver::keyClick(os, 'c', GTKeyboardDriver::key["ctrl"]);
    GTGlobals::sleep(500);
    QString resultMSA = GTClipboard::text(os);

    CHECK_SET_ERR(initialMSA == resultMSA, "The result MSA is wrong");
}

GUI_TEST_CLASS_DEFINITION( test_2225_1 ){
    Runnable *filler = new NCBISearchDialogFiller(os, "rat", true);

    GTUtilsDialog::waitForDialog(os, filler);

    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_FILE), QStringList()<<ACTION_PROJECTSUPPORT__SEARCH_GENBANK, GTGlobals::UseKey);
    GTGlobals::sleep();
}

GUI_TEST_CLASS_DEFINITION( test_2225_2 ){
    Runnable *filler = new NCBISearchDialogFiller(os, "rat", true);

    GTUtilsDialog::waitForDialog(os, filler);

    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_FILE), QStringList()<<ACTION_PROJECTSUPPORT__SEARCH_GENBANK, GTGlobals::UseKey);
    GTGlobals::sleep();
}
GUI_TEST_CLASS_DEFINITION( test_2259 ){
    MainWindow *mw = AppContext::getMainWindow();
    CHECK_SET_ERR(mw != NULL, "MainWindow is NULL");
    QMainWindow *mainWindow = mw->getQMainWindow();
    CHECK_SET_ERR(mainWindow != NULL, "QMainWindow is NULL");

    QAction *menu = mainWindow->findChild<QAction*>(MWMENU_SETTINGS);
    CHECK_SET_ERR(menu->menu()->actions().size()==2, "wrong numder of actions");

    }

GUI_TEST_CLASS_DEFINITION( test_2266_1 ){

    GTUtilsDialog::waitForDialog(os, new StartupDialogFiller(os, testDir + "_common_data/scenarios/sandbox"));

    QMenu* menu=GTMenu::showMainMenu(os, MWMENU_TOOLS);
    GTMenu::clickMenuItemByName(os, menu, QStringList() << "Workflow Designer");
    GTUtilsWorkflowDesigner::addSample(os, "call variants");

    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, "Read assembly"));
    GTMouseDriver::click(os);
    GTUtilsWorkflowDesigner::setDatasetInputFile(os, testDir + "_common_data/ugenedb/", "Klebsislla.sort.bam.ugenedb");

    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, "Read sequence"));
    GTMouseDriver::click(os);
    GTUtilsWorkflowDesigner::setDatasetInputFile(os, testDir + "_common_data/ugenedb/", "Klebsislla_ref.fa");

    //this is the name of output dir. Created here: WorkflowContextCMDLine::createSubDirectoryForRun
    QString baseDirName = QDateTime::currentDateTime().toString("yyyy.MM.dd_hh-mm");
    GTWidget::click(os,GTAction::button(os,"Run workflow"));

    GTUtilsTaskTreeView::waitTaskFinidhed(os);

    GTFileDialog::openFile(os, testDir + "_common_data/scenarios/sandbox/" + baseDirName + "/", "variations.vcf");

    QTreeWidgetItem *seqDoc = GTUtilsProjectTreeView::findItem(os, "pkF70_variations");
    QTreeWidgetItem *seqDoc1 = GTUtilsProjectTreeView::findItem(os, "pkf140_variations");

    CHECK_SET_ERR(NULL != seqDoc, "Document not found!");
    CHECK_SET_ERR(NULL != seqDoc1, "Document not found!");

    }
GUI_TEST_CLASS_DEFINITION( test_2267_1 ){
//     1. Open human_T1.fa
// 
    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");

//     2. Select random subsequence, press Ctrl+N, press Enter
// 
    GTUtilsDialog::waitForDialog(os, new CreateAnnotationWidgetFiller(os, true, "DDD", "D", "10..16"));
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << "ADV_MENU_ADD" << "create_annotation_action"));
    GTMenu::showMainMenu(os, MWMENU_ACTIONS);
    GTGlobals::sleep();
//     3. Press Insert, press '1' key until there is no new symbols in lineedit
// 
    Runnable *filler = new EditQualifierFiller(os, "111111111111111111111111111111111111111111111111111111111111111111111111111111111", "val", GTGlobals::UseMouse, true,true);
    GTUtilsDialog::waitForDialog(os, filler);

    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << ADV_MENU_ADD << "add_qualifier_action"));
    GTMouseDriver::moveTo(os, GTUtilsAnnotationsTreeView::getItemCenter(os, "D"));
    GTMouseDriver::click(os, Qt::RightButton);
//     4. Press Enter
//     Expected state: Edit qualifier window closes
}

GUI_TEST_CLASS_DEFINITION( test_2267_2 ){
//     1. Open human_T1.fa
// 
    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");
//     2. Select random subsequence, press Ctrl+N, press Enter
// 
    GTUtilsDialog::waitForDialog(os, new CreateAnnotationWidgetFiller(os, true, "DDD", "D", "10..16"));
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << "ADV_MENU_ADD" << "create_annotation_action"));
    GTMenu::showMainMenu(os, MWMENU_ACTIONS);
    GTGlobals::sleep();
//     3. Press Insert, set the name of qualifier: '))()((_'
// 
//     4. Press Enter
//     Expected state: Error message appears once
    Runnable *filler = new EditQualifierFiller(os, "))()((_", "val", GTGlobals::UseMouse, true, true);
    GTUtilsDialog::waitForDialog(os, filler);

    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << ADV_MENU_ADD << "add_qualifier_action"));
    GTMouseDriver::moveTo(os, GTUtilsAnnotationsTreeView::getItemCenter(os, "D"));
    GTMouseDriver::click(os, Qt::RightButton);
}

GUI_TEST_CLASS_DEFINITION( test_2268 ) {
//    1. Forbid write access to the t-coffee directory (chmod 555 %t-coffee-dir%).
    // Permissions will be returned to the original state, if UGENE won't crash.
    GTGlobals::sleep();
    ExternalToolRegistry* etRegistry = AppContext::getExternalToolRegistry();
    CHECK_SET_ERR(etRegistry, "External tool registry is NULL");
    ExternalTool* tCoffee = etRegistry->getByName("T-Coffee");
    CHECK_SET_ERR(tCoffee, "T-coffee tool is NULL");
    QFileInfo toolPath(tCoffee->getPath());
    CHECK_SET_ERR(toolPath.exists(), "T-coffee tool is not set");

    QDir toolDir = toolPath.dir();
    toolDir.cdUp();

    PermissionsSetter permSetter;
    QFile::Permissions p = QFile::WriteOwner |
                           QFile::WriteUser |
                           QFile::WriteGroup |
                           QFile::WriteOther;
    bool res = permSetter.setPermissions(toolDir.path(), ~p);
    CHECK_SET_ERR(res, "Can't set permissions");

//    2. Open "sample/CLUSTALW/COI.aln".
    GTFileDialog::openFile(os, dataDir + "/samples/CLUSTALW/", "COI.aln");

//    3. Right click on the MSA -> Align -> Align with T-Coffee.
//    4. Click the "Align" button.
    GTLogTracer lt;
    GTUtilsDialog::waitForDialog(os, new TCoffeeDailogFiller(os));
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << MSAE_MENU_ALIGN << "Align with T-Coffee", GTGlobals::UseMouse));
    GTMenu::showContextMenu(os, GTUtilsMdi::activeWindow(os));

//    Expected: the t-coffee task started and finished well.
    TaskScheduler* scheduler = AppContext::getTaskScheduler();
    CHECK_SET_ERR(scheduler, "Task scheduler is NULL");
    GTGlobals::sleep(5000);
    while(!scheduler->getTopLevelTasks().isEmpty()){
       GTGlobals::sleep();
    }

    GTUtilsLog::check(os, lt);
}

GUI_TEST_CLASS_DEFINITION( test_2316 ) {
    GTUtilsDialog::waitForDialog(os, new StartupDialogFiller(os));
    GTFileDialog::openFile(os, dataDir+"samples/../workflow_samples/Alignment", "basic_align.uwl");

    GTGlobals::sleep(5000);

    CHECK_SET_ERR(!GTUtilsProjectTreeView::isVisible(os), "Project tree view is visible");

//     1. Start UGENE
// 
//     2. Drag an .uwl file inside UGENE to open a workflow
//     Expected state: now the project view is collapsed if a file.uwl is drag'n'dropped to UGENE when there is no project yet.
}
GUI_TEST_CLASS_DEFINITION( test_2269 ){
    //1. Use main menu: {tools->Align short reeds}
    //2. Select Bowtie2 as alignment method
    //3. Try to set incorrect value in "Seed lingth" spinbox(Correct boundaries are: >3, <32)
    AlignShortReadsFiller::Bowtie2Parameters parameters(testDir + "_common_data/scenarios/_regression/1093/",
                                                        "refrence.fa",
                                                        testDir + "_common_data/scenarios/_regression/1093/",
                                                        "read.fa");
    parameters.seedLengthCheckBox = true;
    parameters.seedLength = 33;

    GTUtilsDialog::waitForDialog(os, new AlignShortReadsFiller(os, &parameters));
    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_TOOLS), QStringList() << "Align to reference" << "Align short reads");

    CHECK_SET_ERR( !os.hasError(), "Uncorrect value is available");
}

GUI_TEST_CLASS_DEFINITION( test_2270 ){
    //1. Open file "data/cmdline/snp.uwl"
    //Ecpected state: scheme opened in WD without problems

    GTLogTracer lt;
    GTFileDialog::openFile(os, dataDir+"cmdline/", "snp.uwl");
    GTGlobals::sleep(500);
    GTUtilsLog::check(os, lt);
}

GUI_TEST_CLASS_DEFINITION( test_2281 ){
    //GTUtilsDialog::waitForDialog(os, new StartupDialogFiller(os));
    // 1. Open WD sample "Align Sequences with MUSCLE
    QMenu* menu=GTMenu::showMainMenu(os, MWMENU_TOOLS);
    GTMenu::clickMenuItemByName(os, menu, QStringList() << "Workflow Designer");
   // GTUtilsWorkflowDesigner::addSample(os, "Align sequences with MUSCLE");
    GTGlobals::sleep(500);
    QGraphicsView* sceneView = qobject_cast<QGraphicsView*>(GTWidget::findWidget(os,"sceneView"));
//    GT_CHECK_RESULT(sceneView, "sceneView not found", NULL);
    QList<QGraphicsItem *> items = sceneView->items();
    QString s;
    foreach(QGraphicsItem* it, items) {
        QGraphicsObject *itObj = it->toGraphicsObject();
        QGraphicsTextItem* textItemO = qobject_cast<QGraphicsTextItem*>(itObj);
        if (textItemO) {
            QString text = textItemO->toPlainText();
            s.append(text + "  ");
        }
    }
    /*QList<QWidget*> list = AppContext::getMainWindow()->getQMainWindow()->findChildren<QWidget*>();

    foreach(QWidget* w, list){
        s.append(w->metaObject()->className()).append("  " + w->objectName()).append('\n');


        }*/
    CHECK_SET_ERR(false, s)
    GTGlobals::sleep(1000);
}

GUI_TEST_CLASS_DEFINITION( test_2292 ){
    GTLogTracer l;

    QString destName = testDir + "_common_data/ugenedb/example-alignment.ugenedb";
    GTUtilsDialog::waitForDialog(os, new ConvertAceToSqliteDialogFiller(os, destName));
    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller(os, QMessageBox::Ok, "There is opened view with destination file"));
    GTFileDialog::openFile(os, testDir+"_common_data/ugenedb/", "example-alignment.ugenedb");
    GTFileDialog::openFile(os, dataDir+"samples/ACE", "K26.ace");


    GTGlobals::sleep();
    //CHECK_SET_ERR( l.hasError( ), "Error message expected!" );
}
GUI_TEST_CLASS_DEFINITION( test_2282 ) {
    // 1. Open "chrM.sorted.bam" file using {File->Open} menu in UGENE.
    //    Expected state: "Import BAM file" dialog has appeared.
    // 2. Press "Enter".
    //    Expected state:
    //      1) The dialog has been closed.
    //      2) A new project has been created.
    QString assFileName = testDir + "_common_data/scenarios/sandbox/test_2282.chrM.sorted.ugenedb";
    QString assDocName = "test_2282.chrM.sorted.ugenedb";
    GTUtilsDialog::waitForDialog(os, new ImportBAMFileFiller(os, assFileName));
    GTFileDialog::openFile(os, testDir + "_common_data/bam", "chrM.sorted.bam");
    GTGlobals::sleep(5000);

    //      3) The Project View with document "chrM.sorted.bam.ugenedb" has been opened.
    QTreeWidgetItem *assDoc = GTUtilsProjectTreeView::findItem(os, assDocName);
    CHECK(NULL != assDoc, );

    // 3. Delete "chrM.sorted.bam.ugenedb" from the file system (i.e. not from UGENE).
    bool deleteResult = QFile::remove(assFileName);
    CHECK(true == deleteResult, );
    GTGlobals::sleep();

    // Expected state: the project has been removed.
    GTUtilsProject::checkProject(os, GTUtilsProject::NotExists);
}

GUI_TEST_CLASS_DEFINITION( test_2284 ){
    //1. Open COI.aln
    GTFileDialog::openFile(os, dataDir + "samples/CLUSTALW/", "COI.aln");

    //2. Press the "Switch on/off collapsing" button
    GTWidget::click(os, GTToolbar::getWidgetForActionName(os, GTToolbar::getToolbar(os, "mwtoolbar_activemdi"), "Enable collapsing"));

    //expected state: Mecopoda_elongata__Ishigaki__J and Mecopoda_elongata__Sumatra_ are collapsed
    CHECK_SET_ERR( !GTUtilsMSAEditorSequenceArea::isSequenceVisible(os, QString("Mecopoda_elongata__Sumatra_")),
        "Required sequence is not collapsed");

    //3. Select the first base in last row
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint(0, 16 ), QPoint(0, 16 ) );

    //4. In status bar search field type "AATT"
    GTKeyboardDriver::keyClick( os, 'f', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep(200);
    GTKeyboardDriver::keySequence(os, "AATT");

    //5. Press the "Find forward" button a few times until selection reaches the end of the alignment
    //6. Press the button again
    for(int i = 0; i < 11; i++) {
        GTGlobals::sleep(200);
        GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["enter"]);
    }
}


GUI_TEST_CLASS_DEFINITION( test_2306 ) {
    // 1. Open file "data/samples/CLUSTALW/COI.aln"
    GTFileDialog::openFile(os, dataDir+"samples/CLUSTALW/", "COI.aln");

    // 2. Turn on collapsing mode in MSA
    GTWidget::click(os, GTToolbar::getWidgetForActionName(os, GTToolbar::getToolbar(os, "mwtoolbar_activemdi"), "Enable collapsing"));

    //expected state: Mecopoda_elongata__Ishigaki__J and Mecopoda_elongata__Sumatra_ are collapsed
    CHECK_SET_ERR( !GTUtilsMSAEditorSequenceArea::isSequenceVisible(os, QString("Mecopoda_elongata__Sumatra_")),
        "Required sequence is not collapsed");

    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 5, 10 ), QPoint( 15, 15 ) );

    // 3. Shift the region but don't release left mouse button
    const QPoint mouseDragPosition( 12, 12 );
    GTUtilsMSAEditorSequenceArea::moveTo( os, mouseDragPosition );
    GTMouseDriver::press( os );
    GTUtilsMSAEditorSequenceArea::moveTo( os, mouseDragPosition + QPoint( 3, 0 ) );
    GTGlobals::sleep( 200 );
    GTMouseDriver::release( os );
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["esc"]);
    GTGlobals::sleep( 200 );
    
    // 4. Call context menu
        GTMouseDriver::press( os );
    GTUtilsMSAEditorSequenceArea::selectArea( os, QPoint( 5, 10 ), QPoint( 7, 15 ) );
    GTKeyboardDriver::keyClick( os, 'c', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep( 200 );
    const QString finalMsaContent = GTClipboard::text( os );
    CHECK_SET_ERR("---\n---\n---\n---\n---\n---\n---" == finalMsaContent, "Unexpected MSA content has occurred" );


    }
GUI_TEST_CLASS_DEFINITION( test_2309 ) {
    // 1. Open file "data/samples/CLUSTALW/COI.aln"
    GTFileDialog::openFile(os, dataDir+"samples/CLUSTALW/", "COI.aln");

    // 2. Build tree for the alignment
    GTUtilsDialog::waitForDialog(os, new BuildTreeDialogFiller(os, testDir + "_common_data/scenarios/sandbox/2309.nwk", 0, 0, true));
    GTUtilsDialog::waitForDialogWhichMayRunOrNot(os,new LicenseAgreemntDialogFiller(os));
    QAbstractButton *tree= GTAction::button(os,"Build Tree");
    GTWidget::click(os, tree);
    GTGlobals::sleep(500);

    QStringList initialNames = GTUtilsMSAEditorSequenceArea::getNameList(os);
    QAbstractButton *refresh= GTAction::button(os,"Refresh tree");

    CHECK(NULL != refresh, );
    if(refresh->isVisible()){
        GTWidget::click(os, refresh);
    }else{
        GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList()<<"Refresh tree"));
        QToolBar* toolBar = qobject_cast<QToolBar*>(refresh->parent());
        GTMouseDriver::moveTo(os, toolBar->mapToGlobal(toolBar->geometry().bottomRight())-QPoint(5,15));
        GTMouseDriver::click(os);
    }

    GTGlobals::sleep(500);

    QStringList newNames = GTUtilsMSAEditorSequenceArea::getNameList(os);
    CHECK_SET_ERR(newNames == initialNames, "Wrong sequences order");
}
GUI_TEST_CLASS_DEFINITION( test_2318 ) {
    class FirstItemPopupChooser : public PopupChooser {
    public:
        FirstItemPopupChooser(U2OpStatus& os) :
            PopupChooser(os, QStringList()) {}

        virtual void run() {
            GTGlobals::sleep(1000);
            GTMouseDriver::release(os);
            GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["down"]);
            GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["enter"]);
        }
    };

    class PlusClicker : public FindAnnotationCollocationsDialogFiller {
    public:
        PlusClicker(U2OpStatus& os, const QString& _annName) :
            FindAnnotationCollocationsDialogFiller(os),
            annName(_annName) {}

        virtual void run() {
            QToolButton* plusButton = getPlusButton();
            CHECK_SET_ERR(plusButton, "First plus toolbutton is NULL");
            firstSize = plusButton->size();

            GTUtilsDialog::waitForDialog(os, new FirstItemPopupChooser(os));
            GTWidget::click(os, plusButton);

            GTGlobals::sleep();
            plusButton = getPlusButton();
            CHECK_SET_ERR(plusButton, "Second plus toolbutton is NULL");
            secondSize = plusButton->size();

            bool result = test();
            CHECK_SET_ERR(result, "Incorrect result");

            FindAnnotationCollocationsDialogFiller::run();
        }

    private:
        bool test() const {
            return firstSize == secondSize;
        }

        QString annName;
        QSize firstSize;
        QSize secondSize;
    };

//    1) Open {data/samples/murine.gb}
    GTFileDialog::openFile(os, dataDir + "/samples/Genbank/", "murine.gb");

//    2) Use context menu: {Analyze -> Find annotated regions}
//    3) Click plus button, select any annotation
//    Expected state: plus button changes place, its size wasn't change
    QToolBar* toolbar = GTToolbar::getToolbar(os, "mwtoolbar_activemdi");
    CHECK_SET_ERR(toolbar, "Toolbar is NULL");
    QWidget* farButton = GTToolbar::getWidgetForActionTooltip(os, toolbar, "Find annotated regions...");
    CHECK_SET_ERR(farButton, "Find annotated region button is NULL");

    // Expected state is checked in PlusClicker
    GTUtilsDialog::waitForDialog(os, new PlusClicker(os, "CDS"));
    GTWidget::click(os, farButton);
}

GUI_TEST_CLASS_DEFINITION( test_2351 ) {
//    1. Run UGENE
//    2. Press the "New Project" button on the main toolbar
//    Expected state: the "Create new project" dialog has appeared
//    3. Press the "Create" button or press the "Enter" key
//    4. Repeat steps 2 and 3 as fast as you can more than 10 times
//    Expected state: UGENE does not crash
    class RapidProjectCreator : public Filler {
    public:
        RapidProjectCreator(U2OpStatus& os, const QString& _projectName, const QString& _projectFolder, const QString& _projectFile) :
            Filler(os, "CreateNewProjectDialog"),
            projectName(_projectName),
            projectFolder(_projectFolder),
            projectFile(_projectFile) {}

        virtual void run() {
            QWidget* dialog = QApplication::activeModalWidget();
            if (NULL == dialog) {
                os.setError("Modal dialog not found");
                return;
            }

            QLineEdit *projectNameEdit = qobject_cast<QLineEdit*>(GTWidget::findWidget(os, "projectNameEdit", dialog));
            if (NULL == projectNameEdit) {
                os.setError("projectNameEdit not found");
                return;
            }
            projectNameEdit->setText(projectName);

            QLineEdit *projectFolderEdit = qobject_cast<QLineEdit*>(GTWidget::findWidget(os, "projectFolderEdit", dialog));
            if (NULL == projectFolderEdit) {
                os.setError("projectFolderEdit not found");
                return;
            }
            projectFolderEdit->setText(projectFolder);

            QLineEdit *projectFileEdit = qobject_cast<QLineEdit*>(GTWidget::findWidget(os, "projectFileEdit", dialog));
            if (NULL == projectFileEdit) {
                os.setError("projectFileEdit not found");
                return;
            }
            projectFileEdit->setText(projectFile);

            QAbstractButton *createButton = qobject_cast<QAbstractButton*>(GTWidget::findWidget(os, "createButton", dialog));
            if (NULL == createButton) {
                os.setError("createButton not found");
                return;
            }
            createButton->click();
        }

    private:
        const QString projectName;
        const QString projectFolder;
        const QString projectFile;
    };

    const QString projectName = "test_2351";
    const QString projectFolder = testDir + "_common_data/scenarios/sandbox";
    const QString projectFile = "test_2351";

    for (int i = 0; i < 15; ++i) {
        GTUtilsDialog::waitForDialog(os, new RapidProjectCreator(os, projectName, projectFolder, projectFile));
        GTWidget::click(os,
                        GTToolbar::getWidgetForActionName(os,
                                                          GTToolbar::getToolbar(os, MWTOOLBAR_MAIN),
                                                          ACTION_PROJECTSUPPORT__NEW_PROJECT));
    }
}

GUI_TEST_CLASS_DEFINITION( test_2352 ) {
    //1. Close current project or open empty UGENE
    //2. Open menu {Tools->Build dotplot...}
    //3. Select any binary file as first file in dialog
    //Expected state: file is not selected, no crash

    QString bin = QCoreApplication::applicationFilePath();
    GTUtilsDialog::waitForDialog(os, new BuildDotPlotFiller(os, bin, bin, false, false, false, 5, 5,
                                                            true));
    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller(os, QMessageBox::Ok));

    QMenu *menu=GTMenu::showMainMenu( os, MWMENU_TOOLS );
    GTMenu::clickMenuItemByName( os, menu, QStringList( ) << "Build dotplot" );

    GTGlobals::sleep();
}

GUI_TEST_CLASS_DEFINITION( test_2360 ) {
    // 1. Open "data/samples/COI.aln".
    GTFileDialog::openFile(os, dataDir+"samples/CLUSTALW/", "COI.aln");

    // 2. Right click on document in project.
    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "COI.aln"));

    // 3. Choose the context menu {Export/Import->Export nucleic alignment to amino translation}.
    // Expected state: Export dialog appears.
    // 4. Set "File format to use" to PHYLIP Sequantial.
    // 5. Click "Export".
    GTUtilsDialog::waitForDialog(os, new ExportMSA2MSADialogFiller(os, 6));
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << ACTION_PROJECT__EXPORT_IMPORT_MENU_ACTION << ACTION_PROJECT__EXPORT_TO_AMINO_ACTION));
    GTMouseDriver::click(os, Qt::RightButton);
}

GUI_TEST_CLASS_DEFINITION( test_2364 ) {
    //1. Open WD.'
    GTUtilsWorkflowDesigner::openWorkflowDesigner(os);

    //2. Create a workflow: Read sequence -> Write sequence.
    GTUtilsWorkflowDesigner::addAlgorithm(os, "Read sequence");
    GTUtilsWorkflowDesigner::addAlgorithm(os, "Write sequence");

    GTUtilsWorkflowDesigner::connect(os, GTUtilsWorkflowDesigner::getWorker(os, "Read sequence"),
                                         GTUtilsWorkflowDesigner::getWorker(os, "Write sequence"));

    //3. Set the input sequence file: "data/samples/FASTA/human_T1.fa".
    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, "Read sequence"));
    GTMouseDriver::click(os);
    GTGlobals::sleep(300);
    GTUtilsWorkflowDesigner::setDatasetInputFile(os, dataDir + "samples/FASTA", "human_T1.fa");

    //4. Set the output file: "out.fa".
    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, "Write sequence"));
    GTMouseDriver::click(os);
    GTGlobals::sleep(300);
    GTUtilsWorkflowDesigner::setParameter(os, "Output file", "out.fa", GTUtilsWorkflowDesigner::textValue);

    //5. Validate the workflow.
    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller(os, QMessageBox::Ok));
    GTWidget::click(os, GTAction::button(os,"Validate workflow"));
    GTGlobals::sleep(1000);

    //Expected: the workflow has the warning about FASTA format and annotations.
    CHECK_SET_ERR(GTUtilsWorkflowDesigner::checkErrorList(os, "") != 0, "There is no any messages in infoLog");

    //6. Run the workflow.
    GTWidget::click(os,GTAction::button(os,"Run workflow"));
    GTGlobals::sleep(500);

    //Expected: the button "Load schema" is shown.
    // expected button is in dashboard - it can't be checked for now
}

GUI_TEST_CLASS_DEFINITION( test_2375 ) {
//    1. Open {_common_data/sam/broken_invalid_cigar.sam}
//    Expected state: import dialog appears.

//    2. Fill the import dialog with valid data. Begin the importing.
//    Expected state: importing fails, UGENE doesn't crash.
    QString destUrl = testDir + "_common_data/scenarios/sandbox/test_2375.ugenedb";
    GTLogTracer logtracer;
    GTUtilsDialog::waitForDialog(os, new ImportBAMFileFiller(os, destUrl));
    GTFileDialog::openFile(os, testDir + "_common_data/sam/", "broken_invalid_cigar.sam");

    TaskScheduler* scheduler = AppContext::getTaskScheduler();
    CHECK_SET_ERR(scheduler, "Task scheduler is NULL");
    GTGlobals::sleep(5000);
    while (!scheduler->getTopLevelTasks().isEmpty()) {
        GTGlobals::sleep();
    }

    CHECK_SET_ERR(logtracer.hasError(), "There wasn't errors in the log");
}

GUI_TEST_CLASS_DEFINITION( test_2377 ) {
    GTLogTracer l;

    GTUtilsDialog::waitForDialog( os, new StartupDialogFiller( os ) );

    QMenu *menu=GTMenu::showMainMenu( os, MWMENU_TOOLS );
    GTMenu::clickMenuItemByName( os, menu, QStringList( ) << "Workflow Designer" );

    const QString assemblyReaderName = "Read Assembly";
    const QString assemblyWriterName = "Write Assembly";

    GTUtilsWorkflowDesigner::addAlgorithm( os, assemblyReaderName );
    GTUtilsWorkflowDesigner::addAlgorithm( os, assemblyWriterName );

    WorkflowProcessItem *assemblyReader = GTUtilsWorkflowDesigner::getWorker( os,
        assemblyReaderName );
    WorkflowProcessItem *assemblyWriter = GTUtilsWorkflowDesigner::getWorker( os,
        assemblyWriterName );

    GTUtilsWorkflowDesigner::connect( os, assemblyReader, assemblyWriter );

    GTMouseDriver::moveTo( os, GTUtilsWorkflowDesigner::getItemCenter( os, assemblyReaderName ) );
    GTMouseDriver::click( os );
    GTUtilsWorkflowDesigner::setDatasetInputFile( os, testDir + "_common_data/sam",
        "broken_without_reads.sam" );

    GTMouseDriver::moveTo( os, GTUtilsWorkflowDesigner::getItemCenter( os, assemblyWriterName ) );
    GTMouseDriver::click( os );
    GTUtilsWorkflowDesigner::setParameter( os, "Output file", "test",
        GTUtilsWorkflowDesigner::textValue );

    GTWidget::click( os, GTAction::button( os, "Run workflow" ) );

    GTGlobals::sleep(5000);
    CHECK_SET_ERR( l.hasError( ), "Error message expected!" );
}

GUI_TEST_CLASS_DEFINITION( test_2378 ) {
    GTLogTracer l;
    GTUtilsDialog::waitForDialog(os, new ImportBAMFileFiller(os, testDir + "_common_data/scenarios/sandbox/test_2378.ugenedb"));
    GTFileDialog::openFile(os, testDir + "_common_data/sam/", "scerevisiae.sam");
    CHECK_SET_ERR(!l.hasError( ), "Error message");
}

GUI_TEST_CLASS_DEFINITION( test_2378_1 ) {
    GTLogTracer l;
    // 1. Open WD
    GTUtilsWorkflowDesigner::openWorkflowDesigner(os);

    // 2. Create scheme: read assembly->write assembly
    // 3. set _common_data\sam\scerevisiae.sam as input file
    GTUtilsWorkflowDesigner::addAlgorithm(os, "Read Assembly");
    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, "Read Assembly"));
    GTMouseDriver::click(os);
    GTUtilsWorkflowDesigner::setDatasetInputFile(os, testDir + "_common_data/sam", "scerevisiae.sam");

    GTUtilsWorkflowDesigner::addAlgorithm(os, "Write Assembly");
    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, "Write Assembly"));
    GTMouseDriver::click(os);
    //QString //absPath =
    GTUtilsWorkflowDesigner::setParameter(os, "Output file", QDir(testDir).absolutePath() + "_common_data/scenarios/sandbox/test_2378_1.bam", GTUtilsWorkflowDesigner::textValue);

    GTUtilsWorkflowDesigner::connect(os, GTUtilsWorkflowDesigner::getWorker(os, "Read Assembly"), GTUtilsWorkflowDesigner::getWorker(os, "Write Assembly"));

    // 4. Run scheme
    GTWidget::click( os, GTAction::button( os, "Run workflow" ) );
    GTGlobals::sleep(5000);
    CHECK_SET_ERR(!l.hasError( ), "Error message");
}

GUI_TEST_CLASS_DEFINITION( test_2379 ) {
    class CreateProjectFiller : public Filler {
        // It is a local support class, it is the same as SaveProjectAsDialogFiller,
        // but it clicks the final button with keyboard.
        // I know that it is bad practice to create so useless classes, but I don't need to extend the original class.
        // Do not move it to another place: if you need the same filler than extand the original class.
    public:
        CreateProjectFiller(U2OpStatus &_os,
                            const QString &_projectName,
                            const QString &_projectFolder,
                            const QString &_projectFile) :
            Filler(_os, "CreateNewProjectDialog"),
            projectName(_projectName),
            projectFolder(_projectFolder),
            projectFile(_projectFile) {}

        virtual void run() {
            GTGlobals::sleep();
            QWidget* dialog = QApplication::activeModalWidget();
            if (NULL == dialog) {
                os.setError("activeModalWidget is NULL");
                return;
            }

            QLineEdit *projectNameEdit = qobject_cast<QLineEdit*>(GTWidget::findWidget(os, "projectNameEdit", dialog));
            GTLineEdit::setText(os, projectNameEdit, projectName);

            QLineEdit *projectFolderEdit = qobject_cast<QLineEdit*>(GTWidget::findWidget(os, "projectFolderEdit", dialog));
            GTLineEdit::setText(os, projectFolderEdit, projectFolder);

            QLineEdit *projectFileEdit = qobject_cast<QLineEdit*>(GTWidget::findWidget(os, "projectFileEdit", dialog));
            GTLineEdit::setText(os, projectFileEdit, projectFile);

            GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["enter"]);

            GTGlobals::sleep();
        }

    private:
        const QString projectName;
        const QString projectFolder;
        const QString projectFile;
    };

//    0. Create a project that will be "existing" in the second step
    const QString projectName = "test_2379";
    const QString projectFolder = testDir + "_common_data/scenarios/sandbox";
    const QString projectFile = "test_2379";

    GTUtilsDialog::waitForDialog(os, new CreateProjectFiller(os, projectName, projectFolder, projectFile));
    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_FILE), QStringList()<<ACTION_PROJECTSUPPORT__NEW_PROJECT);
    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_FILE), QStringList()<<ACTION_PROJECTSUPPORT__SAVE_PROJECT);
    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_FILE), QStringList()<<ACTION_PROJECTSUPPORT__CLOSE_PROJECT);

//    1. Press "Create new project" button
//    2. Specify the path to an existing project
//    3. Press "Create" button by using keyboard
//    Expected state: only one dialog with warning message appeared
    GTUtilsDialog::waitForDialog(os, new CreateProjectFiller(os, projectName, projectFolder, projectFile));
    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller(os, QMessageBox::Yes, "Project file already exists"));

    GTMenu::clickMenuItemByName(os, GTMenu::showMainMenu(os, MWMENU_FILE), QStringList()<<ACTION_PROJECTSUPPORT__NEW_PROJECT);
}

GUI_TEST_CLASS_DEFINITION( test_2382 ) {
    // 1. Open "_common_data/ace/capres4.ace".
    // 2. "Import ACE file" dialog appeared. Press ok.
    // Expected state: export succeeded, assembly is displayed.
    GTLogTracer l;

    QString sandboxDir = testDir + "_common_data/scenarios/sandbox/";
    QString assDocName = "test_2382.ugenedb";
    GTUtilsDialog::waitForDialog(os, new ConvertAceToSqliteDialogFiller(os, sandboxDir + assDocName));
    GTFileDialog::openFile(os, testDir + "_common_data/ace/", "capres4.ace");

    GTGlobals::sleep();
    CHECK_SET_ERR( !l.hasError( ), "There must be no errors!" );

    QTreeWidgetItem *assDoc = GTUtilsProjectTreeView::findItem(os, assDocName);
    CHECK_SET_ERR(NULL != assDoc, "Result document was not found!");
}

GUI_TEST_CLASS_DEFINITION( test_2382_1 ) {
    // 1. Open "_common_data/ace/test_new_.cap.ace".
    // 2. "Import ACE file" dialog appeared. Press ok.
    // Expected state: export succeeded, assembly is displayed.
    GTLogTracer l;

    QString sandboxDir = testDir + "_common_data/scenarios/sandbox/";
    QString assDocName = "test_2382_1.ugenedb";
    GTUtilsDialog::waitForDialog(os, new ConvertAceToSqliteDialogFiller(os, sandboxDir + assDocName));
    GTFileDialog::openFile(os, testDir + "_common_data/ace/", "test_new_.cap.ace");

    GTGlobals::sleep();
    CHECK_SET_ERR( !l.hasError( ), "There must be no errors!" );

    QTreeWidgetItem *assDoc = GTUtilsProjectTreeView::findItem(os, assDocName);
    CHECK_SET_ERR(NULL != assDoc, "Result document was not found!");
}

GUI_TEST_CLASS_DEFINITION( test_2392 ) {
    // 1. Open file _common_data/genbank/multi.gb
    // Expected state: Open dialog "Sequence reading options"
    // 2. Select "Join sequences into alignment" option and press OK
    GTUtilsDialog::waitForDialog(os, new SequenceReadingModeSelectorDialogFiller(os, SequenceReadingModeSelectorDialogFiller::Join));
    GTFileDialog::openFile(os, testDir+"_common_data/genbank/", "multi.gb");

    // Expected state: Document opened in MSA view
    QStringList names = GTUtilsMSAEditorSequenceArea::getNameList(os);
    CHECK_SET_ERR(2 == names.size(), "Wrong sequence count");
}

GUI_TEST_CLASS_DEFINITION( test_2401 ) {
    // 1. Open the file "_common_data/ace/ace_test_1.ace".
    // 2. Set the ugenedb path for import: "_common_data/scenarios/sandbox/2401.ugenedb".
    // 3. Click OK
    QString sandbox = testDir + "_common_data/scenarios/sandbox/";
    QString fileName = "2401.ugenedb";
    QString ugenedb = sandbox + fileName;
    GTUtilsDialog::waitForDialog(os, new ConvertAceToSqliteDialogFiller(os, ugenedb));
    GTFileDialog::openFile(os, testDir + "_common_data/ace/", "ace_test_1.ace");

    // Expected: the file is imported without errors, the assembly is opened.
    // 4. Close the project.
    GTUtilsDialog::waitForDialog(os, new SaveProjectDialogFiller(os, QDialogButtonBox::No));
#ifdef Q_OS_MAC
        QMenu *menu = GTMenu::showMainMenu(os, MWMENU_FILE);
        GTMenu::clickMenuItem(os, menu, ACTION_PROJECTSUPPORT__CLOSE_PROJECT);
#else
    GTKeyboardDriver::keyClick(os, 'q', GTKeyboardDriver::key["ctrl"]);
#endif
    GTGlobals::sleep();

    // 5. Open the file "_common_data/ace/ace_test_11_(error).ace".
    // 6. Set the same ugenedb path for import: "_common_data/scenarios/sandbox/2401.ugenedb".
    // 7. Click OK.
    // 8. Click Append.
    GTUtilsDialog::waitForDialog(os, new ConvertAceToSqliteDialogFiller(os, ugenedb, ConvertAceToSqliteDialogFiller::APPEND));
    GTFileDialog::openFile(os, testDir + "_common_data/ace/", "ace_test_11_(error).ace");

    // Expected: the file is not imported but "2401.ugenedb" still exists.
    QString s = GUrl(ugenedb).getURLString();
    CHECK_SET_ERR(QFile::exists(s), "ugenedb file does not exist");

    // 9. Open the file "2401.ugenedb".
    GTFileDialog::openFile(os, sandbox, fileName);
    // Expected: assembly is opened without errors.
    QTreeWidgetItem *item = GTUtilsProjectTreeView::findItem(os, fileName);
    CHECK_SET_ERR(NULL != item, "No assembly");
}

GUI_TEST_CLASS_DEFINITION( test_2406 ) {
//    1. Create the {Read Sequence -> Write Sequence} workflow.
    GTUtilsWorkflowDesigner::openWorkflowDesigner(os);

    const QString sequenceReaderName = "Read Sequence";
    const QString sequenceWriterName = "Write Sequence";

    GTUtilsWorkflowDesigner::addAlgorithm(os, sequenceReaderName);
    GTUtilsWorkflowDesigner::addAlgorithm(os, sequenceWriterName);

    WorkflowProcessItem *sequenceReader = GTUtilsWorkflowDesigner::getWorker(os, sequenceReaderName);
    WorkflowProcessItem *sequenceWriter = GTUtilsWorkflowDesigner::getWorker(os, sequenceWriterName);

    CHECK_SET_ERR(NULL != sequenceReader, "Sequence reader element is NULL");
    CHECK_SET_ERR(NULL != sequenceWriter, "Sequence writer element is NULL");

    GTUtilsWorkflowDesigner::connect(os, sequenceReader, sequenceWriter);

//    2. Click on the output file field and input "TEST" file name and press Enter.
//    Expected: TEST file name appears in the output file name field
    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, sequenceWriterName));
    GTMouseDriver::click(os);
    GTUtilsWorkflowDesigner::setParameter(os, "Output file", "TEST", GTUtilsWorkflowDesigner::textValue);
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["enter"]);
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["enter"]);

    GTGlobals::sleep(500);
    const QString expectedPreValue = "TEST";
    const QString resultPreValue = GTUtilsWorkflowDesigner::getParameter(os, "Output file");
    CHECK_SET_ERR(expectedPreValue == resultPreValue,
                  QString("Unexpected value: expected '%1' get '%2'").
                  arg(expectedPreValue).arg(resultPreValue));

//    3. Change the file format to the genbank
//    Expected: TEST.gb file name appears in the output file name field
    GTUtilsWorkflowDesigner::setParameter(os, "Document format", 2 /*"genbank"*/, GTUtilsWorkflowDesigner::comboValue);

    const QString expectedPostValue = "TEST.gb";
    const QString resultPostValue = GTUtilsWorkflowDesigner::getParameter(os, "Output file");
    CHECK_SET_ERR(expectedPostValue == resultPostValue,
                  QString("Unexpected value: expected '%1' get '%2'").
                  arg(expectedPostValue).arg(resultPostValue));
}

GUI_TEST_CLASS_DEFINITION(test_2407) {
    GTLogTracer l;
    GTFileDialog::openFile(os, testDir+"_common_data/clustal/", "10000_sequences.aln");

    GTGlobals::sleep(20000);

    QTreeWidgetItem *item = GTUtilsProjectTreeView::findItem(os, "10000_sequences.aln");
    GTMouseDriver::moveTo(os, GTTreeWidget::getItemCenter(os, item));
        
    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller(os, QMessageBox::Yes));
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList()<<ACTION_PROJECT__UNLOAD_SELECTED));
    GTMouseDriver::click(os, Qt::RightButton);
    GTGlobals::sleep(5000);

    GTMouseDriver::moveTo(os, GTTreeWidget::getItemCenter(os, item));
    GTMouseDriver::doubleClick(os);

    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["delete"]);

    CHECK_SET_ERR( !l.hasError( ), "File not removed from project!" );

    }

GUI_TEST_CLASS_DEFINITION( test_2410 ) {
    GTFileDialog::openFile( os, dataDir + "samples/FASTA/", "human_T1.fa" );

    GTGlobals::sleep( );

    GTUtilsDialog::waitForDialog(os, new selectSequenceRegionDialogFiller( os, 166740, 166755 ) );

    QWidget *sequenceWidget = GTWidget::findWidget( os, "ADV_single_sequence_widget_0" );
    CHECK_SET_ERR( NULL != sequenceWidget, "sequenceWidget is not present" );

    GTWidget::click( os, sequenceWidget );
    GTKeyboardUtils::selectAll( os );

    QWidget *graphAction = GTWidget::findWidget( os, "GraphMenuAction", sequenceWidget, false );
    Runnable *chooser = new PopupChooser( os, QStringList( ) << "GC Content (%)" );
    GTUtilsDialog::waitForDialog( os, chooser );

    GTWidget::click( os, graphAction );

    GTWidget::click( os, GTAction::button( os,
        "action_zoom_in_human_T1 (UCSC April 2002 chr7:115977709-117855134)" ) );

    QWidget *renderArea = GTWidget::findWidget( os, "GSequenceGraphViewRenderArea", sequenceWidget );
    const QPoint mouseInitialPos( 4 * renderArea->width( ) / 7, renderArea->height( ) / 2 );
    GTWidget::click(os, renderArea, Qt::LeftButton, mouseInitialPos );
    GTGlobals::sleep( 200 );

    const QPoint mouseInitialAbsPos = GTMouseDriver::getMousePosition( );
    const int rightMouseLimit = mouseInitialAbsPos.x( ) * 1.3;

    for ( int x = mouseInitialAbsPos.x( ); x < rightMouseLimit; x += 5 ) {
        const QPoint currentPos( x, mouseInitialAbsPos.y( ) );
        GTMouseDriver::moveTo( os, currentPos );
    }
}

GUI_TEST_CLASS_DEFINITION( test_2415 ) {
    // 1. Open "samples/FASTA/human_T1.fa".
    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");

    // 2. Right click on the object sequence name in the project view.
    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "human_T1 (UCSC April 2002 chr7:115977709-117855134)"));

    // 3. Click the menu {Edit -> Rename}.
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << ACTION_PROJECT__EDIT_MENU << "Rename"));
    GTMouseDriver::click(os, Qt::RightButton);

    // 4. Enter the new name: "name".
    GTKeyboardDriver::keySequence(os, "name");

    // 5. Press Enter.
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["enter"]);

    // Expected state: the sequence is renamed.
    QTreeWidgetItem *item = GTUtilsProjectTreeView::findItem(os, "name");
    CHECK_SET_ERR(NULL != item, "Object is not renamed");
}

GUI_TEST_CLASS_DEFINITION( test_2424 ) {
//     1. Open WD. Add Read Sequence, Write sequence and Filter quality (custm element with script section) workers. Make sure Fitler Quality worker's script text is empty.
    QString workflowOutputDirPath( testDir + "_common_data/scenarios/sandbox" );
    QDir workflowOutputDir( workflowOutputDirPath );
    GTUtilsWorkflowDesigner::openWorkflowDesigner(os);

    GTUtilsWorkflowDesigner::addAlgorithm( os, "Read Sequence" );
    GTUtilsWorkflowDesigner::addAlgorithm( os, "Write Sequence" );
    GTUtilsWorkflowDesigner::addAlgorithm( os, "Quality filter example" );

    WorkflowProcessItem *seqReader = GTUtilsWorkflowDesigner::getWorker( os, "Read Sequence" );
    WorkflowProcessItem *seqWriter = GTUtilsWorkflowDesigner::getWorker( os, "Write Sequence" );
    WorkflowProcessItem *qualFilter = GTUtilsWorkflowDesigner::getWorker( os, "Quality filter example" );

    GTUtilsWorkflowDesigner::connect(os, seqReader, qualFilter);
    GTUtilsWorkflowDesigner::connect(os, qualFilter, seqWriter);
    //     2. Connect workers into schema, set input data and output data (for example eas.fastq from samples)
    GTMouseDriver::moveTo( os, GTUtilsWorkflowDesigner::getItemCenter( os, "Read Sequence" ) );
    GTMouseDriver::click( os );
    QString dirPath = dataDir + "samples/FASTA/";
    GTUtilsWorkflowDesigner::setDatasetInputFile( os, dirPath, "human_T1.fa" );

    const QString outputFilePath = workflowOutputDir.absolutePath( ) + "/test.gb";

    GTMouseDriver::moveTo( os, GTUtilsWorkflowDesigner::getItemCenter( os, "Write Sequence" ) );
    GTMouseDriver::click( os );
    GTUtilsWorkflowDesigner::setParameter( os, "Output file", outputFilePath, GTUtilsWorkflowDesigner::textValue );
    //     3. Launch the schema. 
    //     Expected state: schema didnt start, error "Quality filter example"
    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller(os, QMessageBox::Ok));
    GTWidget::click( os,GTAction::button( os,"Run workflow" ) );
    GTGlobals::sleep( );
    GTUtilsWorkflowDesigner::checkErrorList(os, "Quality Filter Example: Empty script text");
}

GUI_TEST_CLASS_DEFINITION( test_2449 ) {
//    1. Open "COI.aln".
    GTFileDialog::openFile(os, dataDir + "samples/CLUSTALW/", "COI.aln");

//    2. Create a phylogenetic tree for the alignment.
    GTUtilsDialog::waitForDialog(os, new BuildTreeDialogFiller(os, sandBoxDir + "test_2449.nwk", 0, 0, true));
    GTUtilsDialog::waitForDialogWhichMayRunOrNot(os, new LicenseAgreemntDialogFiller(os));
    GTMenu::clickMenuItemByText(os, GTMenu::showMainMenu(os, MWMENU_ACTIONS), QStringList() << "Tree" << "Build Tree");

//    3. Open tree options panel widget (it can be opened automatically after tree building).
//    4. Open font settings on the OP widget.
    GTWidget::click(os, GTWidget::findWidget(os, "lblFontSettings"));

//    There is a font size spinbox. You can set zero value to it: in this case font has its standard size (on mac), but this value is incorrect.
    QSpinBox* sizeSpinBox = qobject_cast<QSpinBox*>(GTWidget::findWidget(os, "fontSizeSpinBox"));
    CHECK_SET_ERR(NULL != sizeSpinBox, "Size spin box not found");

    GTWidget::setFocus(os, sizeSpinBox);
    int prev = 0;
    while (0 < sizeSpinBox->value()) {
        prev = sizeSpinBox->value();
        GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["down"]);
        GTGlobals::sleep(100);
        if (prev <= sizeSpinBox->value()) {
            break;
        }
    }

    CHECK_SET_ERR(0 < sizeSpinBox->value(), "Invalid size spin box bound");
}

GUI_TEST_CLASS_DEFINITION( test_2460 ) {
    //1. Open "COI.aln".
    //2. Remove all sequences except the first one.
    //3. Align the result one-line-msa by kalign with default values.
    //Expected state: Kalign task finishes with error. Redo button is disabled.

    GTLogTracer l;
    GTFileDialog::openFile(os, dataDir+"samples/CLUSTALW/", "COI.aln");

    QStringList list = GTUtilsMSAEditorSequenceArea::getNameList(os);
    GTUtilsMSAEditorSequenceArea::selectArea(os, QPoint(-5, 1), QPoint(-5, list.size() - 1));
    GTKeyboardDriver::keyPress(os, GTKeyboardDriver::key["delete"]);

    GTUtilsMSAEditorSequenceArea::moveTo(os, QPoint(-5, 0));

    GTUtilsDialog::waitForDialog(os, new KalignDialogFiller(os));
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << MSAE_MENU_ALIGN << "align_with_kalign"));
    GTMouseDriver::click(os, Qt::RightButton);

    GTGlobals::sleep();

    CHECK_SET_ERR( l.hasError() == true, "There is no error in the log");

    QAbstractButton *redo= GTAction::button(os,"msa_action_redo");
    CHECK_SET_ERR( NULL != redo, "There is no REDO button");
    CHECK_SET_ERR( redo->isEnabled() == false, "REDO button is enabled");
}

GUI_TEST_CLASS_DEFINITION( test_2475 ) {
    //1. Open WD.
    //2. Open Single-sample Tuxedo Pipeline (NGS samples).
    //3. Set proper input data.
    //4. Validate scheme.
    //Expected state: validation passed.


    class EscClicker : public Filler {
    public:
        EscClicker(U2OpStatus& _os) : Filler(_os, "Tuxedo Wizard"){}
        virtual void run(){
            GTGlobals::sleep();
#ifdef Q_OS_MAC
            //GTKeyboardDriver::keyRelease(os, GTKeyboardDriver::key["shift"]);
            //GTKeyboardDriver::keyRelease(os, GTKeyboardDriver::key["cmd"]);
            GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["esc"]);
            //GTWidget::click(os,GTWidget::findButtonByText(os, "Cancel"));
#else
            GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["esc"]);
#endif
        }
    };

    GTUtilsWorkflowDesigner::openWorkflowDesigner(os);

    GTUtilsDialog::waitForDialog(os, new EscClicker(os));
    GTUtilsDialog::waitForDialog(os, new ConfigureTuxedoWizardFiller(os,
                                                                     ConfigureTuxedoWizardFiller::single_sample,
                                                                     ConfigureTuxedoWizardFiller::singleReads));
    GTUtilsWorkflowDesigner::addSample(os, "RNA-seq analysis with Tuxedo tools");
    GTGlobals::sleep();

    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, "Read RNA-seq Short Reads"));
    GTMouseDriver::click(os);
    GTUtilsWorkflowDesigner::setDatasetInputFile(os, testDir + "_common_data/bowtie2", "reads_1.fq");
    GTUtilsWorkflowDesigner::setDatasetInputFile(os, testDir + "_common_data/bowtie2", "reads_2.fq");

    GTMouseDriver::moveTo(os, GTUtilsWorkflowDesigner::getItemCenter(os, "Find Splice Junctions with TopHat"));
    GTMouseDriver::click(os);
    GTUtilsWorkflowDesigner::setParameter(os, "Bowtie index directory", testDir + "_common_data/bowtie2/index", GTUtilsWorkflowDesigner::textValue);
    GTUtilsWorkflowDesigner::setParameter(os, "Bowtie index basename", "human_T1_cutted", GTUtilsWorkflowDesigner::textValue);

    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller(os, QMessageBox::Ok));
    GTWidget::click(os, GTAction::button(os,"Validate workflow"));
}

GUI_TEST_CLASS_DEFINITION( test_2487 ) {
    // 1. Open "COI.nwk".
    // 2. Select root-node.
    // 3. Call context menu.
    // 'Reroot tree' and 'Collapse ' options must be disabled

    GTFileDialog::openFile(os, dataDir + "samples/Newick/", "COI.nwk");

    QList<QGraphicsItem*> items = GTUtilsPhyTree::getNodes(os);
    CHECK_SET_ERR(items.size() != 0, "Tree is empty");

    QPoint rootCoords = GTUtilsPhyTree::getGlobalCoord(os, items.first());
    GTMouseDriver::moveTo(os, rootCoords);

    GTMouseDriver::click(os, Qt::RightButton);
    GTGlobals::sleep();

    QMenu* activePopupMenu = qobject_cast<QMenu*>(QApplication::activePopupWidget());
    CHECK_SET_ERR(activePopupMenu != NULL, "There is no popup menu appeared");

    const QList<QAction*> menuActions = activePopupMenu->actions();

    foreach (QAction* a, menuActions) {
        if (a->text() == "Reroot tree" || a->text() == "Collapse") {
            CHECK_SET_ERR( !a->isEnabled(), a->text() + " action is enabled");
        }
    }
}

GUI_TEST_CLASS_DEFINITION( test_2496 ) {
    // 1. Open file testDir/_common_data/scenarios/assembly/example-alignment.bam
    // Expected state: import bam dialog appeared

    GTUtilsDialog::waitForDialog(os,
                                 new ImportBAMFileFiller(os, testDir + "_common_data/scenarios/sandbox/example-alignment.bam.ugenedb"));
    GTFileDialog::openFile(os, testDir + "_common_data/scenarios/assembly/", "example-alignment.bam");
}

GUI_TEST_CLASS_DEFINITION( test_2506 ) {
    // 1. Open file _common_data/vcf_consensus/vcf_cons_out_damaged_1.vcf
    // Expected state: error message appears.
    GTLogTracer l;

    GTFileDialog::openFile(os, testDir + "_common_data/vcf_consensus/", "vcf_cons_out_damaged_1.vcf");

    CHECK_SET_ERR(l.hasError() == true, "There is no expected error message in log");
}

GUI_TEST_CLASS_DEFINITION( test_2506_1 ) {
    // 1. Open file _common_data/vcf_consensus/vcf_cons_out_damaged_2.vcf
    // Expected state: error message appears.
    GTLogTracer l;

    GTFileDialog::openFile(os, testDir + "_common_data/vcf_consensus/", "vcf_cons_out_damaged_2.vcf");

    CHECK_SET_ERR(l.hasError() == true, "There is no expected error message in log");
}
GUI_TEST_CLASS_DEFINITION( test_2519 ) {
    // 1. Open {data/samples/FASTA/human_T1.fa}.
    GTFileDialog::openFile(os, dataDir+"samples/FASTA/", "human_T1.fa");

    // 2. Go to position 20000.
    GTUtilsDialog::waitForDialog(os, new GoToDialogFiller(os, 20000));
    GTKeyboardDriver::keyClick(os, 'g', GTKeyboardDriver::key["ctrl"]);

    // 3. Call context menu, select menu item {Edit sequence -> Remove subsequence...}.
    // Expected state: a "Remove subsequence" dialog appears.
    // 4. Set the region (1..190950), other settings should be default, click the "Remove" button.
    // Expected state: UGENE doesn't crash.
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << ADV_MENU_EDIT << ACTION_EDIT_REMOVE_SUBSEQUENCE));
    GTUtilsDialog::waitForDialog(os, new RemovePartFromSequenceDialogFiller(os, "1..190950"));
    GTMenu::showContextMenu(os, GTUtilsMdi::activeWindow(os));
    GTGlobals::sleep(2000);

    // 5. Call this dialog again, remove region (1..8999).
    // Expected state: UGENE doesn't crash.
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << ADV_MENU_EDIT << ACTION_EDIT_REMOVE_SUBSEQUENCE));
    GTUtilsDialog::waitForDialog(os, new RemovePartFromSequenceDialogFiller(os, "1..8999"));
    GTMenu::showContextMenu(os, GTUtilsMdi::activeWindow(os));
    GTGlobals::sleep(5000);
}

GUI_TEST_CLASS_DEFINITION( test_2542 ) {
    // 1. Open "data/samples/CLUSTALW/COI.aln".
    GTFileDialog::openFile(os, dataDir + "samples/CLUSTALW/", "COI.aln");

    // 2. Lock the document
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList() << "action_project__edit_menu"
        << "action_document_lock"));
    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "COI.aln"));
    GTMouseDriver::click(os, Qt::RightButton);

    // 3. Do pairwise alignment with any sequences.
    GTUtilsMSAEditorSequenceArea::selectArea(os, QPoint(0, 0), QPoint(1, 1));
    GTWidget::click(os, GTWidget::findWidget(os, "OP_PAIRALIGN"));
    GTWidget::click(os, GTWidget::findWidget(os, "ArrowHeader_Output settings"));

    QCheckBox *check = qobject_cast<QCheckBox*>(GTWidget::findWidget(os, "inNewWindowCheckBox"));
    CHECK_SET_ERR(check != NULL, "inNewWindowCheckBox not found!");
    GTCheckBox::setChecked(os, check, false);

    // State:
    // Align button not active if file locked for writing (user locking or format didn't support writing) and selected aligning in current file option
    QWidget *alignButton = GTWidget::findWidget(os, "alignButton");
    CHECK_SET_ERR(!alignButton->isEnabled(), "Align button is enabled");
}

GUI_TEST_CLASS_DEFINITION( test_2543 ) {
    //1. Open "samples/CLUSTALW/COI.aln".
    GTFileDialog::openFile( os, dataDir + "samples/CLUSTALW/", "COI.aln" );

    //2. Click the "Build tree" button on the toolbar.
    //Expected state: a "Build Phylogenetic Tree" dialog appeared.
    //3. Set the output file location to any read-only folder.
    const QString outputFilePath = testDir + "_common_data/scenarios/sandbox/gui_regr_2543";
    QDir sandboxDir( testDir + "_common_data/scenarios/sandbox" );
    sandboxDir.mkdir( "gui_regr_2543" );
    PermissionsSetter permSetter;
    const QFile::Permissions p = QFile::WriteOwner | QFile::WriteUser | QFile::WriteGroup
        | QFile::WriteOther;
    bool res = permSetter.setPermissions( outputFilePath, ~p );
    CHECK_SET_ERR( res, "Can't set permissions" );
    GTGlobals::sleep( 2000 );

    GTUtilsDialog::waitForDialog( os, new BuildTreeDialogFiller( os, outputFilePath + "/test.nwk",
        0, 0, true ) );
    GTUtilsDialog::waitForDialogWhichMayRunOrNot( os, new LicenseAgreemntDialogFiller( os ) );
    //Expected state: UGENE does not allow to create tree, the message dialog appears
    GTUtilsDialog::waitForDialog(os, new MessageBoxDialogFiller( os, QMessageBox::Ok ) );

    QAbstractButton *tree= GTAction::button( os, "Build Tree" );
    GTWidget::click( os, tree );
    GTGlobals::sleep( 2000 );
}

GUI_TEST_CLASS_DEFINITION( test_2565 ) {
    //    1. Open "samples/Genbank/murine.gb".
    //    2. Press Ctrl+F.
    //    3. Insert the pattern "GCTAGCTTAAGTAACGCCACTTTT".
    //    4. Click "Search".
    //    Expected: the pattern is not found. Notification with this information appears.
    GTLogTracer l;
    GTFileDialog::openFile(os, dataDir+"samples/Genbank/", "murine.gb");
    GTUtilsOptionsPanel::runFindPatternWithHotKey("GCTAGCTTAAGTAACGCCACTTTT", os);
    CHECK_SET_ERR(l.checkMessage(QString("Searching patterns in sequence task: No results found.")),
                  "No expected message in the log");

}

GUI_TEST_CLASS_DEFINITION( test_2566 ) {
//1. Open "samples/Genbank/murine.gb".
    GTLogTracer l;
    GTFileDialog::openFile( os, dataDir + "samples/Genbank/", "murine.gb" );

//2. Press Ctrl+F.
    GTKeyboardDriver::keyClick( os, 'f', GTKeyboardDriver::key["ctrl"] );
    GTGlobals::sleep( );

//3. Enter the pattern: GCTAGCTTAAGTAACGCCAC
    QWidget *patternInputLine = QApplication::focusWidget( );
    CHECK_SET_ERR( NULL != patternInputLine && patternInputLine->objectName( ) == "textPattern",
        "Focus is not on FindPattern widget" );

    GTKeyboardDriver::keySequence( os, "GCTAGCTTAAGTAACGCCAC" );
    GTGlobals::sleep( 1000 );

//4. Choose the algorithm: Substitute.
     QComboBox *algoBox = static_cast<QComboBox *>( GTWidget::findWidget( os, "boxAlgorithm" ) );
     CHECK_SET_ERR( NULL != algoBox, "Algorithm combobox not found!" );

     GTComboBox::setIndexWithText( os, algoBox, "Substitute" );

//5. Enter the "Should match" value: 30%
     QSpinBox *matchingBox = static_cast<QSpinBox *>( GTWidget::findWidget( os, "spinBoxMatch" ) );
     CHECK_SET_ERR( NULL != matchingBox, "Algorithm match spinbox not found!" );

     GTSpinBox::setValue( os, matchingBox, 30, GTGlobals::UseKeyBoard );

//6. Click "Search".
    QPushButton *searchButton = static_cast<QPushButton *>( GTWidget::findWidget(os, "btnSearch") );
    GTWidget::click( os, searchButton );

//Expected state: the task finished successfully.
    CHECK_SET_ERR( !l.hasError( ), "Unexpected error in log!" );
}

GUI_TEST_CLASS_DEFINITION( test_2577 ) {

    //1. Open {data/samples/CLUSTALW/COI.aln}.
    GTLogTracer l;
    GTFileDialog::openFile( os, dataDir + "samples/CLUSTALW/", "COI.aln" );

    //2. Call context menu somewhere on the alignment, select {Consenssus mode...} menu item.
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os,QStringList()<<"Consensus mode",GTGlobals::UseMouse));
    GTMenu::showContextMenu(os,GTUtilsMdi::activeWindow(os));

    //Expected state: options panel "General" tab opens.
    QWidget *panel = GTWidget::findWidget(os, "OP_MSA_GENERAL");
    CHECK_SET_ERR(NULL != panel, "General OP tab does not appear");
    GTGlobals::sleep(500);

}

GUI_TEST_CLASS_DEFINITION( test_2578 ) {
//    1. Open {data/samples/CLUSTALW/COI.aln}.
    GTFileDialog::openFile(os, dataDir + "/samples/CLUSTALW/", "COI.aln");

//    2. Open options panel 'Highlighting' tab.
    GTWidget::click(os, GTWidget::findWidget(os, "OP_MSA_HIGHLIGHTING"));

//    3. Select 'agreements' highlighting scheme.
    QComboBox* combo = qobject_cast<QComboBox*>(GTWidget::findWidget(os, "highlightingScheme"));
    CHECK_SET_ERR(combo != NULL, "highlightingScheme not found!");
    GTComboBox::setIndexWithText(os, combo , "Agreements");

    QWidget* exportButton = GTWidget::findWidget(os, "exportHighlightning");
    CHECK_SET_ERR(NULL != exportButton, "exportButton not found");
    CHECK_SET_ERR(!exportButton->isEnabled(), "exportButton is enabled unexpectedly");

//    4. Select any reference sequence.
//    Expected state: the "Export" button is active.
    GTWidget::click(os, GTWidget::findWidget(os, "sequenceLineEdit"));
    GTKeyboardDriver::keySequence(os, "Montana_montana");
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["enter"]);
    GTGlobals::sleep(300);
    CHECK_SET_ERR(exportButton->isEnabled(), "exportButton is disabled unexpectedly");

//    5. Select 'General' tab, then select 'Highlighting' tab again.
//    Expected state: the "Export" button is active.
    GTWidget::click(os, GTWidget::findWidget(os, "OP_MSA_GENERAL"));
    GTWidget::click(os, GTWidget::findWidget(os, "OP_MSA_HIGHLIGHTING"));

    exportButton = GTWidget::findWidget(os, "exportHighlightning");
    CHECK_SET_ERR(NULL != exportButton, "exportButton not found");
    CHECK_SET_ERR(exportButton->isEnabled(), "exportButton is disabled unexpectedly");
}

GUI_TEST_CLASS_DEFINITION( test_2605 ) {
    GTLogTracer logTracer;
    // 1. Open file _common_data/fasta/multy_fa.fa as multiple alignment
    GTUtilsDialog::waitForDialog(os, new SequenceReadingModeSelectorDialogFiller(os, SequenceReadingModeSelectorDialogFiller::Join));
    GTFileDialog::openFile(os, testDir + "_common_data/fasta/", "multy_fa.fa");
    // 2. Export subalignment from this msa to any MSA format
    GTUtilsDialog::waitForDialog(os, new PopupChooser(os, QStringList()<<MSAE_MENU_EXPORT<<"Save subalignment"));
    GTUtilsDialog::waitForDialog(os,new ExtractSelectedAsMSADialogFiller(os,
        testDir + "_common_data/scenarios/sandbox/2605.aln",
        QStringList() << "SEQUENCE_1"<<"SEQUENCE_2", 6, 237));
    GTMenu::showContextMenu(os,GTWidget::findWidget(os,"msa_editor_sequence_area"));

    // Expected state: export successfull, no any messages in log like "There is no sequence objects in given file, unable to convert it in multiple alignment"
    CHECK_SET_ERR(!logTracer.hasError(), "Unexpected error");
}

GUI_TEST_CLASS_DEFINITION( test_2612 ) {
    // 1. Open sequence "samples/fasta/human_T1.fa".
    GTFileDialog::openFile(os, dataDir + "samples/FASTA", "human_T1.fa");
    // 2. Search for 20 first nucleotides (TTGTCAGATTCACCAAAGTT) using Find Pattern.
    GTUtilsOptionsPanel::runFindPatternWithHotKey("TTGTCAGATTCACCAAAGTT", os);
    // Expected state: the annotation with pattern created and shown in sequence view.
    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "Annotations"));
    QTreeWidgetItem *item = GTUtilsAnnotationsTreeView::findItem(os, "misc_feature");
    GTMouseDriver::moveTo(os, GTTreeWidget::getItemCenter(os, item));
    // 3. Delete annotation from annotation editor.
    GTMouseDriver::click(os);
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["delete"]);
    // Expected state: there is no annotation in sequence view.
    GTGlobals::sleep(100);
    CHECK_SET_ERR(GTUtilsAnnotationsTreeView::findItem(os, "misc_feature", GTGlobals::FindOptions(false))==NULL, "Annotations document not deleted");
}

GUI_TEST_CLASS_DEFINITION( test_2667 ) {
//    1. Open {/data/samples/genbank/murine.gb}.
//    Expected state: a document was added, it contains two object: an annotation and a sequence
    GTFileDialog::openFile(os, dataDir + "/samples/Genbank/", "murine.gb");

//    2. Select the annotation object in the project view.
    GTMouseDriver::moveTo(os, GTUtilsProjectTreeView::getItemCenter(os, "NC_001363 features"));
    GTMouseDriver::click(os);

//    3. Press "delete" key.
//    Expected state: the annotation object is removed from the document.
    GTKeyboardDriver::keyClick(os, GTKeyboardDriver::key["delete"]);
    GTGlobals::FindOptions options;
    options.failIfNull = false;
    QTreeWidgetItem* annotationsItem = GTUtilsProjectTreeView::findItem(os, GTUtilsProjectTreeView::getTreeWidget(os), "NC_001363 features", options);
    CHECK_SET_ERR(NULL == annotationsItem, "item was not deleted");
}

} // GUITest_regression_scenarios namespace

} // U2 namespace

