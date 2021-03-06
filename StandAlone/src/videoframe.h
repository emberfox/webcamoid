/* Webcamoid, webcam capture application.
 * Copyright (C) 2011-2016  Gonzalo Exequiel Pedone
 *
 * Webcamoid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Webcamoid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Webcamoid. If not, see <http://www.gnu.org/licenses/>.
 *
 * Web-Site: http://webcamoid.github.io/
 */

#ifndef VIDEOFRAME_H
#define VIDEOFRAME_H

#include <QSGTexture>
#include <QGLContext>

#include "akutils.h"

class VideoFrame: public QSGTexture
{
    Q_OBJECT

    public:
        explicit VideoFrame(const AkPacket &packet=AkPacket());
        ~VideoFrame();

        VideoFrame &operator =(const AkPacket &packet);

        void bind();
        bool hasAlphaChannel() const;
        bool hasMipmaps() const;
        int textureId() const;
        QSize textureSize() const;

    private:
        QImage m_image;
        GLuint m_textureId;
};

#endif // VIDEOFRAME_H
