/**
 * NAMEHEREScriptReview.cpp
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "NAMEHEREScriptReview.h"

// Qt Headers
#include <QApplication>
#include <QTimer>
#include <QHeaderView>

// NAMEHERE Headers
#include "script/ScriptContext.h"
#include "v8datamodel/DataModel.h"
#include "rbx/TaskScheduler.h"
#include "rbx/TaskScheduler.Job.h"
#include "rbx/CEvent.h"

// NAMEHERE Studio Headers
#include "NAMEHERECustomWidgets.h"

static const int UpdateInterval = 1000 / 5;

class NAMEHEREScriptReview::ScriptPerfUpdateJob : public RBX::DataModelJob
{
public:
	NAMEHEREScriptReview* m_pScriptReview;
	RBX::CEvent update;		// Set when the UI thread may update
	RBX::CEvent updated;	// Set when the UI thread has updated
	volatile bool isGridInvalid;
	typedef enum { SentUpdateMessage, Updating, Waiting } State;
	volatile State m_state;
	double waitingPhaseStep;
	double timeSinceLastRender;

	ScriptPerfUpdateJob(NAMEHEREScriptReview* scriptReview, boost::shared_ptr<RBX::DataModel> dataModel)
		:RBX::DataModelJob("NAMEHEREScriptReview", RBX::DataModelJob::Read, false, dataModel, RBX::Time::Interval(0)),
		m_pScriptReview(scriptReview),
		update(false),
		updated(false),
		isGridInvalid(true),
		m_state(Waiting)
	{}
	
	void requestUpdate()
	{
		if (!isGridInvalid)
		{
			isGridInvalid = true;
			RBX::TaskScheduler::singleton().reschedule(shared_from_this());
		}
	}

	void setState(ScriptPerfUpdateJob::State state)
	{ m_state = state; }

	virtual RBX::Time::Interval sleepTime(const Stats& stats)
	{
		switch (m_state)
		{
		default:
		case Waiting:
			{
				if (isGridInvalid)
					return RBX::Time::Interval::zero();
				else
					return RBX::Time::Interval::max();
			}

		case Updating:
			return RBX::Time::Interval::zero();

		case SentUpdateMessage:
			return RBX::Time::Interval::max();
		}
	}

	virtual Job::Error error(const Stats& stats)
	{
		switch (m_state)
		{
		default:
		case Waiting:
			{
				RBXASSERT(isGridInvalid);
				waitingPhaseStep = stats.timespanSinceLastStep.seconds();
				Job::Error result;
				result.error = waitingPhaseStep;
				return result;
			}

		case Updating:
			{
				Job::Error result;
			    // The error is the sum of the previous phase plus this phase
				result.error = waitingPhaseStep + stats.timespanSinceLastStep.seconds();	
				result.urgent = true;
				return result;
			}

		case SentUpdateMessage:
			RBXASSERT(false);
			return Job::Error();
		}
	}

	virtual RBX::TaskScheduler::StepResult stepDataModelJob(const Stats& stats)
	{
		timeSinceLastRender = waitingPhaseStep + stats.timespanOfLastStep.seconds();

		switch (m_state)
		{
		case Waiting:
			RBXASSERT(isGridInvalid);
			m_state = SentUpdateMessage;
			QApplication::postEvent(m_pScriptReview, new NAMEHERECustomEvent(SCRIPT_REVIEW_UPDATE));
			break;

		case SentUpdateMessage:
			RBXASSERT(false);
			break;

		case Updating:
			update.Set();
			updated.Wait();
			m_state = Waiting;
			break;

		}

		return RBX::TaskScheduler::Stepped;
	}
};

NAMEHEREScriptReview::NAMEHEREScriptReview()
: m_pTimer(new QTimer(this))
{
	setColumnCount(4);
	QStringList headerLabels;
	headerLabels << "Name" << "Count" << "Activity" << "Rate";
	setHeaderLabels(headerLabels);
	setIndentation(0);

	header()->setDefaultSectionSize(50);
	header()->resizeSection(0, 100);
	header()->setClickable(true);

    m_pTimer->setInterval(UpdateInterval);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
	connect(header(), SIGNAL(sectionClicked(int)), this, SLOT(onSectionClicked(int)));
}

NAMEHEREScriptReview::~NAMEHEREScriptReview()
{}

void NAMEHEREScriptReview::setDataModel(boost::shared_ptr<RBX::DataModel> pDataModel)
{
	if(m_pDataModel == pDataModel)
		return;

	if (m_pDataModel)
	{
		RBX::DataModel::LegacyLock lock(m_pDataModel, RBX::DataModelJob::Write);
		deleteUpdateItemsJob();
		clear();
	}

	m_records.clear();
	m_pDataModel = pDataModel;

	if (m_pDataModel)
	{
		RBX::DataModel::LegacyLock lock(m_pDataModel, RBX::DataModelJob::Write);
		createUpdateItemsJob(m_pDataModel);
		m_pTimer->start();
	}
	else 
		m_pTimer->stop();

	requestUpdate();
}

void NAMEHEREScriptReview::AddValue(const RBX::Reflection::Variant& value)
{
	//DE2502 - Do not show objects with name "Unknown" and count -1
	shared_ptr<const RBX::Reflection::Tuple> tuple = value.cast<shared_ptr<const RBX::Reflection::Tuple> >();
	if (tuple->values[1].cast<std::string>() == "[Unknown]" && tuple->values[2].cast<int>() == -1)
		return;
	
	NAMEHERECategoryItem* pCategoryItem = findCategoryItem("Data Model: Data Model");
	if(!pCategoryItem)
	{
		pCategoryItem = new NAMEHERECategoryItem();
		pCategoryItem->setText(0, "Data Model: Data Model");
		addCategoryItem(pCategoryItem);		
	}
	expandItem(pCategoryItem);

	std::string hash = tuple->values[0].cast<std::string>();
	RecordMap::iterator iter = m_records.find(hash);
	if(iter != m_records.end()){
		QTreeWidgetItem* pRecord = iter->second;
		pRecord->setText(1, RBX::format("%d",	tuple->values[2].cast<int>()).c_str());
		pRecord->setText(2, RBX::format("%.3f%%",tuple->values[3].cast<double>()).c_str());
		pRecord->setText(3, RBX::format("%.1f/s",	tuple->values[4].cast<double>()).c_str());
	}
	else{
		QTreeWidgetItem* pRecord = new QTreeWidgetItem();
		pRecord->setText(0, tuple->values[1].cast<std::string>().c_str());
		pRecord->setText(1, RBX::format("%d", tuple->values[2].cast<int>()).c_str());
		pRecord->setText(2, RBX::format("%.3f%%", tuple->values[3].cast<double>()).c_str());
		pRecord->setText(3, RBX::format("%.1f/s", tuple->values[4].cast<double>()).c_str());
	
		pCategoryItem->addChild(pRecord);
		m_records[hash] = pRecord;
	}
}

void NAMEHEREScriptReview::update()
{
	if (m_pUpdateItemsJob)
	{
		if (!m_pUpdateItemsJob->isGridInvalid)
			return;

		m_pUpdateItemsJob->setState(ScriptPerfUpdateJob::Updating);
		RBX::TaskScheduler::singleton().reschedule(m_pUpdateItemsJob);
		m_pUpdateItemsJob->update.Wait();		
	}

	setUpdatesEnabled(false);

	boost::shared_ptr<RBX::ScriptContext> pScriptContext = RBX::shared_from(RBX::ServiceProvider::create<RBX::ScriptContext>(m_pDataModel.get()));
	if(pScriptContext)
	{		
		pScriptContext->setCollectScriptStats(true);
		if(shared_ptr<const RBX::Reflection::Tuple> stats = pScriptContext->getScriptStats())
		{
			RBX::Reflection::ValueArray::const_iterator curIter = stats->values.begin();
			while (curIter != stats->values.end())
			{
				AddValue(*curIter);
				++curIter;
			}
		}
	}

	setUpdatesEnabled(true);	

	if (m_pUpdateItemsJob)
	{
		m_pUpdateItemsJob->isGridInvalid = false;
		m_pUpdateItemsJob->updated.Set();	
	}
}

void NAMEHEREScriptReview::requestUpdate()
{
	if (m_pUpdateItemsJob)
		m_pUpdateItemsJob->requestUpdate();
	else
		QApplication::postEvent(this, new NAMEHERECustomEvent(SCRIPT_REVIEW_UPDATE));
}

bool NAMEHEREScriptReview::event(QEvent * evt)
{
	if (evt->type() != SCRIPT_REVIEW_UPDATE)
		return QTreeWidget::event(evt);

    if ( isEnabled() )
	{
	    update();
	}
	else
	{
		// if widget is not enabled, make sure we again post the event so it can be evaulated whenever the widget is enabled
		QApplication::postEvent(this, new NAMEHERECustomEvent(SCRIPT_REVIEW_UPDATE));
	}

	return true;
}

void NAMEHEREScriptReview::createUpdateItemsJob(boost::shared_ptr<RBX::DataModel> dataModel)
{
	RBXASSERT(!m_pUpdateItemsJob);
	m_pUpdateItemsJob.reset(new ScriptPerfUpdateJob(this, dataModel));
	RBX::TaskScheduler::singleton().add(m_pUpdateItemsJob);
}

void NAMEHEREScriptReview::deleteUpdateItemsJob()
{
	if (m_pUpdateItemsJob)
	{
		m_pUpdateItemsJob->updated.Set();
		RBX::TaskScheduler::singleton().removeBlocking(m_pUpdateItemsJob);
		m_pUpdateItemsJob.reset();
	}
}

void NAMEHEREScriptReview::setVisible(bool visible)
{
	if(visible && m_pDataModel)
	{
		m_pTimer->start();
	}
	else
	{
		m_pTimer->stop();
	}
	NAMEHEREReportView::setVisible(visible);
}

void NAMEHEREScriptReview::onTimer()
{
    // if the window is hidden, stop the update timer
	if ( !isVisible() )
	{
		m_pTimer->stop();
		return;
	}

	requestUpdate();
}

void NAMEHEREScriptReview::onSectionClicked(int index)
{
	if(!isSortingEnabled())
		setSortingEnabled(true);
}


