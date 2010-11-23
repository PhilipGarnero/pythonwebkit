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

/**
 * @brief  This file contains definitions for the LiTE WebView 
 *         structure.
 * @ingroup LiTE
 * @file webview.h
 * <hr>
 **/

#ifndef __LITE__WEBVIEW_H__
#define __LITE__WEBVIEW_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <lite/box.h>
#include <lite/theme.h>

/* @brief Macro to convert a generic LiteBox into a LiteWebView */
#define LITE_WEBVIEW(l) ((LiteWebView*)(l))

/* @brief LiteWebView structure */
typedef struct _LiteWebView LiteWebView;

/* @brief WebView theme */
typedef struct _LiteWebViewTheme {
     LiteTheme thene;           /**< base LiTE theme */
} LiteWebViewTheme; 
                                   
/* @brief Standard WebView themes */
/* @brief No WebView theme */
#define liteNoWebViewTheme       NULL

/* @brief default WebView theme */
extern LiteWebViewTheme *liteDefaultWebViewTheme;


/* @brief Callback function prototype for WebView navigation
 * 
 * @param webview              IN:     Valid LiteWebView object
 * @param url                  IN:     URL of navigation request
 *
 * @return void
*/
typedef void (*WebViewNavigationFunc)( LiteWebView *webview, void *data, const char *url );

/* @brief Create a new LiteWebView object
 * This function will create a new LiteWebView object.
 *
 * @param parent                IN:     Valid parent LiteBox
 * @rect                        IN:     Rectangle coordinates for the WebView
 * @theme                       IN:     WebView theme
 * @param ret_webview           OUT:    Returns a valid LiteWebView object
 * 
 * @return DFBResult            Returns DFB_OK if successful.     
 */
DFBResult lite_new_webview( LiteBox           *parent,
                            DFBRectangle      *rect,
                            LiteWebViewTheme  *theme,
                            LiteWebView      **ret_webview );


/* @brief Load a web page
 * This function will load a web page.
 * 
 * @param webview               IN:     Valid LiteWebView object
 * @param url                   IN:     URL to load
 *
 * @return DFBResult            Returns DFB_OK if successful.     
 */
DFBResult lite_webview_load( LiteWebView *webview, const char *url );

DFBResult lite_webview_go_back    ( LiteWebView *webview );
DFBResult lite_webview_go_forward ( LiteWebView *webview );
DFBResult lite_webview_stop       ( LiteWebView *webview );
DFBResult lite_webview_reload     ( LiteWebView *webview );

DFBResult lite_webview_scroll_down( LiteWebView *webview );
DFBResult lite_webview_scroll_up  ( LiteWebView *webview );

/* @brief Install the WebView navigation callback
 * This function will install a WebView navigation callback. This
 * callback is triggered every time the WebView navigates to a page.
 * 
 * @param webview               IN:     Valid LiteWebView object
 * @param callback              IN:     Valid callback function
 * @param data                  IN:     Data context
 *
 * @return DFBResult            Returns DFB_OK if successful.     
 */
DFBResult lite_on_webview_navigation( LiteWebView           *webview,
                                      WebViewNavigationFunc  callback,
                                      void                  *data );

DFBResult __lite_webview_navigation ( LiteWebView           *webview,
                                      const char            *url );


#ifdef __cplusplus
}
#endif

#endif /*  __LITE__WEBVIEW_H__  */
