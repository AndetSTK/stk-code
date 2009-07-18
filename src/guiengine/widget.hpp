//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2009 Marianne Gagnon
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.


#ifndef HEADER_WIDGET_HPP
#define HEADER_WIDGET_HPP

#include <irrlicht.h>
#include <map>

#include "guiengine/skin.hpp"
#include "utils/ptr_vector.hpp"
#include "utils/vec3.hpp"

using namespace irr;
using namespace gui;

namespace GUIEngine
{
    
    class RibbonGridWidget;
    
    enum WidgetType
    {
        WTYPE_NONE = -1,
        WTYPE_RIBBON,
        WTYPE_SPINNER,
        WTYPE_BUTTON,
        WTYPE_ICON_BUTTON,
        WTYPE_CHECKBOX,
        WTYPE_LABEL,
        WTYPE_SPACER,
        WTYPE_DIV,
        WTYPE_RIBBON_GRID,
        WTYPE_MODEL_VIEW,
        WTYPE_LIST,
        WTYPE_TEXTBOX
    };
    
    enum Property
    {
        PROP_ID = 100,
        PROP_PROPORTION,
        PROP_WIDTH,
        PROP_HEIGHT,
        PROP_CHILD_WIDTH,
        PROP_CHILD_HEIGHT,
        PROP_WORD_WRAP,
        PROP_GROW_WITH_TEXT, // yet unused
        PROP_X,
        PROP_Y,
        PROP_LAYOUT,
        PROP_ALIGN,
        PROP_TEXT,
        PROP_ICON,
        PROP_TEXT_ALIGN,
        PROP_MIN_VALUE,
        PROP_MAX_VALUE,
        PROP_MAX_WIDTH,
        PROP_MAX_HEIGHT,
        PROP_SQUARE
    };
    
    class Widget : public SkinWidgetContainer
    {
        friend class EventHandler;
    protected:
        friend class RibbonWidget;
        friend class Screen;
        friend class SpinnerWidget;
        friend class Skin;
        friend class RibbonGridWidget;
        
        int getNewID();
        int getNewNoFocusID();
        
        /**
          * Can be used in children to indicate whether a widget is selected or not
          * - in widgets where it makes sense (e.g. ribbon children) and where the
          * irrLicht widget can not directly contain this state
          */
        bool m_selected;
        
        /**
          * called when left/right keys pressed and focus is on widget. 
          * Returns 'true' if main event handler should be notified of a change.
          * Override in children to be notified of left/right events.
          */
        virtual bool rightPressed() { return false; }
        virtual bool leftPressed() { return false; }
        
        /** used when you set eventSupervisors - see m_event_handler explainations below
            called when one of a widget's children is hovered.
            Returns 'true' if main event handler should be notified of a change. */
        virtual bool mouseHovered(Widget* child) { return false; }
        
        /** override in children if you need to know when the widget is focused */
        virtual void focused() {}
        
        void readCoords(Widget* parent=NULL);
        
        IGUIElement* m_parent;
        
        static bool convertToCoord(std::string& x, int* absolute, int* percentage);
        
        /**
         * IrrLicht widget created to represent this object.
         */
        IGUIElement* m_element;
        
    public:
        /**
         * This is set to NULL by default; set to something else in a widget to mean
         * that events happening on this widget should not go straight into the
         * event handler. Instead, they will first be passed to m_event_handler->transmitEvent,
         * which is usually the parent analysing events from its children.
         * This is especially useful with logical widgets built with more than
         * one irrlicht widgets (e.g. Spinner, Ribbon)
         */
        Widget* m_event_handler;
        
        Widget();
        virtual ~Widget() {}
        
        bool m_show_bounding_box;
        
        template<typename T> T* Widget::getIrrlichtElement()
        {
            #if defined(WIN32) || defined(NDEBUG)
                return static_cast<T*>(m_element);
            #else
                T* out = dynamic_cast<T*>(m_element);
                return out;
            #endif
        }
        
        IGUIElement* getIrrlichtElement() { return m_element; }

        
        virtual void update(float delta) { }
        
        /** used when you set parents - see m_event_handler explainations below.
         returns whether main event callback should be notified or not */
        virtual bool transmitEvent(Widget* w, std::string& originator) { return true; }
        
        /**
         * Create and add the irrLicht widget(s) associated with this object.
         * Call after Widget was read from XML file and laid out.
         */
        virtual void add() {} 
        
        void setParent(IGUIElement* parent);
        
        /**
          * If this widget has any children, they go here. Children can be either
          * specified in the XML file (e.g. Ribbon or Div children), or can also
          * be created automatically for logical widgets built with more than
          * one irrlicht widgets (e.g. Spinner)
          */
        ptr_vector<Widget> m_children;
        
        /** Type of this widget */
        WidgetType m_type;
        
        // FIXME... i forgot the m_ everywhere ... XD
        
        /** coordinates of the widget */
        int x, y, w, h;
        
        /** numerical ID used by irrLicht to identify this widget
          * (not the same as the string identificator specified in the XML file)
          */
        int id;
        
        /** A map that holds values for all specified widget properties (in the XML file)*/
        std::map<Property, std::string> m_properties;
        
        static void resetIDCounters();
        
        /**
          * Call to resize/move the widget. Not all widgets can resize gracefully.
          */
        virtual void move(const int x, const int y, const int w, const int h);
        
        
        bool isSelected() const { return m_selected; }
    };

    
}
#endif
