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

    class BorderPrototype {
        public:
            BorderPrototype();
            BorderPrototype( char vert, char hor );
            BorderPrototype( char ls, char rs, char ts,
                    char bs, char tl, char tr, char bl, char br );

            const bool detailed;
            const char ls, rs, ts, bs, tl, tr, bl, br;
    };

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

    class Anchor {
        public:
            Anchor();
            Anchor( const int y, const int x );
            Anchor( const Anchor& base, const Anchor& offset );
            const int y, x;
    };

    class Widget {
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

            virtual ~Widget();

            /*
             * Adds a new child widget to this object and forwards its
             * construction parameters. This avoids unecessary
             * construction/destruction and ensures a safe state for curses
             */
            template< typename... Attr >
            void add_child( Attr... attributes );

            void refresh();
            void redraw();


        private:
            Geometry geometry;
            Anchor anchor;
            std::vector< Widget > children;

        protected:
            class Window {
                public:
                    Window( int, int, int, int );
                    ~Window();

                    void refresh();
                    WINDOW* window;
            };

            Window window;

        private:
            Border decoration;

            virtual void paint();

    };

    class Panel : public Widget {
        public:
        private:
    };

    class Button : public Widget {
        public:
        private:
    };

    class Input : public Widget {
        public:
        private:
    };

    class Label : public Widget {
        public:
            Label( const std::string& text = "Label",
                    const Geometry& g = { 1, 0 },
                    const int maxint = 0 );

            /* The variadic templates "forwards" the available constructors in
             * Widget. Unless geometry is the 2nd argument the templates are
             * triggered. All arguments after the text (or optional int) will
             * be given to the Widget constructor
             */
            template< typename... Args > 
            Label( const std::string&, Args&... );
            template< typename... Args > 
            Label( const std::string& text, const int maxwidth, Args&... );
            ~Label();

        private:
            std::string text;
            void paint();
            static Geometry label_wrap( const std::string&, const int width );
    };

    class Application {
        public:
            void enable_keypad( const bool enable = true );
            void enable_echo( const bool enable = true );
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

    template< typename... Args >
        Label::Label( const std::string& text, Args&... params ) :
            Widget( params... ),
            text( text )
    {}

    template< typename... Args >
        Label::Label( const std::string& text,
                const int maxwidth,
                Args&... params ) :
            Widget( label_wrap( text, maxwidth ), params... ),
            text( text )
    {}

}

#endif //CURSESXX_APPLICATION
