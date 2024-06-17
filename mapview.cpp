#include "mapview.h"

#include <QDebug>
#include <QPainter>

#include <ozf/exceptions/exception.h>

QImage makeImage(const std::vector<char>& tile_data, const std::vector<uint32_t>& palette, unsigned tile_size)
{
    QImage tile((uchar*)tile_data.data(), tile_size, tile_size, QImage::Format_Indexed8);
    tile.setColorTable(QVector<QRgb>::fromStdVector(palette));
    return tile.mirrored().convertToFormat(QImage::Format_RGB32);
}

MapView::MapView(QWidget* parent)
    : QWidget(parent)
{
}

void MapView::setSource(std::unique_ptr<ozf::ozf_stream>&& source)
{
    _stream = std::move(source);
    _scales = _stream->scales();
    _curScaleIndex = 0;
    emit zoomChanged(_scales[_curScaleIndex].zoom);
    repaint();
}

void MapView::zoomOut()
{
    if (_curScaleIndex < _scales.size() - 1) {
        _curScaleIndex++;
        emit zoomChanged(_scales[_curScaleIndex].zoom);
        repaint();
    }
}

void MapView::zoomIn()
{
    if (_curScaleIndex > 0) {
        _curScaleIndex--;
        emit zoomChanged(_scales[_curScaleIndex].zoom);
        repaint();
    }
}

void MapView::paintEvent(QPaintEvent*) try {
    if (_stream) {
        Q_ASSERT(!_scales.empty());

        QPainter painter(this);
        const ozf::scale& scale = _scales[_curScaleIndex];
        for (unsigned j = 0; j < scale.index.row_count() && j < 15; ++j) {
            for (unsigned i = 0; i < scale.index.column_count() && i < 15; ++i) {
                const ozf::tile_index tile_index = scale.index(i, j);
                const std::vector<char> pixels = _stream->get_tile(tile_index);
                const QImage tile = makeImage(pixels, scale.palette, 64);
                painter.drawImage(QPoint(i * 64, j * 64), tile);
            }
        }
    }
} catch (const ozf::exception& e) {
    qWarning() << e.what();
}
