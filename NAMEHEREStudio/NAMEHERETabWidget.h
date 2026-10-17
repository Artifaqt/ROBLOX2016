/**
 * NAMEHERETabWidget.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QTabWidget>

class NAMEHERETabWidget : public QTabWidget
{
    public:

        NAMEHERETabWidget(QWidget* parent)
            : QTabWidget(parent)
        {
        }

        QTabBar& getTabBar() { return *tabBar(); }
};


