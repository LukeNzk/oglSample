#include "window.h"
#include "inputCodes.h"
#include "input.h"

namespace WIN_API
{
#include "wndincl.h"
};

using namespace WIN_API;

namespace Input
{
	IInputManager* GInputManagerInterface = nullptr;
}

void ProcessRawInput( Uint32 wParam, ERIEventType evnt )
{
	void* eventData;

	if ( evnt == ERIEventType::RIE_MOUSEMOVE )
	{
		static POINTS points;
		points = MAKEPOINTS( wParam );
		eventData = ( void* )&points;
	}
	else
	{
		eventData = ( void* )&wParam;
	}

	Input::GInputManagerInterface->DispatchEvent( evnt, eventData );
}

LRESULT CALLBACK WindowProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	extern void ProcessRawInput( Uint32 message, Uint32 wParam );
	static ERIEventType evnt = ERIEventType::RIE_UNKNOWN;
	static Uint32 parameter = 0;
	static Bool inputProcessed = false;
	
	switch ( message )
	{
		//KEY
	case WM_KEYUP:
		evnt = ERIEventType::RIE_UP;
		parameter = static_cast< Uint32 >( wparam );
		inputProcessed = true;
		break;
	case WM_KEYDOWN:
		evnt = ERIEventType::RIE_DOWN;
		parameter = static_cast< Uint32 >( wparam );
		inputProcessed = true;
		break;

		//DESTROY WINDOW
	case WM_DESTROY:
		evnt = ERIEventType::RIE_UP;
		parameter = RI_CLOSE;
		inputProcessed = true;
		break;

		//MOUSE MOVEMENT
	case WM_MOUSEMOVE:
		evnt = ERIEventType::RIE_MOUSEMOVE;
		parameter = static_cast< Uint32 >( lparam );
		inputProcessed = true;
		break;

		//LMB
	case WM_LBUTTONDOWN:
		evnt = ERIEventType::RIE_DOWN;
		parameter = RI_LMB;
		inputProcessed = true;
		break;

	case WM_LBUTTONUP:
		evnt = ERIEventType::RIE_UP;
		parameter = RI_LMB;
		inputProcessed = true;
		break;

		//RMB
	case WM_RBUTTONDOWN:
		evnt = ERIEventType::RIE_DOWN;
		parameter = RI_RMB;
		inputProcessed = true;
		break;

	case WM_RBUTTONUP:
		evnt = ERIEventType::RIE_UP;
		parameter = RI_RMB;
		inputProcessed = true;
		break;

	default:
		break;
	}

	if ( inputProcessed )
		ProcessRawInput( parameter, static_cast< Uint32 >( evnt ) );

	return DefWindowProc( hwnd, message, wparam, lparam );
}

CWindow::CWindow()
{

}

CWindow::CWindow( Input::IInputManager* inputManager )
{
	Input::GInputManagerInterface = inputManager;
}

bool CWindow::Create()
{
	m_hInstance = GetModuleHandle( 0 );

	HINSTANCE hInstance = reinterpret_cast< HINSTANCE >( m_hInstance );

	WNDCLASS wc = { 0 };

	//LRESULT CALLBACK WindowProc( HWND, UINT, WPARAM, LPARAM );

	//wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );

	wc.hInstance = hInstance;
	if ( Input::GInputManagerInterface )
		wc.lpfnWndProc = WindowProc;

	wc.lpszClassName = TEXT( "MainWindow" );
	//wc.style = CS_HREDRAW | CS_VREDRAW;

	HWND consoleHWND = GetConsoleWindow();

	if ( !RegisterClass( &wc ) )
	{
		return false;
	}

	m_hWnd = ( void* )WIN_API::CreateWindowEx(
		0L,
		"MainWindow",
		"Main window",
		WS_OVERLAPPEDWINDOW,
		520, 20, m_width, m_height,
		consoleHWND,
		NULL,
		reinterpret_cast< HINSTANCE >( m_hInstance ),
		NULL
		);

	if ( !m_hWnd )
	{
		return false;
	}

	ShowWindow( reinterpret_cast< HWND >( m_hWnd ), SW_SHOWNORMAL );

	return true;
}

void CWindow::Tick()
{
	static MSG msg;

	while ( PeekMessage( &msg, reinterpret_cast< HWND >( m_hWnd ), 0, 0, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}
