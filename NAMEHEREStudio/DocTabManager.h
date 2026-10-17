/**
 * DocTabWidget.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QRubberBand>
#include <QIcon>

// NAMEHERE Headers
#include "RBX/BaldPtr.h"

// NAMEHERE Studio Headers
#include "INAMEHEREDoc.h"

class QRubberBand;

class NAMEHEREMainWindow;
class NAMEHERETabWidget;

class DocTabManager : public QObject
{
    Q_OBJECT

    public:

        DocTabManager(NAMEHEREMainWindow& mainWindow);
        virtual ~DocTabManager();

        void addDoc(INAMEHEREDoc& doc, int index = -1);
        bool removeDoc(INAMEHEREDoc& doc);
        bool renameDoc(INAMEHEREDoc& doc,const QString& text,const QString& tooltip,const QIcon& icon = QIcon());

        bool setCurrentDoc(INAMEHEREDoc& doc);
        INAMEHEREDoc* getCurrentDoc() const;

        void setDockHoverOverPos(const QPoint& globalPos);
        bool attemptAttach(INAMEHEREDoc& doc,const QPoint& globalPos);

        void restoreAsCentralWidget();

    public Q_SLOTS:
        void showNextTabPage();
        void showPreviousTabPage();

    protected:

        virtual bool eventFilter(QObject* object,QEvent* event);

    private Q_SLOTS:

        void onCurrentChanged(int index);
        bool onTabCloseRequested(int index);
        void onCurrentTabCloseRequested();
        void onDetachDoc();
        void onFocusChanged(QWidget* oldWidget,QWidget* newWidget);
        
    private:

        void onMousePressEvent(QMouseEvent* event);
        void onMouseMoveEvent(QMouseEvent* event);
        void onMouseReleaseEvent(QMouseEvent* event);
        bool isValidHoverOver(const QPoint& globalPos);
    
        void setupTabPageSwitchingShortcuts();
        void setCurrentTabPage(int index);

        typedef QVector< RBX::BaldPtr<INAMEHEREDoc> > tDocList;

        tDocList                		m_Docs;
        NAMEHEREMainWindow&       		m_MainWindow;
        QPoint                  		m_ClickPos;
        bool                    		m_MouseDown;
        int                     		m_DragTabIndex;
        QRubberBand             		m_RubberBand;
        RBX::BaldPtr<QWidget>   		m_FakeTab;
		RBX::BaldPtr<NAMEHERETabWidget>	m_TabWidget;
};

