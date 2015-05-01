//
//  NullDisplayPlugin.cpp
//
//  Created by Bradley Austin Davis on 2014/04/13.
//  Copyright 2015 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//
#include "NullDisplayPlugin.h"

const QString NullDisplayPlugin::NAME("NullDisplayPlugin");

const QString & NullDisplayPlugin::getName() {
    return NAME;
}

QSize NullDisplayPlugin::getRecommendedFramebufferSize() const {
    return QSize(100, 100);
}

glm::ivec2 NullDisplayPlugin::getCanvasSize() const {
    return glm::ivec2(100, 100);
}

bool NullDisplayPlugin::hasFocus() const {
    return false;
}

glm::ivec2 NullDisplayPlugin::getRelativeMousePosition() const {
    return glm::ivec2();
}

glm::ivec2 NullDisplayPlugin::getTrueMousePosition() const {
    return glm::ivec2();
}

PickRay NullDisplayPlugin::computePickRay(const glm::vec2 & pos) const {
    return PickRay();
}

bool NullDisplayPlugin::isMouseOnScreen() {
    return false;
}