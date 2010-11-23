/*
 * Copyright (C) 2007 Kevin Ollivier <kevino@theolliviers.com>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "ChromeClientDFB.h"
#include "Console.h"
#include "FileChooser.h"
#include "FloatRect.h"
#include "FrameLoadRequest.h"
#include "Icon.h"
#include "NotImplemented.h"
#include "PlatformString.h"
#include "PopupMenuDFB.h"
#include "SearchPopupMenuDFB.h"

#include <stdio.h>

//#include "WebBrowserShell.h"
//#include "WebView.h"
//#include "WebViewPrivate.h"

#include <direct/debug.h>

D_DEBUG_DOMAIN( ChromeClient_DFB, "ChromeClientDFB", "ChromeClientDFB" );

namespace WebCore {

ChromeClientDFB::ChromeClientDFB(LiteWebView* webView)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );

     m_webView = webView;
}

ChromeClientDFB::~ChromeClientDFB()
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
}

void ChromeClientDFB::chromeDestroyed()
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void ChromeClientDFB::setWindowRect(const FloatRect&)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

FloatRect ChromeClientDFB::windowRect()
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
    return FloatRect(0, 0, 200, 200);
}

FloatRect ChromeClientDFB::pageRect()
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
    return FloatRect(0, 0, 200, 200);
}

float ChromeClientDFB::scaleFactor()
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
    return 1.0;
}

void ChromeClientDFB::focus()
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void ChromeClientDFB::unfocus()
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

bool ChromeClientDFB::canTakeFocus(FocusDirection)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
    return true;
}

void ChromeClientDFB::takeFocus(FocusDirection)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void ChromeClientDFB::focusedNodeChanged(Node*)
{
}

void ChromeClientDFB::focusedFrameChanged(Frame*)
{
}

Page* ChromeClientDFB::createWindow(Frame*, const FrameLoadRequest& request, const WindowFeatures&, const WebCore::NavigationAction&)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();

    // FIXME: Create a EVT_WEBKIT_NEW_WINDOW event, and only run this code
    // when that event is not handled.
    
    Page* myPage = 0;
#if 0
    DFBWebBrowserShell* newFrame = new DFBWebBrowserShell(wxTheApp->GetAppName());
    
    if (newFrame->webview) {
        newFrame->webview->LoadURL(request.resourceRequest().url().string());
        newFrame->Show(true);

        WebViewPrivate* impl = newFrame->webview->m_impl;
        if (impl)
            myPage = impl->page;
    }
#endif    
    return myPage;
}

Page* ChromeClientDFB::createModalDialog(Frame*, const FrameLoadRequest&)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
    return 0;
}

void ChromeClientDFB::show()
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

bool ChromeClientDFB::canRunModal()
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
    return false;
}

void ChromeClientDFB::runModal()
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void ChromeClientDFB::setToolbarsVisible(bool)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

bool ChromeClientDFB::toolbarsVisible()
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
    return false;
}

void ChromeClientDFB::setStatusbarVisible(bool)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

bool ChromeClientDFB::statusbarVisible()
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
    return false;
}

void ChromeClientDFB::setScrollbarsVisible(bool)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

bool ChromeClientDFB::scrollbarsVisible()
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
    return false;
}

void ChromeClientDFB::setMenubarVisible(bool)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

bool ChromeClientDFB::menubarVisible()
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
    return false;
}

void ChromeClientDFB::setResizable(bool)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void ChromeClientDFB::addMessageToConsole(MessageSource source,
                                          MessageType type,
                                          MessageLevel level,
                                          const String& message,
                                          unsigned int lineNumber,
                                          const String& sourceID)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
/*
    if (m_webView) {
        DFBWebViewConsoleMessageEvent wkEvent(m_webView);
        wkEvent.SetMessage(message);
        wkEvent.SetLineNumber(lineNumber);
        wkEvent.SetSourceID(sourceID);
        wkEvent.SetLevel(static_cast<DFBWebViewConsoleMessageLevel>(level));
        m_webView->GetEventHandler()->ProcessEvent(wkEvent);
    }
*/
}

bool ChromeClientDFB::canRunBeforeUnloadConfirmPanel()
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
    return true;
}

bool ChromeClientDFB::runBeforeUnloadConfirmPanel(const String& string,
                                                  Frame* frame)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
//    wxMessageDialog dialog(NULL, string, wxT("Confirm Action?"), wxYES_NO);
//    return dialog.ShowModal() == wxYES;
    notImplemented();
    return false;
}

void ChromeClientDFB::closeWindowSoon()
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

/*
    Sites for testing prompts: 
    Alert - just type in a bad web address or http://www.htmlite.com/JS002.php
    Prompt - http://www.htmlite.com/JS007.php
    Confirm - http://www.htmlite.com/JS006.php
*/

void ChromeClientDFB::runJavaScriptAlert(Frame* frame, const String& string)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
/*
    if (m_webView) {
        DFBWebViewAlertEvent wkEvent(m_webView);
        wkEvent.SetMessage(string);
        if (!m_webView->GetEventHandler()->ProcessEvent(wkEvent))
            wxMessageBox(string, wxT("JavaScript Alert"), wxOK);
    }
*/
}

bool ChromeClientDFB::runJavaScriptConfirm(Frame* frame, const String& string)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
    bool result = false;
/*
    if (m_webView) {
        DFBWebViewConfirmEvent wkEvent(m_webView);
        wkEvent.SetMessage(string);
        if (m_webView->GetEventHandler()->ProcessEvent(wkEvent))
            result = wkEvent.GetReturnCode() == wxID_YES;
        else {
            wxMessageDialog dialog(NULL, string, wxT("JavaScript Confirm"), wxYES_NO);
            dialog.Centre();
            result = (dialog.ShowModal() == wxID_YES);
        }
    }
*/
    return result;
}

bool ChromeClientDFB::runJavaScriptPrompt(Frame* frame, const String& message, const String& defaultValue, String& result)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
/*
    if (m_webView) {
        DFBWebViewPromptEvent wkEvent(m_webView);
        wkEvent.SetMessage(message);
        wkEvent.SetResponse(defaultValue);
        if (m_webView->GetEventHandler()->ProcessEvent(wkEvent)) {
            result = wkEvent.GetResponse();
            return true;
        }
        else {
            wxTextEntryDialog dialog(NULL, message, wxT("JavaScript Prompt"), wxEmptyString, wxOK | wxCANCEL);
            dialog.Centre();
            if (dialog.ShowModal() == wxID_OK) {
                result = dialog.GetValue();
                return true;
            }
        }
    }
*/
    return false;
}

void ChromeClientDFB::setStatusbarText(const String&)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

bool ChromeClientDFB::shouldInterruptJavaScript()
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
    return false;
}

bool ChromeClientDFB::tabsToLinks() const
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
    return false;
}

IntRect ChromeClientDFB::windowResizerRect() const
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
    return IntRect();
}

void ChromeClientDFB::invalidateWindow(const IntRect&, bool)
{
    if (!m_webView)
        return;

    lite_update_box( LITE_BOX(m_webView), NULL );
}

void ChromeClientDFB::invalidateContentsAndWindow(const IntRect& rect, bool immediate)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s( %d,%d-%dx%d, %simmediate )\n", __FUNCTION__,
                 rect.x(), rect.y(), rect.width(), rect.height(), immediate ? "" : "not " );

     if (!m_webView)
         return;

     DFBRegion region = { rect.x(), rect.y(), rect.x() + rect.width() - 1, rect.y() + rect.height() - 1 };

     lite_update_box( LITE_BOX(m_webView), &region );
}

void ChromeClientDFB::invalidateContentsForSlowScroll(const IntRect& updateRect, bool immediate)
{
     invalidateContentsAndWindow(updateRect, immediate);
}

IntRect ChromeClientDFB::windowToScreen(const IntRect& rect) const
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
    return rect;
}

IntPoint ChromeClientDFB::screenToWindow(const IntPoint& point) const
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
    return point;
}

PlatformPageClient ChromeClientDFB::platformPageClient() const
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    return NULL;//m_webView;
}

void ChromeClientDFB::contentsSizeChanged(Frame*, const IntSize& size) const
{
     D_DEBUG_AT( ChromeClient_DFB, "%s( %dx%d )\n", __FUNCTION__, size.width(), size.height() );
    notImplemented();
}

void ChromeClientDFB::scrollBackingStore(int dx, int dy, 
                    const IntRect& scrollViewRect, 
                    const IntRect& clipRect)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void ChromeClientDFB::updateBackingStore()
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void ChromeClientDFB::mouseDidMoveOverElement(const HitTestResult&, unsigned modifierFlags)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void ChromeClientDFB::setToolTip(const String& tip, TextDirection)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
/*
    wxToolTip* tooltip = m_webView->GetToolTip();
    if (!tooltip || tooltip->GetTip() != wxString(tip))
        m_webView->SetToolTip(tip);
*/
}

void ChromeClientDFB::print(Frame*)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

#if ENABLE(DATABASE)
void ChromeClientDFB::exceededDatabaseQuota(Frame*, const String&)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}
#endif

bool ChromeClientDFB::selectItemWritingDirectionIsNatural()
{
    return false;
}

PassRefPtr<PopupMenu> ChromeClientDFB::createPopupMenu(PopupMenuClient* client) const
{
    return adoptRef(new PopupMenuDFB(client));
}

PassRefPtr<SearchPopupMenu> ChromeClientDFB::createSearchPopupMenu(PopupMenuClient* client) const
{
    return adoptRef(new SearchPopupMenuDFB(client));
}

#if ENABLE(OFFLINE_WEB_APPLICATIONS)
void ChromeClientDFB::reachedMaxAppCacheSize(int64_t spaceNeeded)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void ChromeClientDFB::reachedApplicationCacheOriginQuota(SecurityOrigin*)
{
    notImplemented();
}
#endif

void ChromeClientDFB::scroll(const IntSize&, const IntRect&, const IntRect&)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();

    lite_update_box( LITE_BOX(m_webView), NULL );
/*
    m_webView->Refresh();
    notImplemented();
    */
}

void ChromeClientDFB::runOpenPanel(Frame*, PassRefPtr<FileChooser>)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void ChromeClientDFB::chooseIconForFiles(const Vector<String>& filenames, FileChooser* chooser)
{
    chooser->iconLoaded(Icon::createIconForFiles(filenames));
}

void ChromeClientDFB::setCursor(const Cursor&)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void ChromeClientDFB::requestGeolocationPermissionForFrame(Frame*, Geolocation*)
{
     D_DEBUG_AT( ChromeClient_DFB, "%s()\n", __FUNCTION__ );
    // See the comment in WebCore/page/ChromeClient.h
    notImplemented();
}

}
