//
//  AvatarHashMap.h
//  libraries/avatars/src
//
//  Created by Stephen AndrewMeadows on 1/28/2014.
//  Copyright 2014 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_AvatarHashMap_h
#define hifi_AvatarHashMap_h

#include <QtCore/QHash>
#include <QtCore/QSharedPointer>
#include <QtCore/QUuid>

#include <memory>

#include <DependencyManager.h>
#include <Node.h>

#include "AvatarData.h"
#include <glm/glm.hpp>

class AvatarHashMap : public QObject, public Dependency {
    Q_OBJECT
    SINGLETON_DEPENDENCY
    
public:
    const AvatarHash& getAvatarHash() { return _avatarHash; }
    int size() { return _avatarHash.size(); }
    
public slots:
    void processAvatarMixerDatagram(const QByteArray& datagram, const QWeakPointer<Node>& mixerWeakPointer);
    bool isAvatarInRange(const glm::vec3 & position, const float range);
    
private slots:
    void sessionUUIDChanged(const QUuid& sessionUUID, const QUuid& oldUUID);
    
protected:
    AvatarHashMap();
    
    virtual AvatarSharedPointer newSharedAvatar();
    virtual AvatarSharedPointer addAvatar(const QUuid& sessionUUID, const QWeakPointer<Node>& mixerWeakPointer);
    virtual void removeAvatar(const QUuid& sessionUUID);
    
    AvatarHash _avatarHash;

private:
    void processAvatarDataPacket(const QByteArray& packet, const QWeakPointer<Node>& mixerWeakPointer);
    void processAvatarIdentityPacket(const QByteArray& packet, const QWeakPointer<Node>& mixerWeakPointer);
    void processAvatarBillboardPacket(const QByteArray& packet, const QWeakPointer<Node>& mixerWeakPointer);
    void processKillAvatar(const QByteArray& datagram);

    QUuid _lastOwnerSessionUUID;
};

#endif // hifi_AvatarHashMap_h
