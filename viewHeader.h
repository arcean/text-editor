#ifndef HEADER_H
#define HEADER_H

class ViewHeader : public MWidgetController
{
public:
    ViewHeader(QGraphicsItem *parent = 0) :
        MWidgetController(parent),
        linearLayout(0),
        titleWidget(0)
    {
        setObjectName("CommonHeaderPanel");
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        setContentsMargins(0, 0, 0, 0);
        linearLayout = new QGraphicsLinearLayout(Qt::Horizontal, this);
        linearLayout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        linearLayout->setContentsMargins(0, 0, 0, 0);
        linearLayout->setSpacing(0);
        titleWidget = new MLabel();
        titleWidget->setTextElide(true);
        titleWidget->setStyleName("CommonHeaderInverted");
        linearLayout->addItem(titleWidget);
    }

    void setTitle(const QString &title)
    {
        titleWidget->setText(title);
    }

private:
    QGraphicsLinearLayout *linearLayout;
    MLabel *titleWidget;

};

#endif // HEADER_H
