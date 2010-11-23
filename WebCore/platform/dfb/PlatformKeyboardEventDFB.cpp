/*
 * Copyright (C) 2007 Ryan Leavengood <leavengood@gmail.com>
 * Copyright (C) 2008 Andrea Anzani <andrea.anzani@gmail.com>
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
#include "PlatformKeyboardEvent.h"

#include "KeyboardCodes.h"
#include "NotImplemented.h"

#include <glib.h>
#include <glib/gunicode.h>

#include <directfb.h>


namespace WebCore {


static String keyIdentifierForDFBKeySymbol(DFBInputDeviceKeySymbol symbol)
{
    switch (symbol) {
        case DIKS_MENU:
        case DIKS_ALT:
            return "Alt";
        case DIKS_CLEAR:
            return "Clear";
        case DIKS_CURSOR_DOWN:
            return "Down";
            // "End"
        case DIKS_END:
            return "End";
            // "Enter"
        case DIKS_ENTER:
            return "Enter";
        case DIKS_OK: //Execute:
            return "Execute";
        case DIKS_F1:
            return "F1";
        case DIKS_F2:
            return "F2";
        case DIKS_F3:
            return "F3";
        case DIKS_F4:
            return "F4";
        case DIKS_F5:
            return "F5";
        case DIKS_F6:
            return "F6";
        case DIKS_F7:
            return "F7";
        case DIKS_F8:
            return "F8";
        case DIKS_F9:
            return "F9";
        case DIKS_F10:
            return "F10";
        case DIKS_F11:
            return "F11";
        case DIKS_F12:
            return "F12";
        case DIKS_HELP:
            return "Help";
        case DIKS_HOME:
            return "Home";
        case DIKS_INSERT:
            return "Insert";
        case DIKS_CURSOR_LEFT:
            return "Left";
        case DIKS_PAGE_DOWN:
            return "PageDown";
        case DIKS_PAGE_UP:
            return "PageUp";
        case DIKS_PAUSE:
            return "Pause";
        case DIKS_PRINT:
            return "PrintScreen";
        case DIKS_CURSOR_RIGHT:
            return "Right";
        case DIKS_SELECT:
            return "Select";
        case DIKS_CURSOR_UP:
            return "Up";
            // Standard says that DEL becomes U+007F.
        case DIKS_DELETE:
            return "U+007F";
        case DIKS_TAB:
            return "U+0009";
        default:
            return String::format("U+%04X", symbol);
    }
}

static int windowsKeyCodeForKeyEvent(DFBInputDeviceKeySymbol     symbol,
                                     DFBInputDeviceKeyIdentifier identifier)
{
    switch (identifier) {
        case DIKI_KP_0:
            return VK_NUMPAD0;// (60) Numeric keypad 0 key
        case DIKI_KP_1:
            return VK_NUMPAD1;// (61) Numeric keypad 1 key
        case DIKI_KP_2:
            return VK_NUMPAD2; // (62) Numeric keypad 2 key
        case DIKI_KP_3:
            return VK_NUMPAD3; // (63) Numeric keypad 3 key
        case DIKI_KP_4:
            return VK_NUMPAD4; // (64) Numeric keypad 4 key
        case DIKI_KP_5:
            return VK_NUMPAD5; //(65) Numeric keypad 5 key
        case DIKI_KP_6:
            return VK_NUMPAD6; // (66) Numeric keypad 6 key
        case DIKI_KP_7:
            return VK_NUMPAD7; // (67) Numeric keypad 7 key
        case DIKI_KP_8:
            return VK_NUMPAD8; // (68) Numeric keypad 8 key
        case DIKI_KP_9:
            return VK_NUMPAD9; // (69) Numeric keypad 9 key
        case DIKI_KP_MULT:
            return VK_MULTIPLY; // (6A) Multiply key
        case DIKI_KP_PLUS:
            return VK_ADD; // (6B) Add key
        case DIKI_KP_MINUS:
            return VK_SUBTRACT; // (6D) Subtract key
        case DIKI_KP_DECIMAL:
            return VK_DECIMAL; // (6E) Decimal key
        case DIKI_KP_DIV:
            return VK_DIVIDE; // (6F) Divide key

        case DIKI_0:
            return VK_0;    //  (30) 0) key
        case DIKI_1:
            return VK_1; //  (31) 1 ! key
        case DIKI_2:
            return VK_2; //  (32) 2 & key
        case DIKI_3:
            return VK_3; //case '3': case '#';
        case DIKI_4:
            return VK_4;
        case DIKI_5:
            return VK_5; //  (35) 5 key  '%'
        case DIKI_6:
            return VK_6; //  (36) 6 key  '^'
        case DIKI_7:
            return VK_7; //  (37) 7 key  case '&'
        case DIKI_8:
            return VK_8; //  (38) 8 key  '*'
        case DIKI_9:
            return VK_9; //  (39) 9 key '('

        case DIKI_META_L:
            return VK_LWIN; // (5B) Left Windows key (Microsoft Natural keyboard)
        case DIKI_META_R:
            return VK_RWIN; // (5C) Right Windows key (Natural keyboard)

        case DIKI_SHIFT_L:
            return VK_LSHIFT;
        case DIKI_SHIFT_R:
            return VK_RSHIFT;


        case DIKI_SEMICOLON:
            return VK_OEM_1; //case ';': case ':': return 0xBA;
            // VK_OEM_PLUS (BB) Windows 2000/XP: For any country/region, the '+' key
        case DIKI_EQUALS_SIGN:
            return VK_OEM_PLUS; //case '=': case '+': return 0xBB;
            // VK_OEM_COMMA (BC) Windows 2000/XP: For any country/region, the ',' key
        case DIKI_COMMA:
            return VK_OEM_COMMA; //case ',': case '<': return 0xBC;
            // VK_OEM_MINUS (BD) Windows 2000/XP: For any country/region, the '-' key
        case DIKI_MINUS_SIGN:
            return VK_OEM_MINUS; //case '-': case '_': return 0xBD;
            // VK_OEM_PERIOD (BE) Windows 2000/XP: For any country/region, the '.' key
        case DIKI_PERIOD:
            return VK_OEM_PERIOD; //case '.': case '>': return 0xBE;
            // VK_OEM_2 (BF) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '/?' key
        case DIKI_SLASH:
            return VK_OEM_2; //case '/': case '?': return 0xBF;
            // VK_OEM_3 (C0) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '`~' key
        case DIKI_QUOTE_LEFT:
            return VK_OEM_3; //case '`': case '~': return 0xC0;
            // VK_OEM_4 (DB) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '[{' key
        case DIKI_BRACKET_LEFT:
            return VK_OEM_4; //case '[': case '{': return 0xDB;
            // VK_OEM_5 (DC) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '\|' key
        case DIKI_BACKSLASH:
            return VK_OEM_5; //case '\\': case '|': return 0xDC;
            // VK_OEM_6 (DD) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the ']}' key
        case DIKI_BRACKET_RIGHT:
            return VK_OEM_6; // case ']': case '}': return 0xDD;
            // VK_OEM_7 (DE) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
        case DIKI_QUOTE_RIGHT:
            return VK_OEM_7; // case '\'': case '"': return 0xDE;
            // VK_OEM_8 (DF) Used for miscellaneous characters; it can vary by keyboard.
            // VK_OEM_102 (E2) Windows 2000/XP: Either the angle bracket key or the backslash key on the RT 102-key keyboard
            // VK_PROCESSKEY (E5) Windows 95/98/Me, Windows NT 4.0, Windows 2000/XP: IME PROCESS key
            // VK_PACKET (E7) Windows 2000/XP: Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT,SendInput, WM_KEYDOWN, and WM_KEYUP
            // VK_ATTN (F6) Attn key
            // VK_CRSEL (F7) CrSel key
            // VK_EXSEL (F8) ExSel key
            // VK_EREOF (F9) Erase EOF key
            // VK_PLAY (FA) Play key
            // VK_ZOOM (FB) Zoom key
            // VK_NONAME (FC) Reserved for future use
            // VK_PA1 (FD) PA1 key
            // VK_OEM_CLEAR (FE) Clear key

        default:
            break;
    }

    switch (symbol) {
        case DIKS_BACKSPACE:
            return VK_BACK; // (08) BACKSPACE key
        case DIKS_TAB:
            return VK_TAB; // (09) TAB key
        case DIKS_CLEAR:
            return VK_CLEAR; // (0C) CLEAR key
        case DIKS_ENTER:
            return VK_RETURN; //(0D) Return key
        case DIKS_SHIFT:
            return VK_SHIFT; // (10) SHIFT key
        case DIKS_CONTROL:
            return VK_CONTROL; // (11) CTRL key
        case DIKS_MENU:
        case DIKS_ALT:
            return VK_MENU; // (12) ALT key

        case DIKS_PAUSE:
            return VK_PAUSE; // (13) PAUSE key
        case DIKS_CAPS_LOCK:
            return VK_CAPITAL; // (14) CAPS LOCK key
//        case DIKS_Kana_Lock:
//        case DIKS_Kana_Shift:
//            return VK_KANA; // (15) Input Method Editor (IME) Kana mode
//        case DIKS_Hangul:
//            return VK_HANGUL; // VK_HANGUL (15) IME Hangul mode
            // VK_JUNJA (17) IME Junja mode
            // VK_FINAL (18) IME final mode
//        case DIKS_Hangul_Hanja:
//            return VK_HANJA; // (19) IME Hanja mode
//        case DIKS_Kanji:
//            return VK_KANJI; // (19) IME Kanji mode
        case DIKS_ESCAPE:
            return VK_ESCAPE; // (1B) ESC key
            // VK_CONVERT (1C) IME convert
            // VK_NONCONVERT (1D) IME nonconvert
            // VK_ACCEPT (1E) IME accept
            // VK_MODECHANGE (1F) IME mode change request
        case DIKS_SPACE:
            return VK_SPACE; // (20) SPACEBAR
        case DIKS_PAGE_UP:
            return VK_PRIOR; // (21) PAGE UP key
        case DIKS_PAGE_DOWN:
            return VK_NEXT; // (22) PAGE DOWN key
        case DIKS_END:
            return VK_END; // (23) END key
        case DIKS_HOME:
            return VK_HOME; // (24) HOME key
        case DIKS_CURSOR_LEFT:
            return VK_LEFT; // (25) LEFT ARROW key
        case DIKS_CURSOR_UP:
            return VK_UP; // (26) UP ARROW key
        case DIKS_CURSOR_RIGHT:
            return VK_RIGHT; // (27) RIGHT ARROW key
        case DIKS_CURSOR_DOWN:
            return VK_DOWN; // (28) DOWN ARROW key
        case DIKS_SELECT:
            return VK_SELECT; // (29) SELECT key
        case DIKS_PRINT:
            return VK_PRINT; // (2A) PRINT key
        case DIKS_OK://Execute:
            return VK_EXECUTE;// (2B) EXECUTE key
            //dunno on this
            //case DIKS_PrintScreen:
            //      return VK_SNAPSHOT; // (2C) PRINT SCREEN key
        case DIKS_INSERT:
            return VK_INSERT; // (2D) INS key
        case DIKS_DELETE:
            return VK_DELETE; // (2E) DEL key
        case DIKS_HELP:
            return VK_HELP; // (2F) HELP key
        case 'a':
        case 'A':
            return VK_A; //  (41) A key case 'a'': case 'A'': return 0x41;
        case 'b':
        case 'B':
            return VK_B; //  (42) B key case 'b'': case 'B'': return 0x42;
        case 'c':
        case 'C':
            return VK_C; //  (43) C key case 'c'': case 'C'': return 0x43;
        case 'd':
        case 'D':
            return VK_D; //  (44) D key case 'd'': case 'D'': return 0x44;
        case 'e':
        case 'E':
            return VK_E; //  (45) E key case 'e'': case 'E'': return 0x45;
        case 'f':
        case 'F':
            return VK_F; //  (46) F key case 'f'': case 'F'': return 0x46;
        case 'g':
        case 'G':
            return VK_G; //  (47) G key case 'g'': case 'G'': return 0x47;
        case 'h':
        case 'H':
            return VK_H; //  (48) H key case 'h'': case 'H'': return 0x48;
        case 'i':
        case 'I':
            return VK_I; //  (49) I key case 'i'': case 'I'': return 0x49;
        case 'j':
        case 'J':
            return VK_J; //  (4A) J key case 'j'': case 'J'': return 0x4A;
        case 'k':
        case 'K':
            return VK_K; //  (4B) K key case 'k'': case 'K'': return 0x4B;
        case 'l':
        case 'L':
            return VK_L; //  (4C) L key case 'l'': case 'L'': return 0x4C;
        case 'm':
        case 'M':
            return VK_M; //  (4D) M key case 'm'': case 'M'': return 0x4D;
        case 'n':
        case 'N':
            return VK_N; //  (4E) N key case 'n'': case 'N'': return 0x4E;
        case 'o':
        case 'O':
            return VK_O; //  (4F) O key case 'o'': case 'O'': return 0x4F;
        case 'p':
        case 'P':
            return VK_P; //  (50) P key case 'p'': case 'P'': return 0x50;
        case 'q':
        case 'Q':
            return VK_Q; //  (51) Q key case 'q'': case 'Q'': return 0x51;
        case 'r':
        case 'R':
            return VK_R; //  (52) R key case 'r'': case 'R'': return 0x52;
        case 's':
        case 'S':
            return VK_S; //  (53) S key case 's'': case 'S'': return 0x53;
        case 't':
        case 'T':
            return VK_T; //  (54) T key case 't'': case 'T'': return 0x54;
        case 'u':
        case 'U':
            return VK_U; //  (55) U key case 'u'': case 'U'': return 0x55;
        case 'v':
        case 'V':
            return VK_V; //  (56) V key case 'v'': case 'V'': return 0x56;
        case 'w':
        case 'W':
            return VK_W; //  (57) W key case 'w'': case 'W'': return 0x57;
        case 'x':
        case 'X':
            return VK_X; //  (58) X key case 'x'': case 'X'': return 0x58;
        case 'y':
        case 'Y':
            return VK_Y; //  (59) Y key case 'y'': case 'Y'': return 0x59;
        case 'z':
        case 'Z':
            return VK_Z; //  (5A) Z key case 'z'': case 'Z'': return 0x5A;

        case DIKS_NUM_LOCK:
            return VK_NUMLOCK; // (90) NUM LOCK key

        case DIKS_SCROLL_LOCK:
            return VK_SCROLL; // (91) SCROLL LOCK key

            // VK_LSHIFT (A0) Left SHIFT key
            // VK_RSHIFT (A1) Right SHIFT key
            // VK_LCONTROL (A2) Left CONTROL key
            // VK_RCONTROL (A3) Right CONTROL key
            // VK_LMENU (A4) Left MENU key
            // VK_RMENU (A5) Right MENU key
            // VK_BROWSER_BACK (A6) Windows 2000/XP: Browser Back key
            // VK_BROWSER_FORWARD (A7) Windows 2000/XP: Browser Forward key
            // VK_BROWSER_REFRESH (A8) Windows 2000/XP: Browser Refresh key
            // VK_BROWSER_STOP (A9) Windows 2000/XP: Browser Stop key
            // VK_BROWSER_SEARCH (AA) Windows 2000/XP: Browser Search key
            // VK_BROWSER_FAVORITES (AB) Windows 2000/XP: Browser Favorites key
            // VK_BROWSER_HOME (AC) Windows 2000/XP: Browser Start and Home key
            // VK_VOLUME_MUTE (AD) Windows 2000/XP: Volume Mute key
            // VK_VOLUME_DOWN (AE) Windows 2000/XP: Volume Down key
            // VK_VOLUME_UP (AF) Windows 2000/XP: Volume Up key
            // VK_MEDIA_NEXT_TRACK (B0) Windows 2000/XP: Next Track key
            // VK_MEDIA_PREV_TRACK (B1) Windows 2000/XP: Previous Track key
            // VK_MEDIA_STOP (B2) Windows 2000/XP: Stop Media key
            // VK_MEDIA_PLAY_PAUSE (B3) Windows 2000/XP: Play/Pause Media key
            // VK_LAUNCH_MAIL (B4) Windows 2000/XP: Start Mail key
            // VK_LAUNCH_MEDIA_SELECT (B5) Windows 2000/XP: Select Media key
            // VK_LAUNCH_APP1 (B6) Windows 2000/XP: Start Application 1 key
            // VK_LAUNCH_APP2 (B7) Windows 2000/XP: Start Application 2 key

            // VK_OEM_1 (BA) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the ';:' key
        case DIKS_F1:
        case DIKS_F2:
        case DIKS_F3:
        case DIKS_F4:
        case DIKS_F5:
        case DIKS_F6:
        case DIKS_F7:
        case DIKS_F8:
        case DIKS_F9:
        case DIKS_F10:
        case DIKS_F11:
        case DIKS_F12:
            return VK_F1 + (symbol - DIKS_F1);

        default:
            break;
    }

    return 0;
}

static String singleCharacterString(unsigned int val)
{
    switch (val) {
        case DIKS_ENTER:
            return String("\r");
        default:
            gunichar c = val;
            glong nwc;
            gunichar2* uchar16 = g_ucs4_to_utf16(&c, 1, 0, &nwc, 0);

            String retVal;
            if (uchar16)
                retVal = String((UChar*)uchar16, nwc);
            else
                retVal = String();

            g_free(uchar16);

            return retVal;
    }
}


PlatformKeyboardEvent::PlatformKeyboardEvent(const DFBEvent* event)
    : m_autoRepeat(false)
    , m_isKeypad(false)
    , m_shiftKey(false)
    , m_ctrlKey(false)
    , m_altKey(false)
    , m_metaKey(false)
{
    switch (event->clazz) {
        case DFEC_INPUT:
            m_text = singleCharacterString(event->input.key_symbol);
            m_keyIdentifier = keyIdentifierForDFBKeySymbol(event->input.key_symbol);
            m_windowsVirtualKeyCode = windowsKeyCodeForKeyEvent(event->input.key_symbol,
                                                                event->input.key_id);

            switch (event->input.type) {
                case DIET_KEYPRESS:
                    m_type = KeyDown;
                    break;

                case DIET_KEYRELEASE:
                    m_type = KeyUp;
                    break;

                default:
                    break;
            }
            break;

        case DFEC_WINDOW:
            m_text = singleCharacterString(event->window.key_symbol);
            m_keyIdentifier = keyIdentifierForDFBKeySymbol(event->window.key_symbol);
            m_windowsVirtualKeyCode = windowsKeyCodeForKeyEvent(event->window.key_symbol,
                                                                event->window.key_id);

            switch (event->window.type) {
                case DWET_KEYDOWN:
                    m_type = KeyDown;
                    break;

                case DWET_KEYUP:
                    m_type = KeyUp;
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }

    m_unmodifiedText = m_text;
}

void PlatformKeyboardEvent::disambiguateKeyDownEvent(Type type, bool)
{
    // Can only change type from KeyDown to RawKeyDown or Char, as we lack information for other conversions.
    ASSERT(m_type == KeyDown);
    m_type = type;
    if (type == RawKeyDown) {
        m_text = String();
        m_unmodifiedText = String();
    } else {
        m_keyIdentifier = String();
        m_windowsVirtualKeyCode = 0;
    }
}

bool PlatformKeyboardEvent::currentCapsLockState()
{
    notImplemented();
    return false;
}

void PlatformKeyboardEvent::getCurrentModifierState(bool& shiftKey, bool& ctrlKey, bool& altKey, bool& metaKey)
{
    notImplemented();
    shiftKey = false;
    ctrlKey = false;
    altKey = false;
    metaKey = false;
}

} // namespace WebCore

