#ifndef CURSESXX_APPLICATION
#define CURSESXX_APPLICATION

#include <vector>
#include <string>
#include <ncurses.h>

namespace cursesxx {

    class Color {
        public:
            Color( unsigned int R, unsigned int G, unsigned int B );

        private:
            unsigned int R, G, B;
    };

    /*
     * BorderPrototype is a saveable, passable stateful object that captures a
     * specific border type. As the Border object itself has no idea what kind
     * of borders it draws, this is necessary should some state have to be
     * saved, stored or passed around before the actual drawing.
     */
    class BorderPrototype {
        public:
            BorderPrototype();
            BorderPrototype( char vert, char hor );
            BorderPrototype( char ls, char rs, char ts,
                    char bs, char tl, char tr, char bl, char br );

            const bool detailed;
            const char ls, rs, ts, bs, tl, tr, bl, br;
    };


    /*
     * Draws the border for a particular window
     */
    class Border {
        public:
            Border();
            Border( WINDOW* );
            Border( WINDOW*, char vert, char hor );
            Border( WINDOW*, char ls, char rs, char ts,
                    char bs, char tl, char tr, char bl, char br );
            Border( WINDOW*, const BorderPrototype& );

            Border( const Border& );
            ~Border();

        private:
            WINDOW* win;
    };

    /*
     * Stores and represents an objects geometry, that is its height and width.
     */
    class Geometry {
        public:
            Geometry();
            Geometry( const int height, const int width );
            Geometry( const Geometry& parent, const int height, const int width );

            int height() const;
            int width() const;

            int set_height();
            int set_width(); 

        private:
            int height_;
            int width_;
    };

    /*
     * Stores and represents an objects position. When constructed with two
     * anchors as parameters it calculates the position where relative to
     * base, i.e. it treats base as (0,0)
     */
    class Anchor {
        public:
            Anchor();
            Anchor( const int y, const int x );
            Anchor( const Anchor& base, const Anchor& offset );
            const int y, x;
    };

    class Widget {
        /*
         * Base class for all screen elements. This object is responsible for
         * all frame-and-window related activities; positioning on the screen,
         * drawing, colouring etc. It should also manage and free all resources
         * according to its scope. 
         */
        public:
            Widget();
            Widget( const Geometry& );
            Widget( const Anchor& );
            Widget( const BorderPrototype& );

            Widget( const Geometry&, const Anchor& );
            Widget( const Geometry&, const BorderPrototype& );
            Widget( const Anchor&, const BorderPrototype& );

            Widget( const Geometry&,
                    const Anchor&, const BorderPrototype& );

            Widget( const Widget& parent );

            Widget( const Widget& parent, const Geometry& );
            Widget( const Widget& parent, const Anchor& );
            Widget( const Widget& parent, const BorderPrototype& );

            Widget( const Widget& parent, const Geometry&, const Anchor& );
            Widget( const Widget& parent,
                    const Geometry&, const BorderPrototype& );
            Widget( const Widget& parent,
                    const Anchor&, const BorderPrototype& );
            Widget( const Widget& parent, const Geometry& g,
                    const Anchor& a, const BorderPrototype& b );

            ~Widget();

            int height() const;
            int width() const;

            void refresh();
            void redraw();
            void write( const std::string& str );
            void write( const std::string& str, const int maxlen );

            const Widget& get_widget() const;

        private:
            Geometry geometry;
            Anchor anchor;

            class Window {
                public:
                    Window( int, int, int, int );
                    ~Window();

                    void refresh();
                    void clear();
                    void write( const std::string& );
                    void write( const std::string&, const int maxlen );
                    WINDOW* window;
            };

            Window window;
            Border decoration;

    };

    /*
     * This is ment fordisplaying any text (a somewhat basic widget in some
     * sense) which is curses is practically any picture. Takes size
     * directions, but all drawing anchors the widgets (0,0)
     */
    class Label {
        public:
            Label( const std::string& text = "Label", const int maxint = 0 );

            /* The variadic templates "forwards" the available constructors in
             * Widget. All arguments after the text (or optional int) will
             * be given to the Widget constructor. If Geometry is passed it
             * will trigger a compile error
             */
            template< typename... Args > 
                Label( const std::string&, const Args&... );

            template< typename... Args > 
                Label( const std::string& text, const int maxwidth, const Args&... );

            /* These two constructors allow proxying arbitrary Widgets (not the
             * only the widget object, but others like Label and Panel as well)
             * using the get_widget() method so that relative position can be
             * utilized. The function pointer cast is necessary in order for
             * C++ template type inference to work; it relies on a
             * Widget-returning method to be present (get_widget()). Otherwise
             * it would give a compile error when trying to use other arguments
             * without a parent (a widget) argument.
             */

            template< typename W, typename... Args >
                Label( const W& widget,
                        const std::string& text,
                        const Args&... params );

            template< typename W, typename... Args >
                Label( const W& widget,
                        const std::string& text,
                        const int maxwidth,
                        const Args&... params );

            void redraw();
            const Widget& get_widget() const;

        private:
            std::string text;
            Widget widget;
            static Geometry label_wrap( const std::string&, const int width );

            /* unimplemented, so these should trigger an error */
            template< typename... Args > 
                Label( const std::string&, const Geometry&, const Args&... );

            template< typename... Args >
                Label( const std::string& text, const int maxwidth,
                        const Geometry&, const Args&... );

            Label& operator=( const Label& );
            Label( const Label& );


    };

    class Application {
        public:
            Application& enable_keypad( const bool enable = true );
            Application& enable_echo( const bool enable = true );

        private:
            class Screen {
                public:
                    Screen();
                    ~Screen();
                    /* This wrapper class makes sure the curses initialization
                     * happens before any members tries to create and draw
                     * their windows.
                     */
            };
            Screen screen;
    };


    int mid( int A, int B );
    Anchor mid( const Geometry& child );
    Anchor mid( const Widget& parent, const Geometry& child );

    template< typename... Args >
        Label::Label( const std::string& text, const Args&... params ) :
            text( text ),
            widget( Geometry( 1, text.size() ), params... )
    {}

    template< typename W, typename... Args >
        Label::Label( const W& w,
                const std::string& text,
                const Args&... params ) :
            text( text ),
            widget( w.get_widget(), params... )
    {}

    template< typename... Args >
        Label::Label( const std::string& text,
                const int maxwidth,
                const Args&... params ) :
            text( text ),
            widget( label_wrap( text, maxwidth ), params... )
    {}

    template< typename W, typename... Args >
        Label::Label( const W& w,
                const std::string& text,
                const int maxwidth,
                const Args&... params ) :
            text( text ),
            widget( w.get_widget(), label_wrap( text, maxwidth ), params... )
    {}
}

#endif //CURSESXX_APPLICATION
