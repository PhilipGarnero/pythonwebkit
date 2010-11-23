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
#include "FrameLoaderClientDFB.h"
#include "FrameNetworkingContextDFB.h"

#include <JavaScriptCore/JavaScript.h>
#include <JavaScriptCore/APICast.h>

#include "CString.h"
#include "DocumentLoader.h"
#include "FormState.h"
#include "Frame.h"
#include "FrameLoaderTypes.h"
#include "FrameView.h"
#include "FrameTree.h"
#include "HTMLFormElement.h"
#include "HTMLFrameOwnerElement.h"
#include "NotImplemented.h"
#include "Page.h"
#include "PlatformString.h"
#include "ProgressTracker.h"
#include "RenderPart.h"
#include "ResourceError.h"
#include "ResourceResponse.h"
#include "ScriptController.h"
#include "PluginView.h"
#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>

#include <glib.h>
#include <stdio.h>

//#include "WebFrame.h"
//#include "WebFramePrivate.h"
//#include "WebView.h"
//#include "WebViewPrivate.h"


#include <direct/debug.h>

D_DEBUG_DOMAIN( DFB_FrameLoaderClient, "FrameLoaderClientDFB", "FrameLoaderClientDFB" );


namespace WebCore {

    /*
inline int wxNavTypeFromWebNavType(NavigationType type){
    if (type == NavigationTypeLinkClicked)
        return wxWEBVIEW_NAV_LINK_CLICKED;
    
    if (type == NavigationTypeFormSubmitted)
        return wxWEBVIEW_NAV_FORM_SUBMITTED;
        
    if (type == NavigationTypeBackForward)
        return wxWEBVIEW_NAV_BACK_NEXT;
        
    if (type == NavigationTypeReload)
        return wxWEBVIEW_NAV_RELOAD;
        
    if (type == NavigationTypeFormResubmitted)
        return wxWEBVIEW_NAV_FORM_RESUBMITTED;
        
    return wxWEBVIEW_NAV_OTHER;
}
*/

FrameLoaderClientDFB::FrameLoaderClientDFB()
//    : m_webFrame(0)
     : m_frame(0)
     , m_webView(0)
     , m_pluginView(0)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
}


FrameLoaderClientDFB::~FrameLoaderClientDFB()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
}

void FrameLoaderClientDFB::setFrame(WebCore::Frame *frame)
{
//    m_webFrame = frame;
    m_frame = frame;//m_webFrame->m_impl->frame;
}

void FrameLoaderClientDFB::setWebView(LiteWebView *webview)
{
    m_webView = webview;
}


bool FrameLoaderClientDFB::hasWebView() const
{
    return m_webView != NULL;
}

bool FrameLoaderClientDFB::hasBackForwardList() const
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return true;
}


void FrameLoaderClientDFB::resetBackForwardList()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


bool FrameLoaderClientDFB::provisionalItemIsTarget() const
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return false;
}

void FrameLoaderClientDFB::makeRepresentation(DocumentLoader*)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::forceLayout()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::forceLayoutForNonHTML()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::updateHistoryForCommit()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::updateHistoryForBackForwardNavigation()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::updateHistoryForReload()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::updateHistoryForStandardLoad()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::updateHistoryForInternalLoad()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::updateHistoryAfterClientRedirect()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::setCopiesOnScroll()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    // apparently mac specific
    notImplemented();
}


LoadErrorResetToken* FrameLoaderClientDFB::tokenForLoadErrorReset()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return 0;
}


void FrameLoaderClientDFB::resetAfterLoadError(LoadErrorResetToken*)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::doNotResetAfterLoadError(LoadErrorResetToken*)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::willCloseDocument()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::detachedFromParent2()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::detachedFromParent3()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::dispatchDidHandleOnloadEvents()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
/*
    if (m_webView) {
        DFBWebViewLoadEvent wkEvent(m_webView);
        wkEvent.SetState(wxWEBVIEW_LOAD_ONLOAD_HANDLED);
        wkEvent.SetURL(m_frame->loader()->documentLoader()->request().url().string());
        m_webView->GetEventHandler()->ProcessEvent(wkEvent);
    }
*/
}


void FrameLoaderClientDFB::dispatchDidReceiveServerRedirectForProvisionalLoad()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::dispatchDidCancelClientRedirect()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::dispatchWillPerformClientRedirect(const KURL&,
                                                            double interval,
                                                            double fireDate)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::dispatchDidChangeLocationWithinPage()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::dispatchDidPushStateWithinPage()
{
    notImplemented();
}

void FrameLoaderClientDFB::dispatchDidReplaceStateWithinPage()
{
    notImplemented();
}

void FrameLoaderClientDFB::dispatchDidPopStateWithinPage()
{
    notImplemented();
}

void FrameLoaderClientDFB::dispatchWillClose()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::dispatchDidStartProvisionalLoad()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
/*
    if (m_webView) {
        DFBWebViewLoadEvent wkEvent(m_webView);
        wkEvent.SetState(wxWEBVIEW_LOAD_NEGOTIATING);
        wkEvent.SetURL(m_frame->loader()->provisionalDocumentLoader()->request().url().string());
        m_webView->GetEventHandler()->ProcessEvent(wkEvent);
    }
*/
}


void FrameLoaderClientDFB::dispatchDidReceiveTitle(const String& title)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
/*
    if (m_webView) {
        m_webView->SetPageTitle(title);
        DFBWebViewReceivedTitleEvent wkEvent(m_webView);
        wkEvent.SetTitle(title);
        m_webView->GetEventHandler()->ProcessEvent(wkEvent);
    }
*/
}


void FrameLoaderClientDFB::dispatchDidCommitLoad()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
/*
    if (m_webView) {
        DFBWebViewLoadEvent wkEvent(m_webView);
        wkEvent.SetState(wxWEBVIEW_LOAD_TRANSFERRING);
        wkEvent.SetURL(m_frame->loader()->documentLoader()->request().url().string());
        m_webView->GetEventHandler()->ProcessEvent(wkEvent);
    }
*/
}

void
FrameLoaderClientDFB::dispatchDidChangeIcons()
{

}

void FrameLoaderClientDFB::dispatchDidFinishDocumentLoad()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
/*
    if (m_webView) {
        DFBWebViewLoadEvent wkEvent(m_webView);
        wkEvent.SetState(wxWEBVIEW_LOAD_DOC_COMPLETED);
        wkEvent.SetURL(m_frame->loader()->url().string());
        m_webView->GetEventHandler()->ProcessEvent(wkEvent);
    }
*/
}

void FrameLoaderClientDFB::dispatchDidFinishLoad()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );

    m_millisDidFinishLoad = direct_clock_get_millis();

    D_INFO( "FrameLoaderClientDFB: Did finish load    (%4lldms).\n", m_millisDidFinishLoad - m_millisCreateDocumentLoader );

    notImplemented();
}


void FrameLoaderClientDFB::dispatchDidFirstLayout()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );

    m_millisDidFirstLayout = direct_clock_get_millis();

    D_INFO( "FrameLoaderClientDFB: Did first layout   (%4lldms).\n", m_millisDidFirstLayout - m_millisCreateDocumentLoader );

    notImplemented();
}

void FrameLoaderClientDFB::dispatchDidFirstVisuallyNonEmptyLayout()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::dispatchShow()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::cancelPolicyCheck()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::dispatchWillSubmitForm(FramePolicyFunction function,
                                                 PassRefPtr<FormState>)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    (m_frame->loader()->policyChecker()->*function)(PolicyUse);
    notImplemented();
/*
    // FIXME: Send an event to allow for alerts and cancellation
    if (!m_webFrame)
        return;
    (m_frame->loader()->policyChecker()->*function)(PolicyUse);
*/
}


void FrameLoaderClientDFB::dispatchDidLoadMainResource(DocumentLoader*)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::revertToProvisionalState(DocumentLoader*)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::postProgressStartedNotification()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::postProgressEstimateChangedNotification()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::postProgressFinishedNotification()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
/*
    if (m_webView) {
        DFBWebViewLoadEvent wkEvent(m_webView);
        wkEvent.SetState(wxWEBVIEW_LOAD_DL_COMPLETED);
        wkEvent.SetURL(m_frame->loader()->url().string());
        m_webView->GetEventHandler()->ProcessEvent(wkEvent);
    }
*/
}

void FrameLoaderClientDFB::progressStarted()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::progressCompleted()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::setMainFrameDocumentReady(bool b)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    // this is only interesting once we provide an external API for the DOM
}


void FrameLoaderClientDFB::willChangeTitle(DocumentLoader*)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::didChangeTitle(DocumentLoader *l)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
/*
    setTitle(l->title(), l->url());
*/
}


void FrameLoaderClientDFB::finishedLoading(DocumentLoader*)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    if (m_pluginView) {
        m_pluginView->didFinishLoading();
        m_pluginView = 0;
        m_hasSentResponseToPlugin = false;
    }
}


bool FrameLoaderClientDFB::canShowMIMEType(const String& MIMEType) const
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return true;
}

bool FrameLoaderClientDFB::canShowMIMETypeAsHTML(const String& MIMEType) const
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return false;
}


bool FrameLoaderClientDFB::representationExistsForURLScheme(const String& URLScheme) const
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return false;
}


String FrameLoaderClientDFB::generatedMIMETypeForURLScheme(const String& URLScheme) const
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return String();
}


void FrameLoaderClientDFB::frameLoadCompleted()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::saveViewStateToItem(HistoryItem*)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::restoreViewState()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}
        
void FrameLoaderClientDFB::restoreScrollPositionAndViewState()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::provisionalLoadStarted()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


bool FrameLoaderClientDFB::shouldTreatURLAsSameAsCurrent(const KURL&) const
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return false;
}


void FrameLoaderClientDFB::addHistoryItemForFragmentScroll()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::didFinishLoad()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::prepareForDataSourceReplacement()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}


void FrameLoaderClientDFB::setTitle(const String& title, const KURL&)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );

    long long millisSetTitle = direct_clock_get_millis();

    D_INFO( "FrameLoaderClientDFB: Set title          (%4lldms, %s).\n",
            millisSetTitle - m_millisCreateDocumentLoader, title.utf8().data() );

    notImplemented();
}


String FrameLoaderClientDFB::userAgent(const KURL&)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    // FIXME: Use the new APIs introduced by the GTK port to fill in these values.
    return String("Mozilla/5.0 (Macintosh; U; Intel Mac OS X; en) AppleWebKit/418.9.1 (KHTML, like Gecko) Safari/419.3");
}

void FrameLoaderClientDFB::dispatchDidReceiveIcon()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::frameLoaderDestroyed()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
/*
    if (m_webFrame)
        delete m_webFrame;
    m_webFrame = 0;
*/
    m_frame = 0;
    delete this;
}

bool FrameLoaderClientDFB::canHandleRequest(const WebCore::ResourceRequest&) const
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return true;
}

void FrameLoaderClientDFB::partClearedInBegin()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::updateGlobalHistory()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::updateGlobalHistoryRedirectLinks()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

bool FrameLoaderClientDFB::shouldGoToHistoryItem(WebCore::HistoryItem*) const
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return true;
}

void FrameLoaderClientDFB::dispatchDidAddBackForwardItem(WebCore::HistoryItem*) const
{
}

void FrameLoaderClientDFB::dispatchDidRemoveBackForwardItem(WebCore::HistoryItem*) const
{
}

void FrameLoaderClientDFB::dispatchDidChangeBackForwardIndex() const
{
}

void FrameLoaderClientDFB::didDisplayInsecureContent()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::didRunInsecureContent(WebCore::SecurityOrigin*)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::saveScrollPositionAndViewStateToItem(WebCore::HistoryItem*)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

bool FrameLoaderClientDFB::canCachePage() const
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    return true;
}

void FrameLoaderClientDFB::setMainDocumentError(WebCore::DocumentLoader*, const WebCore::ResourceError&)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::committedLoad(WebCore::DocumentLoader* loader, const char* data, int length)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
/*
    if (!m_webFrame)
        return;
*/
    if (!m_pluginView) {
        loader->commitData(data, length);
//        FrameLoader* fl = loader->frameLoader();
//        fl->writer()->setEncoding(m_response.textEncodingName(), false);
//        fl->addData(data, length);
    }
    
    // We re-check here as the plugin can have been created
    if (m_pluginView) {
        if (!m_hasSentResponseToPlugin) {
            m_pluginView->didReceiveResponse(loader->response());
            // didReceiveResponse sets up a new stream to the plug-in. on a full-page plug-in, a failure in
            // setting up this stream can cause the main document load to be cancelled, setting m_pluginView
            // to null
            if (!m_pluginView)
                return;
            m_hasSentResponseToPlugin = true;
        }
        m_pluginView->didReceiveData(data, length);
    }
}

// copied from WebKit/Misc/WebKitErrors[Private].h
enum {
    WebKitErrorCannotShowMIMEType =                             100,
    WebKitErrorCannotShowURL =                                  101,
    WebKitErrorFrameLoadInterruptedByPolicyChange =             102,
    WebKitErrorCannotUseRestrictedPort = 103,
    WebKitErrorCannotFindPlugIn =                               200,
    WebKitErrorCannotLoadPlugIn =                               201,
    WebKitErrorJavaUnavailable =                                202,
};

WebCore::ResourceError FrameLoaderClientDFB::cancelledError(const WebCore::ResourceRequest& request)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return ResourceError(String(), WebKitErrorCannotShowURL, request.url().string(), String());
}

WebCore::ResourceError FrameLoaderClientDFB::blockedError(const ResourceRequest& request)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return ResourceError(String(), WebKitErrorCannotShowURL, request.url().string(), String());
}

WebCore::ResourceError FrameLoaderClientDFB::cannotShowURLError(const WebCore::ResourceRequest& request)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    return ResourceError(String(), WebKitErrorCannotShowURL, request.url().string(), String());
}

WebCore::ResourceError FrameLoaderClientDFB::interruptForPolicyChangeError(const WebCore::ResourceRequest& request)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return ResourceError(String(), WebKitErrorFrameLoadInterruptedByPolicyChange, request.url().string(), String());
}

WebCore::ResourceError FrameLoaderClientDFB::cannotShowMIMETypeError(const WebCore::ResourceResponse& response)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return ResourceError(String(), WebKitErrorCannotShowMIMEType, response.url().string(), String());
}

WebCore::ResourceError FrameLoaderClientDFB::fileDoesNotExistError(const WebCore::ResourceResponse& response)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return ResourceError(String(), WebKitErrorCannotShowURL, response.url().string(), String());
}

bool FrameLoaderClientDFB::shouldFallBack(const WebCore::ResourceError& error)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return false;
}

WTF::PassRefPtr<DocumentLoader> FrameLoaderClientDFB::createDocumentLoader(const ResourceRequest& request, const SubstituteData& substituteData)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );

    D_INFO( "FrameLoaderClientDFB: Creating DocumentLoader...\n" );

    m_millisCreateDocumentLoader = direct_clock_get_millis();

    return DocumentLoader::create(request, substituteData);
}

void FrameLoaderClientDFB::download(ResourceHandle*, const ResourceRequest&, const ResourceRequest&, const ResourceResponse&)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::assignIdentifierToInitialRequest(unsigned long identifier, DocumentLoader*, const ResourceRequest&)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();   
}

void FrameLoaderClientDFB::dispatchWillSendRequest(DocumentLoader*, unsigned long, ResourceRequest& request, const ResourceResponse& response)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s( %s )\n", __FUNCTION__, request.url().string().utf8().data() );

    long long millisWillSendRequest = direct_clock_get_millis();

    D_INFO( "FrameLoaderClientDFB: Will send request  (%4lldms, %s).\n",
            millisWillSendRequest - m_millisCreateDocumentLoader, request.url().string().utf8().data() );

    notImplemented();

/*
    GOwnPtr<WebKitNetworkResponse> networkResponse(0);

    // We are adding one more resource to the load, or maybe we are
    // just redirecting a load.
    if (redirectResponse.isNull())
        static_cast<WebKit::DocumentLoader*>(loader)->increaseLoadCount(identifier);
    else
        networkResponse.set(webkit_network_response_new_with_core_response(redirectResponse));

    WebKitWebView* webView = getViewFromFrame(m_frame);
    GOwnPtr<gchar> identifierString(toString(identifier));
    WebKitWebResource* webResource = webkit_web_view_get_resource(webView, identifierString.get());
    GOwnPtr<WebKitNetworkRequest> networkRequest(webkit_network_request_new_with_core_request(request));

    if (!redirectResponse.isNull()) {
        // This is a redirect, so we need to update the WebResource's knowledge
        // of the URI.
        g_free(webResource->priv->uri);
        webResource->priv->uri = g_strdup(request.url().string().utf8().data());
    }

    g_signal_emit_by_name(webView, "resource-request-starting", m_frame, webResource, networkRequest.get(), networkResponse.get());

    // Feed any changes back into the ResourceRequest object.
    SoupMessage* message = webkit_network_request_get_message(networkRequest.get());
    if (!message) {
        request.setURL(KURL(KURL(), String::fromUTF8(webkit_network_request_get_uri(networkRequest.get()))));
        return;
    }

    request.updateFromSoupMessage(message);
*/
}

bool FrameLoaderClientDFB::shouldUseCredentialStorage(DocumentLoader*, unsigned long)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return false;
}

void FrameLoaderClientDFB::dispatchDidReceiveAuthenticationChallenge(DocumentLoader*, unsigned long, const AuthenticationChallenge&)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::dispatchDidCancelAuthenticationChallenge(DocumentLoader*, unsigned long, const AuthenticationChallenge&)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::dispatchDidReceiveResponse(DocumentLoader* loader, unsigned long id, const ResourceResponse& response)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    // Update our knowledge of request soup flags - some are only set
    // after the request is done.
    loader->request().setSoupMessageFlags(response.soupMessageFlags());
    m_response = response;
}

void FrameLoaderClientDFB::dispatchDidReceiveContentLength(DocumentLoader* loader, unsigned long id, int length)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::dispatchDidFinishLoading(DocumentLoader*, unsigned long)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::dispatchDidFailLoading(DocumentLoader*, unsigned long, const ResourceError&)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
/*
    if (m_webView) {
        DFBWebViewLoadEvent wkEvent(m_webView);
        wkEvent.SetState(wxWEBVIEW_LOAD_FAILED);
        wkEvent.SetURL(m_frame->loader()->documentLoader()->request().url().string());
        m_webView->GetEventHandler()->ProcessEvent(wkEvent);
    }
*/
}

bool FrameLoaderClientDFB::dispatchDidLoadResourceFromMemoryCache(DocumentLoader*, const ResourceRequest&, const ResourceResponse&, int)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return false;
}

void FrameLoaderClientDFB::dispatchDidFailProvisionalLoad(const ResourceError&)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::dispatchDidFailLoad(const ResourceError&)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

Frame* FrameLoaderClientDFB::dispatchCreatePage(const NavigationAction&)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return false;
}

void FrameLoaderClientDFB::dispatchDecidePolicyForMIMEType(FramePolicyFunction function, const String& mimetype, const ResourceRequest& request)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
/*
    if (!m_webFrame)
        return;
*/
    
    notImplemented();
    (m_frame->loader()->policyChecker()->*function)(PolicyUse);
}

void FrameLoaderClientDFB::dispatchDecidePolicyForNewWindowAction(FramePolicyFunction function, const NavigationAction&, const ResourceRequest& request, PassRefPtr<FormState>, const String& targetName)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
/*
    if (!m_webFrame)
        return;

    if (m_webView) {
        DFBWebViewNewWindowEvent wkEvent(m_webView);
        wkEvent.SetURL(request.url().string());
        wkEvent.SetTargetName(targetName);
        if (m_webView->GetEventHandler()->ProcessEvent(wkEvent)) {
            // if the app handles and doesn't skip the event, 
            // from WebKit's perspective treat it as blocked / ignored
            (m_frame->loader()->policyChecker()->*function)(PolicyIgnore);
            return;
        }
    }
*/
    
    (m_frame->loader()->policyChecker()->*function)(PolicyUse);
}

void FrameLoaderClientDFB::dispatchDecidePolicyForNavigationAction(FramePolicyFunction function, const NavigationAction& action, const ResourceRequest& request, PassRefPtr<FormState>)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();

    (m_frame->loader()->policyChecker()->*function)(PolicyUse);

    if (m_webView)
         __lite_webview_navigation( m_webView, request.url().string().utf8().data() );
/*
    if (!m_webFrame)
        return;
        
    if (m_webView) {
        DFBWebViewBeforeLoadEvent wkEvent(m_webView);
        wkEvent.SetNavigationType(wxNavTypeFromWebNavType(action.type()));
        wkEvent.SetURL(request.url().string());
        
        m_webView->GetEventHandler()->ProcessEvent(wkEvent);
        if (wkEvent.IsCancelled())
            (m_frame->loader()->policyChecker()->*function)(PolicyIgnore);
        else
            (m_frame->loader()->policyChecker()->*function)(PolicyUse);
        
    }
*/
}

void FrameLoaderClientDFB::dispatchUnableToImplementPolicy(const ResourceError&)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::startDownload(const ResourceRequest&)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

PassRefPtr<Frame> FrameLoaderClientDFB::createFrame(const KURL& url, const String& name, HTMLFrameOwnerElement* ownerElement,
                                   const String& referrer, bool allowsScrolling, int marginWidth, int marginHeight)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return NULL;
/*
    WebViewFrameData* data = new WebViewFrameData();
    data->name = name;
    data->ownerElement = ownerElement;
    data->url = url;
    data->referrer = referrer;
    data->allowsScrolling = allowsScrolling;
    data->marginWidth = marginWidth;
    data->marginHeight = marginHeight;

    DFBWebFrame* newFrame = new DFBWebFrame(m_webView, m_webFrame, data);

    RefPtr<Frame> childFrame = adoptRef(newFrame->m_impl->frame);

    m_frame->tree()->appendChild(childFrame);
    childFrame->tree()->setName(name);
    childFrame->init();

    // The creation of the frame may have run arbitrary JavaScript that removed it from the page already.
    if (!childFrame->page())
        return 0;

    childFrame->loader()->loadURLIntoChildFrame(url, referrer, childFrame.get());
    
    // The frame's onload handler may have removed it from the document.
    if (!childFrame->tree()->parent())
        return 0;
    
    return childFrame.release();
*/
}

void FrameLoaderClientDFB::didTransferChildFrameToNewDocument(WebCore::Page*)
{
}

void FrameLoaderClientDFB::transferLoadingResourceFromPage(unsigned long, DocumentLoader*, const ResourceRequest&, Page*)
{
}

ObjectContentType FrameLoaderClientDFB::objectContentType(const KURL& url, const String& mimeType)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return ObjectContentType();
}

PassRefPtr<Widget> FrameLoaderClientDFB::createPlugin(const IntSize& size, HTMLPlugInElement* element, const KURL& url, const Vector<String>& paramNames, const Vector<String>& paramValues, const String& mimeType, bool loadManually)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
/*
#if __WXMSW__ || __WXMAC__
    RefPtr<PluginView> pv = PluginView::create(m_frame, size, element, url, paramNames, paramValues, mimeType, loadManually);
    if (pv->status() == PluginStatusLoadedSuccessfully)
        return pv;
#endif
*/
    return 0;
}

void FrameLoaderClientDFB::redirectDataToPlugin(Widget* pluginWidget)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    ASSERT(!m_pluginView);
    m_pluginView = static_cast<PluginView*>(pluginWidget);
    m_hasSentResponseToPlugin = false;
}

ResourceError FrameLoaderClientDFB::pluginWillHandleLoadError(const ResourceResponse& response)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return ResourceError(String(), WebKitErrorCannotLoadPlugIn, response.url().string(), String());
}

PassRefPtr<Widget> FrameLoaderClientDFB::createJavaAppletWidget(const IntSize&, HTMLAppletElement*, const KURL& baseURL,
                                                    const Vector<String>& paramNames, const Vector<String>& paramValues)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return 0;
}

String FrameLoaderClientDFB::overrideMediaType() const
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
    return String();
}

void FrameLoaderClientDFB::dispatchDidClearWindowObjectInWorld(DOMWrapperWorld* world)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();

    if (world != mainThreadNormalWorld())
        return;

/*
    if (m_webView) {
        DFBWebViewWindowObjectClearedEvent wkEvent(m_webView);
        Frame* coreFrame = m_webView->GetMainFrame()->GetFrame();
        JSGlobalContextRef context = toGlobalRef(coreFrame->script()->globalObject(mainThreadNormalWorld())->globalExec());
        JSObjectRef windowObject = toRef(coreFrame->script()->globalObject(mainThreadNormalWorld()));
        wkEvent.SetJSContext(context);
        wkEvent.SetWindowObject(windowObject);
        m_webView->GetEventHandler()->ProcessEvent(wkEvent);
    }
*/
}

void FrameLoaderClientDFB::documentElementAvailable()
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
}

void FrameLoaderClientDFB::didPerformFirstNavigation() const
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::registerForIconNotification(bool listen)
{
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::savePlatformDataToCachedFrame(CachedFrame*)
{ 
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::transitionToCommittedFromCachedFrame(CachedFrame*)
{ 
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
}

void FrameLoaderClientDFB::transitionToCommittedForNewPage()
{ 
    D_DEBUG_AT( DFB_FrameLoaderClient, "%s()\n", __FUNCTION__ );
    notImplemented();
/*
    ASSERT(m_webFrame);
    ASSERT(m_frame);
    ASSERT(m_webView);
    
    IntSize size = IntRect(m_webView->GetRect()).size();
    // FIXME: This value should be gotten from m_webView->IsTransparent();
    // but transitionToCommittedForNewPage() can be called while m_webView is
    // still being initialized.
    bool transparent = false;
    Color backgroundColor = transparent ? WebCore::Color::transparent : WebCore::Color::white;
    
    if (m_frame)
        m_frame->createView(size, backgroundColor, transparent, IntSize(), false); 
*/
}

void FrameLoaderClientDFB::dispatchDidBecomeFrameset(bool)
{
}

bool FrameLoaderClientDFB::shouldUsePluginDocument(const String &mimeType) const
{
    // NOTE: Plugin Documents are used for viewing PDFs, etc. inline, and should
    // not be used for pages with plugins in them.
    return false;
}

PassRefPtr<FrameNetworkingContext> FrameLoaderClientDFB::createNetworkingContext()
{
    return FrameNetworkingContextDFB::create(m_frame);
}

}
