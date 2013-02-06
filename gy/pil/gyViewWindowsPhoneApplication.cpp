#include "gyViewWindowsPhoneApplication.h"
#include "gyView.h"
#include "gyMainCore.h"

#if defined(__GY_OS_ENABLED_WINSTORE__) || defined(__GY_OS_ENABLED_WINPHONE__)
gy__Application::gy__Application() :
	m_windowClosed(false),
	m_windowVisible(true)
{
}

void gy__Application::Initialize(CoreApplicationView^ applicationView)
{
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &gy__Application::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &gy__Application::OnSuspending);

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(this, &gy__Application::OnResuming);
}

void gy__Application::SetWindow(CoreWindow^ window)
{
	window->SizeChanged += 
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &gy__Application::OnWindowSizeChanged);

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &gy__Application::OnVisibilityChanged);

	window->Closed += 
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &gy__Application::OnWindowClosed);

	window->PointerPressed +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &gy__Application::OnPointerPressed);

	window->PointerMoved +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &gy__Application::OnPointerMoved);
	
	// initialize renderer here
	gy::r3::createRenderer((void *)CoreWindow::GetForCurrentThread());
	gy::r3::setClearColor(0xFF0000FF);
	// prepare (after creating a window)
	gy::type::default_callback_t applicationPrepare = gy::getCallback(gy::type::__cb::A_APPLICATION_PREPARE);
	if (applicationPrepare != NULL) applicationPrepare();
}

void gy__Application::Load(Platform::String^ entryPoint)
{
}

void gy__Application::Run()
{
	//BasicTimer^ timer = ref new BasicTimer();

	while (!m_windowClosed)
	{
		if (m_windowVisible)
		{
			//timer->Update();
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			// application update and renderer update here
			gy::type::default_callback_t applicationUpdate = gy::getCallback(gy::type::__cb::A_APPLICATION_UPDATER);
			if (applicationUpdate != NULL) 
			{
				if (applicationUpdate() == Failed)
				{
					m_windowClosed = true;
					return;
				}
			}
			else
			{
				assert(false);
				m_windowClosed = true;
				return;
			}

			gy::type::default_callback_t rendererUpdate = gy::getCallback(gy::type::__cb::A_RENDERER_UPDATER);
			if (rendererUpdate != NULL) rendererUpdate();
		}
		else
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}
}

void gy__Application::Uninitialize()
{
}

void gy__Application::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	//m_renderer->UpdateForWindowSizeChange();
}

void gy__Application::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
}

void gy__Application::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_windowClosed = true;
}

void gy__Application::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
{
	// Insert your code here.
}

void gy__Application::OnPointerMoved(CoreWindow^ sender, PointerEventArgs^ args)
{
	// Insert your code here.
}

void gy__Application::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	CoreWindow::GetForCurrentThread()->Activate();
}

void gy__Application::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	// Save app state asynchronously after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations. Be
	// aware that a deferral may not be held indefinitely. After about five seconds,
	// the app will be forced to exit.
	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

	create_task([this, deferral]()
	{
		// Insert your code here.

		deferral->Complete();
	});
}

void gy__Application::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	// Restore any data or state that was unloaded on suspend. By default, data
	// and state are persisted when resuming from suspend. Note that this event
	// does not occur if the app was previously terminated.
}



IFrameworkView^ gy__ApplicationSource::CreateView()
{
	gy::setMainView(ref new gy__Application());

	return gy::getMainView();
}

#endif