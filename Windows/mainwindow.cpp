#include "mainwindow.h"
#include "cipher.h"
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setMinimumSize(600, 400);
    const static QString inputStyle = "QLineEdit {padding: 12px;border: 2px solid #c0c4cc;border-radius: 6px;background: white;}QLineEdit:focus {border-color: #409EFF;}";
    const static QString buttonStyle = "QPushButton {background: qlineargradient(x1:0, y1:0, x2:0, y2:1,stop:0 #409EFF, stop:1 #337ecc);color: white;border-radius: 6px;padding: 15px 30px;}QPushButton:hover { background: #66b1ff; }QPushButton:pressed { background: #337ecc; }";
    QLinearGradient gradient(0, 0, width(), height());
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralwidget);
    QHBoxLayout *btnLayout = new QHBoxLayout;
    QPalette palette;
    auto updateValidation = [this](){
        const bool valid = !ui->saltEdit->text().isEmpty() && !ui->inputEdit->text().isEmpty();
        QString border = valid ? "#409EFF" : "#F56C6C";
        ui->saltEdit->setStyleSheet(inputStyle + QString("border-color: %1;").arg(border));
        ui->inputEdit->setStyleSheet(inputStyle + QString("border-color: %1;").arg(border));
    };
    m_restoreTimer = new QTimer(this);
    m_clipboard = QApplication::clipboard();
    m_restoreTimer->setSingleShot(true);
    gradient.setColorAt(0, QColor(245,247,250));
    gradient.setColorAt(1, QColor(195,207,226));
    palette.setBrush(QPalette::Window, gradient);
    setPalette(palette);
    shadow->setBlurRadius(12);
    shadow->setColor(QColor(64,158,255,100));
    shadow->setOffset(0, 3);
    btnLayout->addStretch();
    btnLayout->addWidget(ui->outputButton);
    btnLayout->addStretch();
    mainLayout->setSpacing(30);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->addWidget(ui->saltEdit);
    mainLayout->addWidget(ui->inputEdit);
    mainLayout->addLayout(btnLayout);
    ui->outputButton->setEnabled(false);
    ui->saltEdit->setStyleSheet(inputStyle);
    ui->inputEdit->setStyleSheet(inputStyle);
    ui->outputButton->setStyleSheet(buttonStyle);
    ui->outputButton->setGraphicsEffect(shadow);
    connect(ui->saltEdit, &QLineEdit::textChanged, updateValidation);
    connect(ui->inputEdit, &QLineEdit::textChanged, updateValidation);
    connect(ui->outputButton, &QPushButton::clicked, this, &MainWindow::onCopyClicked);
    connect(m_restoreTimer, &QTimer::timeout, this, &MainWindow::updateOutput);
    connect(ui->saltEdit, &QLineEdit::textChanged, this, &MainWindow::updateOutput);
    connect(ui->inputEdit, &QLineEdit::textChanged, this, &MainWindow::updateOutput);
    updateOutput();
}
MainWindow::~MainWindow() {
    delete ui;
}
void MainWindow::updateOutput() {
    if (!ui->saltEdit->text().isEmpty() && !ui->inputEdit->text().isEmpty()) {
        QString pwd = Cipher::Get(ui->saltEdit->text(), ui->inputEdit->text());
        if (!pwd.contains("invalid")) {
            ui->outputButton->setEnabled(true);
            ui->outputButton->setText("点我复制: " + pwd);
        } else ui->outputButton->setText(pwd);
    } else {
        ui->outputButton->setText("等待输入...");
        ui->outputButton->setEnabled(false);
    }
}
void MainWindow::onCopyClicked() {
    m_clipboard->setText(Cipher::Get(ui->saltEdit->text(), ui->inputEdit->text()));
    ui->outputButton->setText("✅已复制");
    m_restoreTimer->start(1500);
}
