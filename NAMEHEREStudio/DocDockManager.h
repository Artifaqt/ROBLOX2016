/**
 * DocDockWidget.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QDockWidget>
#include <QMap>

// NAMEHERE Headers
#include "RBX/BaldPtr.h"

// NAMEHERE Studio Headers
#include "INAMEHEREDoc.h"

class NAMEHEREMainWindow;
class DocDockWidget;

class DocDockManager : public QObject
{
    Q_OBJECT

    public:

        DocDockManager(NAMEHEREMainWindow& mainWindow);
        virtual ~DocDockManager();

        void addDoc(INAMEHEREDoc& doc);
        bool removeDoc(INAMEHEREDoc& doc);
        bool renameDoc(INAMEHEREDoc& doc,const QString& text,const QString& tooltip);

        bool setCurrentDoc(INAMEHEREDoc& doc);
        INAMEHEREDoc* getCurrentDoc() const;

        void startDrag(INAMEHEREDoc& doc);

    Q_SIGNALS:

        void attachTab(INAMEHEREDoc& doc);

    private:

        typedef QMap< RBX::BaldPtr<INAMEHEREDoc>,RBX::BaldPtr<DocDockWidget> > tDocDockMap;

        NAMEHEREMainWindow&   m_MainWindow;
        tDocDockMap         m_Docs;
};

