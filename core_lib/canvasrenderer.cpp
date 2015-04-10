/*

Pencil - Traditional Animation Software
Copyright (C) 2005-2007 Patrick Corrieri & Pascal Naidon
Copyright (C) 2013-2014 Matt Chiawen Chang

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation;

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

#include "canvasrenderer.h"
#include "object.h"
#include "layerbitmap.h"
#include "layervector.h"
#include "bitmapimage.h"
#include "vectorimage.h"
#include <QPainter>
#include <QLoggingCategory>


CanvasRenderer::CanvasRenderer( QObject* parent )
    : QObject( parent )
    , mLog( "CanvasRenderer" )
{
}

CanvasRenderer::~CanvasRenderer()
{
}

void CanvasRenderer::setCanvas( QPixmap* canvas )
{
    Q_ASSERT( canvas );
    mCanvas = canvas;
}

void CanvasRenderer::setViewTransform( QTransform viewTransform )
{
    mViewTransform = viewTransform;
}

void CanvasRenderer::paint( Object* object, int layer, int frame )
{
    Q_ASSERT( object );
    mObject = object;

    mLayerIndex = layer;
    mFrameNumber = frame;

    QPainter painter( mCanvas );
    painter.setTransform( mViewTransform );
    painter.setRenderHint( QPainter::SmoothPixmapTransform, false );
    painter.setWorldMatrixEnabled( true );

    paintBackground( painter );
    paintOnionSkin( painter );
    paintCurrentFrame( painter );
}

void CanvasRenderer::paintBackground( QPainter& painter )
{
    painter.setPen( Qt::NoPen );
    painter.setBrush( QBrush( Qt::white ) );

    mCanvas->fill( Qt::white );
}

void CanvasRenderer::paintOnionSkin( QPainter& painter )
{
    Layer* layer = mObject->getLayer( mLayerIndex );

    if ( layer->keyFrameCount() == 0 )
    {
        return;
    }

    int iStartFrame = std::max( mFrameNumber - 3, 1 );
    int iEndFrame = mFrameNumber + 3;

    // Paint onion skin before current frame.
    for ( int i = iStartFrame; i < mFrameNumber; ++i )
    {
        switch ( layer->type() )
        {
            case Layer::BITMAP: { paintOnionSkinBitmap( painter, layer, i ); break; }
            case Layer::VECTOR: { paintOnionSkinVector( painter, layer, i ); break; }
            case Layer::CAMERA: break;
            case Layer::SOUND: break;
            default: Q_ASSERT( false ); break;
        }
    }
}

void CanvasRenderer::paintOnionSkinBitmap( QPainter& painter, Layer* layer, int nFrame )
{
    LayerBitmap* bitmapLayer = dynamic_cast< LayerBitmap* >( layer );
    if ( bitmapLayer == nullptr )
    {
        Q_ASSERT( bitmapLayer );
        return;
    }

    qCDebug( mLog ) << "Paint Onion skin bitmap, Frame = " << nFrame;
    BitmapImage* bitmapImage = bitmapLayer->getBitmapImageAtFrame( nFrame );
    if ( bitmapImage == nullptr )
    {
        return;
    }

    bitmapImage->paintImage( painter );
}


void CanvasRenderer::paintOnionSkinVector( QPainter& painter, Layer* layer, int nFrame )
{
    LayerVector* vectorLayer = dynamic_cast< LayerVector* >( layer );
    if ( vectorLayer == nullptr )
    {
        Q_ASSERT( vectorLayer );
        return;
    }

    qCDebug( mLog ) << "Paint Onion skin vector, Frame = " << nFrame;
    VectorImage* vectorImage = vectorLayer->getVectorImageAtFrame( nFrame );
    if ( vectorImage == nullptr )
    {
        return;
    }

    vectorImage->paintImage( painter, true, true, true );
}

void CanvasRenderer::paintCurrentFrame( QPainter& painter )
{

}