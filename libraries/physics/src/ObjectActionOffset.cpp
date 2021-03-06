//
//  ObjectActionOffset.cpp
//  libraries/physics/src
//
//  Created by Seth Alves 2015-6-17
//  Copyright 2015 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "ObjectActionOffset.h"

ObjectActionOffset::ObjectActionOffset(QUuid id, EntityItemPointer ownerEntity) :
    ObjectAction(id, ownerEntity) {
    #if WANT_DEBUG
    qDebug() << "ObjectActionOffset::ObjectActionOffset";
    #endif
}

ObjectActionOffset::~ObjectActionOffset() {
    #if WANT_DEBUG
    qDebug() << "ObjectActionOffset::~ObjectActionOffset";
    #endif
}

void ObjectActionOffset::updateActionWorker(btScalar deltaTimeStep) {
    if (!tryLockForRead()) {
        // don't risk hanging the thread running the physics simulation
        return;
    }

    void* physicsInfo = _ownerEntity->getPhysicsInfo();
    if (!physicsInfo) {
        unlock();
        return;
    }
    ObjectMotionState* motionState = static_cast<ObjectMotionState*>(physicsInfo);
    btRigidBody* rigidBody = motionState->getRigidBody();
    if (!rigidBody) {
        unlock();
        qDebug() << "ObjectActionOffset::updateActionWorker no rigidBody";
        return;
    }

    if (_positionalTargetSet) {
        glm::vec3 offset = _pointToOffsetFrom - bulletToGLM(rigidBody->getCenterOfMassPosition());
        float offsetLength = glm::length(offset);
        float offsetError = _linearDistance - offsetLength;

        // if (glm::abs(offsetError) > IGNORE_POSITION_DELTA) {
        if (glm::abs(offsetError) > 0.0f) {
            float offsetErrorAbs = glm::abs(offsetError);
            float offsetErrorDirection = - offsetError / offsetErrorAbs;
            glm::vec3 previousVelocity = bulletToGLM(rigidBody->getLinearVelocity());

            glm::vec3 velocityAdjustment = glm::normalize(offset) * offsetErrorDirection * offsetErrorAbs / _linearTimeScale;
            rigidBody->setLinearVelocity(glmToBullet(previousVelocity + velocityAdjustment));
            // rigidBody->setLinearVelocity(glmToBullet(velocityAdjustment));
            rigidBody->activate();
        }
    }

    unlock();
}


bool ObjectActionOffset::updateArguments(QVariantMap arguments) {
    bool pOk0 = true;
    glm::vec3 pointToOffsetFrom =
        EntityActionInterface::extractVec3Argument("offset action", arguments, "pointToOffsetFrom", pOk0, true);

    bool pOk1 = true;
    float linearTimeScale =
        EntityActionInterface::extractFloatArgument("offset action", arguments, "linearTimeScale", pOk1, false);

    bool pOk2 = true;
    float linearDistance =
        EntityActionInterface::extractFloatArgument("offset action", arguments, "linearDistance", pOk2, false);

    if (!pOk0) {
        return false;
    }
    if (pOk1 && linearTimeScale <= 0.0f) {
        qDebug() << "offset action -- linearTimeScale must be greater than zero.";
        return false;
    }

    lockForWrite();

    _pointToOffsetFrom = pointToOffsetFrom;
    _positionalTargetSet = true;

    if (pOk1) {
        _linearTimeScale = linearTimeScale;
    } else {
        _linearTimeScale = 0.1f;
    }

    if (pOk2) {
        _linearDistance = linearDistance;
    } else {
        _linearDistance = 1.0f;
    }

    _active = true;
    unlock();
    return true;
}
