/*
 * Copyright (C) 2008 Holger Hans Peter Freyther
 * Copyright (C) 2009 Collabora Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <config.h>

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

#include <directfb.h>

#include <lite/lite.h>

#include <glib.h>
#include <glib-object.h>

#include <webkitdfb.h>
#include <webkit/webkitrender.h>
#include <GraphicsContext.h>


static IDirectFBSurface *
create_surface( IDirectFB    *dfb,
                DFBDimension &size )
{
     IDirectFBSurface      *surface;
     DFBSurfaceDescription  desc;

//     dfb->SetCooperativeLevel( dfb, DFSCL_FULLSCREEN );

     desc.flags       = (DFBSurfaceDescriptionFlags)( DSDESC_WIDTH | DSDESC_HEIGHT | /*DSDESC_PIXELFORMAT |*/ DSDESC_CAPS );
     desc.width       = size.w;
     desc.height      = size.h;
     desc.pixelformat = DSPF_ARGB;
     desc.caps        = (DFBSurfaceCapabilities)( DSCAPS_PRIMARY /*| DSCAPS_FLIPPING*/ );

     dfb->CreateSurface( dfb, &desc, &surface );


     surface->Clear( surface, 0, 0, 0, 0 );
     surface->Flip( surface, NULL, DSFLIP_NONE );

     return surface;
}



typedef struct {
     WebCore::Page *page;
     const char    *url;
} PageLoadContext;

static gboolean
page_load(gpointer data)
{
     PageLoadContext *ctx  = (PageLoadContext*) data;
     WebCore::Page   *page = ctx->page;

     page->mainFrame()->loader()->load( WebCore::KURL(WebCore::KURL(),
                                                      WTF::String( ctx->url )), false );

     return TRUE;
}


typedef struct {
     WebCore::Page    *page;
     IDirectFBSurface *surface;
} PageRenderContext;

static gboolean
page_render(gpointer data)
{
     PageRenderContext  *ctx   = (PageRenderContext*) data;
     WebCore::Frame     *frame = ctx->page->mainFrame();
     WebCore::FrameView *view  = frame->view();
     DFBDimension        size;

     ctx->surface->GetSize( ctx->surface, &size.w, &size.h );

//     view->layoutIfNeededRecursive();

//     view->setFrameRect( WebCore::IntRect(0, 0, 720, 576) );
//     view->forceLayout();
//     view->adjustViewSize();

//    view->setViewportSize( WebCore::IntSize(720,576) );


//     ctx->surface->Clear( ctx->surface, 0, 0, 0, 0 );


//     view->setScrollPosition( WebCore::IntPoint(0,500) );


     long long t1 = direct_clock_get_millis();

#if 0
     WebCore::GraphicsContext *context = new WebCore::GraphicsContext( ctx->surface );

     context->translate( 0, -500 );

     view->paintContents( context, WebCore::IntRect(0,500,720,576) );
#else
     IDirectFBSurface *surface;
//     DFBRectangle      rect = { 0, -500, 720, 576+500 };
//     DFBRectangle      rect = { 0, -200, 720, 276 };
     DFBRectangle      rect = { 0, 0, size.w, size.h };

     ctx->surface->GetSubSurface( ctx->surface, &rect, &surface );

     WebCore::GraphicsContext *context = new WebCore::GraphicsContext( surface );

     view->paintContents( context, WebCore::IntRect(0,0,size.w,size.h) );

     surface->Release( surface );
#endif

     long long t2 = direct_clock_get_millis();

     ctx->surface->Flip( ctx->surface, NULL, DSFLIP_NONE );

//     ctx->surface->Dump( ctx->surface, "/", "webkit" );

     printf( "Frame rendered in %lldms\n", t2 - t1 );




/*

     DFBEvent event;

     event.clazz         = DFEC_WINDOW;
     event.window.type   = DWET_BUTTONDOWN;
     event.window.button = DIBI_LEFT;
     event.window.x      = 67;
     event.window.y      = 364;
     event.window.cx     = 67;
     event.window.cy     = 364;

     WebCore::PlatformMouseEvent pressEvent( &event );

     frame->eventHandler()->handleMousePressEvent( pressEvent );


     event.window.type   = DWET_BUTTONUP;

     WebCore::PlatformMouseEvent releaseEvent( &event );

     frame->eventHandler()->handleMouseReleaseEvent( releaseEvent );
*/
     return TRUE;
}

static void
screen_size( IDirectFB    *dfb,
             DFBDimension &size )
{
     IDirectFBDisplayLayer *layer;
     DFBDisplayLayerConfig  config;

     dfb->GetDisplayLayer( dfb, DSCID_PRIMARY, &layer );

     layer->GetConfiguration( layer, &config );

     layer->Release( layer );


     size.w = config.width;
     size.h = config.height;
}

int main(int argc, char** argv)
{
    GMainLoop             *loop;
    IDirectFB             *dfb;
    IDirectFBSurface      *surface;
    DFBDimension           size;

    g_type_init();

    g_thread_init(NULL);

    WTF::initializeThreading();
    WTF::initializeMainThread();
    JSC::initializeThreading();


    DirectFBInit( &argc, &argv );
    DirectFBCreate( &dfb );

    WebKitDFB_Initialize( dfb );

    screen_size( dfb, size );

//    size.w = 720;
//    size.h = 576;

    lite_open(&argc, &argv);

    surface = create_surface( dfb, size );



    loop = g_main_loop_new(0,TRUE);


    WebCore::Page::PageClients pageClients;

    pageClients.chromeClient = static_cast<WebCore::ChromeClient*>(new WebCore::ChromeClientDFB(NULL));
    pageClients.contextMenuClient = static_cast<WebCore::ContextMenuClient*>(new WebCore::ContextMenuClientDFB);
    pageClients.editorClient = static_cast<WebCore::EditorClient*>(new WebCore::EditorClientDFB);
    pageClients.dragClient = static_cast<WebCore::DragClient*>(new WebCore::DragClientDFB);
    pageClients.inspectorClient = static_cast<WebCore::InspectorClient*>(new WebCore::InspectorClientDFB);

    WebCore::Page *page = new WebCore::Page( pageClients );


    WebCore::Settings *settings = page->settings();

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

    WTF::PassRefPtr<WebCore::Frame> frame = WebCore::Frame::create( page, NULL, client );

    client->setFrame( frame.get() );


    WTF::PassRefPtr<WebCore::FrameView> view = WebCore::FrameView::create( frame.get(), IntSize(size.w,size.h) );

    frame->setView( view );

    frame->init();


//     webview->page->setMainFrame( frame );



//    WebCore::cache()->setCapacities( 4 * 1024 * 1024, 8 * 1024 * 1024, 16 * 1024 * 1024 );
//    WebCore::cache()->setDisabled( false );

    WebCore::pageCache()->setCapacity( 20 /* pages */ );

    WebCore::cacheStorage().setCacheDirectory("/tmp/test");
    WebCore::cacheStorage().setMaximumSize( 16 * 1024 * 1024 );

    D_INFO( "WebKit/CacheStorage: '%s' (%zd bytes)\n",
            WebCore::cacheStorage().cacheDirectory().utf8().data(),
            WebCore::cacheStorage().maximumSize() );

    D_INFO( "WebKit/PageCache: %d pages\n",
            WebCore::pageCache()->capacity() );


    page->mainFrame()->loader()->load( WebCore::KURL(WebCore::KURL(),
                                                     WTF::String( argv[1] ? : "http://www.directfb.org/")), false );


//    PageLoadContext page_load_ctx = { page, argv[1] ? : "http://www.directfb.org/" };

//    g_timeout_add( 10000, page_load, &page_load_ctx );


    PageRenderContext page_render_ctx = { page, surface };

    g_timeout_add( 2000, page_render, &page_render_ctx );


    //g_main_iteration(false);
    g_main_loop_run(loop);



    lite_close();

    dfb->Release( dfb );

    return 0;
}

