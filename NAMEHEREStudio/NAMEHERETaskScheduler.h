/**
 * NAMEHERETaskScheduler.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

// NAMEHERE Headers
#include "rbx/TaskScheduler.h"

// NAMEHERE Studio Headers
#include "NAMEHEREReportView.h"

class TSJobItem;

class ArbiterItem: public NAMEHERECategoryItem
{
public:
	ArbiterItem() {}
	TSJobItem* getOrCreateTSJobItem(boost::shared_ptr<const RBX::TaskScheduler::Job> pJob);
};

class TSJobItem: public QTreeWidgetItem
{
public:
	TSJobItem(ArbiterItem* pParentItem, boost::shared_ptr<const RBX::TaskScheduler::Job> pJob);

	boost::shared_ptr<const RBX::TaskScheduler::Job> getJob() { return m_pJob; }
	void updateValues();

private:
	boost::shared_ptr<const RBX::TaskScheduler::Job>	m_pJob;
};

class NAMEHERETaskScheduler : public NAMEHEREReportView
{
	Q_OBJECT
public:
	NAMEHERETaskScheduler(); 
	virtual ~NAMEHERETaskScheduler();

public Q_SLOTS:
	void setVisible(bool visible);

private Q_SLOTS:
	void updateValues();

private:
	typedef std::vector<boost::shared_ptr<const RBX::TaskScheduler::Job> > Jobs;
	typedef std::map<boost::shared_ptr<RBX::TaskScheduler::Arbiter>, ArbiterItem*> ArbiterItemsMap;
	typedef std::set<TSJobItem*> TSJobItemCollection;

	ArbiterItem* getOrCreateArbiterItem(boost::shared_ptr<RBX::TaskScheduler::Arbiter> pArbiter);
	void syncTSJobItems(const TSJobItemCollection &currentTaskSchedulerItems);

	ArbiterItemsMap				 m_arbiterMap;
	TSJobItemCollection          m_TSJobItems;

	QTimer						*m_pTimer;
};


