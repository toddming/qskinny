/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "QskSkinHintTable.h"
#include "QskAnimationHint.h"

#include <limits>

const QVariant QskSkinHintTable::invalidHint;

inline const QVariant* qskResolvedHint( QskAspect aspect,
    const std::unordered_map< QskAspect, QVariant >& hints,
    QskAspect* resolvedAspect )
{
    auto a = aspect;

    Q_FOREVER
    {
        auto it = hints.find( aspect );
        if ( it != hints.cend() )
        {
            if ( resolvedAspect )
                *resolvedAspect = aspect;

            return &it->second;
        }

#if 1
        /*
            We intend to remove the obscure mechanism of resolving a hint
            by dropping the state bits ony by one in the future. Instead we
            will have methods in QskSkinHintTableEditor, that allow
            to set combinations of states in one call.
         */
        if ( const auto topState = aspect.topState() )
        {
            aspect.clearState( topState );
            continue;
        }
#else
        if ( aspect.hasState() )
        {
            aspect.clearStates();
            continue;
        }
#endif

        if ( aspect.variation() )
        {
            // clear the variation bits and restart
            aspect = a;
            aspect.setVariation( QskAspect::NoVariation );

            continue;
        }

        if ( aspect.section() != QskAspect::Body )
        {
            // try to resolve from QskAspect::Body

            a.setSection( QskAspect::Body );
            aspect = a;

            continue;
        }

        return nullptr;
    }
}

QskSkinHintTable::QskSkinHintTable()
{
}

QskSkinHintTable::~QskSkinHintTable()
{
    delete m_hints;
}

const std::unordered_map< QskAspect, QVariant >& QskSkinHintTable::hints() const
{
    if ( m_hints )
        return *m_hints;

    static std::unordered_map< QskAspect, QVariant > dummyHints;
    return dummyHints;
}

#define QSK_ASSERT_COUNTER( x ) Q_ASSERT( x < std::numeric_limits< decltype( x ) >::max() )

bool QskSkinHintTable::setHint( QskAspect aspect, const QVariant& skinHint )
{
    if ( m_hints == nullptr )
        m_hints = new HintMap();

    auto it = m_hints->find( aspect );
    if ( it == m_hints->end() )
    {
        m_hints->emplace( aspect, skinHint );

        if ( aspect.isAnimator() )
        {
            m_animatorCount++;
            QSK_ASSERT_COUNTER( m_animatorCount );
        }

        m_states |= aspect.states();

        return true;
    }

    if ( it->second != skinHint )
    {
        it->second = skinHint;
        return true;
    }

    return false;
}

#undef QSK_ASSERT_COUNTER

bool QskSkinHintTable::removeHint( QskAspect aspect )
{
    if ( m_hints == nullptr )
        return false;

    const bool erased = m_hints->erase( aspect );

    if ( erased )
    {
        if ( aspect.isAnimator() )
            m_animatorCount--;

        // how to clear m_states ? TODO ...

        if ( m_hints->empty() )
        {
            delete m_hints;
            m_hints = nullptr;
        }
    }

    return erased;
}

QVariant QskSkinHintTable::takeHint( QskAspect aspect )
{
    if ( m_hints )
    {
        auto it = m_hints->find( aspect );
        if ( it != m_hints->end() )
        {
            const auto value = it->second;
            m_hints->erase( it );

            if ( aspect.isAnimator() )
                m_animatorCount--;

            // how to clear m_states ? TODO ...

            if ( m_hints->empty() )
            {
                delete m_hints;
                m_hints = nullptr;
            }

            return value;
        }
    }

    return QVariant();
}

void QskSkinHintTable::clear()
{
    delete m_hints;
    m_hints = nullptr;

    m_animatorCount = 0;
    m_states = QskAspect::NoState;
}

const QVariant* QskSkinHintTable::resolvedHint(
    QskAspect aspect, QskAspect* resolvedAspect ) const
{
    if ( m_hints != nullptr )
        return qskResolvedHint( aspect & m_states, *m_hints, resolvedAspect );

    return nullptr;
}

QskAspect QskSkinHintTable::resolvedAspect( QskAspect aspect ) const
{
    QskAspect a;

    if ( m_hints != nullptr )
        qskResolvedHint( aspect & m_states, *m_hints, &a );

    return a;
}

QskAspect QskSkinHintTable::resolvedAnimator(
    QskAspect aspect, QskAnimationHint& hint ) const
{
    if ( m_hints && m_animatorCount > 0 )
    {
        aspect &= m_states;

        Q_FOREVER
        {
            auto it = m_hints->find( aspect );
            if ( it != m_hints->cend() )
            {
                hint = it->second.value< QskAnimationHint >();
                return aspect;
            }

            if ( const auto topState = aspect.topState() )
                aspect.clearState( topState );
            else
                break;
        }
    }

    return QskAspect();
}

QskAnimationHint QskSkinHintTable::animation( QskAspect aspect ) const
{
    aspect.setAnimator( true );
    return hint< QskAnimationHint >( aspect );
}

bool QskSkinHintTable::setAnimation(
    QskAspect aspect, QskAnimationHint animation )
{
    aspect.setAnimator( true );
    return setHint( aspect, animation );
}

bool QskSkinHintTable::isResolutionMatching(
    QskAspect aspect1, QskAspect aspect2 ) const
{
    // remove states we do not have early
    aspect1 &= m_states;
    aspect2 &= m_states;

    if ( aspect1 == aspect2 )
        return true;

    if ( aspect1.trunk() != aspect2.trunk() )
        return false;

    auto a1 = aspect1;
    auto a2 = aspect2;

    Q_FOREVER
    {
        const auto state1 = aspect1.topState();
        const auto state2 = aspect2.topState();

        if ( state1 > state2 )
        {
            if ( hasHint( aspect1 ) )
                return false;

            aspect1.clearState( state1 );
            continue;
        }

        if ( state2 > state1 )
        {
            if ( hasHint( aspect2 ) )
                return false;

            aspect2.clearState( state2 );
            continue;
        }

        if ( aspect1 == aspect2 )
        {
            if ( hasHint( aspect1 ) )
                return true;

            if ( state1 == 0 )
            {
                if ( aspect1.variation() == QskAspect::NoVariation )
                {
                    if ( aspect1.section() == QskAspect::Body )
                        return true;

                    // clear the section bits and restart with the initial state

                    a1.setSection( QskAspect::Body );
                    a2.setSection( QskAspect::Body );

                    aspect1 = a1;
                    aspect2 = a2;

                }
                else
                {
                    // clear the variation bits and restart with the initial state
                    aspect1 = a1;
                    aspect1.setVariation( QskAspect::NoVariation );

                    aspect2 = a2;
                    aspect2.setVariation( QskAspect::NoVariation );
                }

                continue;
            }
        }
        else
        {
            if ( hasHint( aspect1 ) || hasHint( aspect2 ) )
                return false;
        }

        aspect1.clearState( state1 );
        aspect2.clearState( state2 );
    }
}
