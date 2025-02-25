/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include "Page.h"

class QskAnimator;

class ProgressBarPage : public Page
{
  public:
    ProgressBarPage( QQuickItem* = nullptr );

  private:
    void populate();

    std::unique_ptr< QskAnimator > m_determinateIndicatorsAnimator;
};
