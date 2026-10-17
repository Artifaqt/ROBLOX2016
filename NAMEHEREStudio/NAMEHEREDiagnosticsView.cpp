/**
 * NAMEHEREDiagnosticsView.cpp
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "NAMEHEREDiagnosticsView.h"

// Qt Headers
#include <QTimer>
#include <QHeaderView>

// NAMEHERE Headers
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Stats.h"
#include "util/Profiling.h"

static const int UpdateInterval = 1000 / 5;

NAMEHEREDiagnosticsViewItem::NAMEHEREDiagnosticsViewItem(boost::shared_ptr<RBX::Stats::Item> pItem)
: m_pItem(pItem)
{
	//RBX::DataModel::LegacyLock lock(getTreeWidget()->dataModel(), RBX::DataModelJob::Read);
	setText(0, m_pItem->getName().c_str());
	setText(1, m_pItem->getStringValue().c_str());

	m_pItem->visitChildren(boost::bind(&NAMEHEREDiagnosticsViewItem::onChildAdded, this, _1));
	m_childAddedConnection = m_pItem->getOrCreateChildAddedSignal()->connect(boost::bind(&NAMEHEREDiagnosticsViewItem::onChildAdded, this, _1));
	m_childRemovedConnection = m_pItem->getOrCreateChildRemovedSignal()->connect(boost::bind(&NAMEHEREDiagnosticsViewItem::onChildRemoved, this, _1));
}

NAMEHEREDiagnosticsView *NAMEHEREDiagnosticsViewItem::getTreeWidget()
{	return dynamic_cast<NAMEHEREDiagnosticsView *>(treeWidget()); }

NAMEHEREDiagnosticsViewItem* NAMEHEREDiagnosticsViewItem::getItemParent()
{	return dynamic_cast<NAMEHEREDiagnosticsViewItem*>(parent()); }

void NAMEHEREDiagnosticsViewItem::onChildAdded(boost::shared_ptr<RBX::Instance> child)
{
	if(!child)
		return;

	boost::shared_ptr<RBX::Stats::Item> item = RBX::shared_dynamic_cast<RBX::Stats::Item>(child);
	if (!item)
		return;

	NAMEHEREDiagnosticsViewItem* pViewItem = new NAMEHEREDiagnosticsViewItem(item);
	if(pViewItem)
	{
		addChild(pViewItem);
	}
}

void NAMEHEREDiagnosticsViewItem::onChildRemoved(boost::shared_ptr<RBX::Instance> inst)
{
	if(!inst)
		return;

	boost::shared_ptr<RBX::Stats::Item> item = RBX::shared_dynamic_cast<RBX::Stats::Item>(inst);
	if (!item)
		return;

	NAMEHEREDiagnosticsViewItem* pItemToDelete = NULL;
	for(int ii=0; ii<childCount(); ii++)
	{
		NAMEHEREDiagnosticsViewItem* pViewItem = dynamic_cast<NAMEHEREDiagnosticsViewItem*>(child(ii));
		if(pViewItem->getItem() == item)
		{
			pItemToDelete = dynamic_cast<NAMEHEREDiagnosticsViewItem*>(takeChild(ii));
			break;
		}
	}

	if(pItemToDelete)
	{
		delete pItemToDelete;
		pItemToDelete = NULL;
	}

	if (!isExpanded() && childCount() > 0)					
		setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
}

void NAMEHEREDiagnosticsViewItem::updateValues()
{
	setText(1, m_pItem->getStringValue().c_str());

	for(int ii=0; ii<childCount(); ii++)
	{
		NAMEHEREDiagnosticsViewItem* pViewItem = dynamic_cast<NAMEHEREDiagnosticsViewItem*>(child(ii));
		if(pViewItem)
			pViewItem->updateValues();
	}
}

NAMEHEREDiagnosticsView::NAMEHEREDiagnosticsView(bool createdFromIDEDoc)
: m_pTimer(new QTimer(this))
, m_bPreviousProfiling(false)
, m_bIsCreatedFromIDEDoc(createdFromIDEDoc)
{
	setUniformRowHeights(true);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setAlternatingRowColors(true);

	setColumnCount(2);
	QStringList headerLabels;
	headerLabels << "Name" << "Value";
	setHeaderLabels(headerLabels);

	header()->setDefaultSectionSize(125);

    m_pTimer->setInterval(UpdateInterval);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(updateValues()));
}

NAMEHEREDiagnosticsView::~NAMEHEREDiagnosticsView()
{
	// turn off profiling
	if (!m_bIsCreatedFromIDEDoc)
		RBX::Profiling::setEnabled(false);

//	setDataModel(shared_ptr<RBX::DataModel>());
}

void NAMEHEREDiagnosticsView::setDataModel(boost::shared_ptr<RBX::DataModel> pDataModel)
{
	if(m_pDataModel == pDataModel)
		return;

	m_pTimer->stop();

	if (m_pDataModel)
	{
		RBX::DataModel::LegacyLock lock(m_pDataModel, RBX::DataModelJob::Write);
		m_childAddedConnection.disconnect();
		m_childRemovedConnection.disconnect();
		clear();
	}

	m_pStats.reset();
	m_pDataModel = pDataModel;

	if (m_pDataModel)
	{
		RBX::DataModel::LegacyLock lock(m_pDataModel, RBX::DataModelJob::Write);
		m_pStats = shared_from(RBX::ServiceProvider::create<RBX::Stats::StatsService>(m_pDataModel.get()));
		m_pTimer->start();
		
		m_pStats->visitChildren(boost::bind(&NAMEHEREDiagnosticsView::onChildAdded, this, _1));
		m_childAddedConnection = m_pStats->getOrCreateChildAddedSignal()->connect(boost::bind(&NAMEHEREDiagnosticsView::onChildAdded, this, _1));
		m_childRemovedConnection = m_pStats->getOrCreateChildRemovedSignal()->connect(boost::bind(&NAMEHEREDiagnosticsView::onChildRemoved, this, _1));
	} 
}

boost::shared_ptr<RBX::DataModel> NAMEHEREDiagnosticsView::dataModel()
{	return m_pDataModel; }

void NAMEHEREDiagnosticsView::updateValues()
{
    // if the window is hidden, stop the update timer
    if ( !isVisible() )
    {
        m_pTimer->stop();
        return;
    }

	for(int ii=0; ii<topLevelItemCount(); ii++)
	{
		NAMEHEREDiagnosticsViewItem* pViewItem = dynamic_cast<NAMEHEREDiagnosticsViewItem*>(topLevelItem(ii));
		if(pViewItem)
			pViewItem->updateValues();
	}
}

void NAMEHEREDiagnosticsView::setVisible(bool visible)
{
	if(visible)
	{
		m_bPreviousProfiling = RBX::Profiling::isEnabled();
		if(!m_bPreviousProfiling)
			RBX::Profiling::setEnabled(true);

		m_pTimer->start();
	}
	else
	{
		RBX::Profiling::setEnabled(m_bPreviousProfiling);
		m_pTimer->stop();
	}
	QTreeWidget::setVisible(visible);
}

void NAMEHEREDiagnosticsView::onChildAdded(boost::shared_ptr<RBX::Instance> child)
{
	if(!child)
		return;

	boost::shared_ptr<RBX::Stats::Item> item = RBX::shared_dynamic_cast<RBX::Stats::Item>(child);
	if (!item)
		return;
	
	NAMEHEREDiagnosticsViewItem* pViewItem = new NAMEHEREDiagnosticsViewItem(item);
	if(pViewItem)
	{
		addTopLevelItem(pViewItem);
	}
}

void NAMEHEREDiagnosticsView::onChildRemoved(boost::shared_ptr<RBX::Instance> child)
{
	if(!child)
		return;

	boost::shared_ptr<RBX::Stats::Item> item = RBX::shared_dynamic_cast<RBX::Stats::Item>(child);
	if (!item)
		return;

	NAMEHEREDiagnosticsViewItem* pItemToDelete = NULL;
	for(int ii=0; ii<topLevelItemCount(); ii++)
	{
		NAMEHEREDiagnosticsViewItem* pViewItem = dynamic_cast<NAMEHEREDiagnosticsViewItem*>(topLevelItem(ii));
		if(pViewItem->getItem() == item)
		{
			pItemToDelete = dynamic_cast<NAMEHEREDiagnosticsViewItem*>(takeTopLevelItem(ii));
			break;
		}
	}

	if(pItemToDelete)
	{
		delete pItemToDelete;
		pItemToDelete = NULL;
	}
}



