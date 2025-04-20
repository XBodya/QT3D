#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QListWidget>
#include <QListWidgetItem>
#include "drawablemesh.h"
#include <QMap>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadMesh();
    void changeMeshColor(QString name);

protected:
    QMap<QString, int> m_objectIndexInListWidget;

private slots:
    void on_actionOpen_triggered();

    void on_actionColor_triggered();

    void on_actionUp_z_triggered();

    void on_actionDown_z_triggered();

    void on_actionUp_x_triggered();

    void on_actionDown_x_triggered();

    void on_actionDown_y_triggered();

    void on_actionUp_y_triggered();

    void on_actionReset_triggered();

    void on_actionZoom_In_triggered();

    void on_actionZoom_out_triggered();

    void on_actionRotate_by_Z_axis_triggered();

    void on_actionRotate_by_X_axis_triggered();

    void on_actionRotate_by_Y_axis_triggered();

    void on_actionFit_model_triggered();

    void on_m_checkBox_stateChanged(int arg1);

    void on_m_objectList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
