/**
 * NAMEHEREBrowser.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QWebView>

class NAMEHEREBrowser : public QWebView 
{
	Q_OBJECT

public: 

	NAMEHEREBrowser(QWidget* parent=0);
    virtual ~NAMEHEREBrowser();

public Q_SLOTS:

	bool close();
    void resetLoadingTimer();
    
protected:
    
    virtual void paintEvent(QPaintEvent*);
 	virtual void dropEvent(QDropEvent* evt);

private Q_SLOTS:

    void loadStarted();
    void loadFinished(bool);
    
private:
    
    void drawLoadingWatermark();
    
	NAMEHEREBrowser*  m_pPopup;
	QDialog*        m_pPopupDlg;
    QTimer*         m_loadingTimer;
    float           m_refreshIncr;
};
