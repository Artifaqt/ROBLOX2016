/**
 * NAMEHEREContextualHelp.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

#include "NAMEHEREBrowser.h"
#include "NAMEHEREWebPage.h"

#include <QWidget>

class NAMEHEREContextualHelpService : public QObject
{
    Q_OBJECT
public:
    NAMEHEREContextualHelpService();
    
    static NAMEHEREContextualHelpService& singleton();

public Q_SLOTS:
    void onHelpTopicChanged(const QString& helpTopic);

Q_SIGNALS:
    void helpTopicChanged(const QString&);

private:
    QString m_helpTopic;
};

class NAMEHEREContextualHelp;

class NAMEHEREHelpWebView : public NAMEHEREBrowser
{
    Q_OBJECT
public:
    NAMEHEREHelpWebView(NAMEHEREContextualHelp* widget);

protected:
    void paintEvent(QPaintEvent* event);

private Q_SLOTS:
    void loadProgress(int);

private:
    NAMEHEREContextualHelp*    m_contextualHelpWidget;
};

class NAMEHEREContextualHelp : public QWidget
{
	Q_OBJECT
	
public:
    NAMEHEREContextualHelp();

    void updateURL();

public Q_SLOTS:
    void onAuthenticationChanged(bool isAuthenticated);
    void onHelpTopicChanged(const QString& helpTopic);
    
private Q_SLOTS:
	void linkClicked(const QUrl& url);
    
private:
    void setupWebView();
   
    NAMEHEREHelpWebView*      m_pWebView;
    NAMEHEREWebPage*          m_pWebPage;
    QString                 m_urlString;
    bool                    m_urlDirty;
};

