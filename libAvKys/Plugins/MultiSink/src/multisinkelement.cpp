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

#include "multisinkelement.h"
#include "multisinkglobals.h"

Q_GLOBAL_STATIC(MultiSinkGlobals, globalMultiSink)

template<typename T>
inline QSharedPointer<T> ptr_init(QObject *obj=nullptr)
{
    if (!obj)
        return QSharedPointer<T>(new T());

    return QSharedPointer<T>(static_cast<T *>(obj));
}

MultiSinkElement::MultiSinkElement():
    AkElement(),
    m_mediaWriter(ptr_init<MediaWriter>())
{
    this->m_showFormatOptions = false;

    QObject::connect(globalMultiSink,
                     SIGNAL(codecLibChanged(const QString &)),
                     this,
                     SIGNAL(codecLibChanged(const QString &)));
    QObject::connect(globalMultiSink,
                     SIGNAL(codecLibChanged(const QString &)),
                     this,
                     SLOT(codecLibUpdated(const QString &)));

    this->codecLibUpdated(globalMultiSink->codecLib());
}

MultiSinkElement::~MultiSinkElement()
{
    this->setState(AkElement::ElementStateNull);
}

QObject *MultiSinkElement::controlInterface(QQmlEngine *engine,
                                            const QString &controlId) const
{
    Q_UNUSED(controlId)

    if (!engine)
        return NULL;

    // Load the UI from the plugin.
    QQmlComponent component(engine, QUrl(QStringLiteral("qrc:/MultiSink/share/qml/main.qml")));

    if (component.isError()) {
        qDebug() << "Error in plugin "
                 << this->metaObject()->className()
                 << ":"
                 << component.errorString();

        return NULL;
    }

    // Create a context for the plugin.
    QQmlContext *context = new QQmlContext(engine->rootContext());
    context->setContextProperty("MultiSink", const_cast<QObject *>(qobject_cast<const QObject *>(this)));
    context->setContextProperty("controlId", this->objectName());

    // Create an item with the plugin context.
    QObject *item = component.create(context);

    if (!item) {
        delete context;

        return NULL;
    }

    context->setParent(item);

    return item;
}

QString MultiSinkElement::location() const
{
    return this->m_location;
}

QStringList MultiSinkElement::supportedFormats() const
{
    return this->m_supportedFormats;
}

QString MultiSinkElement::outputFormat() const
{
    return this->m_outputFormat;
}

QVariantMap MultiSinkElement::formatOptions() const
{
    return this->m_formatOptions;
}

QVariantList MultiSinkElement::streams()
{
    return this->m_mediaWriter->streams();
}

QString MultiSinkElement::codecLib() const
{
    return globalMultiSink->codecLib();
}

bool MultiSinkElement::showFormatOptions() const
{
    return this->m_showFormatOptions;
}

QVariantList MultiSinkElement::userControls() const
{
    return this->m_userControls;
}

QVariantMap MultiSinkElement::userControlsValues() const
{
    return this->m_userControlsValues;
}

QStringList MultiSinkElement::fileExtensions(const QString &format) const
{
    return this->m_fileExtensions.value(format);
}

QString MultiSinkElement::formatDescription(const QString &format) const
{
    return this->m_formatDescription.value(format);
}

QStringList MultiSinkElement::supportedCodecs(const QString &format,
                                              const QString &type)
{
    return this->m_mediaWriter->supportedCodecs(format, type);
}

QString MultiSinkElement::defaultCodec(const QString &format,
                                       const QString &type)
{
    return this->m_mediaWriter->defaultCodec(format, type);
}

QString MultiSinkElement::codecDescription(const QString &codec) const
{
    return this->m_codecDescription.value(codec);
}

QString MultiSinkElement::codecType(const QString &codec) const
{
    return this->m_codecType.value(codec);
}

QVariantMap MultiSinkElement::defaultCodecParams(const QString &codec) const
{
    return this->m_defaultCodecParams.value(codec);
}

QVariantMap MultiSinkElement::addStream(int streamIndex,
                                        const AkCaps &streamCaps,
                                        const QVariantMap &codecParams)
{
    return this->m_mediaWriter->addStream(streamIndex,
                                          streamCaps,
                                          codecParams);
}

QVariantMap MultiSinkElement::updateStream(int index,
                                           const QVariantMap &codecParams)
{
    return this->m_mediaWriter->updateStream(index, codecParams);
}

void MultiSinkElement::setLocation(const QString &location)
{
    if (this->m_location == location)
        return;

    this->m_location = location;
    emit this->locationChanged(location);
}

void MultiSinkElement::setOutputFormat(const QString &outputFormat)
{
    if (this->m_outputFormat == outputFormat)
        return;

    this->m_outputFormat = outputFormat;
    emit this->outputFormatChanged(outputFormat);
}

void MultiSinkElement::setFormatOptions(const QVariantMap &formatOptions)
{
    if (this->m_formatOptions == formatOptions)
        return;

    this->m_formatOptions = formatOptions;
    emit this->formatOptionsChanged(formatOptions);
}

void MultiSinkElement::setCodecLib(const QString &codecLib)
{
    globalMultiSink->setCodecLib(codecLib);
}

void MultiSinkElement::setShowFormatOptions(bool showFormatOptions)
{
    if (this->m_showFormatOptions == showFormatOptions)
        return;

    this->m_showFormatOptions = showFormatOptions;
    emit this->showFormatOptionsChanged(showFormatOptions);
}

void MultiSinkElement::setUserControls(const QVariantList &userControls)
{
    if (this->m_userControls == userControls)
        return;

    this->m_userControls = userControls;
    emit this->userControlsChanged(userControls);
}

void MultiSinkElement::setUserControlsValues(const QVariantMap &userControlsValues)
{
    if (this->m_userControlsValues == userControlsValues)
        return;

    this->m_userControlsValues = userControlsValues;
    emit this->userControlsValuesChanged(userControlsValues);
}

void MultiSinkElement::resetLocation()
{
    this->setLocation("");
}

void MultiSinkElement::resetOutputFormat()
{
    this->setOutputFormat("");
}

void MultiSinkElement::resetFormatOptions()
{
    this->setFormatOptions({});
}

void MultiSinkElement::resetCodecLib()
{
    globalMultiSink->resetCodecLib();
}

void MultiSinkElement::resetShowFormatOptions()
{
    this->setShowFormatOptions(false);
}

void MultiSinkElement::resetUserControls()
{
    this->setUserControls({});
}

void MultiSinkElement::resetUserControlsValues()
{
    this->setUserControlsValues({});
}

void MultiSinkElement::clearStreams()
{
    this->m_mediaWriter->clearStreams();
}

AkPacket MultiSinkElement::iStream(const AkPacket &packet)
{
    this->m_mutex.lock();

    if (this->state() != ElementStatePlaying) {
        this->m_mutex.unlock();

        return AkPacket();
    }

    this->m_mutexLib.lock();
    this->m_mediaWriter->enqueuePacket(packet);
    this->m_mutexLib.unlock();
    this->m_mutex.unlock();

    return AkPacket();
}

bool MultiSinkElement::setState(AkElement::ElementState state)
{
    AkElement::ElementState curState = this->state();
    QMutexLocker locker(&this->m_mutexLib);
    this->m_mutex.lock();

    if (curState == AkElement::ElementStateNull) {
        if (state != AkElement::ElementStateNull
            && (!this->m_mediaWriter->init())) {
            this->m_mutex.unlock();

            return false;
        }
    } else {
        if (state == AkElement::ElementStateNull)
            this->m_mediaWriter->uninit();
    }

    this->m_mutex.unlock();

    return AkElement::setState(state);
}

void MultiSinkElement::codecLibUpdated(const QString &codecLib)
{
    auto state = this->state();
    this->setState(AkElement::ElementStateNull);

    auto location = this->m_mediaWriter->location();
    auto streams = this->m_mediaWriter->streams();

    this->m_mutexLib.lock();

    this->m_mediaWriter =
            ptr_init<MediaWriter>(this->loadSubModule("MultiSink", codecLib));

    this->m_supportedFormats.clear();
    this->m_fileExtensions.clear();
    this->m_formatDescription.clear();
    this->m_supportedCodecs.clear();
    this->m_codecDescription.clear();
    this->m_codecType.clear();
    this->m_defaultCodecParams.clear();

    for (const QString &format: this->m_mediaWriter->supportedFormats()) {
        this->m_supportedFormats << format;
        this->m_fileExtensions[format] = this->m_mediaWriter->fileExtensions(format);
        this->m_formatDescription[format] = this->m_mediaWriter->formatDescription(format);

        for (const QString &codec: this->m_mediaWriter->supportedCodecs(format))
            if (!this->m_supportedCodecs.contains(codec)) {
                this->m_supportedCodecs << codec;
                this->m_codecDescription[codec] = this->m_mediaWriter->codecDescription(codec);
                this->m_codecType[codec] = this->m_mediaWriter->codecType(codec);
                this->m_defaultCodecParams[codec] = this->m_mediaWriter->defaultCodecParams(codec);
            }
    }

    QObject::connect(this->m_mediaWriter.data(),
                     &MediaWriter::locationChanged,
                     this,
                     &MultiSinkElement::locationChanged);
    QObject::connect(this->m_mediaWriter.data(),
                     &MediaWriter::outputFormatChanged,
                     this,
                     &MultiSinkElement::outputFormatChanged);
    QObject::connect(this->m_mediaWriter.data(),
                     &MediaWriter::formatOptionsChanged,
                     this,
                     &MultiSinkElement::formatOptionsChanged);
    QObject::connect(this->m_mediaWriter.data(),
                     &MediaWriter::streamsChanged,
                     this,
                     &MultiSinkElement::streamsChanged);
    QObject::connect(this->m_mediaWriter.data(),
                     &MediaWriter::streamUpdated,
                     this,
                     &MultiSinkElement::streamUpdated);
    QObject::connect(this,
                     &MultiSinkElement::locationChanged,
                     this->m_mediaWriter.data(),
                     &MediaWriter::setLocation);
    QObject::connect(this,
                     &MultiSinkElement::outputFormatChanged,
                     this->m_mediaWriter.data(),
                     &MediaWriter::setOutputFormat);
    QObject::connect(this,
                     &MultiSinkElement::formatOptionsChanged,
                     this->m_mediaWriter.data(),
                     &MediaWriter::setFormatOptions);

    this->m_mutexLib.unlock();

    this->m_mediaWriter->setLocation(location);

    if (!this->m_supportedFormats.isEmpty())
        this->setOutputFormat(this->m_supportedFormats.first());

    for (int i = 0; i < streams.size(); i++) {
        auto stream = streams[i].toMap();
        this->m_mediaWriter->addStream(stream["index"].toInt(),
                                       stream["caps"].value<AkCaps>());
    }

    emit this->supportedFormatsChanged(this->supportedFormats());

    this->setState(state);
}
