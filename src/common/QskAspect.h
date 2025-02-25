/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#ifndef QSK_ASPECT_H
#define QSK_ASPECT_H

#include "QskGlobal.h"

#include <qmetaobject.h>
#include <qnamespace.h>

class QSK_EXPORT QskAspect
{
    Q_GADGET

  public:

    enum Type : quint8
    {
        NoType = 0,

        Metric = 1,
        Color  = 2,
    };
    Q_ENUM( Type )

    static constexpr uint typeCount = 3;

    enum Section : quint8
    {
        Body = 0,

        Header,
        Footer,

        Card,

        Floating
    };
    Q_ENUM( Section )

    static constexpr uint FirstUserSection = Section::Floating + 1;
    static constexpr uint LastSection = ( 1 << 4 ) - 1;

    enum Primitive : quint8
    {
        NoPrimitive = 0,

        Alignment,
        Direction,
        Style,
        Option,

        GraphicRole,
        FontRole,

        Symbol,

        TextColor,
        StyleColor,
        LinkColor,

        StrutSize,
        Size,
        Position,

        Margin,
        Padding,
        Spacing,

        Shadow,
        Shape,
        Border,

        Graduation
    };
    Q_ENUM( Primitive )

    enum Variation : quint8
    {
        NoVariation = 0,

        Vertical = Qt::Vertical,
        Horizontal = Qt::Horizontal,

        Lower  = 1,
        Upper  = 2,

        Top    = 1,
        Left   = 2,
        Right  = 3,
        Bottom = 4,

        Tiny  = 1,
        Small = 2,
        Large = 3,
        Huge  = 4
    };
    Q_ENUM( Variation )

    enum Subcontrol : quint16
    {
        NoSubcontrol   = 0,
        LastSubcontrol = ( 1 << 12 ) - 1
    };

    Q_ENUM( Subcontrol )

    enum State : quint16
    {
        NoState          = 0,

        FirstSystemState = 1 << 0,
        FirstUserState   = 1 << 5,
        LastUserState    = 1 << 11,
        LastSystemState  = 1 << 15,

        AllStates        =   0xFFFF
    };

    Q_ENUM( State )
    Q_DECLARE_FLAGS( States, State )

    constexpr QskAspect() noexcept;

    constexpr QskAspect( Subcontrol ) noexcept;
    constexpr QskAspect( Section ) noexcept;
    constexpr QskAspect( Type ) noexcept;
    constexpr QskAspect( Variation ) noexcept;

    constexpr QskAspect( const QskAspect& ) noexcept = default;
    constexpr QskAspect( QskAspect&& ) noexcept = default;

    QskAspect& operator=( const QskAspect& ) noexcept = default;

    bool operator==( const QskAspect& ) const noexcept;
    bool operator!=( const QskAspect& ) const noexcept;

    bool operator<( const QskAspect& ) const noexcept;

    constexpr QskAspect operator|( Subcontrol ) const noexcept;
    constexpr QskAspect operator|( Section ) const noexcept;
    constexpr QskAspect operator|( Type ) const noexcept;
    constexpr QskAspect operator|( Primitive ) const noexcept;
    constexpr QskAspect operator|( Variation ) const noexcept;

    constexpr QskAspect operator|( State ) const noexcept;
    QskAspect& operator|=( State ) noexcept;

    constexpr QskAspect operator&( State ) const noexcept;
    QskAspect& operator&=( State ) noexcept;

    constexpr QskAspect operator|( States ) const noexcept;
    QskAspect& operator|=( States ) noexcept;

    constexpr QskAspect operator&( States ) const noexcept;
    QskAspect& operator&=( States ) noexcept;

    constexpr QskAspect stateless() const noexcept;
    constexpr QskAspect trunk() const noexcept;

    constexpr quint64 value() const noexcept;

    constexpr bool isAnimator() const noexcept;
    void setAnimator( bool on ) noexcept;

    constexpr Subcontrol subControl() const noexcept;
    void setSubcontrol( Subcontrol ) noexcept;
    constexpr bool hasSubcontrol() const noexcept;
    void clearSubcontrol() noexcept;

    constexpr Section section() const noexcept;
    void setSection( Section ) noexcept;

    constexpr Type type() const noexcept;
    void setType( Type ) noexcept;

    constexpr bool isMetric() const noexcept;
    constexpr bool isColor() const noexcept;

    constexpr Variation variation() const noexcept;
    void setVariation( Variation ) noexcept;

    constexpr States states() const noexcept;
    constexpr bool hasStates() const noexcept;

    State topState() const noexcept;
    void clearState( State ) noexcept;

    void setStates( States ) noexcept;
    void addStates( States ) noexcept;
    void clearStates( States = AllStates ) noexcept;

    constexpr Primitive primitive() const noexcept;
    void setPrimitive( Type, Primitive primitive ) noexcept;
    void clearPrimitive() noexcept;

    constexpr Primitive colorPrimitive() const noexcept;
    constexpr Primitive metricPrimitive() const noexcept;

    const char* toPrintable() const;

    static State registerState( const QMetaObject*, State, const char* );
    static Subcontrol nextSubcontrol( const QMetaObject*, const char* );

    static QByteArray subControlName( Subcontrol );
    static QVector< QByteArray > subControlNames( const QMetaObject* = nullptr );
    static QVector< Subcontrol > subControls( const QMetaObject* );

    static quint8 primitiveCount();
    static void reservePrimitives( quint8 count );

  private:
    constexpr QskAspect( Subcontrol, Section, Type, Variation ) noexcept;

    constexpr QskAspect( uint subControl, uint section, uint type, bool isAnimator,
        uint primitive, uint variation, uint states ) noexcept;

    struct Bits
    {
        uint subControl : 12;
        uint section    : 4;

        uint type       : 3;
        uint isAnimator : 1;

        uint primitive : 5;
        uint variation : 3;
        uint reserved1 : 4;

        uint states    : 16;

        uint reserved2 : 16;
    };

    union
    {
        Bits m_bits;
        quint64 m_value;
    };
};

Q_DECLARE_TYPEINFO( QskAspect, Q_MOVABLE_TYPE );
Q_DECLARE_OPERATORS_FOR_FLAGS( QskAspect::States )

constexpr inline QskAspect::State operator<<( QskAspect::State a, const int b ) noexcept
{
    using underlying = typename std::underlying_type< QskAspect::State >::type;
    return static_cast< QskAspect::State >( static_cast< underlying >( a ) << b );
}

constexpr inline QskAspect::State operator>>( QskAspect::State a, const int b ) noexcept
{
    using underlying = typename std::underlying_type< QskAspect::State >::type;
    return static_cast< QskAspect::State >( static_cast< underlying >( a ) >> b );
}

inline constexpr QskAspect::QskAspect() noexcept
    : QskAspect( NoSubcontrol, Body, NoType, NoVariation )
{
}

inline constexpr QskAspect::QskAspect( Subcontrol subControl ) noexcept
    : QskAspect( subControl, Body, NoType, NoVariation )
{
}

inline constexpr QskAspect::QskAspect( Section section ) noexcept
    : QskAspect( NoSubcontrol, section, NoType, NoVariation )
{
}

inline constexpr QskAspect::QskAspect( Type type ) noexcept
    : QskAspect( NoSubcontrol, Body, type, NoVariation )
{
}

inline constexpr QskAspect::QskAspect( Variation variation ) noexcept
    : QskAspect( NoSubcontrol, Body, NoType, variation )
{
}

inline constexpr QskAspect::QskAspect(
        Subcontrol subControl, Section section, Type type, Variation variation ) noexcept
    : QskAspect( subControl, section, type, false, 0, variation, NoState )
{
}

inline constexpr QskAspect::QskAspect( uint subControl, uint section, uint type,
        bool isAnimator, uint primitive, uint variation, uint states ) noexcept
    : m_bits { subControl, section, type, isAnimator, primitive, variation, 0, states, 0 }
{
}

inline bool QskAspect::operator==( const QskAspect& other ) const noexcept
{
    return m_value == other.m_value;
}

inline bool QskAspect::operator!=( const QskAspect& other ) const noexcept
{
    return m_value != other.m_value;
}

inline bool QskAspect::operator<( const QskAspect& other ) const noexcept
{
    return m_value < other.m_value;
}

inline constexpr QskAspect QskAspect::operator|( Subcontrol subControl ) const noexcept
{
    return QskAspect( subControl, m_bits.section, m_bits.type,
        m_bits.isAnimator, m_bits.primitive, m_bits.variation, m_bits.states );
}

inline constexpr QskAspect QskAspect::operator|( Section section ) const noexcept
{
    return QskAspect( m_bits.subControl, section, m_bits.type,
        m_bits.isAnimator, m_bits.primitive, m_bits.variation, m_bits.states );
}

inline constexpr QskAspect QskAspect::operator|( Type type ) const noexcept
{
    return QskAspect( m_bits.subControl, m_bits.section, type,
        m_bits.isAnimator, m_bits.primitive, m_bits.variation, m_bits.states );
}

inline constexpr QskAspect QskAspect::operator|( Primitive primitive ) const noexcept
{
    return QskAspect( m_bits.subControl, m_bits.section, m_bits.type,
        m_bits.isAnimator, primitive, m_bits.variation, m_bits.states );
}

inline constexpr QskAspect QskAspect::operator|( Variation variation ) const noexcept
{
    return QskAspect( m_bits.subControl, m_bits.section, m_bits.type,
        m_bits.isAnimator, m_bits.primitive, variation, m_bits.states );
}

inline constexpr QskAspect QskAspect::operator|( State state ) const noexcept
{
    return QskAspect( m_bits.subControl, m_bits.section, m_bits.type,
        m_bits.isAnimator, m_bits.primitive, m_bits.variation, m_bits.states | state );
}

inline QskAspect& QskAspect::operator|=( State state ) noexcept
{
    m_bits.states |= state;
    return *this;
}

inline constexpr QskAspect QskAspect::operator&( State state ) const noexcept
{
    return QskAspect( m_bits.subControl, m_bits.section, m_bits.type, m_bits.isAnimator,
        m_bits.primitive, m_bits.variation, m_bits.states & state );
}

inline QskAspect& QskAspect::operator&=( State state ) noexcept
{
    m_bits.states &= state;
    return *this;
}

inline constexpr QskAspect QskAspect::operator|( States states ) const noexcept
{
    return QskAspect( m_bits.subControl, m_bits.section, m_bits.type, m_bits.isAnimator,
        m_bits.primitive, m_bits.variation, m_bits.states | states );
}

inline QskAspect& QskAspect::operator|=( States states ) noexcept
{
    m_bits.states |= states;
    return *this;
}

inline constexpr QskAspect QskAspect::operator&( States states ) const noexcept
{
    return QskAspect( m_bits.subControl, m_bits.section, m_bits.type,
        m_bits.isAnimator, m_bits.primitive, m_bits.variation, m_bits.states & states );
}

inline QskAspect& QskAspect::operator&=( States states ) noexcept
{
    m_bits.states &= states;
    return *this;
}

inline constexpr QskAspect QskAspect::stateless() const noexcept
{
    return QskAspect( m_bits.subControl, m_bits.section, m_bits.type,
        m_bits.isAnimator, m_bits.primitive, m_bits.variation, 0 );
}

inline constexpr QskAspect QskAspect::trunk() const noexcept
{
    return QskAspect( m_bits.subControl, 0, m_bits.type,
        m_bits.isAnimator, m_bits.primitive, 0, 0 );
}

inline constexpr quint64 QskAspect::value() const noexcept
{
    return m_value;
}

inline constexpr bool QskAspect::isAnimator() const noexcept
{
    return m_bits.isAnimator;
}

inline void QskAspect::setAnimator( bool on ) noexcept
{
    m_bits.isAnimator = on;
}

inline constexpr QskAspect::Subcontrol QskAspect::subControl() const noexcept
{
    return static_cast< Subcontrol >( m_bits.subControl );
}

inline void QskAspect::setSubcontrol( Subcontrol subControl ) noexcept
{
    m_bits.subControl = subControl;
}

inline constexpr bool QskAspect::hasSubcontrol() const noexcept
{
    return m_bits.subControl != 0;
}

inline void QskAspect::clearSubcontrol() noexcept
{
    m_bits.subControl = 0;
}

inline constexpr QskAspect::Section QskAspect::section() const noexcept
{
    return static_cast< Section >( m_bits.section );
}

inline void QskAspect::setSection( Section section ) noexcept
{
    m_bits.section = section;
}

inline constexpr QskAspect::Type QskAspect::type() const noexcept
{
    return static_cast< Type >( m_bits.type );
}

inline void QskAspect::setType( Type type ) noexcept
{
    m_bits.type = type;
}

inline constexpr bool QskAspect::isMetric() const noexcept
{
    return type() == Metric;
}

inline constexpr bool QskAspect::isColor() const noexcept
{
    return type() == Color;
}

inline constexpr QskAspect::States QskAspect::states() const noexcept
{
    return static_cast< States >( m_bits.states );
}

inline void QskAspect::setStates( States states ) noexcept
{
    m_bits.states = states;
}

inline void QskAspect::addStates( States states ) noexcept
{
    m_bits.states |= states;
}

inline constexpr bool QskAspect::hasStates() const noexcept
{
    return m_bits.states;
}

inline void QskAspect::clearState( State state ) noexcept
{
    m_bits.states &= ~state;
}

inline void QskAspect::clearStates( States states ) noexcept
{
    m_bits.states &= ~states;
}

inline constexpr QskAspect::Primitive QskAspect::primitive() const noexcept
{
    return static_cast< QskAspect::Primitive >( m_bits.primitive );
}

inline void QskAspect::setPrimitive( Type type, QskAspect::Primitive primitive ) noexcept
{
    m_bits.type = type;
    m_bits.primitive = primitive;
}

inline constexpr QskAspect::Primitive QskAspect::colorPrimitive() const noexcept
{
    return ( m_bits.type == Color )
        ? static_cast< Primitive >( m_bits.primitive ) : NoPrimitive;
}

inline constexpr QskAspect::Primitive QskAspect::metricPrimitive() const noexcept
{
    return ( m_bits.type == Metric )
        ? static_cast< Primitive >( m_bits.primitive ) : NoPrimitive;
}

inline void QskAspect::clearPrimitive() noexcept
{
    m_bits.primitive = NoPrimitive;
}

inline constexpr QskAspect::Variation QskAspect::variation() const noexcept
{
    return static_cast< Variation >( m_bits.variation );
}

inline void QskAspect::setVariation( Variation variation ) noexcept
{
    m_bits.variation = variation;
}

inline constexpr QskAspect operator|(
    QskAspect::State state, const QskAspect& aspect ) noexcept
{
    return aspect | state;
}

inline constexpr QskAspect operator|(
    QskAspect::States states, const QskAspect& aspect ) noexcept
{
    return aspect | states;
}

inline constexpr QskAspect operator|(
    QskAspect::Subcontrol subControl, const QskAspect& aspect ) noexcept
{
    return aspect | subControl;
}

inline constexpr QskAspect operator|(
    QskAspect::Section section, const QskAspect& aspect ) noexcept
{
    return aspect | section;
}

inline constexpr QskAspect operator|(
    QskAspect::Type type, const QskAspect& aspect ) noexcept
{
    return aspect | type;
}

inline constexpr QskAspect operator|(
    QskAspect::Variation variation, const QskAspect& aspect ) noexcept
{
    return aspect | variation;
}

inline constexpr QskAspect operator|(
    QskAspect::Subcontrol subControl, QskAspect::Section section ) noexcept
{
    return QskAspect( subControl ) | section;
}

inline constexpr QskAspect operator|(
    QskAspect::Section section, QskAspect::Subcontrol subControl ) noexcept
{
    return subControl | section;
}

inline constexpr QskAspect operator|(
    QskAspect::Subcontrol subControl, QskAspect::Type type ) noexcept
{
    return QskAspect( subControl ) | type;
}

inline constexpr QskAspect operator|(
    QskAspect::Type type, QskAspect::Subcontrol subControl ) noexcept
{
    return subControl | type;
}

inline constexpr QskAspect operator|(
    QskAspect::Subcontrol subControl, QskAspect::State state ) noexcept
{
    return QskAspect( subControl ) | state;
}

inline constexpr QskAspect operator|(
    QskAspect::Subcontrol subControl, QskAspect::States states ) noexcept
{
    return QskAspect( subControl ) | states;
}

inline constexpr QskAspect operator|(
    QskAspect::Type type, QskAspect::Variation variation ) noexcept
{
    return QskAspect( type ) | variation;
}

inline constexpr QskAspect operator|(
    QskAspect::Variation variation, QskAspect::Type type ) noexcept
{
    return type | variation;
}

inline constexpr QskAspect operator|(
    QskAspect::State state, QskAspect::Subcontrol subControl ) noexcept
{
    return subControl | state;
}

inline constexpr QskAspect operator|(
    QskAspect::States states, QskAspect::Subcontrol subControl ) noexcept
{
    return subControl | states;
}

inline constexpr QskAspect operator|(
    QskAspect::Subcontrol subControl, QskAspect::Primitive primitive ) noexcept
{
    return QskAspect( subControl ) | primitive;
}

inline constexpr QskAspect operator|(
    QskAspect::Primitive primitive, QskAspect::Subcontrol subControl ) noexcept
{
    return subControl | primitive;
}

inline constexpr QskAspect operator|(
    QskAspect::Subcontrol subControl, QskAspect::Variation variation ) noexcept
{
    return QskAspect( subControl ) | variation;
}

inline constexpr QskAspect operator|(
    QskAspect::Variation variation, QskAspect::Subcontrol subControl ) noexcept
{
    return subControl | variation;
}

namespace std
{
    template< > struct hash< QskAspect >
    {
        constexpr size_t operator()( const QskAspect& aspect ) const noexcept
        {
            return aspect.value();
        }
    };
}

inline QskHashValue qHash( const QskAspect aspect, QskHashValue seed = 0 ) noexcept
{
    return qHash( aspect.value(), seed );
}

#ifndef QT_NO_DEBUG_STREAM

class QDebug;

QSK_EXPORT QDebug operator<<( QDebug, QskAspect );
QSK_EXPORT QDebug operator<<( QDebug, QskAspect::Type );
QSK_EXPORT QDebug operator<<( QDebug, QskAspect::Subcontrol );
QSK_EXPORT QDebug operator<<( QDebug, QskAspect::Primitive );
QSK_EXPORT QDebug operator<<( QDebug, QskAspect::Variation );
QSK_EXPORT QDebug operator<<( QDebug, QskAspect::States );

QSK_EXPORT void qskDebugStates( QDebug, const QMetaObject*, QskAspect::States );
QSK_EXPORT void qskDebugAspect( QDebug, const QMetaObject*, QskAspect );

#endif

#define QSK_SUBCONTROL( type, name ) \
    const QskAspect::Subcontrol type::name = \
        QskAspect::nextSubcontrol( &type::staticMetaObject, #type "::" #name );

#define QSK_STATE( type, name, value ) \
    static_assert( \
        ( ( value ) >= QskAspect::FirstUserState ) && ( ( value ) <= QskAspect::LastUserState ), \
        "Invalid state" \
    ); \
    const QskAspect::State type::name = \
        QskAspect::registerState( &type::staticMetaObject, value, #type "::" #name );

#define QSK_SYSTEM_STATE( type, name, value ) \
    static_assert( \
        ( ( value ) >= QskAspect::FirstSystemState && ( value ) < QskAspect::FirstUserState ) || \
        ( ( value ) > QskAspect::LastUserState && ( value ) <= QskAspect::LastSystemState ), \
        "Invalid system state" \
    ); \
    const QskAspect::State type::name = \
        QskAspect::registerState( &type::staticMetaObject, value, #type "::" #name );

#if !defined( _MSC_VER )

#define QSK_SUBCONTROLS( ... ) static const QskAspect::Subcontrol __VA_ARGS__;
#define QSK_STATES( ... ) static const QskAspect::State __VA_ARGS__;

#else

/*
    Working around a MSVC bug: when static member are defined in one statement
    only the first one is exported. Unfortuately the code below is also not
    compliant with ISO C++11 and gcc -pedantic f.e. would fail.
 */

#define _QSK_EXPAND(x) x

#define _QSK_EVAL_0( m, x, ... ) m(x)
#define _QSK_EVAL_1( m, x, ... ) m(x) _QSK_EXPAND( _QSK_EVAL_0( m, __VA_ARGS__ ) )
#define _QSK_EVAL_2( m, x, ... ) m(x) _QSK_EXPAND( _QSK_EVAL_1( m, __VA_ARGS__ ) )
#define _QSK_EVAL_3( m, x, ... ) m(x) _QSK_EXPAND( _QSK_EVAL_2( m, __VA_ARGS__ ) )
#define _QSK_EVAL_4( m, x, ... ) m(x) _QSK_EXPAND( _QSK_EVAL_3( m, __VA_ARGS__ ) )
#define _QSK_EVAL_5( m, x, ... ) m(x) _QSK_EXPAND( _QSK_EVAL_4( m, __VA_ARGS__ ) )
#define _QSK_EVAL_6( m, x, ... ) m(x) _QSK_EXPAND( _QSK_EVAL_5( m, __VA_ARGS__ ) )
#define _QSK_EVAL_7( m, x, ... ) m(x) _QSK_EXPAND( _QSK_EVAL_6( m, __VA_ARGS__ ) )
#define _QSK_EVAL_8( m, x, ... ) m(x) _QSK_EXPAND( _QSK_EVAL_7( m, __VA_ARGS__ ) )
#define _QSK_EVAL_9( m, x, ... ) m(x) _QSK_EXPAND( _QSK_EVAL_8( m, __VA_ARGS__ ) )
#define _QSK_EVAL_A( m, x, ... ) m(x) _QSK_EXPAND( _QSK_EVAL_9( m, __VA_ARGS__ ) )
#define _QSK_EVAL_B( m, x, ... ) m(x) _QSK_EXPAND( _QSK_EVAL_A( m, __VA_ARGS__ ) )
#define _QSK_EVAL_C( m, x, ... ) m(x) _QSK_EXPAND( _QSK_EVAL_B( m, __VA_ARGS__ ) )
#define _QSK_EVAL_D( m, x, ... ) m(x) _QSK_EXPAND( _QSK_EVAL_C( m, __VA_ARGS__ ) )
#define _QSK_EVAL_E( m, x, ... ) m(x) _QSK_EXPAND( _QSK_EVAL_D( m, __VA_ARGS__ ) )
#define _QSK_EVAL_F( m, x, ... ) m(x) _QSK_EXPAND( _QSK_EVAL_E( m, __VA_ARGS__ ) )

#define _QSK_EVAL_NARG( ... ) _QSK_EVAL_NARG_( __VA_ARGS__, _QSK_EVAL_RSEQ_N() )
#define _QSK_EVAL_NARG_( ... ) _QSK_EXPAND( _QSK_EVAL_ARG_N(  __VA_ARGS__ ) )

#define _QSK_EVAL_ARG_N( _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, A, B, C, D, E, F, N, ... ) N
#define _QSK_EVAL_RSEQ_N() F, E, D, C, B, A, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define _QSK_CAT(x,y) x ## y
#define _QSK_EVAL_( N, m, ... ) _QSK_EXPAND( _QSK_CAT( _QSK_EVAL_, N )( m, __VA_ARGS__ ) )
#define _QSK_EVAL( m, ... ) _QSK_EVAL_( _QSK_EVAL_NARG( __VA_ARGS__ ), m, __VA_ARGS__ )

#define _QSK_SUBCONTROLS_HELPER( name ) static const QskAspect::Subcontrol name;
#define QSK_SUBCONTROLS( ... ) _QSK_EVAL( _QSK_SUBCONTROLS_HELPER, __VA_ARGS__ )

#define _QSK_STATES_HELPER( name ) static const QskAspect::State name;
#define QSK_STATES( ... ) _QSK_EVAL( _QSK_STATES_HELPER, __VA_ARGS__ )

#endif

#endif
