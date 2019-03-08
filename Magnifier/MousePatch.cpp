// ////////////////////////////////////////////////////////////////////
// Copyright (C) 2001 Robert Wiejak
// e-mail: roberw@telus.net
// ////////////////////////////////////////////////////////////////////

HHOOK hMHook = NULL;       // handle to the mouse hook
int   bNcLButtonDown = 0;  // status of non-client left button down

/*
 * bNcLButtonDown = 0 : 滑鼠左鍵未被壓下
 * bNcLButtonDown = 1 : 滑鼠左鍵在視窗標題位置被壓下
 * bNcLButtonDown = 2 : 滑鼠左鍵在視窗邊框位置被壓下
 */

// ////////////////////////////////////////////////////////////////////
// Mouse Hook Process

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) 
{ 
    if(nCode == HC_ACTION)
    { 
        // get a pointer to the mouse hook struct. 
        PMOUSEHOOKSTRUCT mhs = (PMOUSEHOOKSTRUCT) lParam; 
        
        // intercept messages for left button down and up
        switch( wParam ) 
        { 
            case WM_NCLBUTTONDOWN:
                {
                    // get the pointer to the main window 
                    CWnd *pWnd = AfxGetMainWnd();

                    // if the message is from your window 
                    if(mhs->hwnd == pWnd->GetSafeHwnd())
					{
						UINT HitCode = mhs->wHitTestCode;
						//1 - if the hit test indicates title bar 
						if(HitCode == HTCAPTION)
	                    { 
		                    // indicate non-client left button down (moving window)
			                bNcLButtonDown = 1;

				            // there is no problem with this message
					        // so you dont't have to do anything else
						}

						//2 - and if the hit test indicates window border
						if((HitCode == HTBOTTOM) || (HitCode == HTBOTTOMLEFT) || (HitCode == HTBOTTOMRIGHT) || (HitCode == HTLEFT)
							|| (HitCode == HTRIGHT)|| (HitCode == HTTOP) || (HitCode == HTTOPLEFT) || (HitCode == HTTOPRIGHT))
						{ 
							// indicate non-client left button down (sizing window)
							bNcLButtonDown = 2;
						}
					}
                }
                break;

            case WM_NCLBUTTONUP:
                // you will get this message if you double-click on the title bar 
                // reset the status
                bNcLButtonDown = 0; 
                break; 

            case WM_LBUTTONUP:
                {
                    // get the pointer to the main window 
                    CWnd *pWnd = AfxGetMainWnd();

                    // if the message is from your window and 
                    // non-client left button is down 
                    if(mhs->hwnd == pWnd->GetSafeHwnd())
					{
                        // then post WM_NCLBUTTONUP message directly into your window message pump 
                        // Note: I'm hardcoding HTCAPTION because the left button was down, 
                        // and while it is down, the mouse does not move in respect to the window 
                        // but it does in respect to the screen, so the mouse should still be 
                        // over the caption bar of your window when you release the button. 

						if(bNcLButtonDown == 1)
	                        pWnd->PostMessage(WM_NCLBUTTONUP, HTCAPTION, MAKELONG(mhs->pt.x, mhs->pt.y));

						if(bNcLButtonDown == 2)
	                        pWnd->PostMessage(WM_NCLBUTTONUP, HTSIZE, MAKELONG(mhs->pt.x, mhs->pt.y));

                        // reset non-client left button down 
                        bNcLButtonDown = 0;
                    }
                } 
                break; 

            default: 
                break; 
        } 
    }

    // let the	messages through to the next hook 
	return CallNextHookEx(hMHook, nCode, wParam, lParam);
}
