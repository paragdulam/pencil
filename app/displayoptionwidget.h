#ifndef DISPLAYOPTIONDOCKWIDGET_H
#define DISPLAYOPTIONDOCKWIDGET_H

#include "basedockwidget.h"

namespace Ui
{
    class DisplayOption;
}
class Editor;
class QToolButton;


class DisplayOptionWidget : public BaseDockWidget
{
    Q_OBJECT
public:
    explicit DisplayOptionWidget( QWidget* parent );
    virtual ~DisplayOptionWidget();

    void initUI() override;
    void updateUI() override;

    void makeConnectionToEditor(Editor* editor);

private:
    Ui::DisplayOption* ui = nullptr;
};

#endif // DISPLAYOPTIONDOCKWIDGET_H
