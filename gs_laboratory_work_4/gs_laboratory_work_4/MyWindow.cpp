//#include "MyWindow.h"
////#include "stdafx.h"//#include "MyWindow.h"
//
//
//
//MyWindow::MyWindow()
//{
//	title = nullptr;
//	x = 0;
//	y = 0;
//	w = 0;
//	h = 0;
//	flags = 0;
//	fps = 60;
//	quit = 0;
//
//	window = nullptr;
//	renderer = nullptr;
//	window_event = nullptr;
//}
//
//MyWindow::MyWindow(const char* title, int x, int y, int w, int h, Uint32 flags, Uint32 fps)
//{
//	Init(title, x, y, w, h, flags, fps);
//}
//
//
//MyWindow::~MyWindow()
//{
//	if (event_getter->joinable())
//	{
//		event_getter->join();
//	}
//	delete window_event;
//	SDL_GL_DeleteContext(gl_context);
//	SDL_DestroyRenderer(renderer);
//	SDL_DestroyWindow(window);
//}
//
//SDL_Window* MyWindow::GetWindow()
//{
//	return window;
//}
//
//int MyWindow::Init(const char* title, int x, int y, int w, int h, Uint32 flags, Uint32 fps)
//{
//	this->title = title;
//	this->x = x;
//	this->y = y;
//	this->w = w;
//	this->h = h;
//	this->flags = flags;
//	this->fps = fps;
//	quit = 0;
//
//	return 0;
//}
//
//int MyWindow::Create(const char* title, int x, int y, int w, int h, Uint32 flags, Uint32 fps)
//{
//	if (title != nullptr)
//		Init(title, x, y, w, h, flags, fps);
//
//	window = SDL_CreateWindow(this->title, this->x, this->y, this->w, this->h, this->flags | SDL_WINDOW_HIDDEN);
//
//	if (window == nullptr)
//	{
//		std::cout << "Window " << title << " couldn't be created" << std::endl;
//		return 0;
//	}
//
//	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//	SDL_RenderClear(renderer);
//
//	window_event = new SDL_Event;
//
//	return 0;
//}
//
//int MyWindow::StartLoop()
//{
//	SDL_ShowWindow(window);
//
//	Uint32 frame_start_time = 0;
//	int wi = sm.getWidth();
//	int he = sm.getHeight();
//	bool is_updated = false;
//	while (!quit)
//	{
//		SDL_PollEvent(window_event);
//		int i;
//		/*int step = 4 * (wi / w);
//		if (step < 4)
//		step = 4;*/
//		if (!is_updated)
//		{
//			for (i = 0; i < sm.getSize(); i += 4)
//			{
//				float b = (unsigned char)(*(ch + i)) / 255.0;
//				float g = (unsigned char)(*(ch + i + 1)) / 255.0;
//				float r = (unsigned char)(*(ch + i + 2)) / 255.0;
//				/*bool is_black = true;
//				if (is_black)
//				{
//				b = (int)b;
//				g = (int)g;
//				r = (int)r;
//
//				if (b || g || r)
//				b = g = r = 1;
//				}*/
//
//				{
//					float black = (b + g + r) / 3.0;
//					/*if (black < 0.6)
//					black += 0.3;*/
//					b = g = r = black/* - 1*/;
//				}
//				/*if (i < 1000)
//				std::cout << (int)((unsigned char)*(ch+i+3)) << std::endl;*/
//				//glColor3b(r,g,b);
//				glColor3f(r, g, b);
//
//				int yy = /*1080 -*/ (i / 4) / wi;
//				int xx = (i / 4) % wi;
//
//				/*if (xx >= w)
//				{
//				i += 4 * (1920 - w) - 4;
//
//				continue;
//				}
//				if (yy >= h)
//				{
//
//				break;
//				}*/
//				/*if (xx == 799)
//				std::cout << xx << std::endl;
//				if (yy >= he)
//				break;*/
//				glBegin(GL_POINTS);
//				glVertex2f(xx, yy);
//				glEnd();
//
//			}
//			//std::cout << i << std::endl;
//			is_updated = true;
//			glFlush();
//			SDL_GL_SwapWindow(window);
//		}
//		/*SDL_Delay(2000);
//
//		SDL_HideWindow(window);
//		break;*/
//		//SDL_WaitEvent(window_event);
//		switch (window_event->type)
//		{
//
//		case SDL_QUIT:
//			quit = true;
//			break;
//		case SDL_KEYDOWN:
//		{
//			switch (window_event->key.keysym.sym)
//			{
//			case SDLK_ESCAPE:
//				quit = true;
//				break;
//			case SDLK_F3:
//				SDL_ShowWindow(window);
//				is_updated = false;
//				break;
//			case SDLK_F4:
//				SDL_HideWindow(window);
//				break;
//				/*case SDLK_F11:
//				Uint32 flags = SDL_GetWindowFlags(window);
//				if (flags & SDL_WINDOW_FULLSCREEN)
//				{
//				SDL_SetWindowPosition(window, init_pos, init_pos);
//				SDL_SetWindowSize(window, init_w, init_h);
//				}
//				else
//				{
//				SDL_SetWindowPosition(window, 0, 0);
//				SDL_SetWindowSize(window, desktop_mode->w, desktop_mode->h);
//				}
//				flags ^= SDL_WINDOW_FULLSCREEN;
//				SDL_SetWindowFullscreen(window, flags);
//				break;*/
//			}
//		}
//		}
//
//		if (window_event->type == SDL_WINDOWEVENT)
//		{
//			switch (window_event->window.event)
//			{
//			case SDL_WINDOWEVENT_RESIZED:
//				w = window_event->window.data1;
//				h = window_event->window.data2;
//
//				is_updated = false;
//
//				SDL_GL_DeleteContext(gl_context);
//				gl_context = SDL_GL_CreateContext(window);
//
//				glClearColor((double)0 / 255, (double)0 / 255, (double)0 / 255, 1);
//				glClear(GL_COLOR_BUFFER_BIT);
//				//extern SmartBytes sm;
//				SDL_GL_SwapWindow(window);
//				glMatrixMode(GL_PROJECTION);
//				glLoadIdentity();
//				//gluOrtho2D(0, 800, 0, 400);
//				glOrtho(0, wi/*this->w*/, 0, he/*this->h*/, 0, 1);
//				//std::cout << window_event->window.data1 << " " << window_event->window.data2 << std::endl;
//				break;
//
//			case SDL_WINDOWEVENT_RESTORED:
//			case SDL_WINDOWEVENT_MAXIMIZED:
//				is_updated = false;
//				break;
//
//			}
//		}
//
//		/*SDL_ShowWindow(window);*/
//
//		//SDL_RenderPresent(renderer);
//		Sint32 delay_time = 1000.0 / fps - (SDL_GetTicks() - frame_start_time);
//		if (delay_time > 0)
//			SDL_Delay((Uint32)delay_time);
//		frame_start_time = SDL_GetTicks();
//	}
//	//SDL_Delay(2000);
//
//	return 0;
//}
//
//int MyWindow::Run()
//{
//	event_getter = new std::thread(&MyWindow::Join, this, nullptr, 0, 0, 0, 0, 0, 0);
//	return 0;
//}
//
//int MyWindow::Run(const char* title, int x, int y, int w, int h, Uint32 flags, Uint32 fps)
//{
//	//std::thread t1();
//	//int(*p)() = StartLoop;
//	//MyWindow w2("My window", 150, 150, 600, 400, SDL_WINDOW_HIDDEN);
//	//MyWindow w3;
//	//std::thread(&MyWindow::StartLoop, this).detach();
//	event_getter = new std::thread(&MyWindow::Join, this, title, x, y, w, h, flags, fps);
//	//event_getter = new std::thread(StartLoop);
//	//event_getter->detach();
//	//event_getter->join();
//	//event_getter = &(std::thread(&MyWindow::StartLoop, this));
//
//	return 0;
//}
//
//int MyWindow::Join(const char* title, int x, int y, int w, int h, Uint32 flags, Uint32 fps)
//{
//	if (title == nullptr)
//	{
//		Create();
//	}
//	else
//		Create(title, x, y, w, h, flags, fps);
//
//	gl_context = SDL_GL_CreateContext(window);
//	glClearColor((double)0 / 255, (double)0 / 255, (double)0 / 255, 1);
//	glClear(GL_COLOR_BUFFER_BIT);
//	//extern SmartBytes sm;
//
//	//SDL_GL_D
//
//
//	SDL_GL_SwapWindow(window);
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	//gluOrtho2D(0, 800, 0, 400);
//	glOrtho(0, sm.getWidth()/*this->w*/, 0, sm.getHeight()/*this->h*/, 0, 1);
//
//
//	//glColor3f(1, 1, 1);
//	//for (int i = 0; i < this->w; i++)
//	//{
//	//	for (int j = 0; j < this->h; j++)
//	//	{
//	//		glBegin(GL_POINTS);
//	//		glVertex2f(j, i);
//	//		//glVertex3f(i, j, 0);
//	//		glEnd();
//	//		glFlush();
//	//		SDL_GL_SwapWindow(window);
//	//	}
//	//}
//
//
//
//	StartLoop();
//	return 0;
//}