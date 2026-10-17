/**
 * PluginAction.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

#include <QAction>

class PluginAction : public QAction
{
public:
	PluginAction(const QString& string, QObject* obj) : QAction(string, obj){}
};