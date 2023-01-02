#include "MainItem.h"

#include "DashboardPage.h"
#include "DevicesPage.h"
#include "MenuBar.h"
#include "MembersPage.h"
#include "RoomsPage.h"
#include "StatisticsPage.h"
#include "StoragePage.h"

#include <QskGesture.h>
#include <QskEvent.h>
#include <QskLinearBox.h>
#include <QskStackBoxAnimator.h>

#include <QQuickFramebufferObject>
#include <QGuiApplication>
#include <QQuickWindow>

namespace
{
    Qsk::Direction direction( const int from, const int to )
    {
        if( from < Cube::Top ) // side
        {
            if( to < Cube::Top ) // side to side
            {
                return ( to > from ) ? Qsk::LeftToRight : Qsk::RightToLeft; // ### 2x case
            }
            else
            {
                return ( to == Cube::Top ) ? Qsk::BottomToTop : Qsk::TopToBottom; // ### 2x case
            }
        }
        else if( from == Cube::Top )
        {
            return Qsk::TopToBottom; // ### 2x case
        }
        else
        {
            return Qsk::BottomToTop; // ### 2x case
        }
    }
}

Cube::Cube( QQuickItem* parent )
    : QskStackBox( false, parent )
{
}

void Cube::startAnimation( Qsk::Direction direction )
{
    using Animator = QskStackBoxAnimator4;

    auto animator = qobject_cast< Animator* >( this->animator() );

    if ( animator == nullptr )
    {
        animator = new Animator( this );
        animator->setEasingCurve( QEasingCurve::InOutQuad );
        animator->setDuration( 1000 );

        setAnimator( animator );
    }

    const auto orientation = ( direction == Qsk::LeftToRight || direction == Qsk::RightToLeft )
            ? Qt::Horizontal : Qt::Vertical;
    animator->setOrientation( orientation );

    const bool inverted = ( direction == Qsk::LeftToRight || direction == Qsk::TopToBottom );
    animator->setInverted( inverted );

    int newIndex = 0;

    switch( direction )
    {
        case Qsk::LeftToRight:
        case Qsk::TopToBottom:
            newIndex = currentIndex() + 1;
            break;
        case Qsk::RightToLeft:
        case Qsk::BottomToTop:
            newIndex = currentIndex() - 1;
            break;
    }

    newIndex %= itemCount();
    if( newIndex < 0 )
        newIndex += itemCount();

    setCurrentIndex( newIndex );
}

MainItem::MainItem( QQuickItem* parent )
    : QskControl( parent )
    , m_mainLayout( new QskLinearBox( Qt::Horizontal, this ) )
    , m_menuBar( new MenuBar( m_mainLayout ) )
    , m_cube( new Cube( m_mainLayout ) )
    , m_currentIndex( 0 )
{
    setAutoLayoutChildren( true );
    setAcceptedMouseButtons( Qt::LeftButton );
    setFiltersChildMouseEvents( true );

    m_panRecognizer.setOrientations( Qt::Horizontal | Qt::Vertical );
    m_panRecognizer.setMinDistance( 50 );
    m_panRecognizer.setWatchedItem( this );

    m_mainLayout->setSpacing( 0 );

    connect( m_menuBar, &MenuBar::pageChangeRequested, this, &MainItem::switchToPage );

    auto* const dashboardPage = new DashboardPage( m_cube );
    auto* const roomsPage = new RoomsPage( m_cube );
    auto* const devicesPage = new DevicesPage( m_cube );
    auto* const statisticsPage = new StatisticsPage( m_cube );
    auto* const storagePage = new StoragePage( m_cube );
    auto* const membersPage = new MembersPage( m_cube );

    m_cube->addItem( dashboardPage );
    m_cube->addItem( roomsPage );
    m_cube->addItem( devicesPage );
    m_cube->addItem( statisticsPage );
    m_cube->addItem( storagePage );
    m_cube->addItem( membersPage );

    m_cube->setCurrentItem( dashboardPage );
}

void MainItem::gestureEvent( QskGestureEvent* event )
{
    if( event->gesture()->state() == QskGesture::Finished
            && event->gesture()->type() == QskGesture::Pan )
    {
        auto* panGesture = static_cast< const QskPanGesture* >( event->gesture().get() );

        const auto delta = panGesture->origin() - panGesture->position();

        Qsk::Direction direction;

        if( qAbs( delta.x() ) > qAbs( delta.y() ) )
        {
            direction = ( delta.x() < 0 ) ? Qsk::LeftToRight : Qsk::RightToLeft;
        }
        else
        {
            direction = ( delta.y() < 0 ) ? Qsk::TopToBottom : Qsk::BottomToTop;
        }

        m_cube->startAnimation( direction );
    }
}

bool MainItem::gestureFilter( QQuickItem* item, QEvent* event )
{
    auto& recognizer = m_panRecognizer;

    if( event->type() == QEvent::MouseButtonPress )
    {
        const auto mouseEvent = static_cast< QMouseEvent* >( event );

        if( ( item != this ) || ( recognizer.timeout() < 0 ) )
        {
            if( recognizer.hasProcessedBefore( mouseEvent ) )
            {
                return false;
            }
        }

        recognizer.setTimeout( ( item == this ) ? -1 : 100 );
    }

    return recognizer.processEvent( item, event, false );
}

void MainItem::switchToPage( const int index )
{
    if( m_currentIndex == index )
        return;

    const auto d = direction( m_currentIndex, index );
    m_cube->startAnimation( d );
    m_menuBar->setActivePage( index );
    m_currentIndex = index;
}

#include "moc_MainItem.cpp"
