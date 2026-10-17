/**
 * NAMEHEREApplicationManager.cpp
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "NAMEHEREApplicationManager.h"

// Qt Headers
#include <QStringList>
#include <QCoreApplication>
#include <QProcess>
#include <QLocalSocket>
#include <QLocalServer>

// NAMEHERE Headers
#include "NAMEHERECookieJar.h"
#include "NAMEHEREMainWindow.h"
#include "NAMEHERENetworkAccessManager.h"
#include "SharedLauncher.h"
#include "UpdateUIManager.h"
#include "QtUtilities.h"
#include "StudioDeviceEmulator.h"
#include "StudioUtilities.h"

const char* kChildProcessServerName   = "rbxDeleteChildProcessesServer_%1";
const char* kDeleteChildProcessesMsg  = "rbxDeleteChildProcesses";
const char* kChildProcessesDeletedMsg = "rbxChildProcessesDeleted";

NAMEHEREApplicationManager::NAMEHEREApplicationManager()
: childProcessHandler(NULL)
{
	sharedMemory.setKey("QT_NAMEHERE_STUDIO");
}

NAMEHEREApplicationManager& NAMEHEREApplicationManager::instance()
{
	static NAMEHEREApplicationManager instance;
	return instance;
}

bool NAMEHEREApplicationManager::createNewStudioInstance(
    const QString&  script, 
    const QString&  fileLocation,
    bool            isTestMode,
    bool            isAvatarMode,
	NewInstanceMode mode)
{
	// Save the current layout before launching
	UpdateUIManager::Instance().getMainWindow().saveApplicationStates();

	// Persist our cookies (to make sure we have the most recent ones in the new app)
	NAMEHERECookieJar* cookieJar = dynamic_cast<NAMEHERECookieJar*>(NAMEHERENetworkAccessManager::Instance().cookieJar());
	if (cookieJar)
		cookieJar->saveCookiesToDisk();

	QStringList	args;
    args << SharedLauncher::IDEArgument;
	composeArgList(args, script, fileLocation, isTestMode, isAvatarMode);

	QString namehereApp = QCoreApplication::applicationFilePath();

	if (mode == NewInstanceMode_Detached)
		return QProcess::startDetached(namehereApp, args);

	if (!childProcessHandler)
		childProcessHandler = new ChildProcessHandler(&UpdateUIManager::Instance().getMainWindow());
	return childProcessHandler->startChildProcess(namehereApp, args, mode);
}

int NAMEHEREApplicationManager::getApplicationCount()
{
	SharedData data;
	if (!sharedMemory.lock())
        return 0;
	char* sharedData = (char*)sharedMemory.data();

	if (!sharedData) // Odd scenario, should this ever happen?
	{
		sharedMemory.unlock();
		return 0;
	}
	memcpy(&data, sharedData, sizeof(SharedData));
	
	sharedMemory.unlock();
	
	return data.numStudioApplications;
}

void NAMEHEREApplicationManager::registerApplication()
{
	sharedMemory.attach();
	sharedMemory.create(sizeof(SharedData)); // Create it if it doesn't exist
	modifyApplicationCount(1); // Increment our count
}

void NAMEHEREApplicationManager::unregisterApplication()
{
	modifyApplicationCount(-1); // Decrement our count
	sharedMemory.detach();
}

void NAMEHEREApplicationManager::modifyApplicationCount(int delta)
{
	if (!sharedMemory.lock())
        return;

	SharedData data;
	
	char* sharedData = (char*)sharedMemory.data();

	if (sharedData)
	{
		memcpy(&data, sharedData, sizeof(SharedData));
	}
	else
	{ 
		// Should never be here?
		data.numStudioApplications = 0;
	}

	data.numStudioApplications += delta;
	
	memcpy(sharedData, &data, sizeof(SharedData));

	sharedMemory.unlock();

}

void NAMEHEREApplicationManager::composeArgList(
    QStringList&    argsContainer, 
    const QString&  script, 
    const QString&  fileLocation,
    bool            isTestMode,
    bool            isAvatarMode )
{
	if ( !script.isEmpty() )
		argsContainer << SharedLauncher::ScriptArgument << script;
	if ( !fileLocation.isEmpty() )
		argsContainer << SharedLauncher::FileLocationArgument << fileLocation;
    if ( isTestMode )
        argsContainer << SharedLauncher::TestModeArgument;
	if ( isAvatarMode )
        argsContainer << SharedLauncher::AvatarModeArgument;

	if (StudioDeviceEmulator::Instance().isEmulatingTouch())
		argsContainer << StudioUtilities::EmulateTouchArgument;

	int studioWidth =  StudioDeviceEmulator::Instance().getCurrentDeviceWidth();
	int studioHeight = StudioDeviceEmulator::Instance().getCurrentDeviceHeight();

	if (studioWidth)
		argsContainer << StudioUtilities::StudioWidthArgument << QString("%1").arg(studioWidth);

	if (studioHeight)
		argsContainer << StudioUtilities::StudioHeightArgument << QString("%1").arg(studioHeight);
}

bool NAMEHEREApplicationManager::hasChildProcesses()
{
	if (childProcessHandler)
		return childProcessHandler->hasChildProcesses();
	return false;
}

void NAMEHEREApplicationManager::cleanupChildProcesses(int timeout)
{
	if (childProcessHandler)
		childProcessHandler->cleanupChildProcesses(timeout);
}

void NAMEHEREApplicationManager::startLocalServer()
{
	if (!childProcessHandler)
		childProcessHandler = new ChildProcessHandler(&UpdateUIManager::Instance().getMainWindow());
	childProcessHandler->startLocalServer();
}

bool NAMEHEREApplicationManager::hasLocalServer()
{
	if (childProcessHandler)
		return childProcessHandler->hasLocalServer();
	return false;
}

ChildProcessHandler::ChildProcessHandler(QObject* parent)
: QObject(parent)
, m_pLocalServer(NULL)
{
}

bool ChildProcessHandler::startChildProcess(const QString& namehereApp, const QStringList &args, NewInstanceMode mode)
{
	QProcess *newProcess = new QProcess();
	newProcess->start(namehereApp, args);

	if (!newProcess->waitForStarted())
		return false;

	if (mode == NewInstanceMode_Detached)
		return true;

	// child process can be managed if it's Server or Player
	(mode == NewInstanceMode_Server) ? serverProcesses.append(newProcess) : playerProcesses.append(newProcess);
	connect(newProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onProcessFinished()));
	return true;
}

void ChildProcessHandler::startLocalServer()
{
	// if we already have a local server, do not do anything
	if (m_pLocalServer)
		return;

	m_pLocalServer = new QLocalServer(this);
	connect(m_pLocalServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

	int processId = 0;
#ifdef Q_WS_WIN32
	processId = ::GetCurrentProcessId();
#else
	processId = getpid();
#endif

	// set name for the local server (attach process id to make it unique)
	m_pLocalServer->listen(QString(kChildProcessServerName).arg(processId));
}

bool ChildProcessHandler::hasLocalServer()
{  return m_pLocalServer != NULL; }

void ChildProcessHandler::cleanupChildProcesses(int timeout)
{
	// first cleanup players launched from this instance of studio
	QProcess* childProcess = NULL;
	for (int ii = 0; ii < playerProcesses.size(); ++ii) 
	{
		childProcess = playerProcesses.at(ii);
		if (childProcess && childProcess->state() != QProcess::NotRunning)
		{
			childProcess->kill();
		}
	}
	playerProcesses.clear();

	// now time to remove server processes
	for (int ii = 0; ii < serverProcesses.size(); ++ii) 
	{
		childProcess = serverProcesses.at(ii);
		if (childProcess && childProcess->state() != QProcess::NotRunning)
		{			
			int processId = QtUtilities::toInt(childProcess->pid());
			if (!processId)
				continue;

			// since server launches player (in Ribbon mode), we need to make sure those launched players also gets cleaned up
			QLocalSocket localSocket;
			localSocket.connectToServer(QString(kChildProcessServerName).arg(processId));
			if (localSocket.waitForConnected(timeout))
			{
				QString message(kDeleteChildProcessesMsg);
				localSocket.write(message.toUtf8());
				localSocket.waitForBytesWritten(timeout);

				localSocket.waitForReadyRead(timeout);
			}

			// now we are done with players cleanup, all set to kill the server
			if (childProcess && childProcess->state() != QProcess::NotRunning)
				childProcess->kill();
		}
	}
	serverProcesses.clear();
}

bool ChildProcessHandler::hasChildProcesses()
{ return !serverProcesses.isEmpty() || !playerProcesses.isEmpty(); }

void ChildProcessHandler::onProcessFinished()
{
	QProcess *pProcess = qobject_cast<QProcess *>(sender());
	if (!pProcess)
		return;

	// check if we can remove process from list
	bool removed = serverProcesses.removeOne(pProcess);
	if (!removed)
		removed = playerProcesses.removeOne(pProcess);

	// if we do not have any child processes pending update toolbars
	if (removed && serverProcesses.isEmpty() && playerProcesses.isEmpty())
		UpdateUIManager::Instance().updateToolBars();
}

void ChildProcessHandler::onNewConnection()
{
	if (!m_pLocalServer)
		return;

	QLocalSocket *localSocket = m_pLocalServer->nextPendingConnection();
	if (localSocket && localSocket->waitForReadyRead(3000))
	{
		QString receivedMessage = QString::fromUtf8(localSocket->readAll().constData());
		if (!receivedMessage.isEmpty() && (receivedMessage == kDeleteChildProcessesMsg))
		{
			// kill all child process
			cleanupChildProcesses();
			// update socket about the completion
			localSocket->write(kChildProcessesDeletedMsg);
		}
	}
}
