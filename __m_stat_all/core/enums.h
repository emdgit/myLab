#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>

/// Enums for using in QML
class Enums : public QObject
{

    Q_GADGET

public:
    explicit Enums(QObject *parent = nullptr);

    /// For GroupsForm. Mode of 3d Rectangle
    enum GF {
        None,
        NewGroup,
        NewRecord
    };
    Q_ENUM(GF)

    /// Color scheme type
    enum ColorScheme {
        GreenColorScheme,
        RedColorScheme
    };
    Q_ENUM(ColorScheme)

};

#endif // ENUMS_H
