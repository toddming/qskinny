/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include "GridAccessor.h"
#include <QQuickWidget>

class QGraphicsGridLayout;

class GridQuick : public QQuickWidget, public GridAccessor
{
  public:
    GridQuick( QWidget* parent = nullptr );
    ~GridQuick() override;

    void insert( const QByteArray& colorName,
        int row, int column, int rowSpan, int columnSpan ) override;

    void setSpacing( Qt::Orientations, int spacing ) override;

    void setStretchFactor( int pos, Qt::Orientation, int stretch ) override;
    void setSizeHint( int pos, Qt::Orientation, Qt::SizeHint, int hint ) override;

    void setSizeHintAt( int index, Qt::Orientation, Qt::SizeHint, int hint ) override;
    void setSizePolicyAt( int index, Qt::Orientation, int policy ) override;
    void setAlignmentAt( int index, Qt::Alignment ) override;
    void setRetainSizeWhenHiddenAt( int index, bool on ) override;
    void setVisibleAt( int index, bool on ) override;

    QSize preferredSize() const override;

  protected:
    void resizeEvent( QResizeEvent* ) override;

  private:
    QQuickItem* m_grid;
};
