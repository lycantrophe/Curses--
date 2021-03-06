#include <algorithm>
#include <ncurses.h>
#include <string>
#include "curses++.h"

WINDOW* cursesxx::Format::get_win( const Widget& widget ) {
    return widget.window.get();
}

cursesxx::Format::~Format() {
    wattroff( this->win, this->bitmask );
}

cursesxx::BorderStyle::BorderStyle() : 
    detailed( true ),
    ls( '|' ),
    rs( '-' ),
    ts( ' ' ), bs( ' ' ),
    tl( ' ' ), tr( ' ' ),
    bl( ' ' ), br( ' ' )
{}

cursesxx::BorderStyle::BorderStyle( char vert, char hor ) : 
    detailed( false ),
    ls( vert ),
    rs( hor ),
    ts( ' ' ), bs( ' ' ),
    tl( ' ' ), tr( ' ' ),
    bl( ' ' ), br( ' ' )
{}

cursesxx::BorderStyle::BorderStyle( char ls, char rs, char ts,
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

static WINDOW* draw_border( WINDOW* win, const cursesxx::BorderStyle& proto ) {
    if( !proto.detailed )
        box( win, proto.ls, proto.rs );
    else
        wborder( win, proto.ls, proto.rs, proto.ts, proto.bs, proto.tl,
                proto.tr, proto.bl, proto.br );

    return win;
}

cursesxx::Border::Border( WINDOW* win, const BorderStyle& proto ) :
    win( draw_border( win, proto ) )
{}

void cursesxx::Border::set( const cursesxx::BorderStyle& style ) {
    /* Cannot set border unless set during encapsulation object construction */
    if( this->win == nullptr ) return; 

    draw_border( this->win, style );
}

void cursesxx::Border::set( const cursesxx::BorderStyle&& style ) {
    /* Cannot set border unless set during encapsulation object construction */
    if( this->win == nullptr ) return; 

    wborder( this->win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' );
    draw_border( this->win, style );
}

cursesxx::Border::~Border() {
    if( this->win == nullptr ) return;

    wborder( this->win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' );
}

cursesxx::Geometry::Geometry( const bool border ) :
    height_( border ? LINES - 3 : 0 ),
    width_( border ? COLS - 3 : 0 )
{}

cursesxx::Geometry::Geometry( const int height,
        const int width,
        const bool border ) :

    height_( std::min( LINES - 1, height ) - ( border ? 2 : 0 ) ),
    width_( std::min( COLS - 1, width ) - ( border ? 2 : 0 ) )
{}

cursesxx::Geometry::Geometry( const Geometry& parent,
        const int height,
        const int width,
        const bool border ) :
    height_( std::min( height, parent.height_ ) - ( border ? 2 : 0 )  ),
    width_( std::min( width, parent.width_ ) - ( border ? 2 : 0 ) )
{}

int cursesxx::Geometry::height() const {
    return this->height_;
}

int cursesxx::Geometry::width() const {
    return this->width_;
}

cursesxx::Anchor::Anchor( const bool border ) :
    y( border ? 1 : 0 ),
    x( border ? 1 : 0 )
{}

cursesxx::Anchor::Anchor( const int y, const int x, const bool border ) :
    y( y + border ? 1 : 0 ),
    x( x + border ? 1 : 0 )
{}

cursesxx::Anchor::Anchor( const Anchor& base, const Anchor& offset ) :
    y( base.y + offset.y ),
    x( base.x + offset.x )
{}

cursesxx::Widget::Widget() :
    window( newwin(
                this->geometry.height(),
                this->geometry.width(),
                this->anchor.y,
                this->anchor.x ) )
{}

cursesxx::Widget::Widget( const Geometry& g ) :
    geometry( g ),
    window( newwin(
                g.height(),
                g.width(),
                this->anchor.y,
                this->anchor.x ) )
{}

cursesxx::Widget::Widget( const Anchor& a ) :
    anchor( a ),
    window( newwin(
                this->geometry.height(),
                this->geometry.width(),
                a.y,
                a.x ) )
{}

cursesxx::Widget::Widget( const BorderStyle& b ) :
    geometry( true ),
    anchor( true ),
    window( newwin(
                this->geometry.height() + 2,
                this->geometry.width() + 2,
                this->anchor.y - 1,
                this->anchor.x - 1 ) ),
    decoration( this->window.get(), b )
{}

cursesxx::Widget::Widget( const Geometry& g, const Anchor& a ) :
    geometry( g ),
    anchor( a ),
    window( newwin( 
                g.height(),
                g.width(),
                a.y,
                a.x ) )
{}

cursesxx::Widget::Widget( const Geometry& g, const BorderStyle& b ) :
    geometry( g ),
    anchor( true ),
    window( newwin(
                g.height() + 2,
                g.width() + 2,
                this->anchor.y - 1,
                this->anchor.x - 1 ) ),
    decoration( this->window.get(), b )
{}

cursesxx::Widget::Widget( const Anchor& a, const BorderStyle& b ) :
    geometry( true ),
    anchor( a ),
    window( newwin( 
                this->geometry.height() + 2,
                this->geometry.width() + 2,
                a.y - 1,
                a.x - 1 ) ),
    decoration( this->window.get(), b )
{}

cursesxx::Widget::Widget( const Geometry& g,
        const Anchor& a, const BorderStyle& b ) :
    geometry( g ),
    anchor( a ),
    window( newwin( 
                g.height() + 2,
                g.width() + 2,
                a.y - 1,
                a.x - 1 ) ),
    decoration( this->window.get(), b )
{}

cursesxx::Widget::Widget( const Widget& parent ) :
    geometry( parent.geometry ),
    anchor( parent.anchor ),
    window( newwin( 
                this->geometry.height(),
                this->geometry.width(),
                this->anchor.y,
                this->anchor.x ) ),
    decoration( this->window.get() )
{}

cursesxx::Widget::Widget( const Widget& parent, const Geometry& g ) :
    geometry( g ),
    anchor( parent.anchor ),
    window( newwin(
                g.height(),
            g.width(),
            this->anchor.y,
            this->anchor.x ) )
{}

cursesxx::Widget::Widget( const Widget& parent, const Anchor& a ) :
    geometry( parent.geometry ),
    anchor( parent.anchor, a ),
    window( newwin( 
                this->geometry.height(),
                this->geometry.width(),
                this->anchor.y,
                this->anchor.x ) )
{}

cursesxx::Widget::Widget( const Widget& parent, const BorderStyle& b ) :
    geometry( parent.geometry ),
    window( newwin(
                this->geometry.height() + 2,
                this->geometry.width() + 2,
                this->anchor.y - 1,
                this->anchor.x - 1 ) ),
    decoration( this->window.get(), b )
{}

cursesxx::Widget::Widget( const Widget& parent,
        const Geometry& g,
        const Anchor& a ) :
    geometry( g ),
    anchor( parent.anchor, a ),
    window( newwin(
                this->geometry.height(),
                this->geometry.width(),
                this->anchor.y,
                this->anchor.x ) )
{}

cursesxx::Widget::Widget( const Widget& parent,
        const Anchor& a, const BorderStyle& b ) :
    geometry( parent.geometry ),
    anchor( parent.anchor, a ),
    window( newwin( 
                this->geometry.height() + 2,
                this->geometry.width() + 2,
                this->anchor.y - 1,
                this->anchor.x - 1 ) ),
    decoration( this->window.get(), b )
{}

cursesxx::Widget::Widget( const Widget& parent, const Geometry& g,
        const Anchor& a, const BorderStyle& b ) :
    geometry( g ),
    anchor( parent.anchor, a ),
    window( newwin( 
                this->geometry.height() + 2,
                this->geometry.width() + 2,
                this->anchor.y - 1,
                this->anchor.x - 1 ) ),
    decoration( this->window.get(), b )
{}

cursesxx::Widget::~Widget() {
}

int cursesxx::Widget::height() const {
    return this->geometry.height();
}

int cursesxx::Widget::width() const {
    return this->geometry.width();
}

void cursesxx::Widget::redraw() {
    wrefresh( this->window.get() );
}

void cursesxx::Widget::clear() {
    wclear( this->window.get() );
}

void cursesxx::Widget::mvhorizontal( int pos ) {
    this->x = std::max( std::min( this->geometry.width(), x + pos ), 0 );
}

void cursesxx::Widget::mvvertical( int pos ) {
    this->y = std::max( std::min( this->geometry.height(), y + pos ), 0 );
}

void cursesxx::Widget::move( int y, int x ) {
    this->mvvertical( y );
    this->mvhorizontal( x );
}

void cursesxx::Widget::write( const std::string& str ) {
    const Anchor& a = this->anchor;
    wmove( this->window.get(), a.y + y, a.x + x );
    waddstr( this->window.get(), str.c_str() );
}

void cursesxx::Widget::write( const std::string& str, const int maxlen ) {
    const Anchor& a = this->anchor;
    wmove( this->window.get(), a.y + y, a.x + x );
    waddnstr( this->window.get(), str.c_str(), maxlen );
}

void cursesxx::Widget::decorate( const cursesxx::BorderStyle& b ) {
    this->decoration.set( b );
}

void cursesxx::Widget::put( char c ) {
    wechochar( this->window.get(), c );
}

void cursesxx::Widget::put( char c, int y, int x ) {
    const Anchor& a = this->anchor;
    mvwaddch( this->window.get(), a.y + y, a.x + x, c );
}

/*
 * TEXTFIELD
 */

void cursesxx::Textfield::write() {
    this->widget.clear();
    this->widget.move( 0, 0 );
    this->widget.write( this->text );
}

void cursesxx::Textfield::write( const std::string& str ) {
    this->text = str;
    this->write();
}

void cursesxx::Textfield::append( const std::string& str ) {
    this->text.append( str );
}

void cursesxx::Textfield::redraw() {
    this->widget.redraw();
}

const cursesxx::Widget& cursesxx::Textfield::get_widget() const {
    return this->widget;
}

void cursesxx::Textfield::decorate( const cursesxx::BorderStyle& b ) {
    this->widget.decorate( b );
}

cursesxx::Geometry cursesxx::Textfield::text_wrap( const std::string& str ) {
    const auto lines = std::count( str.begin(), str.end(), '\n' );

    size_t longest_line = 0;
    size_t previous_pos = 0;
    size_t current_pos = 0;

    while( ( current_pos = str.find( '\n', current_pos ) ) ) {
        longest_line = std::max( longest_line, current_pos - previous_pos );
        previous_pos = current_pos;

        if( current_pos == std::string::npos ) break;
    }

    const auto cols = std::min( longest_line , str.size() );
    return cursesxx::Geometry( lines + 1, cols );
}

cursesxx::Geometry cursesxx::Textfield::text_wrap(
        const std::string& str, const int width ) {

    return cursesxx::Geometry( ( str.size() / width ) + 1, width );
}

/*
 * LABEL
 */
cursesxx::Label::Label( const std::string& text ) :
    widget( text )
{}

void cursesxx::Label::redraw() {
    this->widget.write();
    this->widget.redraw();
}

const cursesxx::Widget& cursesxx::Label::get_widget() const {
    return this->widget.get_widget();
}

cursesxx::Application::Screen::Screen() {
    initscr();
}

cursesxx::Application::Screen::~Screen() {
    endwin();
}

cursesxx::Application& cursesxx::Application::keypad( const bool enable ) {
    ::keypad( stdscr, enable );
    return *this;
}

cursesxx::Application& cursesxx::Application::echo( const bool enable ) {
    enable ? ::echo() : noecho();
    return *this;
}

cursesxx::Application& cursesxx::Application::cursor( const bool enable ) {
    curs_set( enable ? 1 : 0 );
    return *this;
}

int cursesxx::mid( int A, int B ) {
    return ( A - B ) / 2;
}

cursesxx::Anchor cursesxx::mid( const cursesxx::Geometry& child ) {

    return { mid( LINES - 1, child.height() ), mid( COLS - 1, child.width() ) };
}

/* rvalue? */
cursesxx::Anchor cursesxx::mid(
        const cursesxx::Widget& parent,
        const cursesxx::Geometry& child ) {

    return { mid( parent.height(), child.height() ),
        mid( parent.width(), child.width() ) };
}
