#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QWidget>

#include <memory>
#include <ozf/ozf_stream.h>

class MapView : public QWidget {
    Q_OBJECT

public:
    explicit MapView(QWidget* parent = nullptr);
    void setSource(std::unique_ptr<ozf::ozf_stream>&& source);
    void zoomOut();
    void zoomIn();

signals:
    void zoomChanged(float zoom);

private:
    std::size_t _curScaleIndex = 0;
    std::unique_ptr<ozf::ozf_stream> _stream;
    std::vector<ozf::scale> _scales;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent*);
};

#endif // MAPVIEW_H
