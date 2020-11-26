#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


/**
 * 选择要处理的图片
 */
void MainWindow::on_pushButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "选择图片", "/home",
                                                tr("Images (*.png *.xpm *.jpg)"));
    if (path.isEmpty())
        return ;

    QPixmap pixmap(path, "1"); // 不加参数二有些图片读不出来？
    ui->label->setPixmap(pixmap);

    ui->listWidget->clear();
    auto colors = ImageUtil::extractImageThemeColors(pixmap.toImage(), 7);
    if (!colors.size())
        return ;
    int maxCount = colors.first().count;
    foreach (auto color, colors)
    {
        auto item = new QListWidgetItem(ui->listWidget);
        QColor qc(color.red, color.green, color.blue);
        item->setText(QString::number(color.count * 100.0 / maxCount, 'f', 1) + "%");
        item->setBackground(qc);
        item->setForeground(ImageUtil::getInvertColor(qc));
    }

    QColor bg, fg;
    ImageUtil::getBgFgColor(colors, &bg, &fg);

    QPalette pa;
    pa.setColor(QPalette::Background, bg);
    pa.setColor(QPalette::Text, fg);
    pa.setColor(QPalette::WindowText, fg);
    setPalette(pa);
}
