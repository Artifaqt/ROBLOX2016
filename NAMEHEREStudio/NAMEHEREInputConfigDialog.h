/**
 * NAMEHEREInputConfigDialog.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

#include <QAction>
#include <QDialog>
#include <QString>
#include <QWidget>

#include "rbx/BaldPtr.h"

class NAMEHEREMainWindow;
class QEvent;
class QTreeView;
class QDialogButtonBox;
class QAbstractButton;
class NAMEHEREKeyboardConfigWidget;
class NAMEHEREMouseConfigWidget;

//
// A dialog for configuring mouse and keyboard shortcuts for all actions
// under the NAMEHEREMainWindow.
//
class NAMEHEREInputConfigDialog : public QDialog
{
    Q_OBJECT
    
public:

    NAMEHEREInputConfigDialog(NAMEHEREMainWindow& MainWindow);
    virtual ~NAMEHEREInputConfigDialog();
    
private Q_SLOTS:
    void accept();
    void cancel();
    void clicked(QAbstractButton* button);
    
    void dataChanged();
    
private:
    virtual void closeEvent(QCloseEvent* event);
    void restoreAllDefaults();
    void initialize();

    QDialogButtonBox*           m_dialogButtonBox;
    NAMEHEREKeyboardConfigWidget* m_keyboardConfigWidget;
    NAMEHEREMouseConfigWidget*    m_mouseConfigWidget;
    NAMEHEREMainWindow&           m_mainWindow;
    bool                        m_dataChanged;
};

