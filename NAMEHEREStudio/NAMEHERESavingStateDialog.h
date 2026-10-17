/**
 * NAMEHERESavingStateDialog.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QDialog>
#include <QSettings>

template <typename T>
class NAMEHERESavingStateDialog : public T
{
public:
	NAMEHERESavingStateDialog(QWidget* parent, const QString geometrySettingName)
		: T(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
		, m_geometrySettingName(geometrySettingName)
	{
		// restore settings
		QSettings settings;
        QDialog::restoreGeometry(settings.value(m_geometrySettingName).toByteArray());
	}

protected:
	QString m_geometrySettingName;
	virtual ~NAMEHERESavingStateDialog()
	{
		QSettings settings;
		settings.setValue(m_geometrySettingName, QDialog::saveGeometry());
	}

};