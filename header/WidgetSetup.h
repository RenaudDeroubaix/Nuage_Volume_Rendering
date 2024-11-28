#ifndef WIDGETSETUP_H
#define WIDGETSETUP_H
#include <QWidget>
#include <QLayout>
#include <QDoubleSpinBox>
#include <QSlider>

QBoxLayout* createLayout(QWidget* parent,
                      bool isVertical = false,
                      const QList<QWidget*>& widgets = {},
                      const QList<QLayout*>& layouts = {}) {
    QBoxLayout* layout;
    if (isVertical) {
        layout = new QVBoxLayout(parent);
    } else {
        layout = new QHBoxLayout(parent);
    }

    for (QWidget* widget : widgets) {
        layout->addWidget(widget);
    }

    for (QLayout* childLayout : layouts) {
        layout->addLayout(childLayout);
    }

    return layout;
}

QWidget* createWidget(QWidget* parent, QLayout* layout) {
    QWidget* widget = new QWidget(parent);
    widget->setLayout(layout);
    return widget;
}

void setSpinBox(QDoubleSpinBox *sb, float rangeStart, float rangeEnd, float step, float value, int width){
    sb->setRange(rangeStart, rangeEnd);
    sb->setSingleStep(step);
    sb->setValue(value);
    sb->setFixedWidth(width);
}

void setSpinBox(QSpinBox *sb, float rangeStart, float rangeEnd, float step, float value, int width){
    sb->setRange(rangeStart, rangeEnd);
    sb->setSingleStep(step);
    sb->setValue(value);
    sb->setFixedWidth(width);
}

void setSlider(QSlider *s, float rangeStart, float rangeEnd,  float value, int width){
    s->setRange(rangeStart, rangeEnd);
    s->setValue(value);
    s->setFixedWidth(width);
}

#endif // WIDGETSETUP_H
