/*
 * Focus and activation functions
 *
 * Copyright 1993 David Metcalfe
 * Copyright 1995 Alex Korobka
 * Copyright 1994, 2002 Alexandre Julliard
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include "user_private.h"
#include "wine/server.h"


/*******************************************************************
 *		SetForegroundWindow  (USER32.@)
 */
BOOL WINAPI SetForegroundWindow( HWND hwnd )
{
    return NtUserSetForegroundWindow( hwnd );
}


/*******************************************************************
 *		GetActiveWindow  (USER32.@)
 */
HWND WINAPI GetActiveWindow(void)
{
    GUITHREADINFO info;
    info.cbSize = sizeof(info);
    return NtUserGetGUIThreadInfo( GetCurrentThreadId(), &info ) ? info.hwndActive : 0;
}


/*****************************************************************
 *		GetFocus  (USER32.@)
 */
HWND WINAPI GetFocus(void)
{
    GUITHREADINFO info;
    info.cbSize = sizeof(info);
    return NtUserGetGUIThreadInfo( GetCurrentThreadId(), &info ) ? info.hwndFocus : 0;
}


/*******************************************************************
*		SetShellWindow (USER32.@)
*/
BOOL WINAPI SetShellWindow( HWND hwnd )
{
    return NtUserSetShellWindowEx( hwnd, hwnd );
}


/*******************************************************************
*		GetShellWindow (USER32.@)
*/
HWND WINAPI GetShellWindow(void)
{
    return NtUserGetShellWindow();
}


/***********************************************************************
 *		SetProgmanWindow (USER32.@)
 */
HWND WINAPI SetProgmanWindow ( HWND hwnd )
{
    SERVER_START_REQ(set_global_windows)
    {
        req->flags          = SET_GLOBAL_PROGMAN_WINDOW;
        req->progman_window = wine_server_user_handle( hwnd );
        if (wine_server_call_err( req )) hwnd = 0;
    }
    SERVER_END_REQ;
    return hwnd;
}


/***********************************************************************
 *		GetProgmanWindow (USER32.@)
 */
HWND WINAPI GetProgmanWindow(void)
{
    HWND ret = 0;

    SERVER_START_REQ(set_global_windows)
    {
        req->flags = 0;
        if (!wine_server_call_err(req))
            ret = wine_server_ptr_handle( reply->old_progman_window );
    }
    SERVER_END_REQ;
    return ret;
}


/***********************************************************************
 *		SetTaskmanWindow (USER32.@)
 * NOTES
 *   hwnd = MSTaskSwWClass
 *          |-> SysTabControl32
 */
HWND WINAPI SetTaskmanWindow ( HWND hwnd )
{
    SERVER_START_REQ(set_global_windows)
    {
        req->flags          = SET_GLOBAL_TASKMAN_WINDOW;
        req->taskman_window = wine_server_user_handle( hwnd );
        if (wine_server_call_err( req )) hwnd = 0;
    }
    SERVER_END_REQ;
    return hwnd;
}

/***********************************************************************
 *		GetTaskmanWindow (USER32.@)
 */
HWND WINAPI GetTaskmanWindow(void)
{
    HWND ret = 0;

    SERVER_START_REQ(set_global_windows)
    {
        req->flags = 0;
        if (!wine_server_call_err(req))
            ret = wine_server_ptr_handle( reply->old_taskman_window );
    }
    SERVER_END_REQ;
    return ret;
}
