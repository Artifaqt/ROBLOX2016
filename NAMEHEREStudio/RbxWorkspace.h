/**
 * RbxWorkspace.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QObject>

// boost headers
#include <boost/enable_shared_from_this.hpp>

// NAMEHERE Headers
#include "v8xml/XmlElement.h"
#include "v8tree/Instance.h"

// If you comment the ENABLE_EDIT_PLACE_IN_STUDIO then the Play Button will start working
// The reason is if the Webpage has a StartGame implementation then the NPNAMEHERE Plugin does not get loaded & that prevents the Play button to invoke the Player
#define ENABLE_EDIT_PLACE_IN_STUDIO
#define ENABLE_DRAG_DROP_TOOLBOX

namespace RBX
{
	class DataModel;
	class Workspace;
}

class RbxContent;
class QMessageBox;
class QAbstractButton;

// This class is used as the JS Hookup from the WebView
class RbxWorkspace
	: public QObject
	, public boost::enable_shared_from_this<RbxWorkspace>
{
Q_OBJECT
// Edit Buton Show up only if we return true for the IsNAMEHEREAppIDE property
Q_PROPERTY(bool IsNAMEHEREAppIDE READ getIsNAMEHEREAppIDE) // MFC Studio && QT Studio (All Studio)
Q_PROPERTY(bool IsNAMEHERE2App READ getIsNAMEHERE2App) // QT Studio only
Q_PROPERTY(bool IsNAMEHEREABApp READ getIsNAMEHEREABApp) // Admin Build -- Play button will call window.external.StartGame to play inside of Studio as opposed to launching the NAMEHERE Player
  
public:
	RbxWorkspace(QObject *parent, RBX::DataModel* dm);
	~RbxWorkspace();
	bool getIsNAMEHEREAppIDE() const 	{ return true; }
	bool getIsNAMEHERE2App() const 	{ return true; }
	bool getIsNAMEHEREABApp() const 	
	{ 
#ifdef STUDIO_ADMIN_BUILD
		return true; 
#else
		return false;
#endif
	}

private:
	QString m_selectorForFileUpload;
	QMessageBox* m_screenShotInProgressMsgBox;
	RBX::DataModel *m_pDataModel;
	RbxContent *m_pContent;
	QObject *m_pParent;
	
	std::auto_ptr<XmlElement> writeSelectionToXml();
	void writeToStream(const XmlElement* root, std::ostream& stream);
	RBX::Workspace* getWorkspace() const;
	
	void insert(RBX::ContentId contentId, bool insertInto);
	void insert(std::istream& stream, bool insertInto);
	void insert(RBX::Instances& instances, bool insertInto);
	void loadContent(RBX::ContentId contentId, RBX::Instances& instances, bool& hasError);
	
	void Close() { Q_EMIT closeEvent(); }
	void Hide() { Q_EMIT hideEvent(); }
	void Show() { Q_EMIT showEvent(); }

	static void pluginInstallCompleteHelper(weak_ptr<RbxWorkspace> weakWorkspace, bool succeeded, int assetId);

public:
	static bool isScriptAssetUploadEnabled;
	static bool isAnimationAssetUploadEnabled;
	static bool isImageModelAssetUploadEnabled;
	void onScreenshotFinished(const std::string &fileName);
	Q_INVOKABLE void onScreenShotFinished_MT(QString fileName);

    void setDataModel(RBX::DataModel *pDataModel) { m_pDataModel = pDataModel; }
    
Q_SIGNALS:
	void thumbnailTaken(int width, int height);
	void closeEvent();
	void hideEvent();
	void showEvent();
	void PluginInstallComplete(bool succeeded, int assetId);

public Q_SLOTS:
	// Publish To NAMEHERE JS Callback
    bool SaveUrl( const QString &saveUrl );
    bool Save();

	void handleTakeThumbnail(QAbstractButton*);

#ifdef ENABLE_EDIT_PLACE_IN_STUDIO
	// Build & Edit Button Hookup for JS Callback to open the Place in Studio
	bool StartGame(const QString &ticket, const QString &url, const QString &script);
#endif
	
	// Opens a recent file -- called from the StartPage -- file name is looked up in recent files for safety (prevent arbitrary file opening)
	bool OpenRecentFile(const QString &recentFileName);

	// Publish Selection As Model to NAMEHERE JS Callback, it returns another JS Callboack object RbxContent which does the actual publishing
	QObject* WriteSelection();
	
	// HTML Toolbox related JS Callback
	bool Insert(const QString &url);

	QString GetStudioSessionID();
	
	bool TakeThumbnail(QString selector);

#ifdef ENABLE_DRAG_DROP_TOOLBOX
	bool StartDrag(const QString& url);
#endif

	// Opens picture folder
	bool OpenPicFolder();
	
	// Posts given image to NAMEHERE site
	void PostImage(bool doPost, int postSetting, const QString &fileName, const QString &seoStr);

	void InstallPlugin(int assetId, int assetVersion);
	QString GetInstalledPlugins();
	void SetPluginEnabled(int assetId, bool enabled);
	void UninstallPlugin(int assetId);

#ifdef Q_WS_WIN32 // must be Q_WS_WIN32 so moc picks it up
	bool OpenVideoFolder();
	void UploadVideo(const QString &token, bool doPost, int postSetting, const QString &title);
#endif

    void OpenUniverse(int universeId);
	void PublishAsNewUniverse();
	void PublishAsNewGroupUniverse(int groupId);
	void PublishToUniverse(int universeId);

	void ImportAsset(int assetId);
	QString GetGameAnimations(int universeId);
};
