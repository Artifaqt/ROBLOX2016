/**
 * NAMEHEREBasicDoc.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

#include <QIcon>

// Standard C/C++ Headers
#include <map>

// NAMEHERE Studio Headers
#include "INAMEHEREDoc.h"

typedef std::map<QString, IExternalHandler*> HandlersMap;

class NAMEHEREBasicDoc: public INAMEHEREDoc
{
public:
    virtual const QIcon& titleIcon() const { return m_emptyIcon; }
    virtual const QString& titleTooltip() const { return m_titleToolTip; }
	virtual QString windowTitle() const { return displayName(); }

	virtual void addExternalHandler(IExternalHandler* extHandler);
	virtual void removeExternalHandler(const QString &handlerId);
	virtual IExternalHandler* getExternalHandler(const QString &handlerId);

	virtual void updateUI(bool state) {}

	virtual bool handleDrop(const QString &fileName) { return true; }
	virtual bool doHandleAction(const QString& actionID, bool isChecked) { return false; }
#ifdef _WIN32 
	bool close() sealed; // do not override -- sealed is CLI, so at least we prevent it at compile time in Windows.
#else
	bool close(); // do not override
#endif

protected:
	NAMEHEREBasicDoc();
	virtual ~NAMEHEREBasicDoc();

	HandlersMap     m_handlersMap;
	bool            m_bActive;
    QIcon           m_emptyIcon;
    QString         m_titleToolTip;

private:
	virtual bool doClose() = 0;
	bool handleAction(const QString &actionID, bool isChecked = true);

};
