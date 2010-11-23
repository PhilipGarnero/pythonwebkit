/*
 * Copyright (C) 2006, 2007 Apple Inc.
 * Copyright (C) 2007 Alp Toker <alp@atoker.com>
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

#include <direct/thread.h>

#include <glib.h>
#include <glib-object.h>

#include <lite/lite.h>
#include <lite/window.h>

#include <leck/textbutton.h>
#include <leck/textline.h>

#include <webkitdfb.h>
#include <webview.h>

static gboolean
handle_events(gpointer data)
{
     lite_window_event_loop( data, -1 );

     return TRUE;
}

/*
static gboolean
register_handler(gpointer data)
{
     g_timeout_add( 20, handle_events, data );

     return FALSE;
}

static gboolean
refresh_webview(gpointer data)
{
     LiteWebView *webview = data;

     lite_update_box( LITE_BOX(webview), NULL );

     return TRUE;
}
*/

static void
webview_navigation( LiteWebView *webview, void *ctx, const char *url )
{
     LiteTextLine *text_line = ctx;

     lite_set_textline_text( text_line, url );
}

static void
back_button_press( LiteTextButton *button, void *ctx )
{
     LiteWebView *webview = ctx;

     lite_webview_go_back( webview );
}

static void
forward_button_press( LiteTextButton *button, void *ctx )
{
     LiteWebView *webview = ctx;

     lite_webview_go_forward( webview );
}

static void
stop_button_press( LiteTextButton *button, void *ctx )
{
     LiteWebView *webview = ctx;

     lite_webview_stop( webview );
}

static void
reload_button_press( LiteTextButton *button, void *ctx )
{
     LiteWebView *webview = ctx;

     lite_webview_reload( webview );
}

static void
text_line_enter( const char *text, void *ctx )
{
     LiteWebView *webview = ctx;

     printf("enter: %s\n", text);

     lite_webview_load( webview, text );
}

/*
static void *
glib_main_thread( DirectThread *thread,
                  void         *ctx )
{
     while (true) {
          g_main_iteration( TRUE );
     }

     return NULL;
}
*/

static DFBResult
window_keyboard( DFBWindowEvent* evt, void *ctx )
{
     LiteWebView *webview = ctx;

     switch (evt->key_symbol) {
          case DIKS_CURSOR_UP:
               if (evt->type == DWET_KEYDOWN)
                    lite_webview_scroll_up( webview );
               return DFB_FAILURE;

          case DIKS_CURSOR_DOWN:
               if (evt->type == DWET_KEYDOWN)
                    lite_webview_scroll_down( webview );
               return DFB_FAILURE;

          default:
               break;
     }

     return DFB_OK;
}

static int timer_id;

static DFBResult timeout_cb(void* data)
{
    printf("lite timer, fire g_loop\n");
    g_main_context_iteration(NULL, FALSE);
    lite_enqueue_window_timeout(200, timeout_cb, NULL, &timer_id);
    return DFB_OK;
}

void webkit_set_lite_global_mainwin(LiteWindow *win);


int
main (int argc, char* argv[])
{
     GMainLoop      *loop;
     LiteWindow     *window;
     LiteWebView    *webview;
     LiteTextButton *back_button;
     LiteTextButton *forward_button;
     LiteTextButton *stop_button;
     LiteTextButton *reload_button;
     LiteTextLine   *text_line;
//     DirectThread   *glib_thread;

     g_type_init();

     g_thread_init(NULL);



     lite_open( &argc, &argv );

     WebKitDFB_Initialize( lite_get_dfb_interface() );

     IDirectFBDisplayLayer *layer;
     DFBDisplayLayerConfig  config;
     lite_get_layer_interface( &layer );
     layer->GetConfiguration( layer, &config );

     DFBRectangle windowRect        = {   0,  0, config.width, config.height };
     DFBRectangle backButtonRect    = {   5,  5,  80,  25 };
     DFBRectangle forwardButtonRect = {  90,  5,  80,  25 };
     DFBRectangle stopButtonRect    = { 175,  5,  80,  25 };
     DFBRectangle reloadButtonRect  = { 260,  5,  80,  25 };
     DFBRectangle textlineRect      = { 345,  5, config.width - 350,  25 };
     DFBRectangle webviewRect       = {   0, 35, config.width, config.height - 35 };


     lite_new_window( NULL, &windowRect, DWCAPS_NONE, liteNoWindowTheme, "WebKitDFB", &window );

     webkit_set_lite_global_mainwin(window);

     lite_new_webview( LITE_BOX(window), &webviewRect, liteDefaultWebViewTheme, &webview );


     lite_on_raw_window_keyboard( window, window_keyboard, webview );


     lite_new_text_button_theme( DATADIR"/textbuttonbgnd.png", &liteDefaultTextButtonTheme );

     lite_new_text_button( LITE_BOX(window), &backButtonRect, "Back", liteDefaultTextButtonTheme, &back_button );
     lite_on_text_button_press( back_button, back_button_press, webview );

     lite_new_text_button( LITE_BOX(window), &forwardButtonRect, "Forward", liteDefaultTextButtonTheme, &forward_button );
     lite_on_text_button_press( forward_button, forward_button_press, webview );

     lite_new_text_button( LITE_BOX(window), &stopButtonRect, "Stop", liteDefaultTextButtonTheme, &stop_button );
     lite_on_text_button_press( stop_button, stop_button_press, webview );

     lite_new_text_button( LITE_BOX(window), &reloadButtonRect, "Reload", liteDefaultTextButtonTheme, &reload_button );
     lite_on_text_button_press( reload_button, reload_button_press, webview );


     lite_new_textline( LITE_BOX(window), &textlineRect, liteDefaultTextLineTheme, &text_line );

     lite_on_textline_enter( text_line, text_line_enter, webview );

     lite_focus_box( LITE_BOX(text_line) );


     lite_set_window_opacity( window, 0xff );


     //loop = g_main_loop_new(0,TRUE);


     lite_on_webview_navigation( webview, webview_navigation, text_line );

    printf("pre load\n");
     lite_webview_load( webview, argv[1] ? : "http://www.directfb.org/" );
    printf("post load\n");


     //g_timeout_add( 20, handle_events, window );
/*
     g_timeout_add( 500, register_handler, window );

     g_timeout_add( 1000, refresh_webview, webview );

     g_main_loop_run(loop);
*/

//     glib_thread = direct_thread_create( DTT_DEFAULT, glib_main_thread, NULL, "GLib Main" );

     lite_enqueue_window_timeout(200, timeout_cb, NULL, &timer_id);
     while (true) {
          g_main_iteration( FALSE );

          lite_window_event_loop( window, 20 );
     }


     /*g_main_loop_run(loop);*/


     lite_close();

     return 0;
}
