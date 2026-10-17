/**
 * ShortcutHelpDialog.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QDialog>

// NAMEHERE Headers
#include "rbx/BaldPtr.h"

class QTreeWidget;
class NAMEHEREMainWindow;

/**
 * Dialog that displays all the shortcut key sequences for actions in the main window.
 *  Also displays the actions' icon and statustip text.
 *  The dialog does not delete on close.
 */
class ShortcutHelpDialog : public QDialog
{
    public:

        ShortcutHelpDialog(NAMEHEREMainWindow& MainWindow);
        virtual ~ShortcutHelpDialog();

    private:

        void initialize();
        void populate();

        RBX::BaldPtr<QTreeWidget>   mTree;
        NAMEHEREMainWindow&           mMainWindow;
};