#ifndef CLICKABLEVIEWHEADER_H
#define CLICKABLEVIEWHEADER_H

#include <MImageWidget>
#include <QGraphicsLinearLayout>
#include <MLabel>
#include <MButton>

class ClickableViewHeader : public MButton
{
public:
    ClickableViewHeader(QGraphicsItem *parent = 0) :
        MButton(parent),
        linearLayout(0),
        titleWidget(0)
    {
        setObjectName("CommonHeaderPanel");
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        setContentsMargins(0, 0, 0, 0);
        linearLayout = new QGraphicsLinearLayout(Qt::Horizontal, this);
        linearLayout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        // Set left margin to 8, margin needed by selectorImage
        linearLayout->setContentsMargins(0, 0, 8, 0);
        linearLayout->setSpacing(0);
        titleWidget = new MLabel();
        titleWidget->setTextElide(true);
        titleWidget->setStyleName("CommonHeaderInverted");
        selectorImage = new MImageWidget();
        selectorImage->setStyleName("SelectorImage");

        linearLayout->addItem(titleWidget);
        linearLayout->addItem(selectorImage);
        linearLayout->setAlignment(selectorImage, Qt::AlignVCenter);
    }

    void setTitle(const QString &title)
    {
        titleWidget->setText(title);
    }

private:
    QGraphicsLinearLayout *linearLayout;
    MLabel *titleWidget;
    MImageWidget *selectorImage;

};

#endif // CLICKABLEVIEWHEADER_H
