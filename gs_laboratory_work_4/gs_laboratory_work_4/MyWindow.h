//#pragma once
//#include <iostream>
//#include <SDL.h>
//#include <SDL_opengl.h>
//#include <Windows.h>
//#include <thread>
//#include <gl\GL.h>
//#include <gl\GLU.h>
//
//#include "SmartBytes.h"
//
//extern SmartBytes sm;
//
//class MyWindow
//{
//public:
//	MyWindow();
//	MyWindow(const char* title, int x, int y, int w, int h, Uint32 flags = 0, Uint32 fps = 60);
//	~MyWindow();
//
//	SDL_Window* GetWindow();
//
//	int Init(const char* title, int x, int y, int w, int h, Uint32 flags = 0, Uint32 fps = 60);
//	int Create(const char* title = nullptr, int x = 0, int y = 0, int w = 0, int h = 0, Uint32 flags = 0, Uint32 fps = 60);
//
//	int StartLoop();
//	//int StartLoop(const char* title, int x, int y, int w, int h, Uint32 flags);
//	int Run();
//	int Run(const char* title, int x, int y, int w, int h, Uint32 flags = 0, Uint32 fps = 60);
//	//int Join();
//	int Join(const char* title = nullptr, int x = 0, int y = 0, int w = 0, int h = 0, Uint32 flags = 0, Uint32 fps = 60);
//private:
//	SDL_Window* window;
//	SDL_Renderer* renderer;
//	SDL_Event* window_event;
//	SDL_GLContext gl_context;
//
//	std::thread* event_getter;
//
//	const char* title;
//	Uint32 flags;
//	int w;
//	int h;
//	int x;
//	int y;
//	bool quit;
//	Uint32 fps;
//	//	SDL_GLContext gl_context;
//};
//
