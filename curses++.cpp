#include <algorithm>
#include <ncurses.h>
#include "curses++.h"

cursesxx::BorderPrototype::BorderPrototype() : 
    detailed( false ),
    ls( '|' ),
    rs( '-' ),
    ts( ' ' ), bs( ' ' ),
    tl( ' ' ), tr( ' ' ),
    bl( ' ' ), br( ' ' )
{}

cursesxx::BorderPrototype::BorderPrototype( char vert, char hor ) : 
    detailed( false ),
    ls( vert ),
    rs( hor ),
    ts( ' ' ), bs( ' ' ),
    tl( ' ' ), tr( ' ' ),
    bl( ' ' ), br( ' ' )
{}

cursesxx::BorderPrototype::BorderPrototype( char ls, char rs, char ts,
        char bs, char tl, char tr, char bl, char br ) :
    detailed( true ),
    ls( ls ), rs( rs ),
    ts( ts ), bs( bs ),
    tl( tl ), tr( tr ),
    bl( bl ), br( br )
{}

cursesxx::Border::Border() : win( nullptr )
{}

cursesxx::Border::Border( WINDOW* win ) : win( win ) {
    box( win, '|', '-' );
}

cursesxx::Border::Border( WINDOW* win, char vert, char hor ) : win( win ) {
    box( win, vert, hor );
}

cursesxx::Border::Border( WINDOW* win, char ls, char rs, char ts,
        char bs, char tl, char tr, char bl, char br ) : win( win ) {

    wborder( win, ls, rs, ts, bs, tl, tr, bl, br );
}

cursesxx::Border::Border( WINDOW* win, const BorderPrototype& proto ) :
    win( win ) {
        if( proto.detailed )
            box( win, proto.ls, proto.rs );
        else
            wborder( win, proto.ls, proto.rs, proto.ts, proto.bs, proto.tl,
                    proto.tr, proto.bl, proto.br );
    }

cursesxx::Border::~Border() {
    if( this->win == nullptr ) return;

    box( this->win, ' ', ' ' );
}

cursesxx::Geometry::Geometry() :
    height_( 0 ),
    width_( 0 )
{}

cursesxx::Geometry::Geometry( const int height, const int width ) :
    height_( std::max( LINES - 1, height ) ),
    width_( std::max( COLS - 1, width ) )
{}

cursesxx::Geometry::Geometry( const Geometry& parent,
        const int height,
        const int width ) :
    height_( std::max( height, parent.height_ ) ),
    width_( std::max( width, parent.width_ ) )
{}

cursesxx::Anchor::Anchor() : y( 0 ), x( 0 ) {}

cursesxx::Anchor::Anchor( const int y, const int x ) : y( y ), x( x ) {}

cursesxx::Anchor::Anchor( const Anchor& base, const Anchor& offset ) : 
    y( base.y + offset.y ),
    x( base.x + offset.x )
{}

cursesxx::Widget::Widget() {
    this->window = newwin( this->geometry.height(), this->geometry.width(),
            this->anchor.y, this->anchor.x );
}

cursesxx::Widget::Widget( const Geometry& g ) : geometry( g ) {
    this->window = newwin( this->geometry.height(), this->geometry.width(),
            this->anchor.y, this->anchor.x );
}

cursesxx::Widget::Widget( const Anchor& ) : anchor( anchor ) {
    this->window = newwin( this->geometry.height(), this->geometry.width(),
            this->anchor.y, this->anchor.x );
}

cursesxx::Widget::Widget( const BorderPrototype& b ) {
    this->window = newwin( this->geometry.height(), this->geometry.width(),
            this->anchor.y, this->anchor.x );

    this->decoration = Border( this->window, b );
}

cursesxx::Widget::Widget( const Geometry& g, const Anchor& a ) :
    geometry( g ), anchor( a ) {
        this->window = newwin( this->geometry.height(), this->geometry.width(),
                this->anchor.y, this->anchor.x );
    }

cursesxx::Widget::Widget( const Geometry& g, const BorderPrototype& b ) :
    geometry( g ) {
        this->window = newwin( this->geometry.height(), this->geometry.width(),
                this->anchor.y, this->anchor.x );
        this->decoration = Border( this->window, b );
    }

cursesxx::Widget::Widget( const Anchor& a, const BorderPrototype& b ) : 
    anchor( a ) {
        this->window = newwin( this->geometry.height(), this->geometry.width(),
                this->anchor.y, this->anchor.x );

        this->decoration = Border( this->window, b );
    }

cursesxx::Widget::Widget( const Geometry& g,
        const Anchor& a, const BorderPrototype& b ) :
    geometry( g ), anchor( a ) {
        this->window = newwin( this->geometry.height(), this->geometry.width(),
                this->anchor.y, this->anchor.x );

        this->decoration = Border( this->window, b );
    }

cursesxx::Widget::Widget( const Widget& parent ) :
    geometry( parent.geometry ),
    anchor( parent.anchor ) {
        this->window = newwin( this->geometry.height(), this->geometry.width(),
                this->anchor.y, this->anchor.x );

        this->decoration = Border( this->window );
    }

cursesxx::Widget::Widget( const Widget& parent, const Geometry& g ) :
    geometry( g ),
    anchor( parent.anchor ) {
        this->window = newwin( this->geometry.height(), this->geometry.width(),
                this->anchor.y, this->anchor.x );
    }

cursesxx::Widget::Widget( const Widget& parent, const Anchor& a ) : 
    geometry( parent.geometry ),
    anchor( parent.anchor, a ) {
        this->window = newwin( this->geometry.height(), this->geometry.width(),
                this->anchor.y, this->anchor.x );
    }

cursesxx::Widget::Widget( const Widget& parent, const BorderPrototype& b ) : 
    geometry( parent.geometry ) {
        this->window = newwin( this->geometry.height(), this->geometry.width(),
                this->anchor.y, this->anchor.x );

        this->decoration = Border( this->window, b );
    }

cursesxx::Widget::Widget( const Widget& parent,
        const Anchor& a, const BorderPrototype& b ) : 
    geometry( parent.geometry ), 
    anchor( parent.anchor, a ) {
        this->window = newwin( this->geometry.height(), this->geometry.width(),
                this->anchor.y, this->anchor.x );

        this->decoration = Border( this->window, b );
    }

cursesxx::Widget::Widget( const Widget& parent, const Geometry& g,
        const Anchor& a, const BorderPrototype& b ) : 
    geometry( g ),
    anchor( parent.anchor, a ) {
        this->window = newwin( this->geometry.height(), this->geometry.width(),
                this->anchor.y, this->anchor.x );

        this->decoration = Border( this->window, b );
    }

cursesxx::Widget::~Widget() {
    wclear( this->window );
    wrefresh( this->window );
    delwin( this->window );
}
