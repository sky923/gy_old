#include "gyRenderer.h"
#include "gyView.h"
#ifdef __GY_OS_ENABLED_LINUX__
// For Linux
namespace gy
{    
	namespace type
	{
		Display *view_context_t::display = NullPtr;
	}

    #define VIEW_CONTEXT_SIZE 256
    type::view_context_t view_contexts[VIEW_CONTEXT_SIZE] = {{0, NullPtr},};
    view_t main_view_id = 0;
    void addViewContext(view_t vid, int screen, int posX, int posY, size_t width, size_t height, XVisualInfo *visual, const XSetWindowAttributes &attributes)
    {
        for (size_t i = 0; i < VIEW_CONTEXT_SIZE; ++i)
        {
            if (view_contexts[i].view != 0)
                continue;
                
            view_contexts[i].view = vid;
            view_contexts[i].screen = screen;
            view_contexts[i].visual = visual;
            view_contexts[i].attributes = attributes;
            view_contexts[i].posX = posX;
            view_contexts[i].posY = posY;
            view_contexts[i].width = width;
            view_contexts[i].height = height;
            
            return;
        }
    }

    XVisualInfo *getVisualInfoFromView(view_t vid)
    {
        for (size_t i = 0; i < VIEW_CONTEXT_SIZE; ++i)
        {
            if (view_contexts[i].view == vid)
            {
                return view_contexts[i].visual;
            }
        }
        
        return NullPtr;
    }
	
    view_t getMainView()
    {
        return main_view_id;
    }
    
    int setMainView(view_t vid)
    {
        main_view_id = vid;
        
        return Success;
    }
	
    view_t getParentView(view_t vid)
    {
        Display *display = view_context_t::display;
        if (display == NullPtr)
            return 0;
		
        view_t rootWindowID = 0;
        view_t parentWindowID = 0;
        view_t *childsWindowID = {NullPtr,};
        uint childsWindowSize = 0;
        if (XQueryTree(display, vid, &rootWindowID, &parentWindowID, &childsWindowID, &childsWindowSize) == BadWindow)
            return 0;
		
        return parentWindowID;
    }
    
    int setParentView(view_t vid, view_t parentWindowID)
    {
        Display *display = view_context_t::display;
        if (display == NullPtr)
            return Failed;
		
        if (parentWindowID == 0)
        {
            int screen = DefaultScreen(display);
            view_t root = RootView(display, screen);
            parentWindowID = root;
            //parentWindowID = DefaultRootView(display);
        }
		
        int ret = XReparentView(display, vid, parentWindowID, 0, 0);
        if (ret == BadMatch || ret == BadWindow)
            return Failed;
        
        return Success;
    }

	view_t createView(const char *title, size_t width, size_t height, int posX, int posY)
	{
		Display *display = view_context_t::display;
        if (display == NullPtr)
            return Failed;
            
        int screen = DefaultScreen(display);

        /* attributes for a single buffered visual in RGBA format with at least
         * 4 bits per color and a 16 bit depth buffer */
        static int attrListSgl[] =
        { GLX_RGBA, GLX_RED_SIZE, 4, GLX_GREEN_SIZE, 4, GLX_BLUE_SIZE, 4,
                GLX_DEPTH_SIZE, 16, 0 };
        
        /* attributes for a double buffered visual in RGBA format with at least
         * 4 bits per color and a 16 bit depth buffer */
        static int attrListDbl[] =
        { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_RED_SIZE, 4, GLX_GREEN_SIZE, 4, GLX_BLUE_SIZE,
                4, GLX_DEPTH_SIZE, 16, 0 };

        XVisualInfo *visual = glXChooseVisual(display, screen, attrListDbl);
        if (visual == NullPtr)
            visual = glXChooseVisual(display, screen, attrListSgl);
        
        if (visual == NullPtr)
            return Failed;
                        
        XSetWindowAttributes attributes;
        attributes.override_redirect = True;
        attributes.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;
        attributes.colormap = XCreateColormap(display, RootView(display, screen), visual->visual, AllocNone);
        attributes.border_pixel = 0;
        
        view_t root = RootView(display, screen);
		//view_t vid = XCreateSimpleWindow(display, parent ? parent : XDefaultRootView(display), posX, posY, width, height, 2, BlackPixel(display, 0), WhitePixel(display, 0));
        view_t vid = XCreateWindow(display, root, posX, posY, width, height, 0, visual->depth, InputOutput, visual->visual, CWBorderPixel | CWColormap | CWEventMask, &attributes);
        if (vid == 0)
            return Failed;
            
        Atom WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", True);
        XSetWMProtocols(display, vid, &WM_DELETE_WINDOW, 1);
        XSetStandardProperties(display, vid, title, title, 0, NULL, 0, NULL);
                            
        addViewContext(vid, screen, posX, posY, width, height, visual, attributes);
		setMainView(vid);
        
        return vid;
	}

	int showView(view_t vid)
	{
		Display *display = view_context_t::display;
        if (display == NullPtr)
            return Failed;

        XMapRaised(display, vid);
        XGrabKeyboard(display, vid, True, GrabModeAsync, GrabModeAsync, CurrentTime);
        XGrabPointer(display, vid, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, vid, 0, CurrentTime);

		XMapWindow(display, vid);
		XFlush(display);
		return Success;
	}
    
    int destroyView(view_t vid)
    {
        Display *display = view_context_t::display;
        if (display == NullPtr)
            return Failed;
        
        XUnmapWindow(display, vid);
        XDestroyWindow(display, vid);
        
        return Success;
    }
    
	int defaultInitializeView()
	{
		if (view_context_t::display == NullPtr)
			view_context_t::display = XOpenDisplay(NullPtr);
		
		if (view_context_t::display == NullPtr)
			return Failed;

		return Success;
	}

	int defaultInitializeApplication()
	{
		return Success;
	}
	
    int defaultExecuteApplication()
    {
		Display *display = view_context_t::display;
		if (display == NullPtr)
			return Failed;

		static Display *dr = NullPtr;
		if (dr == NullPtr)
		{
			dr = display;
			if (dr != NullPtr)
			{
				printf("0x%08X\n", display);
			}
		}
		
        int ret = False;
		XEvent e;
		while (true)
		{
			while (XPending(display) > 0)
			{
				XNextEvent(display, &e);
				ret = True;
				
				switch (e.type)
				{
					case Expose:
					{
					}
						break;
						
					case ConfigureNotify:
						//if ((e.xconfigure.width != (int) (wi.Width))
						//        || (e.xconfigure.height != (int) (wi.Height)))
						//{
						//wi.Width = event.xconfigure.width;
						//wi.Height = event.xconfigure.height;
						printf("Resize event\n");
						//resizeGLScene(event.xconfigure.width,
						//      event.xconfigure.height);
						//}
						break;
					case KeyPress:
					{
						int key = XLookupKeysym(&e.xkey, 0); //KeySym
						switch (key)
						{
							case XK_Escape:
								printf("Exiting sanely...\n");
								XCloseDisplay(display);
								break;
							case XK_F1:
								break;
						}
						printf("key : %d", key);
					}
						break;
					case ClientMessage:
						if (*XGetAtomName(display, e.xclient.message_type) == *"WM_PROTOCOLS")
						{
							if (e.xany.window == getMainView())
							{
								//printf("Exiting sanely...\n");
								ret = Success;
								XCloseDisplay(display);
							}
							else
							{
								destroyView(e.xany.window);
							}
						}
						break;
						
					default:
						break;
				}
			}
		}
		
		return ret;
    }

	int defaultFinalizeApplication()
	{
		return Success;
	}
}
#endif
