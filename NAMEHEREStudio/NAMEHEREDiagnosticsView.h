/**
 * NAMEHEREDiagnosticsView.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

//this will make sure that we can use qt with boost (else moc compiler errors)
#ifndef QT_NO_KEYWORDS
    #define QT_NO_KEYWORDS
#endif

// Qt Headers
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMutex>

// NAMEHERE Headers
#include "rbx/signal.h"

namespace RBX {
	class Instance;
	class DataModel;
	namespace Stats
	{
		class Item;
		class StatsService;
	}
}

class NAMEHEREDiagnosticsView : public QTreeWidget
{
	Q_OBJECT
public:
	NAMEHEREDiagnosticsView(bool createdFromIDEDoc = false); 
	virtual ~NAMEHEREDiagnosticsView();

	void setDataModel(boost::shared_ptr<RBX::DataModel> pDataModel);
	boost::shared_ptr<RBX::DataModel> dataModel();	
	
public Q_SLOTS:
	void setVisible(bool visible);

private Q_SLOTS:
	void updateValues();
		
private:
	void onChildAdded(boost::shared_ptr<RBX::Instance> child);
	void onChildRemoved(boost::shared_ptr<RBX::Instance> child);

	boost::shared_ptr<RBX::DataModel>				m_pDataModel;
	boost::shared_ptr<RBX::Stats::StatsService>		m_pStats;
	rbx::signals::scoped_connection					m_childAddedConnection;
	rbx::signals::scoped_connection					m_childRemovedConnection;
	QTimer										   *m_pTimer;

	bool											m_bPreviousProfiling;
	bool                                            m_bIsCreatedFromIDEDoc;
};

class NAMEHEREDiagnosticsViewItem: public QTreeWidgetItem
{
public:
	NAMEHEREDiagnosticsViewItem(boost::shared_ptr<RBX::Stats::Item> pItem);

	NAMEHEREDiagnosticsView     *getTreeWidget();
	NAMEHEREDiagnosticsViewItem *getItemParent();

	boost::shared_ptr<RBX::Stats::Item> getItem() { return m_pItem; }

	void updateValues();

private:
	void onChildAdded(boost::shared_ptr<RBX::Instance> child);
	void onChildRemoved(boost::shared_ptr<RBX::Instance> child);

	boost::shared_ptr<RBX::Stats::Item>		m_pItem;
	rbx::signals::scoped_connection			m_childAddedConnection;
	rbx::signals::scoped_connection			m_childRemovedConnection;	
};

