#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include "../ObjReader/reader.h"
#include "../ObjReader/basetypes.h"
#include <QColorDialog>

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

void MainWindow::loadMesh()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select a mesh", QString(), "OBJ file (*.obj)");
    //QString fileName = "C:/Users/games/gits/r3ds/ObjectRsAndUnittests/ObjViewport/teapot.obj";
    if(fileName.isEmpty())
        return;
    ObjData objData;
    QString errorMsg;
    if(!ObjReadingTools::readFile(fileName, objData, errorMsg))
    {
        QMessageBox::warning(this, "Failed to load mesh", errorMsg);
        return;
    }

    ui->m_viewport->makeCurrent();
    ui->m_viewport->addObject(objData.m_vertices, objData.m_polygonVertexIndices, objData.m_startPolygonVertexIndices);
    ui->m_viewport->update();
}

void MainWindow::changeMeshColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose color");
    if(!color.isValid())
    {
        return;
    }
    ui->m_viewport->changeFragmentColor(color);
    ui->m_viewport->makeCurrent();
    ui->m_viewport->update();
}



void MainWindow::on_actionOpen_triggered()
{
    loadMesh();
}



void MainWindow::on_actionColor_triggered()
{
    changeMeshColor();
}


void MainWindow::on_actionUp_z_triggered()
{
    ui->m_viewport->m_camera.move(QVector3D{0, 0, 0.1});
    ui->m_viewport->makeCurrent();
    ui->m_viewport->update();
}


void MainWindow::on_actionDown_z_triggered()
{
    ui->m_viewport->m_camera.move(QVector3D{0, 0, -0.1});
    ui->m_viewport->makeCurrent();
    ui->m_viewport->update();
}


void MainWindow::on_actionUp_x_triggered()
{
    ui->m_viewport->m_camera.move(QVector3D{0.1, 0, 0});
    ui->m_viewport->makeCurrent();
    ui->m_viewport->update();
}


void MainWindow::on_actionDown_x_triggered()
{
    ui->m_viewport->m_camera.move(QVector3D{-0.1, 0, 0});
    ui->m_viewport->makeCurrent();
    ui->m_viewport->update();
}


void MainWindow::on_actionDown_y_triggered()
{
    ui->m_viewport->m_camera.move(QVector3D{0, -0.1, 0});
    ui->m_viewport->makeCurrent();
    ui->m_viewport->update();
}


void MainWindow::on_actionUp_y_triggered()
{
    ui->m_viewport->m_camera.move(QVector3D{0, 0.1, 0});
    ui->m_viewport->makeCurrent();
    ui->m_viewport->update();
}


void MainWindow::on_actionReset_triggered()
{
    ui->m_viewport->m_camera.resetCamera();
    ui->m_viewport->m_rotateAngles = {0.0f, 0.0f, 0.0f};
    ui->m_viewport->makeCurrent();
    ui->m_viewport->update();
}


void MainWindow::on_actionZoom_In_triggered()
{
    ui->m_viewport->m_camera.zoom(0.1);
    ui->m_viewport->makeCurrent();
    ui->m_viewport->update();

}



void MainWindow::on_actionZoom_out_triggered()
{
    ui->m_viewport->m_camera.zoom(-0.1);
    ui->m_viewport->makeCurrent();
    ui->m_viewport->update();
}


void MainWindow::on_actionRotate_by_Z_axis_triggered()
{
    ui->m_viewport->changeRotateAngles(QVector3D(0, 0, 10));
    ui->m_viewport->makeCurrent();
    ui->m_viewport->update();
}


void MainWindow::on_actionRotate_by_X_axis_triggered()
{
    ui->m_viewport->changeRotateAngles(QVector3D(10, 0, 0));
    ui->m_viewport->makeCurrent();
    ui->m_viewport->update();
}


void MainWindow::on_actionRotate_by_Y_axis_triggered()
{
    // float newAngle = (ui->m_viewport->m_rotateAngles[1] + 10.0f);
    // if(newAngle > 360) newAngle = (newAngle - 360.0f);;
    // ui->m_viewport->m_rotateAngles[1] = newAngle;
    ui->m_viewport->changeRotateAngles(QVector3D(0, 10, 0));
    ui->m_viewport->makeCurrent();
    ui->m_viewport->update();
}


void MainWindow::on_actionFit_model_triggered()
{
    ui->m_viewport->updateFitToView();
    ui->m_viewport->makeCurrent();
    ui->m_viewport->update();
}


void MainWindow::on_m_checkBox_stateChanged(int arg1)
{
    ui->m_viewport->m_drawPixelByNormalCoords = arg1;
    ui->m_viewport->makeCurrent();
    ui->m_viewport->update();
}

