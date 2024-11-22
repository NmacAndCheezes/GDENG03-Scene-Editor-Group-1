#pragma once
class EditorBackend
{
public:
	enum EditorState { PLAY, PAUSE, EDIT };

	void setState(EditorState newState);
	EditorState getState();

	//Initialize the GraphicsEngine and DirectX 11 Device
	static void initialize();
	static void destroy();
	static EditorBackend* get();

private:
	EditorBackend() = default;
	static EditorBackend* sharedInstance;
	EditorBackend(EditorBackend const&) {}
	EditorBackend& operator=(EditorBackend const&) {};
	~EditorBackend() = default;

private:
	EditorState currentState = EditorState::EDIT;
};

