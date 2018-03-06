#include "window.h"
#include "inputCodes.h"
#include "graphics.h"
#include "input.h"

#include "../../utils/public/macros.h"
#include "../../utils/public/wndincl.h"

class WindowProcedure
{
public:
	LRESULT WindowProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
	{
		static ERIEventType evnt = ERIEventType::RIE_UNKNOWN;
		static Uint32 parameter = 0;
		static Bool inputProcessed = false;

		switch ( message )
		{
		case WM_SIZE:
			graphics::ResizeViewport( LOWORD( lparam ), HIWORD( lparam ) );
			break;
			// KEY UP/DOWN
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
			ProcessRawInput( parameter, evnt );

		return DefWindowProc( hwnd, message, wparam, lparam );
	}

	void SetInputManager( IInputManager* input ) { m_input = input; }

private:
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

		if ( m_input )
			m_input->DispatchEvent( evnt, eventData );
	}

	IInputManager* m_input;
};

LRESULT CALLBACK WindowProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	WindowProcedure* winProc = nullptr;

	if ( message == WM_NCCREATE )
	{
		CREATESTRUCT* createStruct = ( CREATESTRUCT* )lparam;
		winProc = reinterpret_cast< WindowProcedure* >( createStruct->lpCreateParams );

#pragma warning( push )
#pragma warning( disable : 4390)
		if ( SetWindowLongPtr( hwnd, GWLP_USERDATA, ( LONG_PTR )winProc ) == 0 )
			SC_ASSERT( GetLastError() == 0, "Failed to get user data pointer." );
#pragma warning( pop )
	}
	else
	{
		winProc = reinterpret_cast< WindowProcedure* >( GetWindowLongPtr( hwnd, GWLP_USERDATA ) );
	}

	return winProc->WindowProc( hwnd, message, wparam, lparam );
}

MainWindow::MainWindow()
	: m_winProc( new WindowProcedure )
{
}

MainWindow::~MainWindow()
{
	delete m_winProc;
}

bool MainWindow::Create( Uint32 width, Uint32 height )
{
	m_width = width;
	m_height = height;

	m_hInstance = GetModuleHandle( 0 );

	HINSTANCE hInstance = reinterpret_cast< HINSTANCE >( m_hInstance );

	WNDCLASS wc = { 0 };

	//wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );

	wc.hInstance = hInstance;
	wc.lpfnWndProc = WindowProc;

	wc.lpszClassName = TEXT( "MainWindow" );
	//wc.style = CS_HREDRAW | CS_VREDRAW;

	HWND consoleHWND = GetConsoleWindow();

	if ( !RegisterClass( &wc ) )
	{
		return false;
	}

	RECT wndRect;
	{
		wndRect.left = 0;
		wndRect.top = 0;
		wndRect.right = width;
		wndRect.bottom = height;

		const DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;           // Window Extended Style
		const DWORD dwStyle = WS_OVERLAPPEDWINDOW;                            // Windows Style

		AdjustWindowRectEx( &wndRect, dwStyle, FALSE, dwExStyle );
	}

	m_hWnd = ( void* )CreateWindowEx(
		0L,
		"MainWindow",
		"Main window",
		WS_OVERLAPPEDWINDOW,
		520, 20,
		wndRect.right - wndRect.left,
		wndRect.bottom - wndRect.top,
		consoleHWND,
		NULL,
		reinterpret_cast< HINSTANCE >( m_hInstance ),
		m_winProc
		);

	if ( !m_hWnd )
	{
		return false;
	}

	ShowWindow( reinterpret_cast< HWND >( m_hWnd ), SW_SHOWNORMAL );

	return true;
}

void MainWindow::Tick()
{
	static MSG msg;

	while ( PeekMessage( &msg, reinterpret_cast< HWND >( m_hWnd ), 0, 0, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

void MainWindow::SetInputManager( IInputManager* input )
{
	m_winProc->SetInputManager( input );
}
