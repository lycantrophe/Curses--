#ifndef CURSESXX_APPLICATION
#define CURSESXX_APPLICATION

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

            ~Widget();

            void refresh();
            void redraw();

        private:
            WINDOW* window;
            Geometry geometry;
            Anchor anchor;
            Border decoration;
    };
}

#endif //CURSESXX_APPLICATION
