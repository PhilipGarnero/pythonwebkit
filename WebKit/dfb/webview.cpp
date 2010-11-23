/* 
   (c) Copyright 2001-2002  convergence integrated media GmbH.
   (c) Copyright 2002-2005  convergence GmbH.

   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include <direct/mem.h>
}

#include <lite/util.h>
#include <lite/window.h>


#include <ChromeClientDFB.h>
#include <ContextMenuClientDFB.h>
#include <EditorClientDFB.h>
#include <DragClientDFB.h>
#include <InspectorClientDFB.h>
#include <FrameLoaderClientDFB.h>

#include <InitializeThreading.h>

#include <GraphicsContext.h>
#include <Frame.h>
#include <FrameView.h>
#include <Page.h>
#include <Settings.h>

#include <CString.h>

#include <ApplicationCacheStorage.h>
#include <PageCache.h>

#include <PlatformMouseEvent.h>
#include <PlatformWheelEvent.h>

#include "webview.h"

D_DEBUG_DOMAIN(LiteWebViewDomain, "LiTE/WebView", "LiteWebView");

LiteWebViewTheme *liteDefaultWebViewTheme = NULL;

struct _LiteWebView {
     LiteBox                 box;
     LiteWebViewTheme       *theme;

     WebCore::Page          *page;

     WebViewNavigationFunc   navigation;
     void                   *navigation_data;
};

static int on_button_down( LiteBox *box, int x, int y,
                           DFBInputDeviceButtonIdentifier button );

static int on_button_up( LiteBox *box, int x, int y,
                         DFBInputDeviceButtonIdentifier button );

static int on_motion( LiteBox *box, int x, int y,
                      DFBInputDeviceButtonMask buttons );

static int on_wheel( LiteBox *box, DFBWindowEvent *evt );

static DFBResult draw_webview(LiteBox *box, const DFBRegion *region, DFBBoolean clear);
static DFBResult destroy_webview(LiteBox *box);

DFBResult 
lite_new_webview(LiteBox         *parent, 
                 DFBRectangle    *rect,
                 LiteWebViewTheme *theme,
                 LiteWebView     **ret_webview)
{
     DFBResult    res; 
     LiteWebView *webview = NULL;

     webview = (LiteWebView*) D_CALLOC(1, sizeof(LiteWebView));

     webview->box.parent = parent;
     webview->theme = theme;
     webview->box.rect = *rect;

     res = lite_init_box(LITE_BOX(webview));
     if (res != DFB_OK) {
          D_FREE(webview);
          return res;
     }

     webview->box.type    = LITE_TYPE_BOX_USER;
     webview->box.Draw    = draw_webview;
     webview->box.Destroy = destroy_webview;

     webview->box.OnButtonDown = on_button_down;
     webview->box.OnButtonUp   = on_button_up;
     webview->box.OnMotion     = on_motion;
     webview->box.OnWheel      = on_wheel;


     WTF::initializeThreading();
     WTF::initializeMainThread();
     JSC::initializeThreading();

     WebCore::Page::PageClients pageClients;

     pageClients.chromeClient = static_cast<WebCore::ChromeClient*>(new WebCore::ChromeClientDFB(webview));
     pageClients.contextMenuClient = static_cast<WebCore::ContextMenuClient*>(new WebCore::ContextMenuClientDFB);
     pageClients.editorClient = static_cast<WebCore::EditorClient*>(new WebCore::EditorClientDFB);
     pageClients.dragClient = static_cast<WebCore::DragClient*>(new WebCore::DragClientDFB);
     pageClients.inspectorClient = static_cast<WebCore::InspectorClient*>(new WebCore::InspectorClientDFB);

     webview->page = new WebCore::Page( pageClients );


     WebCore::Settings *settings = webview->page->settings();

     if (!settings) {
          D_ERROR( "Could not get page settings!\n" );
          //FIXME: deinit
          return DFB_INIT;
     }



//     WebCore::Settings* settings = frame->settings();

     settings->setLoadsImagesAutomatically( true );
     settings->setJavaScriptEnabled( true );
     settings->setDefaultFontSize(14);
     settings->setDefaultFixedFontSize(14);
     settings->setMinimumFontSize(10);
     settings->setMinimumLogicalFontSize(10);
     settings->setOfflineWebApplicationCacheEnabled(true);
     settings->setUsesPageCache(true);
     settings->setLocalStorageEnabled(true);
     settings->setUsesEncodingDetector(true);






     WebCore::FrameLoaderClientDFB *client = new WebCore::FrameLoaderClientDFB();

     WTF::PassRefPtr<WebCore::Frame> frame = WebCore::Frame::create( webview->page, NULL, client );


     client->setFrame( frame.get() );
     client->setWebView( webview );





     WTF::PassRefPtr<WebCore::FrameView> view = WebCore::FrameView::create( frame.get(), IntSize(rect->w,rect->h) );

     frame->setView( view );

     frame->init();


//     webview->page->setMainFrame( frame );


//     WebCore::cache()->setCapacities( 4 * 1024 * 1024, 8 * 1024 * 1024, 16 * 1024 * 1024 );
//     WebCore::cache()->setDisabled( false );

     WebCore::pageCache()->setCapacity( 20 /* pages */ );

     WebCore::cacheStorage().setCacheDirectory("/tmp/test");
     WebCore::cacheStorage().setMaximumSize( 16 * 1024 * 1024 );

     D_INFO( "WebKit/CacheStorage: '%s' (%zd bytes)\n",
             WebCore::cacheStorage().cacheDirectory().utf8().data(),
             WebCore::cacheStorage().maximumSize() );

     D_INFO( "WebKit/PageCache: %d pages\n",
             WebCore::pageCache()->capacity() );



     lite_update_box(LITE_BOX(webview), NULL);

     *ret_webview = webview;

     D_DEBUG_AT(LiteWebViewDomain, "Created new webview object: %p\n", webview);

     return DFB_OK;
}

DFBResult 
lite_webview_load(LiteWebView *webview, const char *url)
{
     LITE_NULL_PARAMETER_CHECK(webview);
     LITE_BOX_TYPE_PARAMETER_CHECK(LITE_BOX(webview), LITE_TYPE_BOX_USER);

     webview->page->mainFrame()->loader()->load( WebCore::KURL(WebCore::KURL(),
                                                               WTF::String( url )), false );

     return DFB_OK;
}

DFBResult
lite_webview_go_back( LiteWebView *webview )
{
     LITE_NULL_PARAMETER_CHECK(webview);
     LITE_BOX_TYPE_PARAMETER_CHECK(LITE_BOX(webview), LITE_TYPE_BOX_USER);

     webview->page->goBack();

     return DFB_OK;
}

DFBResult
lite_webview_go_forward( LiteWebView *webview )
{
     LITE_NULL_PARAMETER_CHECK(webview);
     LITE_BOX_TYPE_PARAMETER_CHECK(LITE_BOX(webview), LITE_TYPE_BOX_USER);

     webview->page->goForward();

     return DFB_OK;
}

DFBResult
lite_webview_stop( LiteWebView *webview )
{
     LITE_NULL_PARAMETER_CHECK(webview);
     LITE_BOX_TYPE_PARAMETER_CHECK(LITE_BOX(webview), LITE_TYPE_BOX_USER);

     webview->page->mainFrame()->loader()->stop();

     return DFB_OK;
}

DFBResult
lite_webview_reload( LiteWebView *webview )
{
     LITE_NULL_PARAMETER_CHECK(webview);
     LITE_BOX_TYPE_PARAMETER_CHECK(LITE_BOX(webview), LITE_TYPE_BOX_USER);

     webview->page->mainFrame()->loader()->reload();

     return DFB_OK;
}

DFBResult
lite_webview_scroll_down( LiteWebView *webview )
{
     LITE_NULL_PARAMETER_CHECK(webview);

     WebCore::PlatformWheelEvent event( 0, 0, 1 );

     webview->page->mainFrame()->eventHandler()->handleWheelEvent( event );

     return DFB_OK;
}

DFBResult
lite_webview_scroll_up( LiteWebView *webview )
{
     LITE_NULL_PARAMETER_CHECK(webview);

     WebCore::PlatformWheelEvent event( 0, 0, -1 );

     webview->page->mainFrame()->eventHandler()->handleWheelEvent( event );

     return DFB_OK;
}

DFBResult 
lite_on_webview_navigation(LiteWebView           *webview,
                           WebViewNavigationFunc  func,
                           void                  *funcdata)
{
     LITE_NULL_PARAMETER_CHECK(webview);
     LITE_BOX_TYPE_PARAMETER_CHECK(LITE_BOX(webview), LITE_TYPE_BOX_USER);

     webview->navigation      = func;
     webview->navigation_data = funcdata;

     return DFB_OK;
}

DFBResult
__lite_webview_navigation( LiteWebView *webview,
                           const char  *url )
{
     LITE_NULL_PARAMETER_CHECK(webview);
     LITE_NULL_PARAMETER_CHECK(url);

     if (webview->navigation)
          webview->navigation( webview, webview->navigation_data, url );

     return DFB_OK;
}

/* internals */

static DFBResult 
destroy_webview(LiteBox *box)
{
     LiteWebView *webview =  NULL;

     D_ASSERT(box != NULL);

     webview = LITE_WEBVIEW(box);

     D_DEBUG_AT(LiteWebViewDomain, "Destroy webview: %p\n", webview);

     if (!webview)
          return DFB_FAILURE;

     return lite_destroy_box(box);
}

static DFBResult 
draw_webview(LiteBox         *box, 
             const DFBRegion *region, 
             DFBBoolean       clear)
{
     IDirectFBSurface *surface = box->surface;
     LiteWebView      *webview = LITE_WEBVIEW(box);

     D_ASSERT(box != NULL);

     surface->SetClip(surface, region);

     /* Fill the background */
     if (clear)
          lite_clear_box(box, region);


     WebCore::Frame     *frame = webview->page->mainFrame();
     WebCore::FrameView *view  = frame->view();

    if (view && view->needsLayout())
        view->forceLayout();

     if (view && frame->document() && frame->contentRenderer()) {
        frame->view()->updateLayoutAndStyleIfNeededRecursive();

/*     DFBDimension        size;

     surface->GetSize( surface, &size.w, &size.h );

     view->setFrameRect( WebCore::IntRect(0, 0, size.w, size.h) );
     view->forceLayout();
     view->adjustViewSize();
*/
//    view->setViewportSize( WebCore::IntSize(720,576) );

//          view->layoutIfNeededRecursive();


          long long t1 = direct_clock_get_millis();


          WebCore::GraphicsContext *context = new WebCore::GraphicsContext( surface );

          WebCore::IntRect rect(region->x1, region->y1,//+ view->scrollOffset().height(),
                                region->x2 - region->x1 + 1,
                                region->y2 - region->y1 + 1);

//     context->clip( rect );
//     context->translate( 0, - view->scrollOffset().height() );

          printf("rect region painting %d %d %d %d\n", rect.x(), rect.y(), rect.width(), rect.height());

          view->paint( context, rect );

          long long t2 = direct_clock_get_millis();


//     ctx->surface->Dump( ctx->surface, "/", "webview" );

          printf( "Frame rendered in %lldms\n", t2 - t1 );
     }

     return DFB_OK;
}

static int 
on_button_down(LiteBox                       *box, 
               int                            x, 
               int                            y,
               DFBInputDeviceButtonIdentifier button)
{
     LiteWebView *webview = LITE_WEBVIEW(box);

     D_ASSERT(box != NULL);

     WebCore::PlatformMouseEvent event( x, y, button, true );

     webview->page->mainFrame()->eventHandler()->handleMousePressEvent( event );

     return 1;
}

static int 
on_button_up(LiteBox                       *box, 
             int                            x, 
             int                            y,
             DFBInputDeviceButtonIdentifier button)
{
     LiteWebView *webview = LITE_WEBVIEW(box);

     D_ASSERT(box != NULL);

     WebCore::PlatformMouseEvent event( x, y, button, false );

     webview->page->mainFrame()->eventHandler()->handleMouseReleaseEvent( event );

     return 1;
}

static int 
on_motion(LiteBox                 *box, 
          int                      x, 
          int                      y,
          DFBInputDeviceButtonMask buttons)
{
     LiteWebView *webview = LITE_WEBVIEW(box);

     D_ASSERT(box != NULL);

     WebCore::PlatformMouseEvent event( x, y, buttons );

     webview->page->mainFrame()->eventHandler()->handleMouseMoveEvent( event );

     return 1;
}

static int
on_wheel( LiteBox *box, DFBWindowEvent *evt )
{
     LiteWebView *webview = LITE_WEBVIEW(box);

     D_ASSERT(box != NULL);
printf("wheel %d\n",evt->step);
     WebCore::PlatformWheelEvent event( evt->x, evt->y, evt->step );

     webview->page->mainFrame()->eventHandler()->handleWheelEvent( event );

     return 1;
}

