/* Webcamod, webcam capture plasmoid.
 * Copyright (C) 2011-2012  Gonzalo Exequiel Pedone
 *
 * Webcamod is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Webcamod is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Webcamod. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email     : hipersayan DOT x AT gmail DOT com
 * Web-Site 1: http://github.com/hipersayanX/Webcamoid
 * Web-Site 2: http://kde-apps.org/content/show.php/Webcamoid?content=144796
 */

#ifndef SYNCELEMENT_H
#define SYNCELEMENT_H

#include "worker.h"

class SyncElement: public QbElement
{
    Q_OBJECT
    Q_PROPERTY(bool waitUnlock READ waitUnlock WRITE setWaitUnlock RESET resetWaitUnlock)
    Q_PROPERTY(bool noFps READ noFps WRITE setNoFps RESET resetNoFps)

    public:
        explicit SyncElement();
        ~SyncElement();

        Q_INVOKABLE bool waitUnlock() const;
        Q_INVOKABLE bool noFps() const;

    private:
        bool m_ready;
        bool m_unlocked;
        Worker *m_worker;
        QThread *m_thread;

    signals:
        void ready(int id);
        void oDiscardFrames(int nFrames);

    public slots:
        void setWaitUnlock(bool waitUnlock);
        void setNoFps(bool noFps);
        void resetWaitUnlock();
        void resetNoFps();
        void iDiscardFrames(int nFrames);

        void iStream(const QbPacket &packet);
        void setState(ElementState state);
};

#endif // SYNCELEMENT_H
