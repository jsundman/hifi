//
//  SimpleDisplayPlugin.h
//
//  Created by Bradley Austin Davis on 2014/04/13.
//  Copyright 2015 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//
#pragma once

#include "DisplayPlugin.h"

#include <QCursor>

#include <QOpenGLContext>
#include <GLMHelpers.h>
#include <RenderUtil.h>
#include <GlWindow.h>

class SimpleGlDisplayPlugin : public DisplayPlugin {
public:
    virtual void activate();
    virtual void display(GLuint sceneTexture, const glm::uvec2& sceneSize,
                         GLuint overlayTexture, const glm::uvec2& overlaySize);
};

template <typename T>
class SimpleDisplayPlugin : public SimpleGlDisplayPlugin {
public:
    virtual glm::ivec2 getTrueMousePosition() const {
        return toGlm(_window->mapFromGlobal(QCursor::pos()));
    }

protected:
    void makeCurrent() final {
        _window->makeCurrent();
    }

    void doneCurrent() final {
        _window->doneCurrent();
    }

    void swapBuffers() final {
        _window->swapBuffers();
    }

protected:
    T * _window{ nullptr };
};


class GlWindowDisplayPlugin : public SimpleDisplayPlugin<GlWindow> {
public:
    virtual void activate();
    virtual void deactivate();
    virtual QSize getDeviceSize() const final;
    virtual glm::ivec2 getCanvasSize() const final;
    virtual bool hasFocus() const;
    virtual bool eventFilter(QObject* object, QEvent* event);
};
