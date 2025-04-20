#include <QTimer>
#include <QPalette>
#include <QClipboard>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include "./ui_mainwindow.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void updateOutput();
    void onCopyClicked();
private:
    Ui::MainWindow *ui;
    QTimer *m_restoreTimer;
    QClipboard *m_clipboard;
};
