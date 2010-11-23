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
#include "EditorClientDFB.h"

#include "EditCommand.h"
#include "Editor.h"
#include "FocusController.h"
#include "Frame.h"
#include "FrameView.h"
#include "KeyboardEvent.h"
#include "KeyboardCodes.h"
#include "NotImplemented.h"
#include "Page.h"
#include "PlatformKeyboardEvent.h"
#include "PlatformString.h"
#include "SelectionController.h"

//#include "WebFrame.h"
//#include "WebFramePrivate.h"
//#include "WebView.h"
//#include "WebViewPrivate.h"

#include <stdio.h>

namespace WebCore {

static const unsigned CtrlKey = 1 << 0;
static const unsigned AltKey = 1 << 1;
static const unsigned ShiftKey = 1 << 2;

struct KeyDownEntry {
    unsigned virtualKey;
    unsigned modifiers;
    const char* name;
};

struct KeyPressEntry {
    unsigned charCode;
    unsigned modifiers;
    const char* name;
};

static const KeyDownEntry keyDownEntries[] = {
    { VK_LEFT,   0,                  "MoveLeft"                                    },
    { VK_LEFT,   ShiftKey,           "MoveLeftAndModifySelection"                  },
    { VK_LEFT,   CtrlKey,            "MoveWordLeft"                                },
    { VK_LEFT,   CtrlKey | ShiftKey, "MoveWordLeftAndModifySelection"              },
    { VK_RIGHT,  0,                  "MoveRight"                                   },
    { VK_RIGHT,  ShiftKey,           "MoveRightAndModifySelection"                 },
    { VK_RIGHT,  CtrlKey,            "MoveWordRight"                               },
    { VK_RIGHT,  CtrlKey | ShiftKey, "MoveWordRightAndModifySelection"             },
    { VK_UP,     0,                  "MoveUp"                                      },
    { VK_UP,     ShiftKey,           "MoveUpAndModifySelection"                    },
    { VK_PRIOR,  ShiftKey,           "MovePageUpAndModifySelection"                },
    { VK_DOWN,   0,                  "MoveDown"                                    },
    { VK_DOWN,   ShiftKey,           "MoveDownAndModifySelection"                  },
    { VK_NEXT,   ShiftKey,           "MovePageDownAndModifySelection"              },
    { VK_PRIOR,  0,                  "MovePageUp"                                  },
    { VK_NEXT,   0,                  "MovePageDown"                                },
    { VK_HOME,   0,                  "MoveToBeginningOfLine"                       },
    { VK_HOME,   ShiftKey,           "MoveToBeginningOfLineAndModifySelection"     },
    { VK_HOME,   CtrlKey,            "MoveToBeginningOfDocument"                   },
    { VK_HOME,   CtrlKey | ShiftKey, "MoveToBeginningOfDocumentAndModifySelection" },

    { VK_END,    0,                  "MoveToEndOfLine"                             },
    { VK_END,    ShiftKey,           "MoveToEndOfLineAndModifySelection"           },
    { VK_END,    CtrlKey,            "MoveToEndOfDocument"                         },
    { VK_END,    CtrlKey | ShiftKey, "MoveToEndOfDocumentAndModifySelection"       },

    { VK_BACK,   0,                  "DeleteBackward"                              },
    { VK_BACK,   ShiftKey,           "DeleteBackward"                              },
    { VK_DELETE, 0,                  "DeleteForward"                               },
    { VK_BACK,   CtrlKey,            "DeleteWordBackward"                          },
    { VK_DELETE, CtrlKey,            "DeleteWordForward"                           },
    
    { 'B',       CtrlKey,            "ToggleBold"                                  },
    { 'I',       CtrlKey,            "ToggleItalic"                                },

    { VK_ESCAPE, 0,                  "Cancel"                                      },
    //FIXME: this'll never happen. We can trash it or make it a normal period
    { VK_OEM_PERIOD, CtrlKey,        "Cancel"                                      },
    { VK_TAB,    0,                  "InsertTab"                                   },
    { VK_TAB,    ShiftKey,           "InsertBacktab"                               },
    { VK_RETURN, 0,                  "InsertNewline"                               },
    { VK_RETURN, CtrlKey,            "InsertNewline"                               },
    { VK_RETURN, AltKey,             "InsertNewline"                               },
    { VK_RETURN, AltKey | ShiftKey,  "InsertNewline"                               },
    { 'A',       CtrlKey,            "SelectAll"                                   },
    { 'Z',       CtrlKey,            "Undo"                                        },
    { 'Z',       CtrlKey | ShiftKey, "Redo"                                        },
};

static const KeyPressEntry keyPressEntries[] = {
    { '\t',   0,                  "InsertTab"                                   },
    { '\t',   ShiftKey,           "InsertBacktab"                               },
    { '\r',   0,                  "InsertNewline"                               },
    { '\r',   CtrlKey,            "InsertNewline"                               },
    { '\r',   AltKey,             "InsertNewline"                               },
    { '\r',   AltKey | ShiftKey,  "InsertNewline"                               },
};

EditorClientDFB::~EditorClientDFB()
{
    m_page = NULL;
}

void EditorClientDFB::setPage(Page* page)
{
    m_page = page;
}

void EditorClientDFB::pageDestroyed()
{
    delete this;
}

bool EditorClientDFB::shouldDeleteRange(Range*)
{
    notImplemented();
    return true;
}

bool EditorClientDFB::shouldShowDeleteInterface(HTMLElement*)
{
    notImplemented();
    return false;
}

bool EditorClientDFB::smartInsertDeleteEnabled()
{
    notImplemented();
    return false;
}

bool EditorClientDFB::isSelectTrailingWhitespaceEnabled()
{
    notImplemented();
    return false;
}

bool EditorClientDFB::isContinuousSpellCheckingEnabled()
{
    notImplemented();
    return false;
}

void EditorClientDFB::toggleContinuousSpellChecking()
{
    notImplemented();
}

bool EditorClientDFB::isGrammarCheckingEnabled()
{
    notImplemented();
    return false;
}

void EditorClientDFB::toggleGrammarChecking()
{
    notImplemented();
}

int EditorClientDFB::spellCheckerDocumentTag()
{
    notImplemented();
    return 0;
}

bool EditorClientDFB::selectWordBeforeMenuEvent()
{
    notImplemented();
    return false;
}

bool EditorClientDFB::isEditable()
{
    notImplemented();
/*
    Frame* frame = m_page->focusController()->focusedOrMainFrame();

    if (frame) {
        DFBWebView* webKitWin = dynamic_cast<DFBWebView*>(frame->view()->hostWindow()->platformPageClient());
        if (webKitWin) 
            return webKitWin->IsEditable();
    }
*/
    return false;
}

bool EditorClientDFB::shouldBeginEditing(Range*)
{
    notImplemented();
    return true;
}

bool EditorClientDFB::shouldEndEditing(Range*)
{
    notImplemented();
    return true;
}

bool EditorClientDFB::shouldInsertNode(Node*, Range*,
                                       EditorInsertAction)
{
    notImplemented();
    return true;
}

bool EditorClientDFB::shouldInsertText(const String&, Range*,
                                       EditorInsertAction)
{
    notImplemented();
    return true;
}

bool EditorClientDFB::shouldApplyStyle(CSSStyleDeclaration*,
                                       Range*)
{
    notImplemented();
    return true;
}

bool EditorClientDFB::shouldMoveRangeAfterDelete(Range*, Range*)
{
    notImplemented();
    return true;
}

bool EditorClientDFB::shouldChangeSelectedRange(Range* fromRange, Range* toRange, 
                                EAffinity, bool stillSelecting)
{
    notImplemented();
    return true;
}

void EditorClientDFB::didBeginEditing()
{
    notImplemented();
}

void EditorClientDFB::respondToChangedContents()
{
    notImplemented();
}

void EditorClientDFB::didEndEditing()
{
    notImplemented();
}

void EditorClientDFB::didWriteSelectionToPasteboard()
{
    notImplemented();
}

void EditorClientDFB::didSetSelectionTypesForPasteboard()
{
    notImplemented();
}

void EditorClientDFB::registerCommandForUndo(PassRefPtr<EditCommand> command)
{
    notImplemented();
/*
    Frame* frame = m_page->focusController()->focusedOrMainFrame();

    if (frame) {
        DFBWebView* webKitWin = dynamic_cast<DFBWebView*>(frame->view()->hostWindow()->platformPageClient());
        if (webKitWin) {
            webKitWin->m_impl->undoStack.append(EditCommandDFB(command));
        }
    }
*/
}

void EditorClientDFB::registerCommandForRedo(PassRefPtr<EditCommand> command)
{
    notImplemented();
/*
    Frame* frame = m_page->focusController()->focusedOrMainFrame();

    if (frame) {
        DFBWebView* webKitWin = dynamic_cast<DFBWebView*>(frame->view()->hostWindow()->platformPageClient());
        if (webKitWin) {
            webKitWin->m_impl->redoStack.insert(0, EditCommandDFB(command));
        }
    }
*/
}

void EditorClientDFB::clearUndoRedoOperations()
{
    notImplemented();
/*
    Frame* frame = m_page->focusController()->focusedOrMainFrame();
    
    if (frame) {
        DFBWebView* webKitWin = dynamic_cast<DFBWebView*>(frame->view()->hostWindow()->platformPageClient());
        if (webKitWin) {
            webKitWin->m_impl->redoStack.clear();
            webKitWin->m_impl->undoStack.clear();
        }
    }
*/
}

bool EditorClientDFB::canUndo() const
{
    notImplemented();
/*
    Frame* frame = m_page->focusController()->focusedOrMainFrame();

    if (frame) {
        DFBWebView* webKitWin = dynamic_cast<DFBWebView*>(frame->view()->hostWindow()->platformPageClient());
        if (webKitWin) {
            return webKitWin->m_impl->undoStack.size() != 0;
        }
    }
*/
    return false;
}

bool EditorClientDFB::canRedo() const
{
    notImplemented();
/*
    Frame* frame = m_page->focusController()->focusedOrMainFrame();

    if (frame) {
        DFBWebView* webKitWin = dynamic_cast<DFBWebView*>(frame->view()->hostWindow()->platformPageClient());
        if (webKitWin && webKitWin) {
            return webKitWin->m_impl->redoStack.size() != 0;
        }
    }
*/
    return false;
}

void EditorClientDFB::undo()
{
    notImplemented();
/*
    Frame* frame = m_page->focusController()->focusedOrMainFrame();

    if (frame) {
        DFBWebView* webKitWin = dynamic_cast<DFBWebView*>(frame->view()->hostWindow()->platformPageClient());
        if (webKitWin) {
            webKitWin->m_impl->undoStack.last().editCommand()->unapply();
            webKitWin->m_impl->undoStack.removeLast();
        }
    }
*/
}

void EditorClientDFB::redo()
{
    notImplemented();
/*
    Frame* frame = m_page->focusController()->focusedOrMainFrame();

    if (frame) {    
        DFBWebView* webKitWin = dynamic_cast<DFBWebView*>(frame->view()->hostWindow()->platformPageClient());
        if (webKitWin) {
            webKitWin->m_impl->redoStack.first().editCommand()->reapply();
            webKitWin->m_impl->redoStack.remove(0);
        }
    }
*/
}

bool EditorClientDFB::handleEditingKeyboardEvent(KeyboardEvent* event)
{
    notImplemented();
    return false;
/*
    Node* node = event->target()->toNode();
    ASSERT(node);
    Frame* frame = node->document()->frame();
    ASSERT(frame);

    const PlatformKeyboardEvent* keyEvent = event->keyEvent();

    //NB: this is what windows does, but they also have a keypress event for Alt+Enter which clearly won't get hit with this
    if (!keyEvent || keyEvent->altKey())  // do not treat this as text input if Alt is down
        return false;

    Editor::Command command = frame->editor()->command(interpretKeyEvent(event));

    if (keyEvent->type() == PlatformKeyboardEvent::RawKeyDown) {
        // WebKit doesn't have enough information about mode to decide how commands that just insert text if executed via Editor should be treated,
        // so we leave it upon WebCore to either handle them immediately (e.g. Tab that changes focus) or if not to let a CHAR event be generated
        // (e.g. Tab that inserts a Tab character, or Enter).
        return !command.isTextInsertion() && command.execute(event);
    }

     if (command.execute(event))
        return true;

    // Don't insert null or control characters as they can result in unexpected behaviour
    if (event->charCode() < ' ')
        return false;

    return frame->editor()->insertText(event->keyEvent()->text(), event);
*/
}

const char* EditorClientDFB::interpretKeyEvent(const KeyboardEvent* evt)
{
    notImplemented();
    return NULL;
/*
    ASSERT(evt->keyEvent()->type() == PlatformKeyboardEvent::RawKeyDown || evt->keyEvent()->type() == PlatformKeyboardEvent::Char);

    static HashMap<int, const char*>* keyDownCommandsMap = 0;
    static HashMap<int, const char*>* keyPressCommandsMap = 0;

    if (!keyDownCommandsMap) {
        keyDownCommandsMap = new HashMap<int, const char*>;
        keyPressCommandsMap = new HashMap<int, const char*>;

        for (unsigned i = 0; i < WXSIZEOF(keyDownEntries); i++)
            keyDownCommandsMap->set(keyDownEntries[i].modifiers << 16 | keyDownEntries[i].virtualKey, keyDownEntries[i].name);

        for (unsigned i = 0; i < WXSIZEOF(keyPressEntries); i++)
            keyPressCommandsMap->set(keyPressEntries[i].modifiers << 16 | keyPressEntries[i].charCode, keyPressEntries[i].name);
    }

    unsigned modifiers = 0;
    if (evt->shiftKey())
        modifiers |= ShiftKey;
    if (evt->altKey())
        modifiers |= AltKey;
    if (evt->ctrlKey())
        modifiers |= CtrlKey;

    if (evt->keyEvent()->type() == PlatformKeyboardEvent::RawKeyDown) {
        int mapKey = modifiers << 16 | evt->keyCode();
        return mapKey ? keyDownCommandsMap->get(mapKey) : 0;
    }

    int mapKey = modifiers << 16 | evt->charCode();
    return mapKey ? keyPressCommandsMap->get(mapKey) : 0;
*/
}


void EditorClientDFB::handleInputMethodKeydown(KeyboardEvent* event)
{
// NOTE: we don't currently need to handle this. When key events occur,
// both this method and handleKeyboardEvent get a chance at handling them.
// We might use this method later on for IME-specific handling.
}

void EditorClientDFB::handleKeyboardEvent(KeyboardEvent* event)
{
    notImplemented();
/*
    if (handleEditingKeyboardEvent(event))
        event->setDefaultHandled();
*/
}

void EditorClientDFB::textFieldDidBeginEditing(Element*)
{
    notImplemented();
}

void EditorClientDFB::textFieldDidEndEditing(Element*)
{
    notImplemented();
}

void EditorClientDFB::textDidChangeInTextField(Element*)
{
    notImplemented();
}

bool EditorClientDFB::doTextFieldCommandFromEvent(Element*, KeyboardEvent*)
{
    notImplemented();
    return false;
}

void EditorClientDFB::textWillBeDeletedInTextField(Element*)
{
    notImplemented();
}

void EditorClientDFB::textDidChangeInTextArea(Element*)
{
    notImplemented();
}

void EditorClientDFB::respondToChangedSelection()
{
    notImplemented();
}

void EditorClientDFB::ignoreWordInSpellDocument(const String&) 
{ 
    notImplemented(); 
}

void EditorClientDFB::learnWord(const String&) 
{ 
    notImplemented(); 
}

void EditorClientDFB::checkSpellingOfString(const UChar*, int length, int* misspellingLocation, int* misspellingLength) 
{ 
    notImplemented(); 
}

void EditorClientDFB::checkGrammarOfString(const UChar*, int length, Vector<GrammarDetail>&, int* badGrammarLocation, int* badGrammarLength) 
{ 
    notImplemented(); 
}

void EditorClientDFB::updateSpellingUIWithGrammarString(const String&, const GrammarDetail& detail) 
{ 
    notImplemented(); 
}

void EditorClientDFB::updateSpellingUIWithMisspelledWord(const String&) 
{ 
    notImplemented(); 
}

void EditorClientDFB::showSpellingUI(bool show) 
{ 
    notImplemented(); 
}

bool EditorClientDFB::spellingUIIsShowing() 
{ 
    notImplemented(); 
    return false;
}

void EditorClientDFB::getGuessesForWord(const String&, Vector<String>& guesses) 
{ 
    notImplemented(); 
}

String EditorClientDFB::getAutoCorrectSuggestionForMisspelledWord(const String&)
{
    notImplemented();
    return String();
}

void EditorClientDFB::willSetInputMethodState()
{
    notImplemented();
}

void EditorClientDFB::setInputMethodState(bool enabled)
{
    notImplemented();
}

}
