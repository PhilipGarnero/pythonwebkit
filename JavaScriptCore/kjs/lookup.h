// -*- c-basic-offset: 2 -*-
/*
 *  Copyright (C) 1999-2000 Harri Porten (porten@kde.org)
 *  Copyright (C) 2003, 2006, 2007, 2008 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef KJS_lookup_h
#define KJS_lookup_h

#include "ExecState.h"
#include "JSFunction.h"
#include "JSGlobalObject.h"
#include "JSObject.h"
#include "identifier.h"
#include <stdio.h>
#include <wtf/Assertions.h>

namespace KJS {

    // Hash table generated by the create_hash_table script.

    struct HashTableValue {
        const char* key; // property name
        intptr_t value; // integer or function
        unsigned char attributes; // JSObject attributes
        unsigned char length; // number of arguments for function
    };

    struct HashEntry {
        UString::Rep* key;
        union {
            intptr_t integerValue;
            NativeFunction functionValue;
        };
        unsigned char attributes; // JSObject attributes
        unsigned char length; // number of arguments for function
    };

    struct HashTable {
        int hashSizeMask; // Precomputed size for the hash table (minus 1).
        const HashTableValue* values; // Fixed values generated by script.
        mutable const HashEntry* table; // Table allocated at runtime.

        ALWAYS_INLINE void initializeIfNeeded(JSGlobalData* globalData) const
        {
            if (!table)
                createTable(globalData);
        }

        ALWAYS_INLINE void initializeIfNeeded(ExecState* exec) const
        {
            if (!table)
                createTable(&exec->globalData());
        }

        // Find an entry in the table, and return the entry.
        ALWAYS_INLINE const HashEntry* entry(JSGlobalData* globalData, const Identifier& identifier) const
        {
            initializeIfNeeded(globalData);
            return entry(identifier);
        }

        ALWAYS_INLINE const HashEntry* entry(ExecState* exec, const Identifier& identifier) const
        {
            initializeIfNeeded(exec);
            return entry(identifier);
        }

    private:
        ALWAYS_INLINE const HashEntry* entry(const Identifier& identifier) const
        {
            ASSERT(table);
            const HashEntry* entry = &table[identifier.ustring().rep()->computedHash() & hashSizeMask];
            if (entry->key != identifier.ustring().rep())
                return 0;
            return entry;
        }

        // Convert the hash table keys to identifiers.
        void createTable(JSGlobalData*) const;
    };

    /**
     * @internal
     * Helper for getStaticPropertySlot
     */
    JSValue* staticFunctionGetter(ExecState*, const Identifier& propertyName, const PropertySlot&);

    void setUpStaticFunctionSlot(ExecState*, const HashEntry*, JSObject* thisObject, const Identifier& propertyName, PropertySlot&);

    /**
     * @internal
     * Helper for getStaticValueSlot and getStaticPropertySlot
     */
    template <class ThisImp>
    JSValue* staticValueGetter(ExecState* exec, const Identifier&, const PropertySlot& slot)
    {
        ThisImp* thisObj = static_cast<ThisImp*>(slot.slotBase());
        const HashEntry* entry = slot.staticEntry();
        return thisObj->getValueProperty(exec, entry->integerValue);
    }

    /**
     * Helper method for property lookups
     *
     * This method does it all (looking in the hashtable, checking for function
     * overrides, creating the function or retrieving from cache, calling
     * getValueProperty in case of a non-function property, forwarding to parent if
     * unknown property).
     *
     * Template arguments:
     * @param FuncImp the class which implements this object's functions
     * @param ThisImp the class of "this". It must implement the getValueProperty(exec,token) method,
     * for non-function properties.
     * @param ParentImp the class of the parent, to propagate the lookup.
     *
     * Method arguments:
     * @param exec execution state, as usual
     * @param propertyName the property we're looking for
     * @param table the static hashtable for this class
     * @param thisObj "this"
     */
    template <class ThisImp, class ParentImp>
    inline bool getStaticPropertySlot(ExecState* exec, const HashTable* table, ThisImp* thisObj, const Identifier& propertyName, PropertySlot& slot)
    {
        const HashEntry* entry = table->entry(exec, propertyName);

        if (!entry) // not found, forward to parent
            return thisObj->ParentImp::getOwnPropertySlot(exec, propertyName, slot);

        if (entry->attributes & Function)
            slot.setStaticEntry(thisObj, entry, staticFunctionGetter);
        else
            slot.setStaticEntry(thisObj, entry, staticValueGetter<ThisImp>);

        return true;
    }

    /**
     * Simplified version of getStaticPropertySlot in case there are only functions.
     * Using this instead of getStaticPropertySlot allows 'this' to avoid implementing
     * a dummy getValueProperty.
     */
    template <class ParentImp>
    inline bool getStaticFunctionSlot(ExecState* exec, const HashTable* table, JSObject* thisObj, const Identifier& propertyName, PropertySlot& slot)
    {
        if (static_cast<ParentImp*>(thisObj)->ParentImp::getOwnPropertySlot(exec, propertyName, slot))
            return true;

        const HashEntry* entry = table->entry(exec, propertyName);
        if (!entry)
            return false;

        setUpStaticFunctionSlot(exec, entry, thisObj, propertyName, slot);
        return true;
    }

    /**
     * Simplified version of getStaticPropertySlot in case there are no functions, only "values".
     * Using this instead of getStaticPropertySlot removes the need for a FuncImp class.
     */
    template <class ThisImp, class ParentImp>
    inline bool getStaticValueSlot(ExecState* exec, const HashTable* table, ThisImp* thisObj, const Identifier& propertyName, PropertySlot& slot)
    {
        const HashEntry* entry = table->entry(exec, propertyName);

        if (!entry) // not found, forward to parent
            return thisObj->ParentImp::getOwnPropertySlot(exec, propertyName, slot);

        ASSERT(!(entry->attributes & Function));

        slot.setStaticEntry(thisObj, entry, staticValueGetter<ThisImp>);
        return true;
    }

    /**
     * This one is for "put".
     * It looks up a hash entry for the property to be set.  If an entry
     * is found it sets the value and returns true, else it returns false.
     */
    template <class ThisImp>
    inline bool lookupPut(ExecState* exec, const Identifier& propertyName, JSValue* value, const HashTable* table, ThisImp* thisObj)
    {
        const HashEntry* entry = table->entry(exec, propertyName);

        if (!entry)
            return false;

        if (entry->attributes & Function) // function: put as override property
            thisObj->putDirect(propertyName, value);
        else if (!(entry->attributes & ReadOnly))
            thisObj->putValueProperty(exec, entry->integerValue, value);

        return true;
    }

    /**
     * This one is for "put".
     * It calls lookupPut<ThisImp>() to set the value.  If that call
     * returns false (meaning no entry in the hash table was found),
     * then it calls put() on the ParentImp class.
     */
    template <class ThisImp, class ParentImp>
    inline void lookupPut(ExecState* exec, const Identifier& propertyName, JSValue* value, const HashTable* table, ThisImp* thisObj)
    {
        if (!lookupPut<ThisImp>(exec, propertyName, value, table, thisObj))
            thisObj->ParentImp::put(exec, propertyName, value); // not found: forward to parent
    }

    /**
     * This template method retrieves or create an object that is unique
     * (for a given global object) The first time this is called (for a given
     * property name), the Object will be constructed, and set as a property
     * of the global object. Later calls will simply retrieve that cached object. 
     * Note that the object constructor must take 1 argument, exec.
     */
    template <class ClassCtor>
    inline JSObject* cacheGlobalObject(ExecState* exec, const Identifier& propertyName)
    {
        JSGlobalObject* globalObject = exec->lexicalGlobalObject();
        JSValue* obj = globalObject->getDirect(propertyName);
        if (obj) {
            ASSERT(obj->isObject());
            return static_cast<JSObject* >(obj);
        }
        JSObject* newObject = new (exec) ClassCtor(exec);
        globalObject->putDirect(propertyName, newObject, DontEnum);
        return newObject;
    }

} // namespace KJS

/**
 * Helpers to define prototype objects (each of which simply implements
 * the functions for a type of objects).
 * Sorry for this not being very readable, but it actually saves much copy-n-paste.
 * ParentPrototype is not our base class, it's the object we use as fallback.
 * The reason for this is that there should only be ONE DOMNode.hasAttributes (e.g.),
 * not one in each derived class. So we link the (unique) prototypes between them.
 *
 * Using those macros is very simple: define the hashtable (e.g. "DOMNodePrototypeTable"), then
 * KJS_DEFINE_PROTOTYPE(DOMNodePrototype)
 * KJS_IMPLEMENT_PROTOTYPE("DOMNode", DOMNodePrototype, DOMNodePrototypeFunction)
 * and use DOMNodePrototype::self(exec) as prototype in the DOMNode constructor.
 * If the prototype has a "parent prototype", e.g. DOMElementPrototype falls back on DOMNodePrototype,
 * then the first line will use KJS_DEFINE_PROTOTYPE_WITH_PROTOTYPE, with DOMNodePrototype as the second argument.
 */

// These macros assume that a prototype's only properties are functions
#define KJS_DEFINE_PROTOTYPE(ClassPrototype) \
  class ClassPrototype : public KJS::JSObject { \
  public: \
    static KJS::JSObject* self(KJS::ExecState* exec); \
    virtual const KJS::ClassInfo* classInfo() const { return &info; } \
    static const KJS::ClassInfo info; \
    bool getOwnPropertySlot(KJS::ExecState* , const KJS::Identifier&, KJS::PropertySlot&); \
    ClassPrototype(KJS::ExecState* exec) \
      : KJS::JSObject(exec->lexicalGlobalObject()->objectPrototype()) { } \
    \
  };

#define KJS_DEFINE_PROTOTYPE_WITH_PROTOTYPE(ClassPrototype, ClassPrototypePrototype) \
    class ClassPrototype : public KJS::JSObject { \
    public: \
        static KJS::JSObject* self(KJS::ExecState* exec); \
        virtual const KJS::ClassInfo* classInfo() const { return &info; } \
        static const KJS::ClassInfo info; \
        bool getOwnPropertySlot(KJS::ExecState*, const KJS::Identifier&, KJS::PropertySlot&); \
        ClassPrototype(KJS::ExecState* exec) \
            : KJS::JSObject(ClassPrototypePrototype::self(exec)) { } \
    \
    };

#define KJS_IMPLEMENT_PROTOTYPE(ClassName, ClassPrototype) \
    const ClassInfo ClassPrototype::info = { ClassName"Prototype", 0, &ClassPrototype##Table, 0  }; \
    JSObject* ClassPrototype::self(ExecState* exec) \
    { \
        return KJS::cacheGlobalObject<ClassPrototype>(exec, Identifier(exec, "[[" ClassName ".prototype]]")); \
    } \
    bool ClassPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) \
    { \
      return getStaticFunctionSlot<JSObject>(exec, &ClassPrototype##Table, this, propertyName, slot); \
    }

#endif // KJS_lookup_h
