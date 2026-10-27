// SPDX-License-Identifier: AGPL-3.0-or-later

#include "guideWindow.h"
#include <QWindow>

GuideWindow::GuideWindow(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Guide");
    resize(1000, 600);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // =========================================================
    //                  GLOBAL STRUCTURE
    // =========================================================

    this->setObjectName("guideWindow");

    QVBoxLayout *windowLayout = new QVBoxLayout(this);
    windowLayout->setContentsMargins(0, 0, 0, 0);
    windowLayout->setSpacing(0);

    auto *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(30);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor(25, 60, 105, 30));
    this->setGraphicsEffect(shadow);

    // =========================================================
    //                      TITLE BAR
    // =========================================================

    m_titleBar = new QWidget(this);
    m_titleBar->setObjectName("titleBar");
    m_titleBar->setFixedHeight(25);
    m_titleBar->installEventFilter(this);
    m_titleBar->setAttribute(Qt::WA_StyledBackground, true);

    QHBoxLayout *titleLayout = new QHBoxLayout(m_titleBar);
    titleLayout->setContentsMargins(60, 0, 0, 2);
    titleLayout->setSpacing(0);

    QLabel *title = new QLabel("Guide", m_titleBar);
    QToolButton *closeBtn = new QToolButton(m_titleBar);
    closeBtn->setText(QString::fromUtf8("\u2715"));
    closeBtn->setAutoRaise(true);
    closeBtn->setFixedSize(30,25);
    closeBtn->setObjectName("closeBtn");

    QToolButton *reduceBtn = new QToolButton(m_titleBar);
    reduceBtn->setText(QString::fromUtf8("\u2014"));
    reduceBtn->setAutoRaise(true);
    reduceBtn->setFixedSize(30,25);


    titleLayout->addStretch();
    titleLayout->addWidget(title);
    titleLayout->addStretch();
    titleLayout->addWidget(reduceBtn);
    titleLayout->addWidget(closeBtn);

    // =========================================================
    //                    TEXT AREA
    // =========================================================

    QWidget *mainArea = new QWidget(this);
    mainArea->setObjectName("mainArea");
    mainArea->setAttribute(Qt::WA_StyledBackground, true);

    QVBoxLayout *mainAreaLayout = new QVBoxLayout(mainArea);
    mainAreaLayout->setContentsMargins(0, 0, 0, 0);
    mainAreaLayout->setSpacing(20);

    QTextEdit *textArea = new QTextEdit(this);
    textArea->setObjectName("guideContent");
    textArea->setReadOnly(true);

    QFile file(":/gui/resources/guide.html");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        textArea->setHtml(in.readAll());
        file.close();
    }

    mainAreaLayout->addWidget(textArea);

    // =========================================================
    //                    FINAL ASSEMBLY
    // =========================================================

    windowLayout->addWidget(m_titleBar);
    windowLayout->addWidget(mainArea);

    // =========================================================
    //                     CONNECT
    // =========================================================

    connect(closeBtn, &QToolButton::clicked, this, &QWidget::close);
    connect(reduceBtn, &QToolButton::clicked, this, &QWidget::showMinimized);
}

// =========================================================
//                        METHODS
// =========================================================

bool GuideWindow::eventFilter(QObject *obj, QEvent *event) {
    bool result = QWidget::eventFilter(obj, event);
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonRelease) 
    {
        auto *e = static_cast<QMouseEvent *>(event);
        if (event->type() == QEvent::MouseButtonPress && e->button() == Qt::LeftButton) 
        {
            if (obj==m_titleBar){
                windowHandle()->startSystemMove();
                QMouseEvent release(QEvent::MouseButtonRelease,
                            e->position(),
                            e->globalPosition(),
                            Qt::LeftButton,
                            Qt::NoButton,
                            e->modifiers());
                QCoreApplication::sendEvent(this, &release);

                event->accept();
                result = true;
            }
        }
    }
    return result;
}
