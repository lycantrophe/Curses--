#ifndef CURSESXX_APPLICATION
#define CURSESXX_APPLICATION

#include <vector>
#include <string>
#include <functional>
#include <ncurses.h>

namespace cursesxx {

    class Color {
        public:
            Color( unsigned int R, unsigned int G, unsigned int B );

        private:
            unsigned int R, G, B;
    };

    /*
     * BorderStyle is a saveable, passable stateful object that captures a
     * specific border type. As the Border object itself has no idea what kind
     * of borders it draws, this is necessary should some state have to be
     * saved, stored or passed around before the actual drawing.
     */
    class BorderStyle {
        public:
            BorderStyle();
            BorderStyle( char vert, char hor );
            BorderStyle( char ls, char rs, char ts,
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
            Border( WINDOW*, const BorderStyle& );

            ~Border();

            void set( const BorderStyle& );
            void set( const BorderStyle&& );

        private:
            WINDOW* win;

            /* trigger compile error */
            Border& operator=( const Border& );
            Border( const Border& );
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
            Widget( const BorderStyle& );

            Widget( const Geometry&, const Anchor& );
            Widget( const Geometry&, const BorderStyle& );
            Widget( const Anchor&, const BorderStyle& );

            Widget( const Geometry&,
                    const Anchor&, const BorderStyle& );

            Widget( const Widget& parent );

            Widget( const Widget& parent, const Geometry& );
            Widget( const Widget& parent, const Anchor& );
            Widget( const Widget& parent, const BorderStyle& );

            Widget( const Widget& parent, const Geometry&, const Anchor& );
            Widget( const Widget& parent,
                    const Geometry&, const BorderStyle& );
            Widget( const Widget& parent,
                    const Anchor&, const BorderStyle& );
            Widget( const Widget& parent, const Geometry& g,
                    const Anchor& a, const BorderStyle& b );

            ~Widget();

            int height() const;
            int width() const;

            void refresh();
            void redraw();
            void clear();

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
     * Special case of pre-configured Widget. Takes only geometry hints and
     * calculates center-of-screen placement. Can take a virtual screen as
     * well.
     *
     * Takes only "output types" such as input field, list selectors or boolean
     * selectors. For a simple message dialogue, give it a bool selector with
     * one option and ignore the return value.
     *
     * All constructors requires an -explicit- geometry argument in order to be
     * able to calculate position.
     */



    /*
     * This is ment fordisplaying any text (a somewhat basic widget in some
     * sense) which is curses is practically any picture. Takes size
     * directions, but all drawing anchors the widgets (0,0)
     */
    class Textfield {
        public:

            /* The variadic templates "forwards" the available constructors in
             * Widget. 
             */

            template< typename... Args > 
                Textfield( const std::string& text, const Args&... );

            template< typename... Args > 
                Textfield( const std::string& text, const Geometry&, const Args&... );

            /* This constructor allow proxying arbitrary Widgets (not the
             * only the widget object, but others like Textfield and Panel as well)
             * using the get_widget() method so that relative position can be
             * utilized. The function pointer cast is necessary in order for
             * C++ template type inference to work; it relies on a
             * Widget-returning method to be present (get_widget()). Otherwise
             * it would give a compile error when trying to use other arguments
             * without a parent (a widget) argument.
             */

            template< typename Parent, typename... Args >
                Textfield( const Parent&, const std::string&, const Args&... );

            void write();
            void write( const std::string& );
            void append( const std::string& );

            void redraw();
            void decorate( const BorderStyle& );
            const Widget& get_widget() const;

            static Geometry text_wrap( const std::string&, const int width );
            static Geometry text_wrap( const std::string& );

        private:
            std::string text;
            Widget widget;

            /* unimplemented, so these should trigger an error */

            Textfield& operator=( const Textfield& );
            Textfield( const Textfield& );

    };
    /*
     * Creates a new screen element that is a static label. For now it is
     * "immutable" in the sense that if you want to change a label (and by
     * extension resize it) you have to re-create the object.
     *
     * Label will not accept any size directions; the Geometry parameters are
     * disabled compile-time. Its size is calculated based on the size on label
     * text as well as the horizontal threshold passed as an optional
     * parameter. In short, it is a stricter Textfield.
     *
     * It takes no explicit Geometry parameters or size hints; instead, it
     * inherits the attribute of determining breaks from the newlines in the
     * input string
     */
    class Label {
        public:
            Label( const std::string& text = "Label" );

            template< typename... Args > 
                Label( const Args&... );

            void redraw();
            const Widget& get_widget() const;

        private:
            Textfield widget;

            /* unimplemented, so these should trigger an error */
            template< typename... Pre, typename... Post > 
                Label( const Pre&..., const Geometry&, const Post&... );

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

    /* TEMPLATE IMPLEMENTATIONS */
    /* CONSTRUCTORS */

    template< typename... Args > 
        Textfield::Textfield( const std::string& text, const Args&... args ) :
            Textfield( text, Textfield::text_wrap( text ), args... )
    {}

    template< typename... Args > 
        Textfield::Textfield( const std::string& text,
                const Geometry& g,
                const Args&... args ):
            text( text ),
            widget( g, args... )
    {
        this->widget.write( text );
    }

    template< typename Parent, typename... Args >
        Textfield::Textfield( const Parent& p,
                const std::string& text,
                const Args&... args ) :
            text( text ),
            widget( p.get_widget(), args... )
    {
        this->widget.write( text );
    }

    template< typename... Args > 
        Label::Label( const Args&... args ) :
            widget( args... )
    {}

    {}

    {}
}

#endif //CURSESXX_APPLICATION
