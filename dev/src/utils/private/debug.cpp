#include "debug.h"

#include <windows.h>

namespace Debug
{
	void ShowMessageBox( const AnsiChar* const title, const AnsiChar* const message )
	{
		MessageBox(
			NULL,
			title,
			message,
			MB_ICONEXCLAMATION | MB_DEFBUTTON2
			);
	}

	void ShowErrorMessage( const AnsiChar* const title, const AnsiChar* const message )
	{
		MessageBox(
			NULL,
			title,
			message,
			MB_ICONERROR | MB_DEFBUTTON2
			);
	}
}