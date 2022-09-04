/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#ifndef QSK_HINT_ANIMATOR_H
#define QSK_HINT_ANIMATOR_H

#include "QskAspect.h"
#include "QskVariantAnimator.h"
#include "QskAnimationHint.h"

#include <qpointer.h>

class QskControl;

class QSK_EXPORT QskHintAnimator : public QskVariantAnimator
{
    using Inherited = QskVariantAnimator;

  public:
    QskHintAnimator() noexcept;
    ~QskHintAnimator() override;

    void setAspect( QskAspect ) noexcept;
    QskAspect aspect() const noexcept;

    void setControl( QskControl* ) noexcept;
    QskControl* control() const noexcept;

    void setUpdateFlags( QskAnimationHint::UpdateFlags ) noexcept;
    QskAnimationHint::UpdateFlags updateFlags() const noexcept;

    void advance( qreal value ) override;

    bool operator<( const QskHintAnimator& ) const noexcept;

  private:
    QskAspect m_aspect;
    QskAnimationHint::UpdateFlags m_updateFlags;
    QPointer< QskControl > m_control;
};

class QSK_EXPORT QskHintAnimatorTable
{
  public:
    QskHintAnimatorTable();
    ~QskHintAnimatorTable();

    void start( QskControl*, QskAspect,
        QskAnimationHint, const QVariant& from, const QVariant& to );

    const QskHintAnimator* animator( QskAspect ) const;
    QVariant currentValue( QskAspect ) const;

    bool cleanup();

  private:
    void reset();

    class PrivateData;
    PrivateData* m_data;
};

inline QskAspect QskHintAnimator::aspect() const noexcept
{
    return m_aspect;
}

inline QskAnimationHint::UpdateFlags QskHintAnimator::updateFlags() const noexcept
{
    return m_updateFlags;
}

inline QskControl* QskHintAnimator::control() const noexcept
{
    return m_control;
}

inline bool QskHintAnimator::operator<( const QskHintAnimator& other ) const noexcept
{
    return m_aspect < other.m_aspect;
}

#endif
