/**
 * NAMEHEREScriptReview.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

// Standard C/C++ Headers
#include <map>

// 3rd Party Headers
#include "boost/shared_ptr.hpp"

// NAMEHERE Studio Headers
#include "NAMEHEREReportView.h"

namespace RBX 
{
	class DataModel;
	namespace Reflection
	{
		class Variant;
	}
}

class QTreeWidgetItem;

typedef std::map<std::string, QTreeWidgetItem*>	RecordMap;

class NAMEHEREScriptReview : public NAMEHEREReportView
{
	Q_OBJECT

public:

	NAMEHEREScriptReview(); 
	virtual ~NAMEHEREScriptReview();

	class ScriptPerfUpdateJob;

	void setDataModel(boost::shared_ptr<RBX::DataModel> pDataModel);
	boost::shared_ptr<RBX::DataModel> dataModel();	

	virtual void requestUpdate();

public Q_SLOTS:

	void setVisible(bool visible);

protected:

	void AddValue(const RBX::Reflection::Variant& value);
	void createUpdateItemsJob(boost::shared_ptr<RBX::DataModel> dataModel);
	void deleteUpdateItemsJob();

private Q_SLOTS:

	void update();
	void onTimer();
	void onSectionClicked(int index);
	
private:

	virtual bool event(QEvent * evt);

	RecordMap                               m_records;

	boost::shared_ptr<RBX::DataModel>	    m_pDataModel;
	boost::shared_ptr<ScriptPerfUpdateJob>  m_pUpdateItemsJob;
	
	QTimer*                                 m_pTimer;
};


