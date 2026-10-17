/**
 * NAMEHEREBasicDoc.cpp
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "NAMEHEREBasicDoc.h"

// Qt Headers
#include <QString>

// NAMEHERE Studio Headers
#include "IExternalHandler.h"
#include "UpdateUIManager.h"
#include "NAMEHEREDocManager.h"

NAMEHEREBasicDoc::NAMEHEREBasicDoc()
    : m_bActive(false)
{
}

NAMEHEREBasicDoc::~NAMEHEREBasicDoc()
{
}

void NAMEHEREBasicDoc::addExternalHandler(IExternalHandler* extHandler)
{
	if(extHandler == NULL) 
		return;

	if(extHandler->handlerId().isEmpty())
		return;

	m_handlersMap[extHandler->handlerId()] = extHandler;
}

void NAMEHEREBasicDoc::removeExternalHandler(const QString &handlerId)
{
	if(handlerId.isEmpty())
		return;

	m_handlersMap.erase(handlerId);
}

IExternalHandler* NAMEHEREBasicDoc::getExternalHandler(const QString &handlerId)
{
	return m_handlersMap[handlerId];
}

bool NAMEHEREBasicDoc::handleAction( const QString &actionID, bool isChecked /*= true*/ )
{
	// Try to see if inherited class handles this action
	return doHandleAction(actionID, isChecked);
}

/** do not override **/
bool NAMEHEREBasicDoc::close()
{
	if (doClose())
	{
		NAMEHEREDocManager::Instance().updateWindowMenu();
		return true;
	}
	return false;
}
