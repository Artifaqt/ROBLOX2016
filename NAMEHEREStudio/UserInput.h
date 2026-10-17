/**
 * UserInput.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

// NAMEHERE Headers
#include "util/UserInputBase.h"
#include "util/RunStateOwner.h"
#include "util/Rect.h"
#include "V8DataModel/InputObject.h"
#include "SDLGameController.h"

class NAMEHEREView;

namespace RBX {
	class DataModel;
}

class NAMEHERECriticalSection
{
private:
public:
	int callers;
	std::string recentCaller;
	std::string olderCaller;
	NAMEHERECriticalSection() 
		: callers(0)
	{}
};

class NAMEHERECritSecLoc
{
private:
	NAMEHERECriticalSection& namehereCriticalSection;

public:
	NAMEHERECritSecLoc( NAMEHERECriticalSection& cs, const std::string& location) 
		: namehereCriticalSection(cs)
	{
/*
	#ifdef __RBX_NOT_RELEASE			// should compile away the string manipulations
		RBXASSERT(namehereCriticalSection.callers == 0);					// Show THESE ASSERTS TO David/Erik - they imply multiple entry points to User Input Object - trying to track down
		namehereCriticalSection.callers++;
		namehereCriticalSection.olderCaller = namehereCriticalSection.recentCaller;
		namehereCriticalSection.recentCaller = location;
	#endif
*/
	}
	
	~NAMEHERECritSecLoc()
	{
/*
	#ifdef __RBX_NOT_RELEASE			// should compile away the string manipulations
		RBXASSERT(namehereCriticalSection.callers == 1);
		namehereCriticalSection.callers--;
		namehereCriticalSection.recentCaller = "";
		namehereCriticalSection.olderCaller = "";
	#endif
*/
	}
};


class UserInput 
	: public RBX::UserInputBase
{
private:
	int debugI;

	mutable NAMEHERECriticalSection diSection;

	rbx::signals::scoped_connection processedMouseConnection;

	size_t tableSize;

	// Mouse Stuff
	bool isMouseCaptured;			// poor man's tracker of button state
	bool leftMouseButtonDown;
	bool middleMouseButtonDown;
    bool rightMouseButtonDown;
	G3D::Vector2 wrapMousePosition;		// in normalized coordinates (center is 0,0. radius is getWrapRadius)
	bool wrapping;
	bool leftMouseDown;
	bool rightMouseDown;
	bool autoMouseMove;

	// Keyboard Stuff
	int externallyForcedKeyDown;		// + this from exteranal source (like a gui button)

	class NAMEHEREView * wnd;
	shared_ptr<RBX::RunService> runService;

	// InputObject stuff
	boost::unordered_map<RBX::InputObject::UserInputType,shared_ptr<RBX::InputObject> > inputObjectMap;

	shared_ptr<SDLGameController> sdlGameController;


	////////////////////////////////
	//
	// Events


	void sendEvent(shared_ptr<RBX::InputObject> event, bool processed = false);
	void sendMouseEvent(RBX::InputObject::UserInputType mouseEventType,
		RBX::InputObject::UserInputState mouseEventState,
		RBX::Vector3& position, RBX::Vector3& delta);

	////////////////////////////////////
	// 
	// Keyboard Mouse
	bool isMouseInside;
	
	G3D::Vector2 posToWrapTo;

	// Studio mobile emulation
	shared_ptr<RBX::InputObject> touchInput;

	// window stuff
	RBX::Vector2int16 getWindowSize() const;
	G3D::Rect2D getWindowRect() const;			// { return windowRect; }
	bool isFullScreenMode() const;
	bool movementKeysDown();
	bool keyDownInternal(RBX::KeyCode code) const;

	void doWrapMouse(const G3D::Vector2& delta, G3D::Vector2& wrapMouseDelta);
	
	G3D::Vector2 getGameCursorPositionInternal();
	G3D::Vector2 getGameCursorPositionExpandedInternal();	// prevent hysteresis
	G3D::Vector2 getWindowsCursorPositionInternal();
	RBX::Vector2 getCursorPositionInternal();
		
	void doDiagnostics();

	void postProcessUserInput(bool cursorMoved, bool leftMouseUp, RBX::Vector2 wrapMouseDelta, RBX::Vector2 mouseDelta);
	void mouseEventWasProcessed(bool datamodelSunkEvent, void* nativeEventObject, const shared_ptr<RBX::InputObject>& inputObject);

	void renderStepped();

public:
	shared_ptr<RBX::DataModel> dataModel;

//	const static int WM_CALL_SETFOCUS = WM_USER + 187;
//	HCURSOR hInvisibleCursor;

	////////////////////////////////
	//
	// UserInputBase

	/*implement*/ RBX::Vector2 getCursorPosition();
	
	// todo: no longer used, remove this
	void doWrapHybrid(bool cursorMoved, bool leftMouseUp, G3D::Vector2& wrapMouseDelta, G3D::Vector2& wrapMousePosition, G3D::Vector2& posToWrapTo);

	/*implement*/ bool keyDown(RBX::KeyCode code) const;
    /*implement*/ void setKeyState(RBX::KeyCode code, RBX::ModCode modCode, char modifiedKey, bool isDown);
    bool isMiddleMouseDown() const { return middleMouseButtonDown; }
    bool isLeftMouseDown() const { return leftMouseButtonDown; }

    bool areEditModeMovementKeysDown();

	/*implement*/ void centerCursor() {wrapMousePosition = RBX::Vector2::zero();}

	/*override*/ RBX::TextureProxyBaseRef getGameCursor(RBX::Adorn* adorn);

	void PostUserInputMessage(RBX::InputObject::UserInputType eventType, RBX::InputObject::UserInputState eventState, int wParam, int lParam, char extraParam = 0, bool processed = false);
	// Call this only within a DataModel lock:
	void ProcessUserInputMessage(RBX::InputObject::UserInputType eventType, RBX::InputObject::UserInputState eventState, int wParam, int lParam, char extraParam = 0, bool processed = false);

	UserInput(NAMEHEREView *wnd, shared_ptr<RBX::DataModel> dataModel);
	~UserInput();

	void setDataModel(shared_ptr<RBX::DataModel> dataModel);

	void onMouseInside();
	void onMouseLeave();

	void resetKeyState();
};
