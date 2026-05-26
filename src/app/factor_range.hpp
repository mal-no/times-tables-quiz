#ifndef OTTQ_20250925_2204_INCLUDE
#define OTTQ_20250925_2204_INCLUDE

#include <QObject>
#include <qqml.h>

struct FactorRange
{
    Q_GADGET
    Q_PROPERTY(int first READ first WRITE setFirst FINAL)
    Q_PROPERTY(int second READ second WRITE setSecond FINAL)
    QML_ANONYMOUS

public:
    FactorRange();

    int first() const;
    int second() const;

    void setFirst(const int first);
    void setSecond(const int second);

    bool operator==(const FactorRange &fr);

private:
    int first_;
    int second_;
};

#endif // OTTQ_20250925_2204_INCLUDE
